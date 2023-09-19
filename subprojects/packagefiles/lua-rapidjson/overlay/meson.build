project(
  'lua-rapidjson',
  'cpp',
  version: '0.7.1',
  default_options: 'cpp_std=c++17',
)

lua_dep = dependency('lua', version: ['>= 5.1'])

foreach _var : ['install_cmod', 'install_lmod']
  _val = lua_dep.get_variable(_var.to_upper(), pkgconfig_define: ['prefix', ''])
  set_variable(_var, _val.startswith('/') ? _val.substring(1) : _val)
endforeach

_static_mod = get_option('default_library') == 'static'
rapidjson_mod = build_target(
  'rapidjson',
  [
    'src/Document.cpp',
    'src/Schema.cpp',
    'src/Userdata.hpp',
    'src/file.hpp',
    'src/luax.hpp',
    'src/rapidjson.cpp',
    'src/values.cpp',
    'src/values.hpp',
  ],
  dependencies: [lua_dep],
  gnu_symbol_visibility: 'inlineshidden',
  include_directories: include_directories('rapidjson/include'),
  install: not _static_mod,
  install_dir: install_cmod,
  install_tag: 'runtime',
  name_prefix: '',
  target_type: _static_mod ? 'static_library' : 'shared_module',
)

meson.override_dependency(
  meson.project_name(),
  declare_dependency(
    link_with: rapidjson_mod,
    version: meson.project_version(),
  ),
)
