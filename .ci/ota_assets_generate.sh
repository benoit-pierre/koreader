#!/usr/bin/env bash

CI_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=/dev/null
source "${CI_DIR}/common.sh"

[[ $# -eq 2 ]] || die "2 arguments expected, got $#"
assets_dir="$1"
channel="$2"
shift 2

latest_make() {
    [[ $# -eq 4 ]] || return
    local mode="$1" file="$2" latest="$3" latest_nightly="$4"
    case "${mode}" in
        copy) run cp "${file}" "${latest}" ;;
        link) run sh -c "echo $(quote "${file}") >$(quote "${latest}")" ;;
        *) return 1 ;;
    esac
    if [[ "${latest}" != "${latest_nightly}" ]]; then
        run cp "${latest}" "${latest_nightly}"
    fi
}

kotasync_make() {
    [[ $# -eq 4 ]] || return
    local txz="$1" kotasync="$2" latest="$3" latest_nightly="$4"
    local cmd=(kotasync make)
    if [[ -e ".ota/${latest_nightly}" ]]; then
        cmd+=(--reorder ".ota/${latest_nightly}")
    fi
    cmd+=("${txz}" "${kotasync}")
    container_exec "${cmd[@]}"
    latest_make copy "${kotasync}" "${latest}" "${latest_nightly}"
}

zsync_make() {
    [[ $# -eq 4 ]] || return
    local tgz="$1" zsync="$2" latest="$3" latest_nightly="$4"
    local cmd=(zsyncmake "${tgz}" -C -u "${tgz##*/}" -o "${zsync}")
    container_exec "${cmd[@]}"
    latest_make copy "${zsync}" "${latest}" "${latest_nightly}"
}

# Fetch latest nightly kotasync files.
if out="$(run gh release view --json assets --jq '.assets[].name | select(test("^koreader-.*-latest-nightly\\.kotasync$"))' ota)" && [[ -n "${out}" ]]; then
    run gh release download --dir="${assets_dir}/.ota" --pattern='koreader-*-latest-nightly.kotasync' ota
    # shellcheck disable=SC2016
    onexit 'run rm -rf "${assets_dir}/.ota"'
fi

# Parse initial list of assets.
initial_assets="$("${CI_DIR}/assets_parse_to_sh.sh" "${assets_dir}"/*)"

printf '%s\n' "${ANSI_DIM}pushd $(quote "${assets_dir}")${ANSI_RESET}" 1>&2
pushd "${assets_dir}" >/dev/null || exit

# Start helper container.
container_start

while read -r line; do
    declare -A asset="(${line})"
    asset[file]="${asset[file]##*/}"

    printf '%s\n' "${ANSI_BLUE}${asset[platform_name]}: ${asset[file]}${ANSI_RESET}"

    latest_files=("koreader-${asset[platform]}"-latest-{"${channel}",nightly})

    case "${asset[platform]}" in

        android-arm) latest_files=("${latest_files[@]/-android-arm-/-android-}") ;&
        android-*)
            case "${asset[extension]}" in
                apk) latest_make link "${asset[file]}" "${latest_files[@]}" ;;
            esac
            echo "${line}"
            if [[ "${asset[stable]}" == 'true' ]] && [[ "${asset[commit_hash]}" ]]; then
                commit_count="$(git rev-list --count "v${asset[base_version]}")"
                run sh -c "printf '%s\n%u\n' 'v${asset[version]}' '${commit_count}' >koreader-android-fdroid-latest"
            fi
            ;;

        cervantes | kindle* | kobo* | pocketbook* | remarkable* | sony-prstux)
            case "${asset[extension]}" in
                tar.xz) kotasync_make "${asset[file]}" "${asset[file]%.tar.xz}.kotasync" "${latest_files[@]/%/.kotasync}" ;;
                targz) zsync_make "${asset[file]}" "${asset[file]%.targz}.zsync" "${latest_files[@]/%/.zsync}" ;;
            esac
            ;;

        linux-*)
            case "${asset[extension]}" in
                AppImage) latest_make link "${asset[file]}" "${latest_files[@]/-linux-/-appimage-}" ;;
                deb) latest_make link "${asset[file]}" "${latest_files[@]/-linux-/-debian-}" ;;
                tar.xz) latest_make link "${asset[file]}" "${latest_files[@]}" ;;
            esac
            ;;

    esac

done <<<"${initial_assets}"

printf '%s\n' "${ANSI_DIM}popd${ANSI_RESET}" 1>&2
popd >/dev/null || exit

# vim: sw=4
