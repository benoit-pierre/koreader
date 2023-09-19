project('miniconv', 'c', default_options: 'c_std=c11')

cc = meson.get_compiler('c')

cdata = configuration_data()
cdata.set('__BIG_ENDIAN__', host_machine.endian() == 'big')
cdata.set('HAVE_STDINT_H', cc.has_header('stdint.h'))

config_h = configure_file(
  configuration: cdata,
  input: 'miniconv_config.meson.h',
  output: 'miniconv_config.h',
)

includes = ['.']
sources = [
  'miniconv.c',
  'precompile.c',
]

if get_option('alias_iconv')
  includes += ['libiconv']
  sources += ['libiconv/iconv.c']
endif

miniconv_lib = library(
  'miniconv',
  config_h,
  sources,
  install: true,
)

miniconv_dep = declare_dependency(
  include_directories: includes,
  link_with: miniconv_lib,
)

meson.override_dependency(get_option('alias_iconv') ? 'iconv' : 'miniconv', miniconv_dep)
