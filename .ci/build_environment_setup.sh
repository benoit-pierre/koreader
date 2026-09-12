#!/usr/bin/env bash

CI_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=/dev/null
source "${CI_DIR}/common.sh"

declare -r CCACHE_VERSION='4.14'
declare -r NINJATRACING_VERSION='084212eaf68f25c70579958a2ed67fb4ec2a9ca4'

[[ $# -eq 1 ]] || die "1 arguments expected, got $#"
matrix_json="$1"
shift 1

# Convert matrix to associative array.
jq <<<"${matrix_json}"
out="$(jq --raw-output 'to_entries | map("[" + (.key | @sh) + "]=" + (.value | tostring | @sh)) | join(" ")' <<<"${matrix_json}")"
declare -A matrix="(${out})"

# Setup macOS environment.
if [[ "${matrix[target]}" = macos-* ]]; then
    # python packages
    run python3 -m pip install --disable-pip-version-check meson ruamel.yaml
    # ninjatracing
    run wget -qO ninjatracing.zip "https://github.com/nico/ninjatracing/archive/${NINJATRACING_VERSION}.zip"
    run unzip -j ninjatracing.zip '*/ninjatracing'
    run install -m755 ninjatracing /usr/local/bin/
    run rm ninjatracing*
    # brew packages
    packages=(
        autoconf
        automake
        bash
        binutils
        coreutils
        findutils
        libtool
        make
        nasm
        ninja
        pkg-config
        util-linux
    )
    # Don't auto-update.
    run export HOMEBREW_NO_AUTO_UPDATE=1
    # Don't upgrade already installed formulas.
    run export HOMEBREW_NO_INSTALL_UPGRADE=1
    # Remove some installed packages to prevent brew
    # from attempting (and failing) to upgrade them.
    run brew uninstall gradle maven
    run brew install --formula --overwrite --quiet "${packages[@]}"
    brew_paths=(
        "$(brew --prefix)/opt/findutils/libexec/gnubin"
        "$(brew --prefix)/opt/make/libexec/gnubin"
        "$(brew --prefix)/opt/util-linux/bin"
    )
    run printf '%s\n' "${brew_paths[@]}" >>"${GITHUB_PATH}"
    # ccache
    run wget -qO ccache.tar.gz "https://github.com/ccache/ccache/releases/download/v${CCACHE_VERSION}/ccache-${CCACHE_VERSION}-darwin.tar.gz"
    run tar xf ccache.tar.gz -C /usr/local/bin --strip-components=1 "ccache-${CCACHE_VERSION}-darwin/ccache"
    run rm -rf ccache.tar.gz
    # xcode
    run sudo xcode-select -s "/Applications/Xcode_${matrix[xcode_version]}.app"
    run xcodebuild -version
    run xcode-select -p
    # environment
    macos_env=(
        MACOSX_DEPLOYMENT_TARGET="${matrix[macosx_deployment_target]}"
    )
    run printf '%s\n' "${macos_env[@]}" >>"${GITHUB_ENV}"
fi

# Determine ccache directory.
CCACHE_DIR="$(ccache --get-config cache_dir)" || CCACHE_DIR="${PWD}/ccache"

# Job environment variables.
declare -a default_env=(
    CCACHE_DIR="${CCACHE_DIR}"
    CI="${CI}"
    CLICOLOR_FORCE=1
    GITHUB_ACTIONS="${GITHUB_ACTIONS}"
    INSTALL_DIR=install
    OUTPUT_DIR=build
    TARGET="${matrix[target]}"
)
declare -a job_env="(${default_env[*]@Q} ${matrix[env]})"
# Export it now so things like `$CLICOLOR_FORCE` are available.
run export "${job_env[@]}"

# Setup GitHub environment.
run printf '%s\n' "${job_env[@]}" >>"${GITHUB_ENV}"

# Setup Docker.
if [[ -n "${matrix[image]}" ]]; then
    docker_opts=(
        "${job_env[@]/#/--env=}"
        --env="GRADLE_USER_HOME=${HOME}/.gradle"
        --volume="${HOME}:${HOME}"
        ${GITHUB_ACTIONS:+--volume=/github:/github}
    )
    if [[ -n "${matrix[platform]}" ]]; then
        docker_opts+=(--platform="${matrix[platform]}")
    fi
    run ./kodev docker create "${matrix[image]}" "${docker_opts[@]}"
    container_id="$(run docker container ls --latest --format json | jq --raw-output .Names)"
    run docker container rename "${container_id}" kontainer
fi

# Generate cache key.
run make TARGET= cache-key 2>&1
