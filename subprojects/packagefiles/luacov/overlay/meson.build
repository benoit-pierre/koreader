project('luacov', version: '0.15.0')

meson.override_dependency(meson.project_name(), declare_dependency(version: meson.project_version()))

lua_dep = dependency('lua', version: ['>= 5.1', '< 5.5'])

foreach _var : ['install_cmod', 'install_lmod']
  _val = lua_dep.get_variable(_var.to_upper(), pkgconfig_define: ['prefix', ''])
  set_variable(_var, _val.startswith('/') ? _val.substring(1) : _val)
endforeach

install_data(
  'src/bin/luacov',
  rename: [
    'luacov',
  ],
  install_dir: get_option('bindir'),
  install_tag: 'runtime',
)

install_data(
  'src/luacov.lua',
  'src/luacov/defaults.lua',
  'src/luacov/hook.lua',
  'src/luacov/linescanner.lua',
  'src/luacov/reporter.lua',
  'src/luacov/reporter/default.lua',
  'src/luacov/runner.lua',
  'src/luacov/stats.lua',
  'src/luacov/tick.lua',
  'src/luacov/util.lua',
  rename: [
    'luacov.lua',
    'luacov/defaults.lua',
    'luacov/hook.lua',
    'luacov/linescanner.lua',
    'luacov/reporter.lua',
    'luacov/reporter/default.lua',
    'luacov/runner.lua',
    'luacov/stats.lua',
    'luacov/tick.lua',
    'luacov/util.lua',
  ],
  install_dir: install_lmod,
  install_tag: 'runtime',
)
