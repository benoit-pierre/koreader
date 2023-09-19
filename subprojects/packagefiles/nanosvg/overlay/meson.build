project('nanosvg', 'c', version: '2022.12.04')

cc = meson.get_compiler('c')

m_dep = cc.find_library('m', required: false)

nanosvg_dep = declare_dependency(
  dependencies: [m_dep],
  include_directories: include_directories('src'),
  )

meson.override_dependency('nanosvg', nanosvg_dep)
