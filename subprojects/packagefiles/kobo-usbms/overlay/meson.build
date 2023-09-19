project('kobo-usbms', 'c', version: '1.3.8')

cc = meson.get_compiler('c')
git = find_program('git')

evdev_dep = dependency('libevdev')
fbink_dep = dependency('fbink')
rt_dep = cc.find_library('rt', required: false)

out = run_command(
  [
    git,
    '-C', meson.project_source_root(),
    'show', '--no-patch', '--format=%ct%n%ci',
  ],
  capture: true,
  check: true,
).stdout().strip().split('\n')
epoch = out[0]
timestamp = out[1]

usbms = executable(
  'usbms',
  [
    'openssh/atomicio.c',
    'usbms.c',
  ],
  c_args: [
    '-DUSBMS_TIMESTAMP="@0@"'.format(timestamp),
    '-DUSBMS_VERSION="@0@"'.format(meson.project_version()),
  ],
  dependencies: [
    evdev_dep,
    fbink_dep,
    rt_dep,
  ],
)

custom_target(
  'kobo-usbms-tgz',
  build_by_default: true,
  command: [
    'kobo-usbms-tgz.py',
    epoch,
    '@OUTPUT@',
    '@CURRENT_SOURCE_DIR@',
    '@INPUT@',
    meson.current_source_dir() / 'l10n',
  ],
  depends: usbms,
  install: true,
  install_dir: 'data',
  install_tag: 'runtime',
  input: [
    'resources/fonts/CaskaydiaCove_NF.otf',
    'resources/img/koreader.png',
    'scripts/end-usbms.sh',
    'scripts/fuser-check.sh',
    'scripts/start-usbms.sh',
    usbms,
  ],
  output: 'KoboUSBMS.tar.gz',
)
