ljver = '2.1'
abiver = '5.1'
verdir = 'luajit-' + ljver
jitdir = get_option('jitdir')
if jitdir == ''
  jitdir = get_option('datadir') / verdir / 'jit'
endif

foreach _var, _subdir : {
  'install_cmod': 'libdir',
  'install_lmod': 'datadir',
}
  _val = get_option(_var)
  if _val != ''
    assert(not fs.is_absolute(_val))
  else
    _val = get_option(_subdir) / 'lua' / abiver
  endif
  set_variable(_var, _val)
endforeach

luajit_h = custom_target(
  capture: true,
  command: [
    'sed',
    '-e', 's/@0@/@1@/'.format(
      'ROLLING',
      meson.project_version().split('.')[-1],
    ),
    '@INPUT@',
  ],
  input: 'luajit_rolling.h',
  output: 'luajit.h',
  install: true,
  install_tag: 'devel',
  install_dir: get_option('includedir') / verdir,
)

subdir('host')

if host_machine.system() == 'windows'
  lj_vm_out = custom_target(
    'lj_vm.o',
    output: 'lj_vm.o',
    command: [
      buildvm_exe,
      '-m', LJVM_MODE,
      '-o', '@OUTPUT@',
    ],
  )
else
  lj_vm_out = custom_target(
    'lj_vm.S',
    output: 'lj_vm.S',
    command: [
      buildvm_exe,
      '-m', LJVM_MODE,
      '-o', '@OUTPUT@',
    ],
  )
endif

lib_sources = files(
  'lib_base.c',
  'lib_bit.c',
  'lib_buffer.c',
  'lib_debug.c',
  'lib_ffi.c',
  'lib_io.c',
  'lib_jit.c',
  'lib_math.c',
  'lib_os.c',
  'lib_package.c',
  'lib_string.c',
  'lib_table.c',
)

buildvm_headers = []
foreach _target : [
  {'mode': 'bcdef', 'output': 'lj_bcdef.h', 'input': lib_sources},
  {'mode': 'ffdef', 'output': 'lj_ffdef.h', 'input': lib_sources},
  {'mode': 'folddef', 'output': 'lj_folddef.h', 'input': 'lj_opt_fold.c'},
  {'mode': 'libdef', 'output': 'lj_libdef.h', 'input': lib_sources},
  {'mode': 'recdef', 'output': 'lj_recdef.h', 'input': lib_sources},
]
  buildvm_headers += custom_target(
    _target['output'],
    input: _target['input'],
    output: _target['output'],
    command: [buildvm_exe, '-m', _target['mode'], '-o', '@OUTPUT@', '@INPUT@'],
  )
endforeach

custom_target(
  'vmdef.lua',
  input: lib_sources,
  output: 'vmdef.lua',
  command: [buildvm_exe, '-m', 'vmdef', '-o', '@OUTPUT@', '@INPUT@'],
  install: true,
  install_dir: jitdir,
  install_tag: 'runtime',
)

luajit_lib_flags = TARGET_XCFLAGS
luajit_lib_flags += [
  '-D_LARGEFILE_SOURCE',
  '-U_FORTIFY_SOURCE',
  ]
if get_option('mcode_reserve_hack')
  luajit_lib_flags += '-DLJ_MCODE_RESERVE_HACK'
endif

luajit_lib_src = [buildvm_headers, lj_vm_out, luajit_h]
if get_option('amalgam')
  luajit_lib_src += 'ljamalg.c'
