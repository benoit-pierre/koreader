def match(filters):
  . as $str | any(filters.[]; . as $f | $str | test("^" + $f + "$"))
;

def cache_key(kind; id):
  [$inputs.cache_epoch, kind, id] | join("-")
;

def cache_key(kind; id; build):
  [$inputs.cache_epoch, kind, id, build] | join("-")
;

def cached:
  . as $key | ($caches[] | select(.key == $key)) and true // false
;

# Isolate sets & jobs, setup initial filters.
(
 to_entries | {
  "sets": [.[] | .value = (.value | flatten) | select(.value[0] | strings)] | from_entries,
  "jobs": [.[] | select(.value[0] | objects)] | from_entries,
  "filters": ($inputs.jobs | split(" +")),
 }
) as $s1 #| debug
# Update filters with matching sets.
| $s1 | .filters += (
  [
    $s1.sets | to_entries.[]
    | select(.key | match($s1.filters))
    | .value
  ] | flatten | unique
) | . as $s2 | $s2 #| debug
# And filter-out matching jobs…
| $s2.jobs | to_entries #| debug
| map(.value = (
  [
    .value.[]
    | select(.id | match($s2.filters))
    # …updating optional fields…
    | .cache_id = (.cache // .id)
    | .cache_key_build = cache_key("build"; .cache_id; $cache_key_build)
    | .cache_key_ccache_exact = cache_key("ccache"; .cache_id; $cache_key_build)
    | .cache_key_ccache_partial = cache_key("ccache"; .cache_id)
    | .cache_size |= ($inputs.cache_size // "512M")
    | .target=(.target // .id)
    # Steps.
    | .build = (.cache_key_build | cached | not)
    | .check_ffi_cdecls = .build and $inputs.ffi_cdecls_check and (.check_ffi_cdecls // true)
    | .test = .test and $inputs.tests_run
    | if $inputs.artifacts or $inputs.artifacts_all then . else del(.artifact) end
    # Only keep a job if it's going to be built, FFI
    # cdecls checked, tested, or generate artifacts.
    | select(.build or .check_ffi_cdecls or .test or .artifact)
  ]
)) #| debug
# Split into 2 groups: emulator and platform jobs.
| group_by(.key == "emulator")
| map({
  "key": (if .[0].key == "emulator" then "emulator" else "platform" end),
  "value": ([.[].value] | flatten),
})
# Sort job list and encode it to json.
| map(.value = (.value | sort_by(.id) | tojson))
# Back to a mapping of [emulator|platform] => jobs.
| from_entries
