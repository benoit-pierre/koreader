project('leptonica', 'c', version: '1.74.1')

cc = meson.get_compiler('c')

c_args = ['-DHAVE_CONFIG_H']
dependencies = [cc.find_library('m', required: false)]

cdata = configuration_data()

foreach _feat, _lib : {
  'libjp2k': 'libopenjp2',
  'libjpeg': 'libjpeg',
  'libpng' : 'libpng',
  'libtiff': 'libtiff-4',
  'libwebp': 'libwebp',
  'libz'   : 'zlib',
}
  _dep = dependency(_lib, required: get_option(_feat))
  if _dep.found()
    cdata.set('HAVE_' + _feat.underscorify().to_upper(), 1)
  endif
  dependencies += [_dep]
endforeach

_dep = cc.find_library(
  'gif',
  has_headers: 'gif_lib.h',
  required: get_option('libgif'),
)
if _dep.found()
  cdata.set('HAVE_LIBGIF', 1)
endif
dependencies += _dep

if cdata.has('HAVE_LIBJP2K')
  c_args += '-DLIBJP2K_HEADER="openjpeg.h"'
endif

if cc.has_header('stdio.h') and cc.has_function('fmemopen')
  cdata.set('HAVE_FMEMOPEN', 1)
endif

config_h = configure_file(
  output: 'config_auto.h',
  configuration: cdata,
)

subdir('src')

lept_lib = library(
  'lept',
  c_args: c_args,
  dependencies: dependencies,
  gnu_symbol_visibility: 'hidden',
  include_directories: include_directories('src'),
  install: true,
  soversion: 5,
  sources: [config_h, endiannes_h] + lept_src,
)

lept_dep = declare_dependency(
  include_directories: include_directories('src'),
  link_with: lept_lib,
)

meson.override_dependency('lept', lept_dep)

install_headers(
  lept_hdr,
  subdir: 'leptonica',
)

pkg = import('pkgconfig')
pkg.generate(lept_lib, subdirs: 'leptonica')
