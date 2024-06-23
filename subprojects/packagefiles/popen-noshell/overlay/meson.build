project('popen-noshell', 'c', version: '2015.06.15')

popen_noshell_lib = library('popen_noshell',
  install: true,
  sources: files('popen_noshell.c'),
  )

popen_noshell_dep = declare_dependency(
  include_directories: include_directories('.'),
  link_with: popen_noshell_lib
  )

meson.override_dependency('libpopen_noshell', popen_noshell_dep)
