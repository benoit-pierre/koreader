project('lua-turbo', 'c', version: '2.1.3')

crypto_dep = dependency('libcrypto')
lua_dep = dependency('lua', version: ['>= 5.1'])
ssl_dep = dependency('libssl')

foreach _var : ['install_cmod', 'install_lmod']
  _val = lua_dep.get_variable(_var.to_upper(), pkgconfig_define: ['prefix', ''])
  set_variable(_var, _val.startswith('/') ? _val.substring(1) : _val)
endforeach

_static_lib = get_option('default_library') == 'static'
tffi_wrap_lib = build_target(
  'tffi_wrap',
  [
    'deps/http-parser/http_parser.c',
    'deps/turbo_ffi_wrap.c',
  ],
  c_args: '-DHTTP_PARSER_STRICT=0',
  dependencies: [
    crypto_dep,
    lua_dep,
    ssl_dep,
  ],
  gnu_symbol_visibility: 'hidden',
  include_directories: include_directories(
    'deps/http-parser',
  ),
  install: not _static_lib,
  install_dir: get_option('libdir'),
  install_tag: 'runtime',
  target_type: _static_lib ? 'static_library' : 'shared_module',
)

meson.override_dependency(
  meson.project_name(),
  declare_dependency(
    link_with: tffi_wrap_lib,
    version: meson.project_version(),
  ),
)

install_data(
  'turbo.lua',
  'turbovisor.lua',
  install_dir: install_lmod,
  install_tag: 'runtime',
)

install_subdir(
  'turbo',
  # Ignore outdated certificates…
  exclude_files: 'ca-certificates.crt',
  install_dir: install_lmod,
  install_tag: 'runtime',
)
