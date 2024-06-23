project('luafilesystem', 'c', version: '1.8.0')

meson.override_dependency(meson.project_name(), declare_dependency(version: meson.project_version()))

lua_dep = dependency('lua', version: ['>= 5.1'])

foreach _var : ['install_cmod', 'install_lmod']
  _val = lua_dep.get_variable(_var.to_upper(), pkgconfig_define: ['prefix', ''])
  set_variable(_var, _val.startswith('/') ? _val.substring(1) : _val)
endforeach

c_args = []

if host_machine.system() == 'android'
  cc = meson.get_compiler('c')
  # Workaround https://github.com/mesonbuild/meson/issues/3519
  # NOTE: to be removed when upgrading requirements to `NDKAPI >= 24`
  # (https://android.googlesource.com/platform/bionic/+/master/docs/32-bit-abi.md)
  if cc.sizeof('void *') == 4 and not meson.get_compiler('c').has_function('fseeko64')
    c_args += ['-DLFS_DO_NOT_USE_LARGE_FILE']
  endif
endif

shared_module(
  'lfs',
  'src/lfs.c',
  c_args: c_args,
  dependencies: lua_dep,
  install: true,
  install_dir: install_cmod,
  name_prefix: '',
)
