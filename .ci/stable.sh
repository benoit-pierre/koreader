#!/usr/bin/env bash

CI_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=/dev/null
source "${CI_DIR}/common.sh"

[[ $# -eq 2 ]] || "2 arguments expected, got $#"
assets_dir="$1"
release_tag="$2"
shift 1

# Download release assets.
run gh release download --dir="${assets_dir}" "${release_tag}"

# Generate OTA assets.
"${CI_DIR}/ota_generate.sh" "${assets_dir}" stable

# Label assets.
out="$("${CI_DIR}/assets_label.sh" "${assets_dir}"/*)"
readarray -t assets <<<"${out}"

# Upload them to the OTA release.
run gh release upload --clobber nightly "${assets[@]}"

# And trim old versions.
"${CI_DIR}/ota_trim.sh"
