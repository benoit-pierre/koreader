project('lua-lj-wpaclient')

meson.override_dependency(meson.project_name(), declare_dependency(version: meson.project_version()))

lua_dep = dependency('lua')

foreach _var : ['install_cmod', 'install_lmod']
  _val = lua_dep.get_variable(_var.to_upper(), pkgconfig_define: ['prefix', ''])
  set_variable(_var, _val.startswith('/') ? _val.substring(1) : _val)
endforeach

install_data(
  'consts_h.lua',
  'poll_h.lua',
  'select_h.lua',
  'socket.lua',
  'socket_h.lua',
  'time_h.lua',
  'wpa_ctrl.lua',
  'wpaclient.lua',
  install_dir: install_lmod / 'lj-wpaclient',
  install_tag: 'runtime',
)
