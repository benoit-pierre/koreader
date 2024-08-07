shared_library(
  'sevunzip',
  [
    'un7zip/7zAssetFile.cpp',
    'un7zip/7zExtractor.cpp',
    'un7zip/7zFunctions.cpp',
    'un7z.cpp',
  ],
  dependencies: [android_dep, log_dep, sevenzip_dep],
  include_directories: include_directories('un7zip'),
  install: true,
)
