include "assets_parse";

[$ARGS.positional[] | asset_parse]
| .[] | .file + "#" + ([
  .platform_name,
  if .commit_number then .base_version + "-" + .commit_number else .base_version end,
  "(" + .extension + ")"
] | join(" "))

# vim: sw=2
