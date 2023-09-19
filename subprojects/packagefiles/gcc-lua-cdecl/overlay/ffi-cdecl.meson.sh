#!/bin/bash

set -eo pipefail

CC=('/usr/bin/ccache' 'gcc')
CXX=('/usr/bin/ccache' 'g++')
INCDIR="include"
LIBDIR="libs.staging"

progdir="$(realpath "$0")"
prog="${progdir##*/}"

incdir=''
pkgdir=''

mapfile -td / pc < <(realpath "$0")
for len in $(seq $((${#pc[@]}-2)) -1 0); do
    dir="$(printf '/%s' "${pc[@]:1:${len}}")"
    if [[ -d "${dir}/meson-uninstalled" ]]; then
        pkgdir="${dir}/meson-uninstalled"
        break
    fi
    if [[ -d "${dir}/${LIBDIR}" ]]; then
        incdir="${dir}/${INCDIR}"
        pkgdir="$dir/${LIBDIR}/pkgconfig"
        break
    fi
done

usage() {
    cat 1>&2 <<-EOF
usage: ${prog} [-c compiler] [-d dependency] [-o output.lua] [-n] [-D…|-I…|-U…]* input.c

    -c compiler    Select compiler: c or c++ (otherwise determined by the input extension)
    -d dependency  Add additional cflags from specified pkg-config dependency
    -o output      Set output file (instead of stdout)

    -D/-I/-U       Those flags are forwarded to the compiler

    -n             Dry-run, show final compiler command only
EOF
    exit 1
}

[[ $# -gt 0 ]] || usage

pkg-config() {
    env \
        PKG_CONFIG_PATH="${pkgdir}" \
        pkg-config --env-only --cflags "$@"
}

compiler=''
dry_run=0
output='/proc/self/fd/1'

read -ra cflags < <(pkg-config --cflags gcc-lua gcc-lua-cdecl)

while getopts 'c:d:o:D:I:U:h:n' opt; do
    case "${opt}" in
        c)
            case "${OPTARG}" in
                c | c++)
                    compiler="${OPTARG}"
                    ;;
                *)
                    usage
                    ;;
            esac
            ;;
        d)
            read -ra a < <(pkg-config --cflags "${OPTARG}")
            cflags+=("${a[@]}")
            ;;
        n)
            dry_run=1
            ;;
        o)
            output="${OPTARG}"
            ;;
        [DIU])
            cflags+=("-${opt}${OPTARG}")
            ;;
        [h?])
            usage
            ;;
    esac
done

shift $((OPTIND - 1))

# echo "OPTIND: ${OPTIND}"
# echo "$#: $@"
# echo "output: ${output}"
# echo "cflags: ${cflags[@]}"
# exit

[[ $# -eq 1 ]] || usage

if [[ -z "${compiler}" ]]; then
    case "$1" in
        *.C | *.cc | *.cpp | *.CPP | *.c++ | *.cp | *.cxx)
            compiler='c++'
            ;;
        *)
            compiler='c'
            ;;
    esac
fi

case "${compiler}" in
    c)
        cmd=("${CC[@]}")
        ;;
    c++)
        cmd=("${CXX[@]}")
        ;;
esac

cmd+=("${cflags[@]}")

if [[ -n "${incdir}" ]]; then
    cmd+=(-I"${incdir}")
fi

cmd+=(-fplugin-arg-gcclua-output="${output}" -S -o /dev/null "$1")

if [[ ${dry_run} -ne 0 ]]; then
    printf '%q ' "${cmd[@]}"
    echo
    exit
fi

exec "${cmd[@]}"

# vim: sw=4
