project('luassert', version: '1.9.0')

meson.override_dependency(meson.project_name(), declare_dependency(version: meson.project_version()))

lua_dep = dependency('lua', version: ['>= 5.1'])
dependency('lua-say', version: ['>= 1.4.0'])

foreach _var : ['install_cmod', 'install_lmod']
  _val = lua_dep.get_variable(_var.to_upper(), pkgconfig_define: ['prefix', ''])
  set_variable(_var, _val.startswith('/') ? _val.substring(1) : _val)
endforeach

install_data(
  'src/array.lua',
  'src/assert.lua',
  'src/assertions.lua',
  'src/compatibility.lua',
  'src/formatters/binarystring.lua',
  'src/formatters/init.lua',
  'src/init.lua',
  'src/languages/ar.lua',
  'src/languages/de.lua',
  'src/languages/en.lua',
  'src/languages/fr.lua',
  'src/languages/is.lua',
  'src/languages/ja.lua',
  'src/languages/nl.lua',
  'src/languages/ru.lua',
  'src/languages/ua.lua',
  'src/languages/zh.lua',
  'src/match.lua',
  'src/matchers/composite.lua',
  'src/matchers/core.lua',
  'src/matchers/init.lua',
  'src/mock.lua',
  'src/modifiers.lua',
  'src/namespaces.lua',
  'src/spy.lua',
  'src/state.lua',
  'src/stub.lua',
  'src/util.lua',
  rename: [
    'luassert/array.lua',
    'luassert/assert.lua',
    'luassert/assertions.lua',
    'luassert/compatibility.lua',
    'luassert/formatters/binarystring.lua',
    'luassert/formatters/init.lua',
    'luassert/init.lua',
    'luassert/languages/ar.lua',
    'luassert/languages/de.lua',
    'luassert/languages/en.lua',
    'luassert/languages/fr.lua',
    'luassert/languages/is.lua',
    'luassert/languages/ja.lua',
    'luassert/languages/nl.lua',
    'luassert/languages/ru.lua',
    'luassert/languages/ua.lua',
    'luassert/languages/zh.lua',
    'luassert/match.lua',
    'luassert/matchers/composite.lua',
    'luassert/matchers/core.lua',
    'luassert/matchers/init.lua',
    'luassert/mock.lua',
    'luassert/modifiers.lua',
    'luassert/namespaces.lua',
    'luassert/spy.lua',
    'luassert/state.lua',
    'luassert/stub.lua',
    'luassert/util.lua',
  ],
  install_dir: install_lmod,
  install_tag: 'runtime',
)
