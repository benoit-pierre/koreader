project('tessdata', version: '4.1.0')

if get_option('install_symlinks')
  install_symlink(
    'eng.traineddata',
    install_dir: 'data/tessdata',
    pointing_to: meson.project_source_root() / 'build/tessdata/eng.traineddata',
  )
else
  install_data(
    'build/tessdata/eng.traineddata',
    install_dir: 'data/tessdata',
    install_tag: 'runtime',
  )
endif
