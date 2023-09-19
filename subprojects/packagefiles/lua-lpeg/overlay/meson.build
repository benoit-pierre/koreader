project('lua-lpeg', 'c', version: '1.0.2')

lua_dep = dependency('lua', version: ['>= 5.1'])

foreach _var : ['install_cmod', 'install_lmod']
  _val = lua_dep.get_variable(_var.to_upper(), pkgconfig_define: ['prefix', ''])
  set_variable(_var, _val.startswith('/') ? _val.substring(1) : _val)
endforeach

_static_mod = get_option('default_library') == 'static'
lpeg_mod = build_target(
  'lpeg',
  [
    'lpcap.c',
    'lpcode.c',
    'lpprint.c',
    'lptree.c',
    'lpvm.c',
  ],
  dependencies: lua_dep,
  install: not _static_mod,
  install_dir: install_cmod,
  install_tag: 'runtime',
  name_prefix: '',
  target_type: _static_mod ? 'static_library' : 'shared_module',
)

meson.override_dependency(
  meson.project_name(),
  declare_dependency(
    link_with: lpeg_mod,
    version: meson.project_version(),
  ),
)

install_data(
  're.lua',
  install_dir: install_lmod,
  install_tag: 'runtime',
)