else
  luajit_lib_src += lib_sources
  luajit_lib_src += [
    'lib_aux.c',
    'lib_init.c',
    'lj_alloc.c',
    'lj_api.c',
    'lj_asm.c',
    'lj_assert.c',
    'lj_bc.c',
    'lj_bcread.c',
    'lj_bcwrite.c',
    'lj_buf.c',
    'lj_carith.c',
    'lj_ccall.c',
    'lj_ccallback.c',
    'lj_cconv.c',
    'lj_cdata.c',
    'lj_char.c',
    'lj_clib.c',
    'lj_cparse.c',
    'lj_crecord.c',
    'lj_ctype.c',
    'lj_debug.c',
    'lj_dispatch.c',
    'lj_err.c',
    'lj_ffrecord.c',
    'lj_func.c',
    'lj_gc.c',
    'lj_gdbjit.c',
    'lj_ir.c',
    'lj_lex.c',
    'lj_lib.c',
    'lj_load.c',
    'lj_mcode.c',
    'lj_meta.c',
    'lj_obj.c',
    'lj_opt_dce.c',
    'lj_opt_fold.c',
    'lj_opt_loop.c',
    'lj_opt_mem.c',
    'lj_opt_narrow.c',
    'lj_opt_sink.c',
    'lj_opt_split.c',
    'lj_parse.c',
    'lj_prng.c',
    'lj_profile.c',
    'lj_record.c',
    'lj_serialize.c',
    'lj_snap.c',
    'lj_state.c',
    'lj_str.c',
    'lj_strfmt.c',
    'lj_strfmt_num.c',
    'lj_strscan.c',
    'lj_tab.c',
    'lj_trace.c',
    'lj_udata.c',
    'lj_vmevent.c',
    'lj_vmmath.c',
  ]
endif

foreach _path : ['lua_path', 'lua_cpath']
  if get_option(_path) != ''
    luajit_lib_flags += '-D@0@_DEFAULT="@1@"'.format(_path.to_upper(), get_option(_path))
  endif
endforeach

luajit_lib = library(
  'luajit-' + abiver,
  luajit_lib_src,
  c_args: luajit_lib_flags,
  dependencies: [m_dep, dl_dep],
  install: true,
  override_options: ['b_sanitize=none'],
  soversion: ljver,
)

luajit_dep = declare_dependency(
  include_directories: include_directories('.'),
  link_with: luajit_lib,
  variables: {
    'INSTALL_CMOD': install_cmod,
    'INSTALL_LMOD': install_lmod,
  },
)
meson.override_dependency('luajit', luajit_dep)

if get_option('default_library') == 'static'

  if host_machine.system() == 'windows'
    error('building a static LuaJIT library is not supported on Windows')
  endif

  luajit_exe_export_dynamic = true
  lua_dep_link_with = []

else

  luajit_exe_export_dynamic = false
  lua_dep_link_with = luajit_lib

endif

luajit_exe = executable(
  'luajit',
  [luajit_h, 'luajit.c'],
  export_dynamic: luajit_exe_export_dynamic,
  install: get_option('luajit'),
  link_with: luajit_lib,
)
meson.override_find_program('luajit', luajit_exe)

lua_dep = declare_dependency(
  include_directories: include_directories('.'),
  link_with: lua_dep_link_with,
  variables: {
    'INSTALL_CMOD': install_cmod,
    'INSTALL_LMOD': install_lmod,
  },
  version: '5.1',
)
meson.override_dependency('lua', lua_dep)

install_data(
  '../COPYRIGHT',
  install_dir: get_option('datadir') / 'licenses/luajit',
  install_tag: 'doc',
)

install_headers(
  'lauxlib.h',
  'lua.h',
  'lua.hpp',
  'luaconf.h',
  'lualib.h',
  subdir: verdir,
)

install_man('../etc/luajit.1')

install_subdir(
  'jit',
  exclude_files: '.gitignore',
  install_dir: jitdir,
  install_tag: 'runtime',
  strip_directory: true,
)

pkg = import('pkgconfig')
pkg.generate(
  luajit_lib,
  description: 'Just-in-time compiler for Lua',
  filebase: 'luajit',
  name: 'LuaJIT',
  subdirs: verdir,
  url: 'https://luajit.org',
  variables: 'abiver=' + abiver,
)
