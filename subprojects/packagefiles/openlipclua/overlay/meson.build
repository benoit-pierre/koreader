project('openlipclua', 'c', version: '2024.06.23')

lua_dep = dependency('lua', version: ['>= 5.1'])

foreach _var : ['install_cmod', 'install_lmod']
  _val = lua_dep.get_variable(_var.to_upper(), pkgconfig_define: ['prefix', ''])
  set_variable(_var, _val.startswith('/') ? _val.substring(1) : _val)
endforeach

_static_mod = get_option('default_library') == 'static'

fake_liblipc = shared_library(
  'lipc',
  configure_file(
    configuration: {},
    macro_name: 'liblipc',
    output: 'liblipc.c',
  ),
)

fake_liblipc_link_args = ['-Wl,--no-as-needed', fake_liblipc.path()]

openlipclua_mod = build_target(
  'openlipclua',
  'openlipclua.c',
  dependencies: lua_dep,
  gnu_symbol_visibility: 'hidden',
  include_directories: [
    'lua-compat-5.3/c-api',
    'openlipc/include',
  ],
  install: not _static_mod,
  install_dir: install_cmod,
  install_tag: 'runtime',
  link_args: fake_liblipc_link_args,
  link_depends: fake_liblipc,
  name_prefix: '',
  target_type: _static_mod ? 'static_library' : 'shared_module',
)

meson.override_dependency(
  meson.project_name(),
  declare_dependency(
    link_args: fake_liblipc_link_args,
    link_with: openlipclua_mod,
    version: meson.project_version(),
  ),
)
