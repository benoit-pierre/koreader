project(
  'openssh-postable',
  'c',
  version: '8.6',
  default_options: 'c_std=gnu99',
)

cc = meson.get_compiler('c')
fs = import('fs')

security_key = get_option('security-key')

bsd_dep = dependency('libbsd', required: false)
crypt_dep = cc.find_library('crypt', required: false)
dl_dep = cc.find_library('dl', required: false)
fido_dep = dependency('libfido2', required: security_key == 'libfido2')
md_dep = cc.find_library('md', required: false)
nsl_dep = cc.find_library('nsl', required: false)
util_dep = cc.find_library('util', required: false)
resolv_dep = cc.find_library('resolv', required: false)
sk_dep = declare_dependency()
ssl_dep = dependency('openssl', required: get_option('openssl'))
zlib_dep = dependency('zlib', required: get_option('zlib'))

cdata = configuration_data()

foreach _attr : [
  # 'bounded',
  'nonnull',
  'sentinel',
]
  cdata.set('HAVE_ATTRIBUTE__@0@__'.format(_attr.to_upper()), cc.has_function_attribute(_attr) ? 1 : false)
endforeach

has_header = {}

partial_ssl_dep = ssl_dep.partial_dependency(compile_args: true, includes: true)

# Ulrix, ignore…
# ['sys/syslog.h'],
foreach c : [
  ['arpa/inet.h'],
  ['arpa/nameser.h'],
  ['blf.h'],
  ['bsd/libutil.h', bsd_dep],
  ['bsd/stdlib.h', bsd_dep],
  ['bsd/string.h', bsd_dep],
  ['bsm/audit.h'],
  ['bstring.h'],
  ['crypt.h'],
  ['crypto/sha2.h'],
  ['ctype.h'],
  ['dirent.h'],
  ['dlfcn.h'],
  ['dev/systrace.h'],
  ['elf.h'],
  ['endian.h'],
  ['err.h'],
  ['fcntl.h'],
  ['features.h'],
  ['floatingpoint.h'],
  ['fnmatch.h'],
  ['getopt.h'],
  ['glob.h'],
  ['grp.h'],
  ['gssapi.h'],
  ['gssapi/generic.h'],
  ['gssapi/gssapi.h'],
  ['gssapi/gssapi/generic.h'],
  ['gssapi/gssapi/krb5.h'],
  ['gssapi/krb5.h'],
  ['ia.h'],
  ['iaf.h'],
  ['ifaddrs.h'],
  ['inttypes.h'],
  ['langinfo.h'],
  ['lastlog.h'],
  ['libaudit.h'],
  ['libgen.h'],
  ['libproc.h'],
  ['libutil.h'],
  ['limits.h'],
  ['linux/audit.h'],
  ['linux/filter.h'],
  ['linux/if.h'],
  ['linux/if_tun.h'],
  ['linux/prctl.h'],
  ['linux/seccomp.h'],
  ['locale.h'],
  ['login.h'],
  ['login/cap.h'],
  ['maillock.h'],
  ['mount.h'],
  ['ndir.h'],
  ['net/if/tun.h'],
  ['net/route.h'],
  ['netdb.h'],
  ['netgroup.h'],
  ['netinet/in.h'],
  ['pam/pam/appl.h'],
  ['paths.h'],
  ['poll.h'],
  ['prot.h'],
  ['priv.h'],
  ['pty.h'],
  ['pwd.h'],
  ['pwdadj.h'],
  ['readpassphrase.h'],
  ['resolv.h'],
  ['rpc/types.h'],
  ['sandbox.h'],
  ['security/pam_appl.h'],
  ['sha2.h'],
  ['shadow.h'],
  ['signal.h'],
  ['stdarg.h'],
  ['stddef.h'],
  ['stdint.h'],
  ['stdio.h'],
  ['stdlib.h'],
  ['string.h'],
  ['strings.h'],
  ['sys/audit.h'],
  ['sys/bitypes.h'],
  ['sys/bsdtty.h'],
  ['sys/byteorder.h'],
  ['sys/capability.h'],
  ['sys/capsicum.h'],
  ['sys/cdefs.h'],
  ['sys/dir.h'],
  ['sys/file.h'],
  ['sys/label.h'],
  ['sys/mman.h'],
  ['sys/mount.h'],
  ['sys/ndir.h'],
  ['sys/param.h'],
  ['sys/poll.h'],
  ['sys/prctl.h'],
  ['sys/pstat.h'],
  ['sys/ptms.h'],
  ['sys/ptrace.h'],
  ['sys/random.h'],
  ['sys/resource.h'],
  ['sys/select.h'],
  ['sys/socket.h'],
  ['sys/stat.h'],
  ['sys/statfs.h'],
  ['sys/statvfs.h'],
  ['sys/stream.h'],
  ['sys/stropts.h'],
  ['sys/strtio.h'],
  ['sys/sysctl.h'],
  ['sys/sysmacros.h'],
  ['sys/time.h'],
  ['sys/timers.h'],
  ['sys/types.h'],
  ['sys/uio.h'],
  ['sys/un.h'],
  ['sys/vfs.h'],
  ['sys/wait.h'],
  ['termios.h'],
  ['time.h'],
  ['tmpdir.h'],
  ['ttyent.h'],
  ['ucred.h'],
  ['unistd.h'],
  ['usersec.h'],
  ['util.h'],
  ['utime.h'],
  ['utmp.h'],
  ['utmpx.h'],
  ['vis.h'],
  ['wchar.h'],
  # OpenSSL
  ['openssl/bn.h', partial_ssl_dep],
  ['openssl/crypto.h', partial_ssl_dep],
  ['openssl/des.h', partial_ssl_dep],
  ['openssl/dh.h', partial_ssl_dep],
  ['openssl/dsa.h', partial_ssl_dep],
  ['openssl/ec.h', partial_ssl_dep],
  ['openssl/ecdsa.h', partial_ssl_dep],
  ['openssl/evp.h', partial_ssl_dep],
  ['openssl/hmac.h', partial_ssl_dep],
  ['openssl/rsa.h', partial_ssl_dep],
]
  _hdr = c.get(0)
  _deps = c.get(1, [])
  _has_hdr = cc.has_header(_hdr, dependencies: _deps)
  if _has_hdr
    cdata.set('HAVE_' + _hdr.underscorify().to_upper(), 1)
  endif
  has_header += {_hdr: true}
endforeach

feature_macros = [
  '-D_GNU_SOURCE',
]

# FIXME:
# _getpty
# b64_ntop
# b64_pton
# bcrypt_pbkdf
# bindresvport_sa
# explicit_memset
# group_from_gid
# login_getcapbool
# login_getpwclass
# md5_crypt
# memset_s
# ngetaddrinfo
# nsleep
# ogetaddrinfo
# openlog_r
# pstat
# readpassphrase
# setdtablesize
# setgroupent
# setpcred
# sigvec

