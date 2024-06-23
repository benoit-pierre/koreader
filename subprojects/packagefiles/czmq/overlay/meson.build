project('czmq', 'c', version: '3.0.0')

cc = meson.get_compiler('c')

compile_args = []
dependencies = [dependency('libzmq'), dependency('threads')]

if host_machine.system() == 'windows'
  dependencies += [
    cc.find_library('ws2_32'),
    cc.find_library('rpcrt4'),
    cc.find_library('iphlpapi'),
  ]
endif

# We still need to be able to export
# the symbols we need when almalgaming.
if false and get_option('default_library') == 'static'
  # compile_args += '-DLIBCZMQ_STATIC'
else
  compile_args += '-DLIBCZMQ_EXPORTS'
endif

cdata = configuration_data()

foreach _header : [
  'linux/wireless.h',
  'net/if.h',
  'net/if_media.h',
]
  if cc.has_header(_header)
    cdata.set('HAVE_' + _header.underscorify().to_upper(), 1)
  endif
endforeach

foreach _function : [
  'getifaddrs',
  'freeifaddrs',
]
  if cc.has_function(_function)
    cdata.set('HAVE_' + _function.underscorify().to_upper(), 1)
  endif
endforeach

platform_h = configure_file(
  output: 'platform.h',
  configuration: cdata,
)

czmq_lib = library(
  'czmq',
  [
    platform_h,
    'src/zactor.c',
    'src/zauth.c',
    'src/zbeacon.c',
    'src/zcert.c',
    'src/zcertstore.c',
    'src/zchunk.c',
    'src/zclock.c',
    'src/zconfig.c',
    'src/zctx.c',
    'src/zdigest.c',
    'src/zdir.c',
    'src/zdir_patch.c',
    'src/zfile.c',
    'src/zframe.c',
    'src/zgossip.c',
    'src/zgossip_msg.c',
    'src/zhash.c',
    'src/zlist.c',
    'src/zloop.c',
    'src/zmonitor.c',
    'src/zmsg.c',
    'src/zmutex.c',
    'src/zpoller.c',
    'src/zproxy.c',
    'src/zrex.c',
    'src/zsock.c',
    'src/zsock_monitor.c',
    'src/zsock_option.c',
    'src/zsocket.c',
    'src/zsockopt.c',
    'src/zstr.c',
    'src/zsys.c',
    'src/zthread.c',
    'src/zuuid.c',
  ],
  c_args: compile_args,
  dependencies: dependencies,
  gnu_symbol_visibility: 'inlineshidden',
  install: true,
  soversion: 1,
)

czmq_dep = declare_dependency(
  compile_args: compile_args,
  include_directories: include_directories('include'),
  link_with: czmq_lib,
)

meson.override_dependency('libczmq', czmq_dep)

install_subdir(
  'include',
  install_dir: get_option('includedir'),
  install_tag: 'devel',
  strip_directory: true,
)

pkg = import('pkgconfig')
pkg.generate(czmq_lib)
