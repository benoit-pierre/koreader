project(
  'lunasvg',
  ['c', 'cpp'],
  version: '2.3.8',
  default_options: ['c_std=c11', 'cpp_std=c++11'],
)

if get_option('default_library') == 'static'
  add_project_arguments('-DLUNASVG_STATIC_DEFINE', language: ['c', 'cpp'])
endif

subdir('include')
subdir('source')
subdir('xtended')
subdir('3rdparty/plutovg')

lunasvg_includes = include_directories(
  'include',
  'source',
  'xtended',
  '3rdparty/plutovg',
)

lunasvg_lib = library(
  'lunasvg',
  gnu_symbol_visibility: 'inlineshidden',
  include_directories: lunasvg_includes,
  install: true,
  sources: [lunasvg_export_h, lunasvg_sources, xtended_sources, plutovg_sources],
)

lunasvg_dep = declare_dependency(
  include_directories: include_directories('include', 'xtended'),
  link_with: lunasvg_lib,
)

meson.override_dependency('lunasvg', lunasvg_dep)