foreach c : [
  [
    [
      'Blowfish_expand0state',
      'Blowfish_expandstate',
      'Blowfish_initstate',
      'Blowfish_stream2word',
      'blf_enc',
    ],
    'blf.h',
  ],
  ['__b64_ntop', 'resolv.h', resolv_dep],
  ['__b64_pton', 'resolv.h', resolv_dep],
  ['arc4random', 'stdlib.h'],
  ['arc4random_buf', 'stdlib.h'],
  ['arc4random_stir', 'stdlib.h'],
  ['arc4random_uniform', 'stdlib.h'],
  ['asprintf', 'stdio.h'],
  ['basename', 'libgen.h'],
  ['bcopy', 'strings.h'],
  ['bzero', 'strings.h'],
  ['calloc', 'stdlib.h'],
  ['cap_rights_limit', 'sys/capability.h'],
  ['clock', 'time.h'],
  ['clock_gettime', 'time.h'],
  ['closefrom', 'unistd.h'],
  ['crypt', 'crypt.h', crypt_dep],
  ['daemon', 'unistd.h'],
  ['dirfd', 'dirent.h'],
  ['dirname', 'libgen.h'],
  ['dlopen', 'dlfcn.h', dl_dep],
  ['endgrent', 'grp.h'],
  ['endutent', 'utmp.h'],
  ['endutxent', 'utmpx.h'],
  ['err', 'err.h'],
  ['errx', 'err.h'],
  ['fchmod', 'sys/stat.h'],
  ['fchmodat', 'sys/stat.h'],
  ['fchown', 'unistd.h'],
  ['fchownat', 'unistd.h'],
  ['flock', 'sys/file.h'],
  ['fnmatch', 'fnmatch.h'],
  ['freeaddrinfo', ['sys/types.h', 'sys/socket.h', 'netdb.h']],
  ['freezero', ['stdlib.h']],
  ['fstatfs', 'sys/vfs.h'],
  ['fstatvfs', 'sys/statvfs.h'],
  ['futimes', 'sys/time.h'],
  ['gai_strerror', 'netdb.h'],
  ['getaddrinfo', 'netdb.h'],
  ['getcwd', 'unistd.h'],
  ['getgrouplist', 'grp.h'],
  ['getline', 'stdio.h'],
  ['getnameinfo', 'netdb.h'],
  ['getopt', 'getopt.h'],
  ['getpagesize', 'unistd.h'],
  ['getpeereid', 'unistd.h'],
  ['getpeerucred', 'ucred.h'],
  ['getpgid', 'unistd.h'],
  ['getpgrp', 'unistd.h'],
  ['getpwanam', 'pwdadj.h'],
  ['getrandom', 'sys/random.h'],
  ['getrlimit', 'sys/resource.h'],
  ['getsid', 'unistd.h'],
  ['gettimeofday', 'sys/time.h'],
  ['getttyent', 'ttyent.h'],
  ['getutent', 'utmp.h'],
  ['getutid', 'utmp.h'],
  ['getutline', 'utmp.h'],
  ['getutxent', 'utmpx.h'],
  ['getutxid', 'utmpx.h'],
  ['getutxline', 'utmpx.h'],
  ['getutxuser', 'utmpx.h'],
  ['glob', 'glob.h'],
  [
    ['inet_aton', 'inet_ntoa', 'inet_ntop'],
    'arpa/inet.h',
    [nsl_dep, resolv_dep],
  ],
  ['innetgr', 'netdb.h'],
  ['isblank', 'ctype.h'],
  ['llabs', 'stdlib.h'],
  ['localtime_r', 'time.h'],
  [['login', 'logout', 'logwtmp'], 'utmp.h'],
  ['malloc', 'stdlib.h'],
  ['mblen', 'stdlib.h'],
  ['mbtowc', 'stdlib.h'],
  ['mkdtemp', 'stdlib.h'],
  ['explicit_bzero', 'string.h'],
  ['memmem', 'string.h'],
  ['memmove', 'string.h'],
  ['nanosleep', 'time.h'],
  ['nl_langinfo', 'langinfo.h'],
  ['openpty', 'pty.h', util_dep],
  ['pledge', 'unistd.h'],
  ['poll', 'poll.h'],
  ['prctl', 'sys/prctl.h'],
  ['pututline', 'utmp.h'],
  ['pututxline', 'utmpx.h'],
  ['raise', 'signal.h'],
  ['realloc', 'stdlib.h'],
  ['reallocarray', 'stdlib.h'],
  ['realpath', 'stdlib.h'],
  ['recallocarray', 'stdlib.h'],
  ['recvmsg', 'sys/socket.h'],
  ['rresvport_af', 'netdb.h'],
  ['sanbox_init', 'sandbox.h'],
  ['sendmsg', 'sys/socket.h'],
  ['setegid', 'unistd.h'],
  ['setenv', 'stdlib.h'],
  ['seteuid', 'unistd.h'],
  ['setgroups', 'grp.h'],
  ['setlinebuf', 'stdio.h'],
  ['setlogin', 'unistd.h'],
  ['setluid', 'prot.h'],
  ['setpassent', 'pwd.h'],
  ['setproctitle', 'unistd.h'],
  ['setregid', 'unistd.h'],
  ['setrlimit', 'sys/resource.h'],
  ['setresgid', 'unistd.h'],
  ['setresuid', 'unistd.h'],
  ['setreuid', 'unistd.h'],
  ['setsid', 'unistd.h'],
  ['setutent', 'utmp.h'],
  ['setutxdb', 'utmpx.h'],
  ['setutxent', 'utmpx.h'],
  ['setvbuf', 'stdio.h'],
  ['sigaction', 'signal.h'],
  ['snprintf', 'stdio.h'],
  ['socketpair', 'sys/socket.h'],
  ['statfs', ['sys/vfs.h', 'sys/param.h', 'sys/mount.h']],
  ['statvfs', 'sys/statvfs.h'],
  ['strcasestr', 'string.h'],
  ['strdup', 'string.h'],
  ['strerror', 'string.h'],
  ['strftime', 'time.h'],
  ['strlcat', 'string.h'],
  ['strlcpy', 'string.h'],
  ['strmode', 'string.h'],
  ['strndup', 'string.h'],
  ['strnlen', 'string.h'],
  ['strnvis', 'vis.h'],
  ['strptime', 'time.h'],
  ['strsep', 'string.h'],
  ['strsignal', 'string.h'],
  ['strtoll', 'stdlib.h'],
  ['strtonum', 'stdlib.h'],
  ['strtoul', 'stdlib.h'],
  ['strtoull', 'stdlib.h'],
  ['swap32', 'endian.h'],
  ['sysconf', 'unistd.h'],
  ['tcgetpgrp', 'unistd.h'],
  ['tcsendbreak', 'termios.h'],
  ['time', 'time.h'],
  ['timingsafe_bcmp', 'string.h'],
  ['truncate', 'unistd.h'],
  ['unsetenv', 'stdlib.h'],
  ['updwtmp', 'utmp.h'],
  ['updwtmpx', 'utmpx.h'],
  ['usleep', 'unistd.h'],
  ['user_from_uid', 'pwd.h'],
  ['utimensat', 'sys/stat.h'],
  ['utimes', 'sys/time.h'],
  ['utmpname', 'utmp.h'],
  ['utmpxname', 'utmpx.h'],
  ['va_copy', 'stdarg.h'],
  ['vasprintf', 'stdio.h'],
  ['vsnprintf', 'stdio.h'],
  ['waitpid', 'sys/wait.h'],
  ['waitpid', 'sys/wait.h'],
  ['warn', 'err.h'],
  ['warn', 'err.h'],
  ['wcwidth', 'wchar.h'],
  ['SHA256Update', 'sha2.h', [md_dep, bsd_dep]],
  ['SHA384Update', 'sha2.h', md_dep],
  ['SHA512Update', 'sha2.h', md_dep],
  # BSD libutil.
  [
    [
      'fmt_scaled',
      'login',
      'logout',
      'logwtmp',
      'openpty',
      'scan_scaled',
      'updwtmp',
    ],
    ['bsd/libutil.h', 'libutil.h'],
    [bsd_dep, util_dep],
  ],
]
  _prefix = ''
  _hdrlist = ','.join(c.get(1, []))
  foreach _hdr : _hdrlist == '' ? [] : _hdrlist.split(',')
    if has_header[_hdr]
      _prefix += '#include<@0@>\n'.format(_hdr)
    endif
  endforeach
  if _prefix == ''
    continue
  endif
  _deps = c.get(2, [])
  foreach _fun : ' '.join(c.get(0)).split()
    if cc.has_function(
      _fun,
      args: feature_macros,
      dependencies: _deps,
      prefix: _prefix,
    )
      cdata.set('HAVE_' + _fun.underscorify().to_upper(), 1)
    endif
  endforeach
