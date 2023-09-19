project(
  'lodepng',
  'c',
  version: '2023.04.10',
)

lodepng_lib = library(
  'lodepng',
  configure_file(
    copy: true,
    input: 'lodepng.cpp',
    output: 'lodepng.c',
  ),
  gnu_symbol_visibility: 'hidden',
  install: true,
)

lodepng_dep = declare_dependency(
  include_directories: include_directories('.'),
  link_with: lodepng_lib,
)

meson.override_dependency('lodepng', lodepng_dep)

install_headers('lodepng.h')

pkg = import('pkgconfig')
pkg.generate(lodepng_lib)
