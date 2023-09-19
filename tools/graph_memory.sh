#!/usr/bin/env bash

# Takes two arguments:
# $1 arguments to pass to pgrep
# $2 process name to pgrep for
function gnuplot_wrapper() {
    # inspired by https://gist.github.com/nicolasazrak/32d68ed6c845a095f75f037ecc2f0436
    trap capture_ctrl_c INT
    TEMP_DIR="$(mktemp --directory /tmp/tmp.koreaderXXX)"
    LOG="${TEMP_DIR}/memory.log"
    SCRIPT_PNG="${TEMP_DIR}/script_png.p"
    SCRIPT_SHOW="${TEMP_DIR}/script_show.p"
    IMAGE_PNG="${TEMP_DIR}/graph.png"

    echo "Memory plot output to ${TEMP_DIR}"

    cat >"${SCRIPT_PNG}" <<EOL
set term pngcairo size 1600,1200
set output "${IMAGE_PNG}"
set ylabel "RSS"
set y2label "VSZ"
set ytics nomirror
set y2tics nomirror in
set yrange [0:*]
set y2range [0:*]
plot "${LOG}" using 3 with lines axes x1y1 title "RSS", "${LOG}" using 2 with lines axes x1y2 title "VSZ"
EOL

    cat >"${SCRIPT_SHOW}" <<EOL
set term qt noraise
set ylabel "RSS"
set y2label "VSZ"
set ytics nomirror
set y2tics nomirror in
set yrange [0:*]
set y2range [0:*]
plot "${LOG}" using 3 with lines axes x1y1 title "RSS", "${LOG}" using 2 with lines axes x1y2 title "VSZ"
pause 1
reread
EOL

    function capture_ctrl_c() {
        kill "${LOOP_PID}"
        kill "${GNUPLOT_PID}"
        gnuplot "${SCRIPT_PNG}"
        exit
    }

    # initialize at 0 so gnuplot has something to show
    echo "0 0 0" >"${LOG}"
    while true; do
        # shellcheck disable=SC2086
        pids="$(pgrep --delimiter ' ' $1 "$2" 2>/dev/null)" &&
            ps -p "${pids}" -o pid=,vsz=,rss= >>"${LOG}"
        sleep 1
    done &
    LOOP_PID=$!
    gnuplot "${SCRIPT_SHOW}" &
    GNUPLOT_PID=$!
    wait
}

if [[ $# -eq 0 ]]; then
    set -- '-f' './luajit -l setupkoenv reader.lua'
fi

gnuplot_wrapper "$@"

# vim: sw=4