endforeach

foreach c : [
  [
    ['AI_NUMERICSERV', 'h_errno'],
    [
      'netdb.h',
      'sys/types.h',
      'sys/socket.h',
    ],
  ],
  [['GLOB_ALTDIRFUNC', 'GLOB_NOMATCH'], 'glob.h'],
  ['MAXSYMLINKS', 'sys/param.h'],
  [
    'O_NONBLOCK',
    [
      'sys/types.h',
      'sys/stat.h',
      'fcntl.h',
    ],
  ],
  [
    'PR_SET_NO_NEW_PRIVS',
    [
      'sys/types.h',
      'linux/prctl.h',
    ],
  ],
  ['RLIMIT_NPROC', 'sys/resource.h'],
  ['RTLD_NOW', 'dlfcn.h'],
  [
    'SECCOMP_MODE_FILTER',
    [
      'sys/types.h',
      'linux/seccomp.h',
    ],
  ],
  [
    ['SHUT_RD', 'getpeereid'],
    [
      'sys/socket.h',
      'sys/types.h',
      'unistd.h',
    ],
  ],
  [
    'SYSTR_POLICY_KILL',
    [
      'sys/types.h',
      'sys/param.h',
      'dev/systrace.h',
    ],
  ],
  [
    'UINT32_MAX',
    [
      'limits.h',
      'stdint.h',
      'sys/limits.h',
    ],
  ],
  [
    'WTMPX_FILE',
    [
      'sys/types.h',
      'utmp.h',
      'utmpx.h',
      'paths.h',
    ],
  ],
  [['_getshort', '_getlong'], ['arpa/nameser.h', 'sys/types.h']],
  ['bzero', 'strings.h'],
  [
    'fd_mask',
    [
      'sys/param.h',
      'sys/select.h',
      'sys/time.h',
      'sys/types.h',
      'unistd.h',
    ],
  ],
  ['getpeerucred', 'ucred.h'],
  [
    ['howmany', 'NFDBITS'],
    [
      'sys/param.h',
      'sys/select.h',
      'sys/sysmacros.h',
      'sys/time.h',
      'sys/types.h',
      'unistd.h',
    ],
  ],
  ['memmem', 'string.h'],
  ['offsetof', 'stddef.h'],
  [['readv', 'writev'], 'sys/uio.h'],
  ['VIS_ALL', 'vis.h'],
]
  _symlist = ' '.join(c.get(0)).split()
  _hdrlist = ' '.join(c.get(1)).split()
  _deps = c.get(2, [])
  foreach _sym : _symlist
    _have_decl = false
    foreach _hdr : _hdrlist
      if (
        has_header[_hdr]
        and cc.has_header_symbol(_hdr, _sym, dependencies: _deps)
      )
        _have_decl = true
        break
      endif
    endforeach
    cdata.set10('HAVE_DECL_' + _sym.underscorify().to_upper(), _have_decl)
  endforeach
endforeach

cdata.set('SIZEOF_INT', cc.sizeof('int'))
cdata.set('SIZEOF_LONG_INT', cc.sizeof('long int'))
cdata.set('SIZEOF_LONG_LONG_INT', cc.sizeof('long long int'))
cdata.set('SIZEOF_SHORT_INT', cc.sizeof('short int'))

