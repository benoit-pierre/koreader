cross_cc = meson.get_compiler('c', native: not meson.is_cross_build())

foreach _source : [
  'array.c',
  'composite.c',
  'enum.c',
  'function.c',
  'pointer.c',
  'scalar.c',
  'variable.c',
  'vector.c',
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
      '-std=gnu99',
      '-fplugin=' + gcc_lua_dep.get_variable('plugin_path'),
      '-fplugin-arg-gcclua-script=' + fs.replace_suffix(_source, '.lua'),
      _source,
    ],
    workdir: meson.current_source_dir(),
  )
endforeach
