subdir('android_native_app_glue')
subdir('ioctl')
subdir('lzma')

shared_library(
  'luajit-launcher',
  [
    'jni_helper.c',
    'main.c',
  ],
  dependencies: [android_dep, dl_dep, log_dep, lua_dep, m_dep],
  include_directories: include_directories('android_native_app_glue'),
  install: true,
  link_whole: android_native_app_glue_lib,
)