foreach c : [
  [
    ['long double', 'long long', 'unsigned long long'],
  ],
  [
    ['intmax_t', 'uintmax_t'],
    ['sys/types.h', 'stdint.h'],
  ],
  [
    ['int8_t', 'int16_t', 'int32_t', 'int64_t'],
    ['sys/types.h', 'stdint.h', 'sys/socket.h', 'sys/bitypes.h'],
    'int64_t a; a = 1;',
  ],
  [
    ['uint8_t', 'uint16_t', 'uint32_t'],
    ['sys/types.h', 'stdint.h', 'inttypes.h'],
  ],
  [
    ['u_char', 'u_int'],
    'sys/types.h',
  ],
  [
    ['u_int8_t', 'u_int16_t', 'u_int32_t', 'u_int64_t'],
    ['sys/types.h', 'sys/socket.h', 'sys/bitypes.h'],
  ],
  [
    ['size_t', 'ssize_t'],
    'sys/types.h',
  ],
  ['clock_t', 'time.h'],
  [
    'fd_mask',
    [
      'sys/param.h',
      'sys/types.h',
      'sys/select.h',
      'sys/time.h',
      'unistd.h',
    ],
  ],
  [
    ['fsblkcnt_t', 'fsfilcnt_t'],
    [
      'sys/types.h',
      'sys/bitypes.h',
      'sys/statfs.h',
      'sys/statvfs.h',
    ],
  ],
  [
    ['in_addr_t', 'in_port_t', 'sa_family_t'],
    ['sys/types.h', 'sys/socket.h', 'netinet/in.h'],
  ],
  [['mode_t', 'pid_t'], 'sys/types.h'],
  ['sig_atomic_t', 'signal.h'],
  ['struct addrinfo', 'netdb.h'],
  ['struct in6_addr', 'netinet/in.h'],
  ['struct sockaddr_in6', 'netinet/in.h'],
  ['struct sockaddr_storage', ['sys/types.h', 'sys/socket.h']],
  ['struct timespec', ['sys/time.h', 'time.h']],
  ['struct timeval', 'sys/time.h'],
]
  _prefix = ''
  _hdrlist = ','.join(c.get(1, []))
  foreach _hdr : _hdrlist == '' ? [] : _hdrlist.split(',')
    if has_header[_hdr]
      _prefix += '#include<' + _hdr + '>\n'
    endif
  endforeach
  if _hdrlist != '' and _prefix == ''
    continue
  endif
  foreach _type : ','.join(c.get(0)).split(',')
    if cc.has_type(_type, prefix: _prefix)
      cdata.set('HAVE_' + _type.underscorify().to_upper(), 1)
    endif
  endforeach
endforeach

foreach c : [
  ['HEADER', 'ad', ['sys/types.h', 'arpa/nameser.h']], 
  [
    'glob_t',
    ['gl_matchc', 'gl_statv'],
    'glob.h',
  ],
  ['struct passwd', 'pw_gecos', ['sys/types.h', 'pwd.h']],
  ['struct sockaddr_in6', 'sin6_scope_id', ['sys/types.h', 'netinet/in.h']],
  ['struct sockaddr_storage', 'ss_family', ['sys/types.h', 'sys/socket.h']],
  [
    'struct statfs',
    ['f_files', 'f_flags'],
    [
      'sys/param.h',
      'sys/types.h',
      'sys/bitypes.h',
      'sys/statfs.h',
      'sys/statvfs.h',
      'sys/vfs.h',
      'sys/mount.h',
    ],
  ],
  ['struct stat', ['st_blksize', 'st_mtim', 'st_mtime'], 'sys/stat.h'],
]
  _type = c.get(0)
  _prefix = ''
  foreach _hdr : ' '.join(c.get(2)).split()
    if has_header[_hdr]
      _prefix += '#include<' + _hdr + '>\n'
    endif
  endforeach
  if _prefix == ''
    continue
  endif
  foreach _member : ' '.join(c.get(1)).split()
    if cc.has_member(_type, _member, prefix: _prefix)
      cdata.set(
        'HAVE_@0@_@1@'.format(
          _type.underscorify().to_upper(),
          _member.underscorify().to_upper(),
        ),
        1,
      )
    endif
  endforeach
endforeach

foreach c : [
  ['addr', ['utmp', 'utmpx']],
  ['addr_v6', ['utmp']],
  ['exit', 'utmp'],
  ['host', ['utmp', 'utmpx']],
  ['id', ['utmp', 'utmpx']],
  ['pid', 'utmp'],
  ['ss', 'utmpx'],
  ['syslen', 'utmpx'],
  ['time', ['utmp', 'utmpx']],
  ['tv', ['utmp', 'utmpx']],
  ['type', ['utmp', 'utmpx']],
]
  _member_id = c.get(0)
  _member = 'ut_' + _member_id
  foreach _hdr_id : ' '.join(c.get(1)).split()
    _hdr = _hdr_id + '.h'
    if (
      has_header[_hdr]
      and cc.has_member(
        'struct utmp',
        _member,
        prefix: '#include<@0@>'.format(_hdr),
      )
    )
      cdata.set(
        'HAVE_@0@_IN_@1@'.format(_member_id.to_upper(), _hdr_id.to_upper()),
        1,
      )
    endif
  endforeach
endforeach

foreach _flag, _code : {
  'GETPGRP_VOID': '''
  #include<unistd.h>
  int main(void) {
    getpgrp();
    return 0;
  }
  ''',
  'HAS_SHADOW_EXPIRE': '''
  #include <sys/types.h>
  #include <shadow.h>
  struct spwd sp;
  int main(void) {
    sp.sp_expire = sp.sp_lstchg = sp.sp_inact = 0;
    return 0;
  }
  ''',
  'HAS_SNPRINTF_CONST': '''
  #include <stdio.h>
  int snprintf(char *a, size_t b, const char *c, ...) { return 0; }
  int main(void) {
    snprintf(0, 0, 0);
    return 0;
  }
  ''',
  'HAVE_CONTROL_IN_MSGHDR': '''
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <sys/uio.h>
  #include <stdlib.h>
  #ifdef msg_control
  #error "msg_control is a macro"
  #endif
  int main(void) {
    struct msghdr m;
    m.msg_control = 0;
    return 0;
  }
  ''',
  'HAVE_CONST_GAI_STRERROR_PROTO': '''
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netdb.h>
  const char *gai_strerror(int);
  int main(void) {
    char *str;
    str = gai_strerror(0);
    return 0;
  }
  ''',
  'HAVE_SO_PEERCRED': '''
  #include <sys/types.h>
  #include <sys/socket.h>
  int i = SO_PEERCRED;
  ''',
  'VARIABLE_LENGTH_ARRAYS': '''
  int main(void) {
    int i; for (i=0; i<3; i++){int a[i]; a[i-1]=0;}
    return 0;
  }
  ''',
}
  if cc.compiles(_code, args: feature_macros, name: _flag)
    cdata.set(_flag, 1)
  endif
endforeach

foreach _flag, _code : {
  'HAVE___func__': '''
  #include <stdio.h>
  int main(void) {
    printf("%s", __func__);
    return 0;
  }
  ''',
  'HAVE___FUNCTION__': '''
  #include <stdio.h>
  int main(void) {
    printf("%s", __FUNCTION__);
    return 0;
  }
  ''',
  'HAVE___PROGNAME': '''
  #include <stdio.h>
  int main(void) {
    extern char *__progname;
    printf("%s", __progname);
    return 0;
  }
  ''',
  'HAVE__RES_EXTERN': '''
  #include <stdio.h>
  #include <sys/types.h>
  #include <netinet/in.h>
  #include <arpa/nameser.h>
  #include <resolv.h>
  extern struct __res_state _res;
  int main(void) {
    struct __res_state *volatile p = &_res;  /* force resolution of _res */
    return 0;
  }
  ''',
  'HAVE___VA_COPY': '''
  #include <stdarg.h>
  va_list x,y;
  int main(void) {
    __va_copy(x,y);
    return 0;
  }
  ''',
  'HAVE_GETOPT_OPTRESET': '''
  #include <getopt.h>
  int main(void) {
    extern int optreset;
    optreset = 0;
    return 0;
  }
  ''',
  'HAVE_VA_COPY': '''
  #include <stdarg.h>
  va_list x,y;
  int main(void) {
    va_copy(x,y);
    return 0;
  }
  ''',
}
  if cc.links(_code, args: feature_macros, name: _flag)
    cdata.set(_flag, 1)
  endif
