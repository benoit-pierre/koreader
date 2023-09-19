project(
  'zsync2',
  ['c', 'cpp'],
  version: '2020.06.02',
)

cc = meson.get_compiler('c')

cpr_dep = dependency('cpr')
curl_dep = dependency('libcurl')
# Yes, 2 zlibs are used ¯\_(ツ)_/¯
zlib_dep = dependency('zlib')

compile_args = ['-DHAVE_CONFIG_H']

if host_machine.system() == 'linux'
  compile_args += '-DAPPIMAGEUPDATE_LINUX'
endif

foreach _header : [
  'inttypes.h',
  # 'memory.h',
  'stdarg.h',
  'stdint.h',
  # 'stdlib.h',
  # 'strings.h',
  # 'sys/stat.h',
  # 'sys/types.h',
  # 'time.h',
  'unistd.h',
]
  if cc.has_header(_header)
    compile_args += '-DHAVE_' + _header.underscorify().to_upper() + '=1'
  endif
endforeach

foreach _function : [
  # 'fseeko',
  # 'getaddrinfo',
  'memcpy',
  # 'mkstemp',
  # 'pread',
  'pwrite',
]
  if cc.has_function(_function)
    compile_args += '-DHAVE_' + _function.underscorify().to_upper() + '=1'
  endif
endforeach

src = []

src += configure_file(
  configuration: {
    'PROJECT_NAME': meson.project_name(),
    'VERSION'     : meson.project_version(),
    'BUILD_NUMBER': '<KOReader>',
    'BUILD_DATE'  : '2020-06-02 02:31:28 +0200',
    'GIT_COMMIT'  : meson.project_version(),
  },
  input: 'src/config.h.in',
  output: 'config.h',
)

internal_libs = []
sources = []

foreach _prefix, _filelist : {
  'lib/hashlib/src': [
    'crc32.cpp',
    'digest.cpp',
    'keccak.cpp',
    'md5.cpp',
    'sha1.cpp',
    'sha256.cpp',
    'sha3.cpp',
  ],
  'lib/librcksum': [
    'hash.c',
    'md4.c',
    'range.c',
    'rsum.c',
    'state.c',
  ],
  'lib/libzsync': [
    'sha1.c',
    'zmap.c',
    'zsync.c',
  ],
  'lib/zlib': [
    'adler32.c',
    'compress.c',
    'crc32.c',
    'deflate.c',
    'gzio.c',
    'inflate.c',
    'inftrees.c',
    'trees.c',
    'zutil.c',
  ],
  'src': [
    'legacy_http.c',
    'legacy_progress.c',
    'zsclient.cpp',
    'zsmake.cpp',
  ],
}
  foreach _file : _filelist
    sources += files(_prefix / _file)
  endforeach
endforeach

executable(
  'zsync2',
  sources,
  c_args: compile_args,
  cpp_args: compile_args,
  dependencies: [cpr_dep, curl_dep, zlib_dep],
  include_directories: include_directories(
    'lib/hashlib/include',
    'lib/librcksum',
    'lib/libzsync',
    'lib',
    'include',
  ),
  install: true,
)

if get_option('tests')
  test_exe = executable(
    'test-zsutil',
    'tests/test_zsutil.cpp',
    dependencies: [cpr_dep, dependency('gtest')],
    include_directories: include_directories('include'),
  )
  test('test-zsutil', test_exe)
endif
