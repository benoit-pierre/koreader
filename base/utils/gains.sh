#!/bin/bash
# shellcheck disable=SC2250

set -eo pipefail

opts=()
tgts=()
for a in "$@"; do
    case "$a" in
        -*) opts+=("$a") ;;
        *) tgts+=("$a") ;;
    esac
done

ruler='━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━'

diffs() {
    local what="$1"
    local build="build/$2"
    shift 2
    local args=()
    for f in "$@"; do
        args+=(-o -path "$build.*/$f" -o -path "$build/$f")
    done
    set -- "${args[@]}"
    shift
    printf '\n%s\n%s\n%s\n' "$ruler" "$what" "$ruler"
    mapfile -t directories < <(find build "$@")
    mapfile -t binaries < <(./utils/binfind.py "${directories[@]}")
    ./utils/ds.py "${opts[@]}" "${binaries[@]}"
}

setarch() {
    case "$1" in
        appimage) arch=x86_64-linux-gnu ;;
        emulator) arch=x86_64-pc-linux-gnu ;;
        android-arm) arch=armv7a-unknown-linux-android18 ;;
        android-arm64) arch=aarch64-unknown-linux-android21 ;;
        kindlepw2) arch=arm-kindlepw2-linux-gnueabi ;;
        kobo) arch=arm-kobo-linux-gnueabihf ;;
        *) exit 1 ;;
    esac
}

for target in "${tgts[@]}"; do
    echo
    setarch "$target"
    case "$target" in
        appimage | emulator)
            diffs "$target koreader" "$arch" luajit common libs
            diffs "$target sdcv" "$arch" sdcv libs/libz.so.\*
            ;;
        android-*)
            diffs "$target koreader" "$arch" libs
            diffs "$target sdcv" "$arch" sdcv libs/libc++_shared.so
            ;;
        kindlepw2 | kobo)
            diffs "$target koreader" "$arch" luajit common libs
            diffs "$target dbclient" "$arch" dbclient libs/libz.so.\*
            diffs "$target dropbear" "$arch" dropbear libs/lib{crypto,z}.so.\*
            diffs "$target fbink" "$arch" fbink
            diffs "$target scp" "$arch" scp
            diffs "$target sdcv" "$arch" sdcv libs/libz.so.\*
            diffs "$target sftp-server" "$arch" sftp-server
            diffs "$target tar" "$arch" tar unpack
            diffs "$target zsync2" "$arch" zsync2 libs/lib{crypto,ssl,z}.so.\*
            ;;
        *) exit 1 ;;
    esac
done
