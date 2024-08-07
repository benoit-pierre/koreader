project('libaes', 'c')

aes_lib = library(
  'aes',
  [
    'aes/aescrypt.c',
    'aes/aeskey.c',
    'aes/aestab.c',
    'aes/fileenc.c',
    'aes/hmac.c',
    'aes/prng.c',
    'aes/pwd2key.c',
    'aes/sha1.c',
  ],
  install: true,
)

aes_dep = declare_dependency(
  include_directories: include_directories('.'),
  link_with: aes_lib,
)

meson.override_dependency('libaes', aes_dep)
