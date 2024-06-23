project(
  'gcc-lua', 'c',
  version: '1.3.0',
  default_options: 'c_std=none',
)

fs = import('fs')
pkg = import('pkgconfig')

native_cc = meson.get_compiler('c', native: true)
cross_cc = meson.get_compiler('c', native: false)

if native_cc.get_id() != 'gcc' or cross_cc.get_id() != 'gcc'
  error('this project only supports the GCC compiler')
endif

foreach _var, _arg : {
  'gcc_plugins_dir': '-print-file-name=plugin',
  'gcc_machine'    : '-dumpmachine',
  'gcc_version'    : '-dumpversion',
}
  res = run_command([cross_cc, _arg], check: true)
  set_variable(_var, res.stdout().strip())
endforeach

if not fs.is_dir(gcc_plugins_dir)
  error(f'plugins directory does not exists: “@gcc_plugins_dir@”')
endif

gcc_includes = include_directories(gcc_plugins_dir / 'include', 'gcc')

native_cc.has_header(
  'gcc-plugin.h',
  include_directories: gcc_includes,
  required: true,
)

lua_dep = dependency('luajit', native: true)
m_dep = native_cc.find_library('m', required: false)

plugin_install_dir = get_option('libdir') / 'gcc' / gcc_machine / gcc_version / 'plugin'

gcc_lua_plugin = shared_module(
  'gcclua',
  'gcc/gcclua.c',
  c_args: ['-x', 'c++'],
  dependencies: [lua_dep, m_dep],
  include_directories: gcc_includes,
  install: not meson.is_cross_build(),
  install_dir: plugin_install_dir,
  install_tag: 'devel',
  name_prefix: '',
  native: meson.is_cross_build(),
  override_options: 'b_sanitize=none',
)

gcc_lua_dep = declare_dependency(
  compile_args: '-fplugin=' + gcc_lua_plugin.full_path(),
  variables: {
    'plugin_path': gcc_lua_plugin.full_path(),
  },
)

meson.override_dependency('gcc-lua', gcc_lua_dep, native: true)

pkg.generate(
  name: 'gcc-lua',
  description: 'Lua plugin for GCC',
  extra_cflags: '-fplugin=${plugin_path}',
  variables: {
    'plugin_path': '${prefix}' / plugin_install_dir / fs.name(gcc_lua_plugin.full_path()),
  },
  uninstalled_variables: {
    'plugin_path': gcc_lua_plugin.full_path(),
  },
)

sh = find_program('sh', required: get_option('tests'))

if get_option('tests').require(sh.found()).allowed()
  subdir('test')
endif
