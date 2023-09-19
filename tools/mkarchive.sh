#!/usr/bin/env bash

set -eo pipefail
# set -x

declare -r USAGE="
USAGE:
  $0 [OPTIONS] [--] ARCHIVE_NAME [PATTERNS]

OPTIONS:
  --manifest ENTRY  add a manifest named ENTRY to the archive
"

declare -r DEFAULT_EXCLUDES=(
    '-xr!*.orig'
    '-xr!*.swo'
    '-xr!*.swp'
    '-xr!*.un~'
    '-xr!.*'
)

# shellcheck disable=SC2016
declare -r AWK_HELPERS='
function print_entry(path, size, crc) {
    sub("/$", "", path)
    print path, crc ? size : "", crc
}
function reverse_entry() {
    $0 = $3" "$2" "$1;
}
'

if ! opt=$(getopt -o '+e:h:m:o:' --long 'epoch:,help,manifest:,options:' --name "$0" -- "$@"); then
    echo "${USAGE}"
    exit 1
fi

epoch=''
manifest=''
options=()

eval set -- "${opt}"
while [[ $# -gt 0 ]]; do
    case "$1" in
        -e | --epoch)
            epoch="$2"
            shift
            ;;
        -h | --help)
            echo "${USAGE}"
            exit 0
            ;;
        -m | --manifest)
            manifest="$2"
            shift
            ;;
        -o | --options)
            read -ra a <<<"$2"
            options+=("${a[@]}")
            shift
            ;;
        --)
            shift
            break
            ;;
    esac
    shift
done

if [[ $# -lt 1 ]]; then
    echo "${USAGE}"
    exit 1
fi

archive="$(realpath "$1")"
shift
patterns=("${DEFAULT_EXCLUDES[@]}" "$@")
if [[ -n "${manifest}" ]]; then
    patterns+=("-x!${manifest}")
fi

case "${archive}" in
    *.7z | *.zip) ;;
    *.targz)
        # Note: create a rsyncable gzipped tar.
        if ! gzip="$(which pigz 2>/dev/null || which gzip)"; then
            echo "ERROR: gzip executable not found!" 1>&2
            exit 2
        fi
        ;;
    *)
        echo "ERROR: unsupported archive format: ${archive##*.}" 1>&2
        exit 2
        ;;
esac

# Ensure a "traditional" sort order.
export LC_ALL=C

# We need to use the full path to the executable to avoid
# a weird issue when using the p7zip project pre-built
# binary (`Can't load './7z.dll' (7z.so)...`).
if ! sevenzip=("$(which 7z)"); then
    echo "ERROR: 7z executable not found!" 1>&2
    exit 2
fi

# Figure out if the `-l` switch (dereference symlinks) is needed.
if "${sevenzip[@]}" -l -ba h /dev/null &>/dev/null; then
    sevenzip+=('-l')
fi

# echo "archive : ${archive}"
# echo "epoch   : ${epoch}"
# echo "manifest: ${manifest}"
# echo "options : ${options[*]} [${#options[@]}]"
# echo "patterns: ${patterns[*]}"
# echo "sevenzip: ${sevenzip[*]}"
# read -r

tmpdir="$(mktemp -d -t mkarchive.XXXXXXXXXX)"
trap 'rm -rf "${tmpdir}"' EXIT
mkdir "${tmpdir}/contents"

# Build manifest.
"${sevenzip[@]}" -ba h "${patterns[@]}" |
    awk "${AWK_HELPERS}"'{ reverse_entry(); print_entry($1, $2, $3) }' |
    sort --output="${tmpdir}/manifest"

# Extract list of paths from manifest.
rev <"${tmpdir}/manifest" | cut -f3- -d' ' | rev >"${tmpdir}/paths"
# Quick sanity check: no path outside the current directory.
if grep '^(/|\.\./)' <"${tmpdir}/paths"; then
    echo "ERROR: ^ some paths are outside the current directory!" 1>&2
    exit 2
fi

# Don't forget the archive's internal manifest, if requested.
if [[ -n "${manifest}" ]]; then
    {
        echo "${manifest}"
        cat "${tmpdir}/paths"
    } | sort --output="${tmpdir}/paths_with_manifest"
    install --mode=0644 -D "${tmpdir}/paths_with_manifest" "${tmpdir}/contents/${manifest}"
    # Can't use 7z's `-w` option in this case…
    pushd "${tmpdir}/contents" >/dev/null
    "${sevenzip[@]}" -ba h |
        awk "${AWK_HELPERS}"'{ reverse_entry(); print_entry($1, $2, $3) }' >>"${tmpdir}/manifest"
    popd >/dev/null
    sort --unique --output="${tmpdir}/manifest" "${tmpdir}/manifest"
fi

# cat "${tmpdir}/manifest" | less
# cat "${tmpdir}/paths" | less

# If the archive already exists, check for changes.
if [[ -r "${archive}" ]]; then
    previous_manifest=''
    case "${archive}" in
        *.targz)
            if [[ "${gzip}" = */pigz ]]; then
                # Try to extract the manifest from the comment field…
                previous_manifest="$("${gzip}" --list --verbose "${archive}" | sed -ne '3,$p')"
            fi
            ;;
        *)
            previous_manifest="$("${sevenzip[@]}" -ba -slt l "${archive}" |
                awk "${AWK_HELPERS}"'
                    /^[^ ]+ = / { e[$1] = $3; }
                    /^$/ && e["Size"] != "" { print_entry(e["Path"], e["Size"], e["CRC"]) }
                    ' | sort
            )"
            ;;
    esac
    if [[ -n "${previous_manifest}" ]] &&
        diff --unified \
            --label 'in archive' - <<<"${previous_manifest}" \
            --label 'to add' "${tmpdir}/manifest"; then
        exit
    fi
    # There's no 7z/zip option to overwrite the archive
    # if it already exists (instead of updating it)…
    rm -f "${archive}"
fi

# Make a copy of everything so we can later patch timestamp to ensure
# reproducibility (including "empty" – with ignored files – directories).
tar --create --dereference --no-recursion \
    --verbatim-files-from --files-from="${tmpdir}/paths" |
    tar --extract --directory="${tmpdir}/contents"

cd "${tmpdir}/contents"

# Fix permissions.
chmod og=rX -R .

# Fix timestamps.
if [[ -n "${epoch}" ]]; then
    find . -depth -print0 | xargs -0 touch --date="${epoch}"
fi

# ranger .

# And create the final archive.
if [[ -n "${manifest}" ]]; then
    filelist="${tmpdir}/paths_with_manifest"
else
    filelist="${tmpdir}/paths"
fi
case "${archive}" in
    *.7z)
        # Note: sort by type (for better compression).
        set -x
        "${sevenzip[@]}" -mqs "${options[@]}" a "${archive}" "-i@${filelist}"
        ;;
    *.targz)
        # Note: create a rsyncable gzipped tar.
        if [[ "${gzip}" = */pigz ]]; then
            gzip_comment_opt=(--comment "$(<"${tmpdir}/manifest")")
        else
            # Standard gzip executable does not support setting the comment field…
            gzip_comment_opt=()
        fi
        set -x
        tar --create --no-recursion --owner=0 --group=0 --verbose --verbatim-files-from --files-from "${filelist}" |
            "${gzip}" "${gzip_comment_opt[@]}" --no-name --rsyncable --stdout >"${archive}"
        ;;
    *.zip)
        # Note: ignore extra file attributes and store UTC times (for reproducibility).
        set -x
        env TZ=UTC0 zip -X "${archive}" -@ <"${filelist}"
        ;;
esac

# vim: sw=4
