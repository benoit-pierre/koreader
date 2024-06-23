project('luasocket', 'c', version: '3.1.0')

lua_dep = dependency('lua', version: ['>= 5.1'])

foreach _var : ['install_cmod', 'install_lmod']
  _val = lua_dep.get_variable(_var.to_upper(), pkgconfig_define: ['prefix', ''])
  set_variable(_var, _val.startswith('/') ? _val.substring(1) : _val)
endforeach

add_project_arguments('-DLUASOCKET_NODEBUG', language: ['c'])

if host_machine.system() == 'darwin'
  add_project_arguments('-DUNIX_HAS_SUN_LEN', language: ['c'])
  socket_src = 'src/usocket.c'
  socket_dep = dependency('', required: false)
elif host_machine.system() in ['android', 'linux']
  socket_src = 'src/usocket.c'
  socket_dep = dependency('', required: false)
elif host_machine.system() == 'windows'
  add_project_arguments('-DWINVER=0x0501', language: ['c'])
  socket_src = 'src/wsocket.c'
  socket_dep = meson.get_compiler('c').find_library('ws2_32')
else
  error('unsupported host system: ' + host_machine.system())
endif

luasocket_headers = []
subdir('include/luasocket')

luasocket_lib = static_library(
  'luasocket',
  [
    socket_src,
    'src/auxiliar.c',
    'src/buffer.c',
    'src/compat.c',
    'src/except.c',
    'src/inet.c',
    'src/io.c',
    'src/options.c',
    'src/select.c',
    'src/tcp.c',
    'src/timeout.c',
    'src/udp.c',
  ],
  dependencies: [lua_dep, socket_dep],
)

luasocket_lib_dep = declare_dependency(
  include_directories: include_directories('include'),
  dependencies: [lua_dep, socket_dep],
  link_with: luasocket_lib,
  sources: luasocket_headers,
)

meson.override_dependency('libluasocket', luasocket_lib_dep)

_static_mod = get_option('default_library') == 'static'

score_mod = build_target(
  'score',
  'src/luasocket.c',
  dependencies: luasocket_lib_dep,
  gnu_symbol_visibility: 'hidden',
  install: not _static_mod,
  install_dir: install_cmod / 'socket',
  install_tag: 'runtime',
  name_prefix: '',
  target_type: _static_mod ? 'static_library' : 'shared_module',
)

mcore_mod = build_target(
  'mcore',
  'src/mime.c',
  dependencies: [lua_dep],
  install: not _static_mod,
  install_dir: install_cmod / 'mime',
  install_tag: 'runtime',
  name_prefix: '',
  target_type: _static_mod ? 'static_library' : 'shared_module',
)

meson.override_dependency(
  meson.project_name(),
  declare_dependency(
    link_with: [score_mod, mcore_mod],
    version: meson.project_version(),
  ),
)

install_data(
  'src/ltn12.lua',
  'src/mime.lua',
  'src/socket.lua',
  'src/ftp.lua',
  'src/headers.lua',
  'src/http.lua',
  'src/smtp.lua',
  'src/tp.lua',
  'src/url.lua',
  rename: [
    'ltn12.lua',
    'mime.lua',
    'socket.lua',
    'socket/ftp.lua',
    'socket/headers.lua',
    'socket/http.lua',
    'socket/smtp.lua',
    'socket/tp.lua',
    'socket/url.lua',
  ],
  install_dir: install_lmod,
  install_tag: 'runtime',
)