endforeach

cdata.set_quoted('_PATH_SSH_PIDDIR', get_option('piddir'))

if cdata.get('HAVE_DECL_VIS_ALL') != 1
  cdata.set('BROKEN_STRNVIS', 1)
endif

if get_option('wtmpx').require(cdata.get('HAVE_DECL_WTMPX_FILE') == 1).disabled()
  cdata.set('DISABLE_WTMPX', 1)
endif

if get_option('4in6').disable_auto_if(host_machine.system() != 'linux').allowed()
  cdata.set('IPV4_IN_IPV6', 1)
endif

if cdata.get('HAVE_DECL_GLOB_ALTDIRFUNC') == 1
  cdata.set('GLOB_HAS_ALTDIRFUNC', 1)
endif

if cdata.get('HAVE_DECL_RLIMIT_NPROC') == 1
  cdata.set('HAVE_RLIMIT_NPROC', true)
endif

if cdata.get('HAVE_STRUCT_SOCKADDR_STORAGE_SS_FAMILY', 0) == 1
  cdata.set('HAVE_SS_FAMILY_IN_SS', 1)
endif

if zlib_dep.found()
  cdata.set('HAVE_LIBZ', 1)
  cdata.set('WITH_ZLIB', 1)
endif

if cdata.get('HAS_SNPRINTF_CONST', 0) == 1
  cdata.set('SNPRINTF_CONST', 'const')
endif

_privsep_user = get_option('privsep_user')
if _privsep_user == ''
  if host_machine.system() == 'cygwin'
    cdata.set('SSH_PRIVSEP_USER', 'CYGWIN_SSH_PRIVSEP_USER')
  else
    _privsep_user = 'sshd'
  endif
endif
if _privsep_user != ''
  cdata.set_quoted('SSH_PRIVSEP_USER', _privsep_user)
endif

if ssl_dep.found()

  foreach c : [
    ['OpenSSL_add_all_algorithms', 'openssl/evp.h'],
    ['DES_crypt', 'openssl/des.h'],
    [
      [
        'DH_get0_key',
        'DH_get0_pqg',
        'DH_set0_key',
        'DH_set0_pqg',
        'DH_set_length',
      ],
      'openssl/dh.h',
    ],
    [
      [
        'DSA_SIG_get0',
        'DSA_SIG_set0',
        'DSA_generate_parameters_ex',
        'DSA_get0_key',
        'DSA_get0_pqg',
        'DSA_set0_key',
        'DSA_set0_pqg',
      ],
      'openssl/dsa.h',
    ],
    [
      [
        'ECDSA_SIG_get0',
        'ECDSA_SIG_set0',
      ],
      'openssl/ecdsa.h',
    ],
    [
      [
        'EVP_CIPHER_CTX_ctrl',
        'EVP_CIPHER_CTX_get_iv',
        'EVP_CIPHER_CTX_get_updated_iv',
        'EVP_CIPHER_CTX_iv',
        'EVP_CIPHER_CTX_iv_noconst',
        'EVP_CIPHER_CTX_set_iv',
        'EVP_DigestFinal_ex',
        'EVP_DigestInit_ex',
        'EVP_MD_CTX_cleanup',
        'EVP_MD_CTX_copy_ex',
        'EVP_MD_CTX_free',
        'EVP_MD_CTX_init',
        'EVP_MD_CTX_new',
        'EVP_PKEY_get0_RSA',
        'EVP_chacha20',
        'EVP_sha256',
        'EVP_sha384',
        'EVP_sha512',
      ],
      'openssl/evp.h',
    ],
    ['HMAC_CTX_init', 'openssl/hmac.h'],
    [
      [
        'OPENSSL_init_crypto',
        'OpenSSL_version',
        'OpenSSL_version_num',
      ],
      'openssl/crypto.h',
    ],
    [
      [
        'RSA_generate_key_ex',
        'RSA_get0_crt_params',
        'RSA_get0_factors',
        'RSA_get0_key',
        'RSA_get_default_method',
        'RSA_meth_dup',
        'RSA_meth_free',
        'RSA_meth_get_finish',
        'RSA_meth_set1_name',
        'RSA_meth_set_finish',
        'RSA_meth_set_priv_dec',
        'RSA_meth_set_priv_enc',
        'RSA_set0_crt_params',
        'RSA_set0_factors',
        'RSA_set0_key',
      ],
      'openssl/rsa.h',
    ],
  ]
    foreach _sym : ' '.join(c.get(0)).split()
      foreach _hdr : ' '.join(c.get(1)).split()
        if (
          has_header[_hdr]
          and cc.has_header_symbol(_hdr, _sym, dependencies: partial_ssl_dep)
        )
          cdata.set('HAVE_' + _sym.underscorify().to_upper(), 1)
          break
        endif
      endforeach
    endforeach
  endforeach

  _ssl_curve_test_code = '''
    #include <openssl/ec.h>
    #include <openssl/ecdh.h>
    #include <openssl/ecdsa.h>
    #include <openssl/evp.h>
    #include <openssl/objects.h>
    #include <openssl/opensslv.h>
    int main(void) {
      EC_KEY *e = EC_KEY_new_by_curve_name(@0@);
      const EVP_MD *m = @1@(); /* We need this too */
      return(e == NULL || m == NULL);
    }
    '''

  foreach _flag, _code : {
    'OPENSSL_HAS_NISTP256': _ssl_curve_test_code.format(
      'NID_X9_62_prime256v1',
      'EVP_sha256',
    ),
    'OPENSSL_HAS_NISTP384': _ssl_curve_test_code.format(
      'NID_secp384r1',
      'EVP_sha384',
    ),
    'OPENSSL_HAS_NISTP521': _ssl_curve_test_code.format(
      'NID_secp521r1',
      'EVP_sha512',
    ),
    'OPENSSL_EVP_DIGESTUPDATE_INT': '''
    #include <stdlib.h>
    #include <string.h>
    #include <openssl/evp.h>
    int main(void) {
      int ret = EVP_DigestUpdate(NULL, NULL, 0);
      return 0;
    }
    ''',
    'OPENSSL_HAVE_EVPCTR':
    '''
    #include <stdlib.h>
    #include <string.h>
    #include <openssl/evp.h>
    int main(void) {
      return(EVP_aes_128_ctr() == NULL ||
             EVP_aes_192_cbc() == NULL ||
             EVP_aes_256_cbc() == NULL);
    }
    ''',
    'OPENSSL_HAVE_EVPGCM': '''
    #include <stdlib.h>
    #include <string.h>
    #include <openssl/evp.h>
    int main(void) {
      int ret = EVP_CIPHER_CTX_ctrl(NULL, 0, 0, NULL);
      return(EVP_aes_128_gcm() == NULL ||
             EVP_aes_256_gcm() == NULL ||
             EVP_CTRL_GCM_SET_IV_FIXED == 0 ||
             EVP_CTRL_GCM_IV_GEN == 0 ||
             EVP_CTRL_GCM_SET_TAG == 0 ||
             EVP_CTRL_GCM_GET_TAG == 0);
    }
    ''',
    'OPENSSL_WORKING_AES': '''
    #include <stdlib.h>
    #include <string.h>
    #include <openssl/evp.h>
    int main(void) {
      return(EVP_aes_192_cbc() == NULL || EVP_aes_256_cbc() == NULL);
    }
    ''',
    #'USE_OPENSSL_ENGINE': '''
    ##include <openssl/engine.h>
    #int main(void) {
    #  ENGINE_load_builtin_engines();
    #  ENGINE_register_all_complete();
    #  return 0;
    #}
    #''',
  }
    if ssl_dep.type_name() == 'internal'
      _supported = cc.compiles(_code, dependencies: partial_ssl_dep, name: _flag)
    elif meson.is_cross_build()
      _supported = cc.links(_code, dependencies: ssl_dep, name: _flag)
    else
      _res = cc.run(_code, dependencies: ssl_dep, name: _flag)
      _supported = _res.compiled() and _res.returncode() == 0
    endif
    if _supported
      cdata.set(_flag, 1)
    endif
  endforeach

  if cdata.get('OPENSSL_EVP_DIGESTUPDATE_INT', 0) != 1
    cdata.set('OPENSSL_EVP_DIGESTUPDATE_VOID', 1)
  endif
  if cdata.get('OPENSSL_WORKING_AES', 0) != 1
    cdata.set('OPENSSL_LOBOTOMISED_AES', 1)
  endif
  if (
    cdata.get('OPENSSL_HAS_NISTP256', 0) == 1
    or cdata.get('OPENSSL_HAS_NISTP384', 0) == 1
    or cdata.get('OPENSSL_HAS_NISTP521', 0) == 1
  )
    cdata.set('OPENSSL_HAS_ECC', 1)
  endif
  cdata.set('OPENSSL_PRNG_ONLY', 1)
  cdata.set('WITH_OPENSSL', 1)

