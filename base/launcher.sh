#!/bin/sh

set -e

kodir="$(realpath "$(dirname "$0")")"

# Fix relative paths.
for arg in "$@"; do
    case "${arg}" in
        -*) ;;
        /*) ;;
        *)
            arg="${PWD}/${arg}"
            ;;
    esac
    set -- "$@" "${arg}"
    shift
done

cd "${kodir}"

while true; do
    status=0
    ./luajit -l setupkoenv reader.lua "$@" || status=$?
    [ ${status} -eq 85 ] || exit ${status}
done

# vim: sw=4
