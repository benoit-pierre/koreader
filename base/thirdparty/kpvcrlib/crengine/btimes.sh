#!/bin/sh

ninjatracing build/.ninja_log | jq -j 'sort_by(-.dur) | .[] | select(.dur >= 1e6) | (.dur*1e-5 | round | ./10), "\n", (.name | sub(".*/CMakeFiles/(?<t>[^/]*)$"; "\(.t)")), "\n"' | xargs -n2 printf '%6.2fs %s\n' | git column --mode=row
