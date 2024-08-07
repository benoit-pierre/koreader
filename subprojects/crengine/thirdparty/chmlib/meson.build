chmlib_lib = static_library(
  'chmlib',
  [
    'src/chm_lib.c',
    'src/lzx.c',
  ],
  gnu_symbol_visibility: 'hidden',
  include_directories: 'src',
)

chmlib_dep = declare_dependency(
  link_with: chmlib_lib,
  include_directories: 'src',
)
