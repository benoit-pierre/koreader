project('lua-dkjson', 'c', version: '2.6')

meson.override_dependency(meson.project_name(), declare_dependency(version: meson.project_version()))

lua_dep = dependency('lua', version: ['>= 5.1', '< 5.5'])

foreach _var : ['install_cmod', 'install_lmod']
  _val = lua_dep.get_variable(_var.to_upper(), pkgconfig_define: ['prefix', ''])
  set_variable(_var, _val.startswith('/') ? _val.substring(1) : _val)
endforeach

install_data(
  'dkjson.lua',
  rename: [
    'dkjson.lua',
  ],
  install_dir: install_lmod,
  install_tag: 'runtime',
)
