#!/bin/bash

set -xeo pipefail

cmakecmd=(
    cmake -B build -S . -G Ninja --fresh
    -DCMAKE_BUILD_TYPE=Debug
    -DCMAKE_COLOR_DIAGNOSTICS=ON
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache
    -DCMAKE_C_COMPILER_LAUNCHER=ccache
    -DGITHUB_ACTIONS=1
    -DNANOSVG=0
)

"${cmakecmd[@]}" "$@"
