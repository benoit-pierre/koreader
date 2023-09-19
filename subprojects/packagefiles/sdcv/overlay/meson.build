project(
  'sdcv',
  'cpp',
  version: '0.5.4',
  default_options: 'cpp_std=c++11',
)

cxx = meson.get_compiler('cpp')

glib_dep = dependency('glib-2.0', version: '>=2.36')
zlib_dep = dependency('zlib')

cdata = configuration_data()

if cxx.has_header('locale.h')
  cdata.set('HAVE_LOCALE_H', 1)
endif

if cxx.has_function('mmap')
  cdata.set('HAVE_MMAP', 1)
endif

config_h = configure_file(
  input: 'config.h.cmake',
  output: 'config.h',
  format: 'cmake@',
  configuration: cdata,
)

executable(
  'sdcv',
  [
    config_h,
    'src/dictziplib.cpp',
    'src/dictziplib.hpp',
    'src/distance.cpp',
    'src/distance.hpp',
    'src/libwrapper.cpp',
    'src/libwrapper.hpp',
    'src/mapfile.hpp',
    'src/readline.cpp',
    'src/readline.hpp',
    'src/sdcv.cpp',
    'src/stardict_lib.cpp',
    'src/stardict_lib.hpp',
    'src/utils.cpp',
    'src/utils.hpp',
  ],
  cpp_args: [
    '-DHAVE_CONFIG_H',
    '-DVERSION="@0@"'.format(meson.project_version()),
  ],
  dependencies: [glib_dep, zlib_dep],
  install: true,
)
