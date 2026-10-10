#!/bin/bash
# shellcheck disable=SC2250

set -eo pipefail
# set -x

export DEBUGINFOD_URLS='https://debuginfod.archlinux.org/'
export SDL_VIDEODRIVER='dummy'

kodir='koreader-emulator-x86_64-pc-linux-gnu-debug/koreader'
script="$(dirname "$0")/leaktest.lua"
precmd='run'
follow=''
gdb=''

run() {
    printf '%q ' "$@"
    printf '\n'
    cd "$kodir"
    "$@" || code=$? && code=$?
    cat git-rev
    exit "$code"
}

tool_asan() {
    local nodlclose="$PWD/nodlclose.so"
    local libasan
    libasan="$(gcc -print-file-name=libasan.so)"
    [[ -r "$nodlclose" ]] || gcc --shared -o "$nodlclose" -x c - <<<$'#include <stdio.h>\n int dlclose(void *handle) { }'
    run env LSAN_OPTIONS="verbosity=1:log_threads=1:suppressions=$PWD/asan.sup" LD_PRELOAD="$libasan:$nodlclose" "$@"
}

tool_cachegrind() {
    run valgrind ${follow:+-trace-children=yes} ${gdb:+--vgdb-error=1} --tool=cachegrind --keep-debuginfo=yes "$@"
}

tool_callgrind() {
    run valgrind ${follow:+-trace-children=yes} ${gdb:+--vgdb-error=1} --tool=callgrind --keep-debuginfo=yes "$@"
}

tool_coz() {
    local perf_event_paranoid
    perf_event_paranoid="$(cat /proc/sys/kernel/perf_event_paranoid)"
    if [[ 1 -ne "${perf_event_paranoid}" ]]; then
        cat >&2 <<EOF
/proc/sys/kernel/perf_event_paranoid is too rectrictive: ${perf_event_paranoid}
change it with: sudo sh -c 'echo 1 >/proc/sys/kernel/perf_event_paranoid'
EOF
        exit 1
    fi
    local cmd=(coz run --verbose)
    local builddir
    builddir="$(dirname "$(realpath "${kodir}/luajit")")"
    cmd+=("--binary-scope=${COZ_BINARY_SCOPE:-${builddir}/%}")
    if [[ -n "${COZ_SOURCE_SCOPE}" ]]; then
        cmd+=("--source-scope=${COZ_SOURCE_SCOPE}")
    fi
    unset COZ_BINARY_SCOPE COZ_SOURCE_SCOPE
    run "${cmd[@]}" --verbose --- "$@"
}

tool_dhat() {
    run valgrind ${follow:+-trace-children=yes} ${gdb:+--vgdb-error=1} --tool=dhat --keep-debuginfo=yes "$@"
}

tool_duma() {
    run env LD_PRELOAD=libduma.so "$@"
}

tool_gdb() {
    local cmd=(gdb)
    if [[ -r "$PWD/gdbinit" ]]; then
        cmd+=(-x "$PWD/gdbinit")
    fi
    cmd+=(--directory="$PWD/base" --args)
    run "${cmd[@]}" "$@"
}

tool_isolate() {
    sudo cpupower -c all frequency-set -g performance && sudo cpupower -c all set -b 0 -t 1
    # sudo sh -c 'echo 0 >/sys/devices/system/cpu/cpufreq/boost'
    sudo sh -c 'echo 0 >/proc/sys/kernel/randomize_va_space'
}

tool_heaptrack() {
    run heaptrack --record-only "$@"
}

tool_kcov() {
    run kcov "$PWD/coverage" "$@"
}

tool_ltrace() {
    run ltrace ${follow:+-f} -e 'calloc+realloc+malloc+free-@libc.so*' "$@"
}

tool_memcheck() {
    run valgrind ${follow:+--trace-children=yes} ${gdb:+--vgdb-error=1} --tool=memcheck --keep-debuginfo=yes --gen-suppressions=no --read-var-info=yes --suppressions="$PWD/valgrind.sup" --leak-check=no --track-origins=yes --show-reachable=yes "$@"
}

tool_strace() {
    run strace ${follow:+--follow-forks} "$@"
}

for a in "$@"; do
    shift
    case "$a" in
        -f) follow=1 ;;
        -g) gdb=1 ;;
        -n) kodir='koreader-emulator-x86_64-pc-linux-gnu/koreader' ;;
        -v) unset SDL_VIDEODRIVER ;;
        -*) exit 1 ;;
        *.lua) script="$a" ;;
        *)
            if type "tool_$a" 2>/dev/null | grep -Fqx "tool_$a is a function"; then
                precmd="tool_$a"
            else
                set -- "$@" "$(realpath "$a")"
            fi
            ;;
    esac
done

"$precmd" ./luajit -l setupkoenv "$script" "$@"
