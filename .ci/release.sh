#!/usr/bin/env bash

CI_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=/dev/null
source "${CI_DIR}/common.sh"

[[ $# -eq 1 ]] || "1 argument expected, got $#"
assets_dir="$1"
shift

if tag_name="$(git describe --tag --exact-match --match='v[0-9]*' 2>/dev/null)"; then
    channel='stable'
    draft=1
    prerelease=
    title="${tag_name}"
else
    tag_name='nightly'
    channel='nightly'
    draft=
    prerelease=1
    title='OTA'
fi

target="$(git rev-parse HEAD)"

if out="$(gh release view "${tag_name}" --json 'assets' --template '{{ range .assets }}{{ .name }}{{ "\n" }}{{ end }}')"; then
    readarray -t old_assets <<<"${out}"
    mode='edit'
else
    old_assets=()
    mode='create'
fi

{
    echo -e "${ANSI_BLUE}mode      : ${mode}${ANSI_RESET}"
    echo -e "${ANSI_BLUE}tag_name  : ${tag_name}${ANSI_RESET}"
    echo -e "${ANSI_BLUE}draft     : ${draft:-0}${ANSI_RESET}"
    echo -e "${ANSI_BLUE}prerelease: ${prerelease:-0}${ANSI_RESET}"
    echo -e "${ANSI_BLUE}title     : ${title}${ANSI_RESET}"
    echo -e "${ANSI_BLUE}target    : ${target}${ANSI_RESET}"
} 1>&2

if [[ "${channel}" = 'nightly' ]]; then
    # Generate OTA assets.
    "${CI_DIR}/ota_generate.sh" "${assets_dir}" "${channel}"
    # Tag the nightly.
    run git config user.name 'Github Actions'
    run git config user.email '<>'
    run git tag -m '' -f "${tag_name}"
    run git push -f origin "refs/tags/${tag_name}"
fi

# Label assets.
out="$("${CI_DIR}/assets_label.sh" "${assets_dir}"/*)"
readarray -t assets <<<"${out}"

# Create / update release.
cmd=(gh release "${mode}" --target="${target}")
if [[ "${mode}" = 'create' ]]; then
    cmd+=(${draft:+--draft} ${prerelease:+--prerelease} --title="${title}" --notes='')
fi
cmd+=("${tag_name}")
run "${cmd[@]}"

# Upload assets.
run gh release upload --clobber "${tag_name}" "${assets[@]}"

# Cleanup:
if [[ "${channel}" = 'nightly' ]]; then
    # - nightly: old versions
    "${CI_DIR}/ota_trim.sh"
else
    # - stable: left-overs from previous version
    out="$(comm -23 <(printf '%s\n' "${old_assets[@]}" | sort) <(printf '%s\n' "${assets[@]}" | sed 's,^.*/,,;s,#.*$,,;' | sort))"
    if [[ -n "${out}" ]]; then
        readarray -t old_assets <<<"${out}"
        for a in "${old_assets[@]}"; do
            run gh release delete-asset -y "${tag_name}" "${a}"
        done
    fi
fi

# vim: sw=4
