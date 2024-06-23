project('lua-term', 'c', version: '0.7')

meson.override_dependency(meson.project_name(), declare_dependency(version: meson.project_version()))

lua_dep = dependency('lua', version: ['>= 5.1'])

foreach _var : ['install_cmod', 'install_lmod']
  _val = lua_dep.get_variable(_var.to_upper(), pkgconfig_define: ['prefix', ''])
  set_variable(_var, _val.startswith('/') ? _val.substring(1) : _val)
endforeach

shared_module(
  'core',
  'core.c',
  dependencies: [lua_dep],
  install: true,
  install_dir: install_cmod / 'term',
  install_tag: 'runtime',
  name_prefix: '',
)

install_data(
  'term/colors.lua',
  'term/cursor.lua',
  'term/init.lua',
  rename: [
    'term/colors.lua',
    'term/cursor.lua',
    'term/init.lua',
  ],
  install_dir: install_lmod,
  install_tag: 'runtime',
)
