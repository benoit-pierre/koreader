project('gcc-lua-cdecl', ['c', 'cpp'], version: '3.0.0')

fs = import('fs')
pkg = import('pkgconfig')

gcc_lua_dep = dependency('gcc-lua', native: true)
lua_dep = dependency('luajit', native: true)

foreach _var : ['install_cmod', 'install_lmod']
  _val = lua_dep.get_variable(_var.to_upper(), pkgconfig_define: ['prefix', ''])
  set_variable(_var, _val.startswith('/') ? _val.substring(1) : _val)
endforeach

cdata = configuration_data()
cdata.set(
  'CC',
  '\'' + '\' \''.join(meson.get_compiler('c').cmd_array()) + '\'',
)
cdata.set(
  'CXX',
  '\'' + '\' \''.join(meson.get_compiler('cpp').cmd_array()) + '\'',
)

foreach _dirname, _dir : {
  'incdir': get_option('includedir'),
  'libdir': get_option('libdir'),
}
  cdata.set_quoted(_dirname.to_upper(), _dir == '.' ? '' : _dir)
endforeach

configure_file(
  configuration: cdata,
  input: 'ffi-cdecl.meson.sh',
  output: 'ffi-cdecl',
  install: true,
  install_dir: get_option('bindir'),
  install_tag: 'devel',
)

install_data(
  'ffi-cdecl.lua',
  install_dir: install_lmod,
  install_tag: 'devel',
)

install_data(
  'ffi-cdecl/ffi-cdecl.lua',
  install_dir: install_lmod / 'ffi-cdecl',
  install_tag: 'devel',
)

install_subdir(
  'gcc',
  install_dir: install_lmod,
  install_tag: 'devel',
)

install_headers(
  'ffi-cdecl/C.c',
  'ffi-cdecl/C99.c',
  'ffi-cdecl/ffi-cdecl.h',
  subdir: 'ffi-cdecl',
)

pkg.generate(
  name: 'gcc-lua-cdecl',
  description: 'C declaration composer for GCC',
  extra_cflags: [
    '-fplugin-arg-gcclua-script=${ffi_cdecl_script}',
    '-I${ffi_cdecl_idir}',
  ],
  # NOTE: do not require `gcc-lua`, as it will result
  # in the `-fplugin=…` argument being added after our
  # cflags, but it needs to come before `-fplugin-arg-…`.
  # requires: 'gcc-lua',
  variables: {
    'ffi_cdecl_idir': '${includedir}' / 'ffi-cdecl',
    'ffi_cdecl_script': '${prefix}' / install_lmod / 'ffi-cdecl.lua',
  },
  uninstalled_variables: {
    'ffi_cdecl_idir': meson.current_source_dir() / 'ffi-cdecl',
    'ffi_cdecl_script': meson.current_source_dir() / 'ffi-cdecl.lua',
  },
)

sh = find_program('sh', required: get_option('tests'))

if get_option('tests').require(sh.found()).allowed()
  subdir('test')
endif
