project(
  'cpr',
  'cpp',
  license: 'MIT',
  version: files('VERSION'),
  default_options: 'cpp_std=c++14',
)

curl_dep = dependency('libcurl')
thread_dep = dependency('threads')

includes = include_directories('include')
subdir('cpr')

cpr_lib = library(
  'cpr',
  cpr_src,
  dependencies: [curl_dep, thread_dep],
  include_directories: includes,
  install: true,
)

cpr_dep = declare_dependency(
  include_directories: includes,
  link_with: cpr_lib,
)

meson.override_dependency('cpr', cpr_dep)
