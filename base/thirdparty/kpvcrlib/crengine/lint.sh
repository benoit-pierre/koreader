#!/bin/bash

set -e

lint_target='lint'

lint_stats() {
    sed -n '/== Stats ==/{n;:_loop;N;$p;b_loop}' "$1"
}

lint_diff() {
    local p=''
    for f in "$@"; do
        if [[ -n "${p}" ]]; then
            diff --color=always -u --label="${p}" <(lint_stats "${p}") --label="${f}" <(lint_stats "${f}") || true
        fi
        p="${f}"
    done
}

lint_run() {
    case $# in
        1) log="$1" ;;
        0) log="$(printf 'log.%03u-%s' "$(git rev-list --count origin/master..)" "$(git log -n1 --format=%h)")" ;;
        *) exit 1 ;;
    esac

    ninja -C build --quiet build.ninja >/dev/null
    # shellcheck disable=SC2016
    ninja -C build "${lint_target}" -vnj1 | sed -n 's/^\[[^]]*\] //p' | sed '/\/lvtinydom\.cpp$/H;${G;s/\n\n/\n/}' | parallel --keep-order --line-buffer |& stdbuf -oL sed -e "s/\x1b\[[0-9;]\+m//g" | stdbuf -oL tee "${log}"
    stats="$(grep -Eo '\[[a-z][^]]+\]$' "${log}" | sort | uniq -c | sort -n)"
    printf '=================================== Stats ===================================\n%s\n' "${stats}" | tee -a "${log}"
}

case "$1" in
    diff)
        shift
        lint_diff "$@"
        ;;
    stats)
        shift
        for f in "$@"; do
            printf '%s\n' "${f}"
            lint_stats "${f}"
        done
        ;;
    lint* | clang-tidy* | cppcheck* | stylelint* | xmllint*)
        lint_target="$1"
        shift
        ;&
    *)
        lint_run "$@"
        ;;
esac
