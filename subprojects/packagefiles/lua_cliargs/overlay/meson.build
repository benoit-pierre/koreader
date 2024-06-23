project('lua_cliargs', version: '3.0')

meson.override_dependency(meson.project_name(), declare_dependency(version: meson.project_version()))

lua_dep = dependency('lua', version: ['>= 5.1'])

foreach _var : ['install_cmod', 'install_lmod']
  _val = lua_dep.get_variable(_var.to_upper(), pkgconfig_define: ['prefix', ''])
  set_variable(_var, _val.startswith('/') ? _val.substring(1) : _val)
endforeach

install_data(
  'src/cliargs.lua',
  'src/cliargs/config_loader.lua',
  'src/cliargs/constants.lua',
  'src/cliargs/core.lua',
  'src/cliargs/parser.lua',
  'src/cliargs/printer.lua',
  'src/cliargs/utils/disect.lua',
  'src/cliargs/utils/disect_argument.lua',
  'src/cliargs/utils/filter.lua',
  'src/cliargs/utils/lookup.lua',
  'src/cliargs/utils/shallow_copy.lua',
  'src/cliargs/utils/split.lua',
  'src/cliargs/utils/trim.lua',
  'src/cliargs/utils/wordwrap.lua',
  rename: [
    'cliargs.lua',
    'cliargs/config_loader.lua',
    'cliargs/constants.lua',
    'cliargs/core.lua',
    'cliargs/parser.lua',
    'cliargs/printer.lua',
    'cliargs/utils/disect.lua',
    'cliargs/utils/disect_argument.lua',
    'cliargs/utils/filter.lua',
    'cliargs/utils/lookup.lua',
    'cliargs/utils/shallow_copy.lua',
    'cliargs/utils/split.lua',
    'cliargs/utils/trim.lua',
    'cliargs/utils/wordwrap.lua',
  ],
  install_dir: install_lmod,
  install_tag: 'runtime',
)
