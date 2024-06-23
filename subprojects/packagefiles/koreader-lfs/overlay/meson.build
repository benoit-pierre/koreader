project('koreader-lfs', 'c', version: '1.8.0')

lua_dep = dependency('lua', version: ['>= 5.1'])

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

_static_mod = get_option('default_library') == 'static'
koreader_lfs_mod = build_target(
  'koreader-lfs',
  'src/lfs.c',
  c_args: c_args,
  dependencies: [lua_dep],
  gnu_symbol_visibility: 'hidden',
  install: not _static_mod,
  install_dir: get_option('libdir'),
  target_type: _static_mod ? 'static_library' : 'shared_module',
)

meson.override_dependency(
  meson.project_name(),
  declare_dependency(
    link_with: koreader_lfs_mod,
    version: meson.project_version(),
  ),
)
