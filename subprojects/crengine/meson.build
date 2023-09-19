project(
  'crengine',
  ['c', 'cpp'],
  version: '3.0.57-ko',
  default_options: 'cpp_std=c++17',
)

cc = meson.get_compiler('c')
cpp = meson.get_compiler('cpp')
fs = import('fs')

freetype_dep    = dependency('freetype2')
fribidi_dep     = dependency('fribidi', required: get_option('fribidi'))
harfbuzz_dep    = dependency('harfbuzz')
jpeg_dep        = dependency('libjpeg')
png_dep         = dependency('libpng')
srell_dep       = dependency('srell', required: get_option('srell_regex'))
utf8proc_dep    = dependency('libutf8proc', required: get_option('utf8proc'))
zlib_dep        = dependency('zlib')
zstd_dep        = dependency('libzstd', required: get_option('zstd'))

# WebP support: need both decode and demux libraries.
webp_dep = declare_dependency()
webp_opt = get_option('webp')
foreach _depname : [
  ['libwebpdecoder', 'libwebp'],
  'libwebpdemux',
]
  webp_dep = declare_dependency(
    dependencies: [
      webp_dep,
      dependency(_depname, disabler: true, required: webp_opt),
    ],
  )
  webp_opt = webp_opt.require(webp_dep.found())
endforeach
if webp_opt.disabled()
  webp_dep = dependency('', required: false)
endif

# Ensure libunibreak has the required `lb_get_char_class` support.
unibreak_opt = get_option('libunibreak')
unibreak_dep = dependency('libunibreak', disabler: true, required: unibreak_opt)
unibreak_opt = unibreak_opt.require(
  cc.has_header_symbol(
    'linebreakdef.h',
    'lb_get_char_class',
    dependencies: unibreak_dep.partial_dependency(
      compile_args: true,
      includes: true,
    ),
    required: unibreak_opt,
  ),
)
if unibreak_opt.disabled()
  unibreak_dep = dependency('', required: false)
endif

err_msg = 'the lunasvg and nanosvg features are mutually exclusive'
lunasvg_dep = dependency(
  'lunasvg',
  required: get_option(
    'lunasvg',
  ).require(
    not get_option('nanosvg').enabled(),
    error_message: err_msg,
  ),
)
nanosvg_dep = dependency(
  'nanosvg',
  required: get_option(
    'nanosvg',
  ).require(
    not lunasvg_dep.found(),
    error_message: err_msg,
  ),
)
if lunasvg_dep.found()
  svg_support = 'lunasvg'
elif nanosvg_dep.found()
  svg_support = 'nanosvg'
else
  svg_support = ''
endif

if get_option('debug')
  debug_flags = ['-D_DEBUG=1', '-DDEBUG=1']
else
  debug_flags = []
endif

subdir('crengine')

crengine_flags = [
  '-DALLOW_KERNING=1',
  '-DCR_EMULATE_GETTEXT=1',
  '-DMATHML_SUPPORT=1',
  '-DUSE_FONTCONFIG=0',
]

crengine_flags += cpp.get_supported_arguments(
  '-Wno-reorder',
  '-ftabstop=4',
)

foreach _flag, _dep : {
  'USE_FRIBIDI'    : fribidi_dep,
  'USE_LIBUNIBREAK': unibreak_dep,
  'USE_LIBWEBP'    : webp_dep,
  'USE_LUNASVG'    : lunasvg_dep,
  'USE_NANOSVG'    : nanosvg_dep,
  'USE_SRELL_REGEX': srell_dep,
  'USE_UTF8PROC'   : utf8proc_dep,
  'USE_ZSTD'       : zstd_dep,
}
  crengine_flags += '-D@0@=@1@'.format(_flag, _dep.found() ? '1' : '0')
endforeach

if (
  8 == meson.get_compiler('cpp').sizeof(
    'off64_t',
    args: '-D_LARGEFILE64_SOURCE=1',
    prefix: '#include<<sys/types.h>',
  )
)
  crengine_flags += [
    '-D_LARGEFILE64_SOURCE=1',
    '-DHAVE_STAT64=1',
  ]
endif

if host_machine.system() != 'windows'
  crengine_flags += [
    '-DLINUX',
    '-D_LINUX=1',
  ]
endif

if get_option('antiword').allowed()
  crengine_flags += [
    '-DENABLE_ANTIWORD=1',
  ]
  subdir('thirdparty/antiword')
else
  crengine_flags += [
    '-DENABLE_ANTIWORD=0',
  ]
  antiword_dep = declare_dependency()
endif

if get_option('chmlib').allowed()
  crengine_flags += [
    '-DCHM_SUPPORT_ENABLED=1',
  ]
  subdir('thirdparty/chmlib')
else
  crengine_flags += [
    '-DCHM_SUPPORT_ENABLED=0',
  ]
  chmlib_dep = declare_dependency()
endif

crengine_lib = library(
  'crengine',
  crengine_sources,
  cpp_args: debug_flags + crengine_flags,
  dependencies: [
    antiword_dep,
    chmlib_dep,
    freetype_dep,
    fribidi_dep,
    harfbuzz_dep,
    jpeg_dep,
    lunasvg_dep,
    nanosvg_dep,
    png_dep,
    srell_dep,
    unibreak_dep,
    utf8proc_dep,
    webp_dep,
    zlib_dep,
    zstd_dep,
  ],
  gnu_symbol_visibility: 'inlineshidden',
  include_directories: crengine_includes,
  install: true,
)

crengine_dep = declare_dependency(
  compile_args: crengine_flags,
  link_with: crengine_lib,
  include_directories: crengine_includes,
  dependencies: [
    # Some public headers include harfbuzz and/or unibreak headers.
    harfbuzz_dep.partial_dependency(compile_args: true, includes: true),
    unibreak_dep.partial_dependency(compile_args: true, includes: true),
  ],
)

meson.override_dependency('crengine', crengine_dep)

_toinstall = []

foreach _data : [
  # Directories.
  'hyph/',
  # Files.
  'NOTES.txt',
  'chm.css',
  'cr3.css',
  'dict.css',
  'doc.css',
  'epub.css',
  'fb2.css',
  'htm.css',
  'html5.css',
  'rtf.css',
  'txt.css',
]
  _toinstall += 'cr3gui/data' / _data
endforeach

foreach _data : _toinstall
  if get_option('install_symlinks')
    install_symlink(
      fs.name(_data),
      install_dir: 'data',
      install_tag: 'runtime',
      pointing_to: meson.project_source_root() / _data,
    )
  else
    if fs.is_file(_data)
      install_data(
        _data,
        install_dir: 'data',
        install_tag: 'runtime',
      )
    else
      install_subdir(
        _data,
        install_dir: 'data',
        install_tag: 'runtime',
      )
    endif
  endif
endforeach

summary(
  {
    'FriBidi'    : fribidi_dep.found(),
    'libunibreak': unibreak_dep.found(),
    'SRELL regex': srell_dep.found(),
    'SVG'        : svg_support == '' ? false: svg_support,
    'utf8proc'   : utf8proc_dep.found(),
    'WebP'       : webp_dep.found(),
    'ZSTD'       : zstd_dep.found(),
  },
  bool_yn: true,
  section: 'Features',
)
