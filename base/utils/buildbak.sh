#!/bin/bash
# shellcheck disable=SC2250

set -eo pipefail

setarch() {
    case "$1" in
        emulator) arch=x86_64-pc-linux-gnu-debug ;;
        android-arm) arch=armv7a-unknown-linux-android18 ;;
        android-arm64) arch=aarch64-unknown-linux-android21 ;;
        kindlepw2) arch=arm-kindlepw2-linux-gnueabi ;;
        kobo) arch=arm-kobo-linux-gnueabihf ;;
        linux) arch=x86_64-pc-linux-gnu ;;
        pocketbookhf) arch=arm-pocketbookhf-linux-gnueabihf ;;
        *) exit 1 ;;
    esac
}

set -x
for arg in "$@"; do
    target="${arg%%:*}"
    if [[ "${arg}" = *:* ]]; then
        tag="${arg#*:}"
    else
        tag=''
    fi
    setarch "$target"
    rm -rf "build/$arch"/{cmake,staging,thirdparty}
    mv --no-target-directory "build/$arch"{,."$(git log -1 --format=%ct)"＿"$(git rev-parse --short @)"${tag:+＿$tag}}
done
