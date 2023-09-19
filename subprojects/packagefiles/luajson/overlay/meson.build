project('luajson', version: '1.3.4')

meson.override_dependency(meson.project_name(), declare_dependency(version: meson.project_version()))

lua_dep = dependency('lua', version: ['>= 5.1'])
dependency('lua-lpeg', version: ['>= 0.8.1'])

foreach _var : ['install_cmod', 'install_lmod']
  _val = lua_dep.get_variable(_var.to_upper(), pkgconfig_define: ['prefix', ''])
  set_variable(_var, _val.startswith('/') ? _val.substring(1) : _val)
endforeach

install_data(
  'lua/json.lua',
  'lua/json/decode.lua',
  'lua/json/decode/composite.lua',
  'lua/json/decode/number.lua',
  'lua/json/decode/others.lua',
  'lua/json/decode/state.lua',
  'lua/json/decode/strings.lua',
  'lua/json/decode/util.lua',
  'lua/json/encode.lua',
  'lua/json/encode/array.lua',
  'lua/json/encode/calls.lua',
  'lua/json/encode/number.lua',
  'lua/json/encode/object.lua',
  'lua/json/encode/others.lua',
  'lua/json/encode/output.lua',
  'lua/json/encode/output_utility.lua',
  'lua/json/encode/strings.lua',
  'lua/json/util.lua',
  rename: [
    'json.lua',
    'json/decode.lua',
    'json/decode/composite.lua',
    'json/decode/number.lua',
    'json/decode/others.lua',
    'json/decode/state.lua',
    'json/decode/strings.lua',
    'json/decode/util.lua',
    'json/encode.lua',
    'json/encode/array.lua',
    'json/encode/calls.lua',
    'json/encode/number.lua',
    'json/encode/object.lua',
    'json/encode/others.lua',
    'json/encode/output.lua',
    'json/encode/output_utility.lua',
    'json/encode/strings.lua',
    'json/util.lua',
  ],
  install_dir: install_lmod,
  install_tag: 'runtime',
)
