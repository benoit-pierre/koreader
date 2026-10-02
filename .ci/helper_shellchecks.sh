#!/usr/bin/env bash

CI_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=/dev/null
source "${CI_DIR}/common.sh"

# shellcheck disable=2016
excludes=(
    platform/android/luajit-launcher/gradlew
    plugins/terminal.koplugin/shfm
)
mapfile -t shellscript_locations < <({ git grep --recurse-submodules -lE '^#!(/usr)?/bin/(env )?(bash|sh)' "${excludes[@]/#/:\!}" && git ls-files --recurse-submodules './*.sh' "${excludes[@]/#/:\!}"; } | sort | uniq)

./base/utils/shellcheck.sh "${shellscript_locations[@]}"