endif

if get_option(
  'pkcs11',
).require(
  ssl_dep.found()
  and cdata.get('HAVE_DECL_RTLD_NOW') == 1
  and cdata.get('HAVE_DLOPEN', 0) == 1,
).allowed()
  cdata.set('ENABLE_PKCS11', true)
endif

_common_reqs = {
  'requires the dlopen function': cdata.get('HAVE_DLOPEN', 0) == 1,
  'requires the OpenSSL library': ssl_dep.found(),
}
foreach _method, _requires : {
  'libfido2': _common_reqs + {
    'requires the libfido2 library': fido_dep.found(),
  },
  'builtin': _common_reqs,
  'disabled': {},
}
  if security_key not in ['auto', _method]
    continue
  endif
  _supported = true
  foreach _req, _available : _requires
    if not _available
      if security_key == _method
        error('@0@ security-key @1@'.format(_method, _req))
      endif
      _supported = false
      break
    endif
  endforeach
  if _supported
    security_key = _method
    break
  endif
endforeach
if security_key != 'disabled'
  cdata.set('ENABLE_SK', 1)
  if security_key == 'builtin'
    cdata.set('ENABLE_SK_INTERNAL', 1)
  elif security_key == 'libfido2'
    sk_dep = fido_dep
  endif
endif

_audit_arch = {
  'aarch64': 'AUDIT_ARCH_AARCH64',
  'arm'    : 'AUDIT_ARCH_ARM',
  'mips'   : 'AUDIT_ARCH_MIPS',
  'riscv64': 'AUDIT_ARCH_RISCV64',
  's390'   : 'AUDIT_ARCH_S390',
  's390x'  : 'AUDIT_ARCH_S390X',
  'x86'    : 'AUDIT_ARCH_I386',
  'x86_64' : 'AUDIT_ARCH_X86_64',
}.get(host_machine.cpu_family(), '')
if _audit_arch == ''
  error('architecture not supported')
endif
cdata.set(
  'SECCOMP_AUDIT_ARCH',
  _audit_arch,
)
if (
  cdata.get('HAVE_DECL_SECCOMP_MODE_FILTER') == 1
  and cc.links(
    '''
    #include <errno.h>
    #include <elf.h>
    #include <linux/audit.h>
    #include <linux/seccomp.h>
    #include <stdlib.h>
    #include <sys/prctl.h>
    int main(void) {
      int i = @0@;
      errno = 0;
      prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, NULL, 0, 0);
      return(errno == EFAULT ? 0 : 1);
    }
    '''.format(_audit_arch),
    name: 'HAVE_SECCOMP_FILTER',
  )
)
  cdata.set('HAVE_SECCOMP_FILTER', 1)
endif

if cdata.get('HAVE_SETRLIMIT', 0) == 1
  if meson.is_cross_build()
    cdata.set('HAS_WORKING_SELECT_RLIMIT_COMBO', 1)
    cdata.set('HAS_WORKING_SETRLIMIT_FSIZE', 1)
    cdata.set('HAS_WORKING_SETRLIMIT_NOFILE_ZERO', 1)
  else
    foreach _flag, _code : {
      'HAS_WORKING_SELECT_RLIMIT_COMBO': '''
      #include <sys/types.h>
      #if @0@
      # include <sys/time.h>
      #endif
      #include <sys/resource.h>
      #if @1@
      # include <sys/select.h>
      #endif
      #include <errno.h>
      #include <fcntl.h>
      #include <stdlib.h>
      int main(void) {
        struct rlimit rl_zero;
        int fd, r;
        fd_set fds;
        struct timeval tv;
        fd = open("/dev/null", O_RDONLY);
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        rl_zero.rlim_cur = rl_zero.rlim_max = 0;
        setrlimit(RLIMIT_FSIZE, &rl_zero);
        setrlimit(RLIMIT_NOFILE, &rl_zero);
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        r = select(fd+1, &fds, NULL, NULL, &tv);
        return (r == -1 ? 1 : 0);
      }
      '''.format(
        has_header['sys/time.h'].to_int(),
        has_header['sys/select.h'].to_int(),
      ),
      'HAS_WORKING_SETRLIMIT_FSIZE': '''
      #include <sys/types.h>
      #include <sys/resource.h>
      #include <stdlib.h>
      int main(void) {
        struct rlimit rl_zero;
        rl_zero.rlim_cur = rl_zero.rlim_max = 0;
        return (setrlimit(RLIMIT_FSIZE, &rl_zero) != 0);
      }
      ''',
      'HAS_WORKING_SETRLIMIT_NOFILE_ZERO': '''
      #include <sys/types.h>
      #if @0@
      # include <sys/time.h>
      #endif
      #include <sys/resource.h>
      #include <errno.h>
      #include <stdlib.h>
      int main(void) {
        struct rlimit rl_zero;
        int r;
        rl_zero.rlim_cur = rl_zero.rlim_max = 0;
        r = setrlimit(RLIMIT_NOFILE, &rl_zero);
        return (r == -1 ? 1 : 0);
      }
      '''.format(has_header['sys/time.h'].to_int()),
    }
      _res = cc.run(_code, args: feature_macros, name: _flag)
      if _res.compiled() and _res.returncode() == 0
        cdata.set(_flag, 1)
      endif
    endforeach
  endif
