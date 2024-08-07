project('7z', 'c', version: '19.00')

flags = []
src = [
  'C/7zAlloc.c',
  'C/7zArcIn.c',
  'C/7zBuf.c',
  'C/7zCrc.c',
  'C/7zCrcOpt.c',
  'C/7zDec.c',
  'C/7zFile.c',
  'C/7zStream.c',
  'C/Bcj2.c',
  'C/Bra.c',
  'C/Bra86.c',
  'C/BraIA64.c',
  'C/CpuArch.c',
  'C/Delta.c',
  'C/LzmaDec.c',
]

if get_option('lzma2')
  src += 'C/Lzma2Dec.c'
else
  flags += '-D_7Z_NO_METHOD_LZMA2'
endif

zstd_dep = dependency('libzstd', required: get_option('zstd'))
if zstd_dep.found()
  flags += '-D_7ZIP_ZSTD_SUPPPORT'
endif

sevenzip_lib = library(
  '7z',
  src,
  c_args: flags,
  dependencies: zstd_dep,
  gnu_symbol_visibility: 'hidden',
  install: true,
)

sevenzip_dep = declare_dependency(
  include_directories: include_directories('C'),
  link_with: sevenzip_lib,
)

meson.override_dependency('lib7z', sevenzip_dep)
