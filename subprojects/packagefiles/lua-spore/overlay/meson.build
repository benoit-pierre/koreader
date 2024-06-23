project('lua-spore', version: '0.3.3')

meson.override_dependency(meson.project_name(), declare_dependency(version: meson.project_version()))

lua_dep = dependency('lua', version: ['>= 5.1'])
dependency('luasocket', version: ['>= 2.0.2'])
dependency('luajson', version: ['>= 1.3'])

foreach _var : ['install_cmod', 'install_lmod']
  _val = lua_dep.get_variable(_var.to_upper(), pkgconfig_define: ['prefix', ''])
  set_variable(_var, _val.startswith('/') ? _val.substring(1) : _val)
endforeach

install_data(
  'src/Spore.lua',
  'src/Spore/Core.lua',
  'src/Spore/GoogleDiscovery.lua',
  'src/Spore/Middleware/Auth/AWS.lua',
  'src/Spore/Middleware/Auth/Basic.lua',
  'src/Spore/Middleware/Auth/Bearer.lua',
  'src/Spore/Middleware/Auth/DataPublica.lua',
  'src/Spore/Middleware/Auth/Digest.lua',
  'src/Spore/Middleware/Auth/OAuth.lua',
  'src/Spore/Middleware/Cache.lua',
  'src/Spore/Middleware/DoNotTrack.lua',
  'src/Spore/Middleware/Format/JSON.lua',
  'src/Spore/Middleware/Format/XML.lua',
  'src/Spore/Middleware/Format/YAML.lua',
  'src/Spore/Middleware/Logging.lua',
  'src/Spore/Middleware/Mock.lua',
  'src/Spore/Middleware/Parameter/Default.lua',
  'src/Spore/Middleware/Parameter/Force.lua',
  'src/Spore/Middleware/Proxy/Basic.lua',
  'src/Spore/Middleware/Redirection.lua',
  'src/Spore/Middleware/Runtime.lua',
  'src/Spore/Middleware/UserAgent.lua',
  'src/Spore/Protocols.lua',
  'src/Spore/Request.lua',
  'src/Spore/Swagger.lua',
  'src/Spore/WADL.lua',
  'src/Spore/XML.lua',
  rename: [
    'Spore.lua',
    'Spore/Core.lua',
    'Spore/GoogleDiscovery.lua',
    'Spore/Middleware/Auth/AWS.lua',
    'Spore/Middleware/Auth/Basic.lua',
    'Spore/Middleware/Auth/Bearer.lua',
    'Spore/Middleware/Auth/DataPublica.lua',
    'Spore/Middleware/Auth/Digest.lua',
    'Spore/Middleware/Auth/OAuth.lua',
    'Spore/Middleware/Cache.lua',
    'Spore/Middleware/DoNotTrack.lua',
    'Spore/Middleware/Format/JSON.lua',
    'Spore/Middleware/Format/XML.lua',
    'Spore/Middleware/Format/YAML.lua',
    'Spore/Middleware/Logging.lua',
    'Spore/Middleware/Mock.lua',
    'Spore/Middleware/Parameter/Default.lua',
    'Spore/Middleware/Parameter/Force.lua',
    'Spore/Middleware/Proxy/Basic.lua',
    'Spore/Middleware/Redirection.lua',
    'Spore/Middleware/Runtime.lua',
    'Spore/Middleware/UserAgent.lua',
    'Spore/Protocols.lua',
    'Spore/Request.lua',
    'Spore/Swagger.lua',
    'Spore/WADL.lua',
    'Spore/XML.lua',
  ],
  install_dir: install_lmod,
  install_tag: 'runtime',
)