endif

sandbox = get_option('sandbox')
foreach _method, _requires : {
  'pledge': {
    'requires pledge(2) support': cdata.get('HAVE_PLEDGE', 0) == 1,
  },
  'systrace': {
    'requires systrace headers and SYSTR_POLICY_KILL support': cdata.get(
      'HAVE_SYSTR_POLICY_KILL',
      0,
    ) == 1,
  },
  'darwin': {
    'requires sandbox.h and sandbox_init function': has_header['sandbox.h']
    and cdata.get('HAVE_SANDBOX_INIT', 0) == 1,
  },
  'seccomp_filter': {
    'not supported on @0@'.format(host_machine.system()): (
      has_header['elf.h']
      and has_header['linux/audit.h']
      and has_header['linux/filter.h']
    ),
    'requires prctl function': cdata.get('HAVE_PRCTL', 0) == 1,
    'requires seccomp headers': cdata.get('HAVE_SECCOMP_FILTER', 0) == 1,
    'requires PR_SET_NO_NEW_PRIVS': cdata.get('HAVE_DECL_PR_SET_NO_NEW_PRIVS') == 1,
  },
  'capsicum': {
    'requires sys/capsicum.h header': has_header['sys/capsicum.h'],
    'requires cap_rights_limit function': cdata.get('HAVE_CAP_RIGHTS_LIMIT', 0) == 1,
  },
  'rlimit': {
    'requires setrlimit function': cdata.get('HAVE_SETRLIMIT', 0) == 1,
    'requires `setrlimit(RLIMIT_NOFILE,{0,0}` to work': cdata.get(
      'HAS_WORKING_SETRLIMIT_NOFILE_ZERO',
      0,
    ) == 1,
    'requires select to work with rlimit': cdata.get(
      'HAS_WORKING_SELECT_RLIMIT_COMBO',
      0,
    ) == 1,
  },
  'disabled': {},
}
  if sandbox not in ['auto', _method]
    continue
  endif
  _supported = true
  foreach _req, _available : _requires
    if not _available
      if sandbox == _method
        error('@0@ sandbox @1@'.format(_method, _req))
      endif
      _supported = false
      break
    endif
  endforeach
  if _supported
    sandbox = _method
    break
  endif
endforeach
if sandbox != 'disabled'
  cdata.set('SANDBOX_' + sandbox.underscorify().to_upper(), 1)
endif
if cdata.get('HAS_WORKING_SETRLIMIT_FSIZE', 0) != 1
  cdata.set('SANDBOX_SKIP_RLIMIT_FSIZE', 1)
endif

_user_path = '/usr/bin:/bin:/usr/sbin:/sbin'
if not meson.is_cross_build()
  _res = cc.run(
    '''
    #include <stdio.h>
    #include <stdlib.h>
    #if @0@
    # include <paths.h>
    #endif
    #ifndef _PATH_STDPATH
    # ifdef _PATH_USERPATH	/* Irix */
    #  define _PATH_STDPATH _PATH_USERPATH
    # else
    #  define _PATH_STDPATH "@1@"
    # endif
    #endif
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    int main(void) {
      return puts(_PATH_STDPATH) < 0;
    }
    '''.format(has_header['paths.h'].to_int(), _user_path),
    name: 'find out what the default PATH is',
  )
  if _res.compiled() and _res.returncode() == 0
    _user_path = _res.stdout().strip()
  endif
endif
_bindir = get_option('prefix') / get_option('bindir')
if _bindir not in _user_path.split(':')
  _user_path += ':' + _bindir
endif
cdata.set_quoted('USER_PATH', _user_path)

if host_machine.endian() == 'big'
  cdata.set('WORDS_BIGENDIAN', 1)
endif

if host_machine.system() == 'linux'
  cdata.set_quoted('_PATH_BTMP', '/var/log/btmp')
  cdata.set('LINK_OPNOTSUPP_ERRNO', 'EPERM')
  cdata.set('LINUX_OOM_ADJUST', 1)
  cdata.set_quoted('LOCKED_PASSWD_PREFIX', '!')
  cdata.set('PAM_TTY_KLUDGE', 1)
  cdata.set('SPT_TYPE', 'SPT_REUSEARGV')
  if has_header['linux/if.h']
    cdata.set('SYS_RDOMAIN_LINUX', 1)
  endif
  if has_header['linux/if_tun.h']
    cdata.set('SSH_TUN_LINUX', 1)
    cdata.set('SSH_TUN_COMPAT_AF', 1)
    cdata.set('SSH_TUN_PREPEND_AF', 1)
  endif
  cdata.set('USE_BTMP', 1)
endif

if not meson.is_cross_build()
  foreach c : [
    ['XAUTH_PATH', 'xauth'],
    ['_PATH_PASSWD_PROG', 'passwd'],
  ]
    _define = c.get(0)
    _name = c.get(1)
    _prog = find_program(_name, required: false)
    if _prog.found()
      cdata.set_quoted(_define, _prog.full_path())
    endif
  endforeach
  if fs.is_dir('/proc/self/fd')
    cdata.set('HAVE_PROC_PID', 1)
  endif
endif

summary(
  {
    'privsep sandboxing': sandbox == 'disabled' ? false : sandbox,
    'PKCS#11 support': cdata.get('ENABLE_PKCS11', false),
    'U2F/FIDO support': security_key == 'disabled' ? false : security_key,
  },
  bool_yn: true,
)

config_h = configure_file(
  input: 'config.meson.h',
  output: 'config.h',
  configuration: cdata,
)

deps = [
  bsd_dep,
  crypt_dep,
  dl_dep,
  md_dep,
  nsl_dep,
  util_dep,
  resolv_dep,
  sk_dep,
  ssl_dep,
  zlib_dep,
]

