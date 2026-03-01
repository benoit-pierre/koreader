#!/usr/bin/env bash

CI_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=/dev/null
source "${CI_DIR}/common.sh"

run() {
    echo -e "${ANSI_GREEN}$(printf '%q ' "$@")${ANSI_RESET}"
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
        koreader-android-*.apk) printf %s "${a}" >"${a%-v[0-9]*}-latest-nightly" ;;
        koreader-kindlepw2-*.tar.xz) kotasync_make "${a}" "${a%-v[0-9]*}-latest-nightly.kotasync" ;;
    esac
done
popd || exit

if [[ "${delete_release}" -ne 0 ]]; then
    run gh release delete --cleanup-tag --yes "${tag}"
fi
if [[ "${create_release}" -ne 0 ]]; then
    run git tag --force "${tag}"
    run git push -f "${GITHUB_SERVER_URL}/${GITHUB_REPOSITORY}" "refs/tags/${tag}"
    run gh release create --notes='.' --prerelease --target="${new_commit}" --title="${tag}" "${tag}"
fi
run gh release upload "${tag}" artifacts/new/*

# vim: sw=4
