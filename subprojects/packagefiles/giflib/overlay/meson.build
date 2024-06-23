project('giflib', 'c', version: '5.1.4')

cc = meson.get_compiler('c')

foreach _hdr : [
  'limits.h',
  'fcntl.h',
  'stdint.h',
  'stdarg.h',
]
  cc.has_header(_hdr, required: true)
endforeach

gif_lib = library(
  'gif',
  [
    'lib/dgif_lib.c',
    'lib/egif_lib.c',
    'lib/gif_err.c',
    'lib/gif_font.c',
    'lib/gif_hash.c',
    'lib/gifalloc.c',
    'lib/openbsd-reallocarray.c',
    'lib/quantize.c',
  ],
  # For `S_IREAD` & `S_IWRITE` in `sys/stat.h`.
  c_args: '-D_GNU_SOURCE',
  install: true,
  version: '7',
)

gif_dep = declare_dependency(
  include_directories: include_directories('lib'),
  link_with: gif_lib,
)

meson.override_dependency('giflib', gif_dep)
