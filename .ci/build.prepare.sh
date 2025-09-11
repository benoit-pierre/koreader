#!/bin/bash

set -eo pipefail

[[ $# -ge 1 ]]

jobs_file="$1"
shift 1

yq="$(which yq4 || which yq)" 2>/dev/null

jobs_rx="$("${yq}" --output-format=props '"^(" + join("|") + ")$"' <<<"[$(printf '%s,' "$@")]")"
printf 'jobs regex: %s\n' "${jobs_rx}" 1>&2

all_jobs="$("${yq}" --output-format=json . "${jobs_file}")"
for variant in lint emulator macos platform; do
    jobs="$(jq --arg variant "${variant}" --arg enabled "${jobs_rx}" --compact-output '
    [.[$variant][] | select(.id | test($enabled)) |
        .cache=(.cache // .id) | .target=(.target // .id)
    ]' <<<"${all_jobs}")"
    {
        printf '%s jobs: ' "${variant}"
        jq --color-output --sort-keys <<<"${jobs}"
    } 1>&2
    [[ "${jobs}" != '[]' ]] || jobs=''
    printf '%s=%s\n' "${variant}" "${jobs}"
done

# vim: sw=4
