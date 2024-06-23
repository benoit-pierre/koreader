project(
  'libk2pdfopt',
  ['c', 'cpp'],
  version: '2.55',
  meson_version: '>= 0.59.0',
)

cc = meson.get_compiler('c')

dependencies = [
  dependency('lept'),
  dependency('tesseract'),
  cc.find_library('m', required: false),
  dependency('threads'),
]

if host_machine.system() == 'android'
  dependencies += cc.find_library('log')
endif

if host_machine.system() == 'windows'
  dependencies += cc.find_library('shlwapi')
endif

cflags = [
  '-DBUILDING_LIBK2PDFOPT',
  '-DNO_FILELIST',
]

if not get_option('legacy-engine')
  cflags += '-DDISABLED_LEGACY_ENGINE'
endif

# Fix crash when compiling with LTO enabled.
cflags += cc.get_supported_arguments('-fno-strict-aliasing')

includes = include_directories(
  'lib',
  'k2pdfoptlib',
  'willuslib',
)

sources = [
  'lib/koptcrop.c',
  'lib/koptimize.c',
  'lib/koptocr.c',
  'lib/koptreflow.c',
  'lib/setting.c',
  'lib/tesscapi.cpp',
  'k2pdfoptlib/bmpregion.c',
  'k2pdfoptlib/devprofile.c',
  'k2pdfoptlib/k2bmp.c',
  'k2pdfoptlib/k2file.c',
  'k2pdfoptlib/k2master.c',
  'k2pdfoptlib/k2mem.c',
  'k2pdfoptlib/k2ocr.c',
  'k2pdfoptlib/k2proc.c',
  'k2pdfoptlib/k2settings.c',
  'k2pdfoptlib/k2sys.c',
  'k2pdfoptlib/k2version.c',
  'k2pdfoptlib/pagelist.c',
  'k2pdfoptlib/pageregions.c',
  'k2pdfoptlib/textrows.c',
  'k2pdfoptlib/textwords.c',
  'k2pdfoptlib/wrapbmp.c',
  'willuslib/ansi.c',
  'willuslib/array.c',
  'willuslib/bmp.c',
  'willuslib/filelist.c',
  'willuslib/gslpolyfit.c',
  'willuslib/linux.c',
  'willuslib/math.c',
  'willuslib/mem.c',
  'willuslib/ocr.c',
  'willuslib/ocrgocr.c',
  'willuslib/ocrtess.c',
  'willuslib/ocrwords.c',
  'willuslib/strbuf.c',
  'willuslib/string.c',
  'willuslib/token.c',
  'willuslib/wfile.c',
  'willuslib/wgui.c',
  'willuslib/willusversion.c',
  'willuslib/wleptonica.c',
  'willuslib/wsys.c',
]

if get_option('buildtype') == 'debug'
  cflags += '-DK2PDFOPT_DEBUG'
  sources += [
    'k2pdfoptlib/k2mark.c',
    'willuslib/dtcompress.c',
    'willuslib/pdffonts.c',
    'willuslib/pdfwrite.c',
    'willuslib/wpdfoutline.c',
    'willuslib/wpdfutil.c',
  ]
endif

k2pdfopt_lib = library(
  'k2pdfopt',
  sources,
  c_args: cflags,
  cpp_args: cflags,
  dependencies: dependencies,
  gnu_symbol_visibility: 'inlineshidden',
  implicit_include_directories: false,
  include_directories: includes,
  install: true,
  soversion: 2,
)

k2pdfopt_dep = declare_dependency(
  dependencies: [dependency('lept').partial_dependency(includes: true)],
  include_directories: includes,
  link_with: k2pdfopt_lib,
)

meson.override_dependency('k2pdfopt', k2pdfopt_dep)

install_headers(
  [
    'lib/context.h',
    'lib/koptcrop.h',
    'lib/koptimize.h',
    'lib/koptocr.h',
    'lib/koptreflow.h',
    'lib/leptonica.h',
    'lib/setting.h',
    'lib/tesseract.h',
    'k2pdfoptlib/k2pdfopt.h',
    'willuslib/willus.h',
  ],
  install_dir: get_option('includedir') / 'k2pdfopt',
)

import('pkgconfig').generate(k2pdfopt_lib, subdirs: 'k2pdfopt')
