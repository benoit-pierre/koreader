include "assets_parse";

[
  $ARGS.positional[] | asset_parse
  # Ignore latest stable / nightly files.
  | select(.latest_ota | not)
]
# Group by stable / nightly…
| group_by(.commit_number and true or false) 
# …and each list in turn by decreasing version.
| map(group_by(.sort_version) | reverse)
# Trim:
| .[0][$stable_keep_count - 1][0].sort_version as $oldest_stable_version
| (
  # - keep the last $stable_keep_count stables
  .[0][$stable_keep_count:] // [],
  # - keep the last $nightly_keep_count nightlies more recent then the oldest kept stable.
  .[1] | group_by(.[0].sort_version > $oldest_stable_version) | (.[0], .[1][$nightly_keep_count:])
)
| select(.) | flatten | .[].file

# vim: sw=2
