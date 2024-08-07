project('luajit-launcher', ['c', 'cpp'])

cc = meson.get_compiler('c')

foreach _lib : ['android', 'dl', 'log', 'm']
  set_variable(_lib + '_dep', cc.find_library(_lib))
endforeach

lua_dep = dependency('luajit')
sevenzip_dep = dependency('lib7z')

subdir('jni')
