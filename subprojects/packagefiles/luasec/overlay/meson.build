project('luasec', 'c', version: '1.3.2')

lua_dep = dependency('lua', version: ['>= 5.1'])
libluasocket_dep = dependency('libluasocket')
dependency('luasocket', version: [])
ssl_dep = dependency('openssl')

foreach _var : ['install_cmod', 'install_lmod']
  _val = lua_dep.get_variable(_var.to_upper(), pkgconfig_define: ['prefix', ''])
  set_variable(_var, _val.startswith('/') ? _val.substring(1) : _val)
endforeach

c_args = ['-DWITH_LUASOCKET']

if host_machine.system() == 'windows'
  c_args += [
    '-DLSEC_API=__declspec(dllexport)',
    '-DWINVER=0x0501',
    '-D_WIN32_WINNT=0x0501',
    '-DNTDDI_VERSION=0x05010300',
  ]
endif

_static_mod = get_option('default_library') == 'static'
ssl_mod = build_target(
  'ssl',
  [
    'src/config.c',
    'src/context.c',
    'src/ec.c',
    'src/options.c',
    'src/ssl.c',
    'src/x509.c',
  ],
  c_args: c_args,
  dependencies: [libluasocket_dep, lua_dep, ssl_dep],
  install: not _static_mod,
  install_dir: install_cmod,
  install_tag: 'runtime',
  name_prefix: '',
  target_type: _static_mod ? 'static_library' : 'shared_module',
)

meson.override_dependency(
  meson.project_name(),
  declare_dependency(
    link_with: ssl_mod,
    version: meson.project_version(),
  ),
)

install_data(
  'src/ssl.lua',
  'src/https.lua',
  rename: [
    'ssl.lua',
    'ssl/https.lua',
  ],
  install_dir: install_lmod,
  install_tag: 'runtime',
)
