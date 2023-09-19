generated_headers += custom_target(
  'generated_headers',
  input: resource_names,
  output: ['name-table.h', 'pdf-name-table.h'],
  command: [namedump_exe, '@INPUT@', '@OUTPUT0@', '@OUTPUT1@'],
  install: true,
  install_dir: [get_option('includedir') / 'mupdf/pdf', false],
  install_tag: 'devel',
)
