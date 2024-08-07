project(
  'zsync2',
  ['c', 'cpp'],
  version: '2020.06.02',
)

langs = ['c', 'cpp']

cc = meson.get_compiler('c')
cxx = meson.get_compiler('cpp')
foreach _lang : langs
  _comp = meson.get_compiler(_lang)
  set_variable(_lang, _comp)
  add_project_arguments(
    [
      _comp.get_supported_arguments(
        '-Wno-unused-function',
        '-Wno-reorder',
      ),
      '-D_XOPEN_SOURCE=500',
      '-DHAVE_CONFIG_H',
    ],
    language: _lang,
  )
endforeach

if host_machine.system() == 'linux'
  add_project_arguments('-DAPPIMAGEUPDATE_LINUX', language: langs)
endif

if _comp.has_function_attribute('visibility:hidden')
  add_project_arguments('-DHAVE_HIDDEN', language: langs)
endif

args_dep = dependency('args')
cpr_dep = dependency('cpr')
curl_dep = dependency('libcurl')
# Yes, 2 zlibs are used ¯\_(ツ)_/¯
zlib_dep = dependency('zlib')

foreach _header : [
  'inttypes.h',
  'stdarg.h',
  'unistd.h',
]
  if cc.has_header(_header)
    add_project_arguments('-DHAVE_' + _header.underscorify().to_upper() + '=1', language: langs)
  endif
endforeach

foreach _function : [
  'memcpy',
  'pwrite',
  'vsnprintf',
]
  if cc.has_function(_function)
    add_project_arguments('-DHAVE_' + _function.underscorify().to_upper() + '=1', language: langs)
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

zsync_src = []

foreach _prefix, _filelist : {
  'lib/hashlib': [
    'src/crc32.cpp',
    'src/keccak.cpp',
    'src/md5.cpp',
    'src/sha1.cpp',
    'src/sha256.cpp',
    'src/sha3.cpp',
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
    zsync_src += files(_prefix / _file)
  endforeach
endforeach

zsync_deps = [
  args_dep,
  cpr_dep,
  curl_dep,
  zlib_dep,
]

zsync_incs = include_directories(
  'lib/hashlib/include',
  'lib/librcksum',
  'lib/libzsync',
  'lib',
  'include',
)

executable(
  'zsync2',
  [zsync_src, 'src/main.cpp'],
  cpp_args: '-DZSYNC_STANDALONE',
  dependencies: zsync_deps,
  include_directories: zsync_incs,
  install: true,
)

if get_option('zsyncmake2').allowed()
  executable(
    'zsyncmake2',
    [zsync_src, 'src/make_main.cpp'],
    dependencies: zsync_deps,
    include_directories: zsync_incs,
    install: true,
  )
endif

tests_opt = get_option('tests')
gtest_dep = dependency('gtest', required: tests_opt)
tests_opt = tests_opt.require(gtest_dep.found())
if tests_opt.allowed()
  test_exe = executable(
    'test-zsutil',
    'tests/test_zsutil.cpp',
    dependencies: [cpr_dep, gtest_dep],
    include_directories: include_directories('include'),
  )
  test('test-zsutil', test_exe)
endif
