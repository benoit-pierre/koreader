project('dropbear', 'c', version: '2024.85')

cc = meson.get_compiler('c')

bsd_dep = cc.find_library('bsd', required: false)
crypt_dep = cc.find_library('crypt', required: false)
util_dep = cc.find_library('util', required: false)
zlib_dep = dependency('zlib', required: get_option('zlib'))

cdata = configuration_data()

cdata.set('BUNDLED_LIBTOM', 1)
cdata.set('DISABLE_LASTLOG', 1)
cdata.set('DISABLE_PAM', 1)

if not zlib_dep.found()
  cdata.set('DISABLE_ZLIB', 1)
endif

default_includes = []

foreach _header : [
  'crypt.h',
  'inttypes.h',
  'lastlog.h',
  'libgen.h',
  'libutil.h',
  'linux/pkt_sched.h',
  'netdb.h',
  'netinet/in.h',
  'netinet/in_systm.h',
  'netinet/tcp.h',
  'pam/pam_appl.h',
  'paths.h',
  'pty.h',
  'security/pam_appl.h',
  'stddef.h',
  'stdint.h',
  'stdio.h',
  'stdlib.h',
  'string.h',
  'strings.h',
  'stropts.h',
  'sys/prctl.h',
  'sys/random.h',
  'sys/socket.h',
  'sys/stat.h',
  'sys/types.h',
  'sys/uio.h',
  'unistd.h',
  'util.h',
  'utmp.h',
  'utmpx.h',
]
  if cc.has_header(_header)
    cdata.set('HAVE_' + _header.underscorify().to_upper(), 1)
    default_includes += f'#include<@_header@>'
  endif
endforeach

default_includes = '\n'.join(default_includes)

cdata.set(
  'HAVE_CRYPT',
  cc.has_function('crypt', dependencies: crypt_dep) ? 1 : false,
)

cdata.set(
  'HAVE_LOGIN',
  cc.has_function('login', dependencies: [bsd_dep, util_dep]) ? 1 : false,
)

cdata.set(
  'HAVE_OPENPTY',
  get_option('openpty').disable_auto_if(
    not cc.has_function('openpty', dependencies: [util_dep]),
  ).allowed() ? 1 : false,
)

cdata.set(
  'HAVE_SHADOW_H',
  get_option(
    'shadow',
  ).require(
    cc.has_header('shadow.h'),
    error_message: 'missing shadow.h header',
  ).allowed() ? 1 : false,
)

foreach _function : [
  '_getpty',
  'basename',
  'clearenv',
  'clock_gettime',
  'daemon',
  'endutent',
  'endutxent',
  'explicit_bzero',
  'fexecve',
  'fork',
  'freeaddrinfo',
  'gai_strerror',
  'getaddrinfo',
  'getgrouplist',
  'getnameinfo',
  'getrandom',
  'getusershell',
  'logout',
  'logwtmp',
  'memset_s',
  'pututline',
  'pututxline',
  'setutent',
  'setutxent',
  'strlcat',
  'strlcpy',
  'writev',
]
  if cc.has_function(_function)
    cdata.set('HAVE_' + _function.underscorify().to_upper(), 1)
  endif
endforeach

if cdata.get('HAVE_GAI_STRERROR', 1) == 1 and cc.compiles(
  '''
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netdb.h>

  const char *gai_strerror(int);

  int main(void) {
    char *str;

    str = gai_strerror(0);
    return 0;
  }
  '''
  )
  cdata.set('HAVE_CONST_GAI_STRERROR_PROTO', 1)
endif

foreach _type : [
  'uint8_t',
  'u_int8_t',
  'uint16_t',
  'u_int16_t',
  'uint32_t',
  'u_int32_t',
]
  if cc.has_type(_type, prefix: default_includes)
    cdata.set('HAVE_' + _type.underscorify().to_upper(), 1)
  endif
endforeach

foreach _type : [
  'struct addrinfo',
  'struct in6_addr',
  'struct sockaddr_in6',
  'struct sockaddr_storage',
]
  if cc.sizeof(_type, prefix: default_includes) > 0
    cdata.set('HAVE_' + _type.underscorify().to_upper(), 1)
  endif
endforeach

foreach _struct, _member_list : {
  'utmp': [
    'ut_host',
    'ut_pid',
    'ut_type',
    'ut_tv',
    'ut_id',
    'ut_addr',
    'ut_addr_v6',
    'ut_exit',
    'ut_time',
  ],
  'utmpx': [
    'ut_host',
    'ut_syslen',
    'ut_type',
    'ut_id',
    'ut_addr',
    'ut_addr_v6',
    'ut_time',
    'ut_tv',
  ],
  'sockaddr_storage': ['ss_family'],
}
  foreach _member : _member_list
    if cc.compiles(
      '''
    @0@
    int main(void) {
      char t[sizeof (((struct @1@ *)0)->@2@) >= 0 ? 1 : -1];
      return 0;
      }
    '''.format(default_includes, _struct, _member),
      name: f'has `struct @_struct@.@_member@` member',
    )
      cdata.set(
        'HAVE_STRUCT_' + (_struct + '_' + _member).underscorify().to_upper(),
        1,
      )
    endif
  endforeach
endforeach

config_h = configure_file(
  output: 'config.h',
  configuration: cdata,
)

cdata = configuration_data()

cdata.set('DEBUG_TRACE', get_option('debug_trace'))
cdata.set('DEFAULT_PATH', get_option('default_path'))
cdata.set('DISABLE_SYSLOG', get_option('disable_syslog'))
cdata.set_quoted('DROPBEAR_DEFPORT', get_option('default_port').to_string())
cdata.set10('DROPBEAR_SMALL_CODE', get_option('small_code'))
cdata.set10('DROPBEAR_X11FWD', get_option('x11_forwarding'))
cdata.set10('INETD_MODE', get_option('inetd_mode'))
cdata.set10('LOG_COMMANDS', get_option('log_commands'))

foreach _opt : [
  'default_path',
  'pidfile',
  'sftpserver_path',
  # Keys.
  'dss_priv_filename',
  'rsa_priv_filename',
  'ecdsa_priv_filename',
  'ed25519_priv_filename',
]
  _flag = _opt.to_upper()
  _value = get_option(_opt)
  if _value != ''
    cdata.set_quoted(_flag, _value)
  else
    cdata.set(_flag, false)
  endif
endforeach

default_options_h = configure_file(
  configuration: cdata,
  output: 'localoptions.h',
)

default_options_guard_h = custom_target(
  'default_options_guard_h',
  capture: true,
  feed: true,
  input: 'src/default_options.h',
  output: 'default_options_guard.h',
  command: [meson.project_source_root() / 'src/ifndef_wrapper.sh'],
)

dropbear_inc = include_directories('src')

subdir('libtommath')
subdir('libtomcrypt/src')
subdir('src')
