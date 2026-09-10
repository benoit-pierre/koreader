#!/bin/bash
# shellcheck disable=SC2250

set -eo pipefail

mntdir="/run/media/$USER/Kindle/koreader"
srcdir='koreader-kindlepw2-arm-kindlepw2-linux-gnueabi/koreader'
dstdir='/mnt/us/koreader'
dsthost='kindle'

filters=(
    'include /data/hyph/English_GB.pattern'
    'include /data/hyph/English_US.pattern'
    'include /data/hyph/French.pattern'
    'include /data/hyph/languages.json'
    'exclude *.dbg'
    'exclude .git'
    'exclude .git/'
    'exclude .gitignore'
    'exclude /cache/'
    'exclude /data/ca-bundle.crt'
    'exclude /data/cr3.ini'
    'exclude /data/dict/'
    'exclude /data/hyph/*'
    'exclude /data/tessdata/*'
    'exclude /defaults.custom.lua*'
    'exclude /dropbear'
    'exclude /ev_replay.py'
    'exclude /history.lua'
    'exclude /history/'
    'exclude /l10n/'
    'exclude /ota/'
    'exclude /resources/fonts/'
    'exclude /resources/icons/src/'
    'exclude /rsync'
    'exclude /settings.reader.lua'
    'exclude /sftp-server'
    'exclude /tools/'
    'protect /*'
    'protect /cache/'
    'protect /fonts/**'
)

rsync_cmd=(
    rsync
    --progress --recursive --stats
    --copy-links --delete --links
    --itemize-changes
    "${filters[@]/#/--filter=}"
)

if [[ -d "$mntdir" ]]; then
    rsync_cmd+=(
        "$srcdir/"
        "$mntdir"
    )
else
    rsync_cmd+=(
        --rsync-path="$dstdir/rsync"
        "$srcdir/"
        "$dsthost:$dstdir"
    )
fi

echo "${rsync_cmd[@]}"

"${rsync_cmd[@]}" "$@"
