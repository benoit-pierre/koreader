#!/usr/bin/env bash

CI_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=/dev/null
source "${CI_DIR}/common.sh"

run() {
    echo -e "${ANSI_GREEN}$(printf '%q ' "$@")${ANSI_RESET}" 1>&2
    "$@"
}

kotasync_make() {
    txz="$1"
    mfst="$2"
    shift 2
    cmd=(kotasync --appimage-extract-and-run make)
    if [[ -f "../old/${mfst}" ]]; then
        cmd+=(--reorder "../old/${mfst}")
    fi
    cmd+=("${txz}" "${mfst}")
    run "${cmd[@]}"
}

zsync_make() {
    txz="$1"
    mfst="$2"
    shift 2
    tar="${txz%.xz}"
    tgz="${tar}.gz"
    run xzcat "${txz}" >"${tar}"
    run pigz -9 --rsyncable "${tar}"
    cmd=(zsyncmake "${tgz}" -C -u "${tgz##*/}" -o "${mfst}")
    run "${cmd[@]}"
}

tag=ota

gh repo set-default "${GITHUB_REPOSITORY}"

new_commit="$(git rev-parse HEAD)"
old_commit="$(gh release view "${tag}" --json targetCommitish | jq -r .targetCommitish || true)"
if [[ -n "${old_commit}" ]]; then
    old_commit="$(git rev-parse "${old_commit}")"
fi

create_release=0
delete_release=0

if [[ -z "${old_commit}" ]]; then
    create_release=1
elif [[ "${old_commit}" != "${new_commit}" ]]; then
    if gh release view "${tag}" --json assets | jq --exit-status '.assets[]|.name|select(test("\\.kotasync$"))'; then
        run gh release download "${tag}" --dir artifacts/old --pattern '*.kotasync'
    fi
    create_release=1
    delete_release=1
fi

pushd artifacts/new || exit
for a in *; do
    case "${a}" in
        koreader-*.AppImage)
            arch="${a##*-}"
            arch="${arch%.AppImage}"
            printf %s "${a}" >"koreader-appimage-${arch}-latest-nightly"
            ;;
        koreader-android-*.apk)
            printf %s "${a}" >"${a%-v[0-9]*}-latest-nightly"
            ;;
        koreader-kindlepw2-*.tar.xz)
            kotasync_make "${a}" "${a%-v[0-9]*}-latest-nightly.kotasync"
            zsync_make "${a}" "${a%-v[0-9]*}-latest-nightly.zsync"
            ;;
    esac
done
popd || exit

if [[ "${delete_release}" -ne 0 ]]; then
    run gh release delete --cleanup-tag --yes "${tag}"
fi
if [[ "${delete_release}" -ne 0 ]] && [[ "${create_release}" -ne 0 ]]; then
    # Workaround for https://github.com/cli/cli/issues/8458…
    sleep 1
fi
if [[ "${create_release}" -ne 0 ]]; then
    run git tag --force "${tag}"
    run git push -f "${GITHUB_SERVER_URL}/${GITHUB_REPOSITORY}" "refs/tags/${tag}"
    run gh release create --notes='.' --prerelease --target="${new_commit}" --title="${tag}" "${tag}"
fi
artifacts=()
for a in artifacts/new/*; do
    case "${a##*/}" in
        koreader-android-arm-*.apk)
            a+='#Android ARM APK'
            ;;
        koreader-android-arm-*-nightly)
            a+='#Android ARM OTA'
            ;;
        koreader-android-arm64-*.apk)
            a+='#Android ARM64 APK'
            ;;
        koreader-android-arm64-*-nightly)
            a+='#Android ARM64 OTA'
            ;;
        koreader-kindlepw2-*.tar.gz)
            a+='#KindlePW2 TAR.GZ'
            ;;
        koreader-kindlepw2-*.tar.xz)
            a+='#KindlePW2 TAR.XZ'
            ;;
        koreader-kindlepw2-*-nightly.kotasync)
            a+='#KindlePW2 OTA (KOTASync)'
            ;;
        koreader-kindlepw2-*-nightly.zsync)
            a+='#KindlePW2 OTA (ZSync)'
            ;;
        koreader-*-x86_64.AppImage)
            a+='#Linux x86_64 AppImage'
            ;;
        koreader-appimage-*-nightly)
            a+='#Linux x86_64 OTA'
            ;;
    esac
    artifacts+=("${a}")
done
readarray -t artifacts < <(printf '%s\n' "${artifacts[@]}" | sort -t\# -k2)
run gh release upload "${tag}" "${artifacts[@]}"

# vim: sw=4