subdir('openbsd-compat')

ssh_lib_src = [
  config_h,
  'addr.c',
  'addrmatch.c',
  'atomicio.c',
  'authfd.c',
  'authfile.c',
  'bitmap.c',
  'canohost.c',
  'chacha.c',
  'channels.c',
  'cipher-aes.c',
  'cipher-aesctr.c',
  'cipher-chachapoly-libcrypto.c',
  'cipher-chachapoly.c',
  'cipher-ctr.c',
  'cipher.c',
  'cleanup.c',
  'compat.c',
  'dh.c',
  'digest-libc.c',
  'digest-openssl.c',
  'dispatch.c',
  'dns.c',
  'ed25519.c',
  'entropy.c',
  'fatal.c',
  'fe25519.c',
  'ge25519.c',
  'gss-genr.c',
  'hash.c',
  'hmac.c',
  'hostfile.c',
  'kex.c',
  'kexc25519.c',
  'kexdh.c',
  'kexecdh.c',
  'kexgen.c',
  'kexgex.c',
  'kexgexc.c',
  'kexgexs.c',
  'kexsntrup761x25519.c',
  'krl.c',
  'log.c',
  'mac.c',
  'match.c',
  'misc.c',
  'moduli.c',
  'monitor_fdpass.c',
  'msg.c',
  'nchan.c',
  'packet.c',
  'platform-misc.c',
  'platform-pledge.c',
  'platform-tracing.c',
  'poly1305.c',
  'progressmeter.c',
  'readpass.c',
  'rijndael.c',
  'sc25519.c',
  'sftp-realpath.c',
  'smult_curve25519_ref.c',
  'sntrup761.c',
  'ssh-dss.c',
  'ssh-ecdsa-sk.c',
  'ssh-ecdsa.c',
  'ssh-ed25519-sk.c',
  'ssh-ed25519.c',
  'ssh-pkcs11.c',
  'ssh-rsa.c',
  'ssh-xmss.c',
  'ssh_api.c',
  'sshbuf-getput-basic.c',
  'sshbuf-getput-crypto.c',
  'sshbuf-io.c',
  'sshbuf-misc.c',
  'sshbuf.c',
  'ssherr.c',
  'sshkey-xmss.c',
  'sshkey.c',
  'ttymodes.c',
  'umac.c',
  'umac128.c',
  'utf8.c',
  'verify.c',
  'xmalloc.c',
  'xmss_commons.c',
  'xmss_fast.c',
  'xmss_hash.c',
  'xmss_hash_address.c',
  'xmss_wots.c',
]

if security_key == 'builtin'
  ssh_lib_src += 'regress/misc/sk-dummy/sk-dummy.c'
endif

ssh_lib = static_library(
  'ssh',
  ssh_lib_src,
  c_args: feature_macros,
  dependencies: deps,
  link_with: bsdcompat_lib,
)

ssh_dep = declare_dependency(
  compile_args: feature_macros,
  dependencies: deps,
  link_with: ssh_lib,
  sources: config_h,
)

_executable_list = []
_progs = get_option('progs')
foreach _name, _src : {
  'scp': [
    'scp.c',
    'progressmeter.c',
  ],
  'sftp-server': [
    'sftp-common.c',
    'sftp-server.c',
    'sftp-server-main.c',
  ],
  'ssh': [
    'clientloop.c',
    'mux.c',
    'readconf.c',
    'ssh-sk-client.c',
    'ssh.c',
    'sshconnect.c',
    'sshconnect2.c',
    'sshtty.c',
  ],
  'ssh-add': [
    'ssh-add.c',
    'ssh-sk-client.c',
  ],
  'ssh-agent': [
    'ssh-agent.c',
    'ssh-pkcs11-client.c',
    'ssh-sk-client.c',
  ],
  'ssh-keygen': [
    'ssh-keygen.c',
    'ssh-sk-client.c',
    'sshsig.c',
  ],
  'ssh-keyscan': [
    'ssh-keyscan.c',
    'ssh-sk-client.c',
  ],
  'ssh-keysign': [
    'readconf.c',
    'ssh-keysign.c',
    'ssh-sk-client.c',
    'uidswap.c',
  ],
  'sshd': [
    'audit-bsm.c',
    'audit-linux.c',
    'audit.c',
    'auth-bsdauth.c',
    'auth-krb5.c',
    'auth-options.c',
    'auth-pam.c',
    'auth-passwd.c',
    'auth-rhosts.c',
    'auth-shadow.c',
    'auth-sia.c',
    'auth.c',
    'auth2-chall.c',
    'auth2-gss.c',
    'auth2-hostbased.c',
    'auth2-kbdint.c',
    'auth2-none.c',
    'auth2-passwd.c',
    'auth2-pubkey.c',
    'auth2.c',
    'groupaccess.c',
    'gss-serv-krb5.c',
    'gss-serv.c',
    'loginrec.c',
    'md5crypt.c',
    'monitor.c',
    'monitor_wrap.c',
    'platform.c',
    'sandbox-capsicum.c',
    'sandbox-darwin.c',
    'sandbox-null.c',
    'sandbox-pledge.c',
    'sandbox-rlimit.c',
    'sandbox-seccomp-filter.c',
    'sandbox-solaris.c',
    'sandbox-systrace.c',
    'servconf.c',
    'serverloop.c',
    'session.c',
    'sftp-common.c',
    'sftp-server.c',
    'srclimit.c',
    'ssh-sk-client.c',
    'sshd.c',
    'sshlogin.c',
    'sshpty.c',
    'uidswap.c',
  ],
}
  if 'all' in _progs or _name in _progs
    _executable_list += [[_name, _src]]
  endif
endforeach
if 'all' in _progs or 'ssh-pkcs11-helper' in _progs
  if cdata.get('ENABLE_PKCS11', false)
    _executable_list += [
      [
        'ssh-pkcs11-helper',
        'ssh-pkcs11-helper.c',
        'ssh-pkcs11.c',
        'ssh-sk-client.c',
      ],
    ]
  elif 'ssh-pkcs11-helper' in _progs
    error('cannot build ssh-pkcs11-helper without PKCS#11 support')
  endif
endif
if 'all' in _progs or 'ssh-sk-helper' in _progs
  if security_key == 'libfido2'
    _executable_list += [
      [
        'ssh-sk-helper',
        'ssh-sk-helper.c',
        'ssh-sk.c',
        'sk-usbhid.c',
      ],
    ]
  elif 'ssh-sk-helper' in _progs
    error('cannot build ssh-sk-helper without U2F/FIDO support using libfido2')
  endif
endif

foreach _exe : _executable_list
  executable(_exe, dependencies: ssh_dep, install: true)
endforeach
