_test_language_standard = {
  'cxx': 'c++98',
  'c'  : 'c99',
}

foreach _source : [
  'event.c',
  'globals.c',
  'tree.c',
  'tree-cp.cxx',
]
  test(
    _source,
    sh,
    args: [
      '-c',
      'exec "$@"',
      '--',
      cross_cc.cmd_array(),
      '-S',
      '-Wall',
      '-pedantic',
      '-std=' + _test_language_standard[_source.split('.')[-1]],
      '-fplugin=' + gcc_lua_plugin.full_path(),
      '-fplugin-arg-gcclua-script=' + fs.replace_suffix(_source, '.lua'),
      # Note: the `./` is important (`globals.lua` checks for `./globals.c`).
      './' + _source,
    ],
    workdir: meson.current_source_dir(),
  )
endforeach
