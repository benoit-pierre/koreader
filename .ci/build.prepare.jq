def match(filters):
  . as $str | any(filters.[]; . as $f | $str | test("^" + $f + "$"))
;

# Isolate sets & jobs, setup initial filters.
(
 to_entries | {
  "sets": [.[] | .value = (.value | flatten) | select(.value[0] | strings)] | from_entries,
  "jobs": [.[] | select(.value[0] | objects)] | from_entries,
  "filters": $ARGS.positional,
 }
) as $s1
# Update filters with matching sets.
| $s1 | .filters += (
  [
    $s1.sets | to_entries.[]
    | select(.key | match($s1.filters))
    | .value
  ] | flatten | unique
) | . as $s2 | $s2
# And filter-out matching $variant jobs…
| [
  $s2.jobs[$variant] | values | values.[]
  | select(.id | match($s2.filters))
  # …updating optional fields.
  | .cache=(.cache // .id)
  | .target=(.target // .id)
  | .check_ffi_cdecls=(.check_ffi_cdecls // true)
]

# vim: sw=2

