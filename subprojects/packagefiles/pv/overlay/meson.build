project('pv', 'c', version: files('doc/VERSION'))

cc = meson.get_compiler('c')
cdata = configuration_data()
feature_macros = ['-D_GNU_SOURCE']

foreach _hdr : [
  'getopt.h',
  'libgen.h',
  'libintl.h',
  'locale.h',
  'stdbool.h',
  'string.h',
  'sys/ipc.h',
  'sys/param.h',
]
  cdata.set('HAVE_' + _hdr.underscorify().to_upper(), cc.has_header(_hdr))
endforeach

foreach _type : [
  '_Bool',
]
  cdata.set(
    'HAVE_' + _type.underscorify().to_upper(),
    cc.has_type(_type, args: feature_macros),
  )
endforeach

foreach _check : [
  ['basename'   , 'libgen.h'  ],
  ['open64'     , 'fcntl.h'   ],
  ['getopt'     , 'unistd.h'  ],
  ['getopt_long', 'getopt.h'  ],
  ['gettext'    , 'libintl.h' ],
  ['memcpy'     , 'string.h'  ],
  ['snprintf'   , 'stdio.h'   ],
  ['splice'     , 'fcntl.h'   ],
  ['stat64'     , 'sys/stat.h'], 
]
  _sym = _check[0]
  _hdr = _check[1]
  cdata.set(
    'HAVE_' + _sym.underscorify().to_upper(),
    cc.has_header_symbol(_hdr, _sym, args: feature_macros),
  )
endforeach

ipc_opt = get_option(
  'ipc',
).require(
  cdata.get('HAVE_SYS_IPC_H'),
  error_message: 'need "sys/ipc.h" header',
)
cdata.set('HAVE_IPC', ipc_opt.allowed())

lfs_enabled = cdata.get('HAVE_OPEN64')
cdata.set('ENABLE_LARGEFILE', lfs_enabled)

nls_opt = get_option('nls')
cdata.set('ENABLE_NLS', nls_opt.allowed())

splice_opt = get_option(
  'splice',
).require(
  cdata.get('HAVE_SPLICE'),
  error_message: 'need "splice" system call',
)
cdata.set('HAVE_SPLICE', splice_opt.allowed())

summary(
  {
    'IPC messaging'          : ipc_opt.allowed(),
    'Large-file support'     : lfs_enabled,
    'Native language support': nls_opt.allowed(),
    'Use splice system call' : splice_opt.allowed(),
  },
  bool_yn: true,
)

foreach _flag : cdata.keys()
  cdata.set(_flag, cdata.get(_flag) ? 1 : false)
endforeach

cdata.set_quoted('LOCALEDIR', get_option('prefix') / get_option('localedir'))
cdata.set_quoted('PROGRAM_NAME', meson.project_name())
cdata.set_quoted('PACKAGE', meson.project_name())
cdata.set_quoted('VERSION', meson.project_version())

subdir('src/include')

pv = executable(
  'pv',
  [
    config_h,
    'src/library/getopt.c',
    'src/library/gettext.c',
    'src/main/debug.c',
    'src/main/help.c',
    'src/main/main.c',
    'src/main/options.c',
    'src/main/remote.c',
    'src/main/version.c',
    'src/pv/cursor.c',
    'src/pv/display.c',
    'src/pv/file.c',
    'src/pv/loop.c',
    'src/pv/number.c',
    'src/pv/signal.c',
    'src/pv/state.c',
    'src/pv/transfer.c',
    'src/pv/watchpid.c',
  ],
  c_args: feature_macros + '-DHAVE_CONFIG_H',
  implicit_include_directories: false,
  include_directories: 'src/include',
  install: true,
)

if get_option('tests').allowed()
  runtests = find_program('autoconf/scripts/run-test.sh')
  foreach _test : [
    '000-cat',
    '001-interval',
    '002-rate',
    '003-progress',
    '004-timer',
    '005a-eta',
    '005b-fineta',
    '006-ratecount',
    '007-bytes',
    '008-numeric',
    '009-quiet',
    '010-pipe',
    '011-cksum',
    '012-averagerate',
    '013-1mboundary',
    '014-1mboundary2',
    '015-cksumpipe',
    '016-numeric-timer',
    '017-numeric-bytes',
    '018-remote-format',
    '019-remote-cksum',
    '020-stop-at-size',
  ]
    test(
      _test,
      runtests,
      args: [
        pv,
        meson.project_source_root(),
        _test,
      ],
      is_parallel: _test != '018-remote-format',
    )
  endforeach
endif
