project('rsync', 'c', version: '3.2.7')

cpp_language_enabled = add_languages('cpp', required: false)
cc = meson.get_compiler('c')
has_header = configuration_data()
has_member = configuration_data()
has_symbol = configuration_data()
has_type = configuration_data()
cdata = configuration_data()
sumup = {}

awk = find_program('awk', native: true)
env = find_program('env', native: true)

add_project_arguments('-DHAVE_CONFIG_H', language: 'c')

acls_dep = dependency('', required: false)
if get_option('acls').disabled()
  _support_acls = false
elif host_machine.system() == 'darwin'
  _acls_variant = 'OSX'
  has_header.set('sys/acl.h', cc.has_header('sys/acl.h'))
  _support_acls = has_header.get('sys/acl.h')
elif host_machine.system() == 'sunos'
  _acls_variant = 'Solaris'
  _support_acls = true
else
  _acls_variant = 'POSIX'
  acls_dep = dependency('libacl', required: false, version: '>= 2.2.6')
  _acls_dep_partial = acls_dep.partial_dependency(compile_args: true, includes: true)
  has_header.set(
    'sys/acl.h',
    cc.has_header_symbol(
      'sys/acl.h',
      'acl_get_entry',
      dependencies: _acls_dep_partial,
    ),
  )
  if has_header.get('sys/acl.h')
    if cc.has_header_symbol('sys/acl.h', 'acl_get_perm_np')
      cdata.set('HAVE_ACL_GET_PERM_NP', 1)
      _support_acls = true
    else
      has_header.set('acl/libacl.h', cc.has_header_symbol('acl/libacl.h', 'acl_get_perm'))
      _support_acls = has_header.get('acl/libacl.h')
    endif
  endif
endif
if get_option('acls').require(_support_acls).allowed()
  cdata.set('HAVE_@0@_ACLS'.format(_acls_variant.to_upper()), 1)
endif
cdata.set('SUPPORT_ACLS', _support_acls ? 1 : false)
sumup += {'Access-control list': _support_acls ? _acls_variant : false}

charset_dep = cc.find_library('charset', has_headers: 'libcharset.h', required: false)
has_header.set('libcharset.h', charset_dep.found())

crypto_dep = dependency('libcrypto', required: get_option('openssl-crypto'))
cdata.set('USE_OPENSSL', crypto_dep.found() ? 1 : false)
sumup += {'OpenSSL crypto': crypto_dep.found()}

lz4_dep = dependency('liblz4', required: get_option('lz4'))
cdata.set('SUPPORT_LZ4', lz4_dep.found() ? 1 : false)
sumup += {'LZ4 compression': lz4_dep.found()}

popt_dep = dependency(
  'popt',
  required: {
    'internal': disabler(),
    'system': true,
  }[get_option('popt')],
)
sumup += {'POPT library': popt_dep.found() ? 'system' : 'internal'}

_xattrs_support = {
  'cygwin' : ['Linux-like', 'HAVE_LINUX_XATTRS    NO_SYMLINK_USER_XATTRS'],
  'darwin' : ['OSX'       , 'HAVE_OSX_XATTRS      NO_DEVICE_XATTRS NO_SPECIAL_XATTRS'],
  'freebsd': ['FreeBSD'   , 'HAVE_FREEBSD_XATTRS'],
  'linux'  : ['Linux-like', 'HAVE_LINUX_XATTRS    NO_SYMLINK_USER_XATTRS'],
  'netbsd' : ['Linux-like', 'HAVE_LINUX_XATTRS    NO_SYMLINK_USER_XATTRS'],
  'sunos'  : ['Solaris'   , 'HAVE_SOLARIS_XATTRS  NO_SYMLINK_XATTRS'],
}.get(host_machine.system(), [])
if get_option(
  'xattrs',
).require(
  _xattrs_support.length() > 0,
  error_message: 'no support for ' + host_machine.system(),
).allowed()
  foreach _flag : _xattrs_support.get(1).split()
    cdata.set(_flag, 1)
  endforeach
  _support_xattrs = true
else
  _support_xattrs = false
endif
cdata.set('SUPPORT_XATTRS', _support_xattrs ? 1 : false)
sumup += {'Extended file attributes': _support_xattrs ? _xattrs_support[0] : false}

xxhash_dep = dependency('libxxhash', required: get_option('xxhash'))
cdata.set('SUPPORT_XXHASH', xxhash_dep.found() ? 1 : false)
sumup += {'xxHash library': xxhash_dep.found()}

zlib_dep = dependency(
  'zlib',
  required: {
    'internal': disabler(),
    'system': true,
  }[get_option('zlib')],
)
cdata.set('EXTERNAL_ZLIB', zlib_dep.found() ? 1 : false)
sumup += {'Z library': zlib_dep.found() ? 'system' : 'internal'}

zstd_dep = dependency('libzstd', required: get_option('zstd'))
cdata.set('SUPPORT_ZSTD', zstd_dep.found() ? 1 : false)
sumup += {'ZSTD compression': zstd_dep.found()}

cdata.set('_GNU_SOURCE', 1)
cdata.set_quoted('NOBODY_GROUP', get_option('nobody-group'))
cdata.set_quoted('NOBODY_USER', get_option('nobody-user'))
cdata.set_quoted('RSYNCD_SYSCONF', get_option('prefix') / get_option('sysconfdir') / 'rsyncd.conf')
cdata.set_quoted('RSYNC_PATH', get_option('rsync-path'))
cdata.set_quoted('RSYNC_RSH', get_option('rsync-rsh'))
cdata.set('WORDS_BIGENDIAN', host_machine.endian() == 'big')

feat_macros = ['-D_GNU_SOURCE']

foreach _hdr : [
  'alloca.h',
  'arpa/inet.h',
  'arpa/nameser.h',
  'attr/xattr.h',
  'bsd/string.h',
  'compat.h',
  'ctype.h',
  'dl.h',
  'errno.h',
  'fcntl.h',
  'float.h',
  'grp.h',
  'iconv.h',
  'inttypes.h',
  'langinfo.h',
  'limits.h',
  'linux/falloc.h',
  'locale.h',
  'lz4.h',
  'malloc.h',
  'mcheck.h',
  'netdb.h',
  'netgroup.h',
  'netinet/in_systm.h',
  'netinet/in.h',
  'netinet/ip.h',
  'openssl/md4.h',
  'openssl/md5.h',
  'signal.h',
  'stdarg.h',
  'stddef.h',
  'stdlib.h',
  'stdio.h',
  'stdint.h',
  'string.h',
  'strings.h',
  'sys/attr.h',
  'sys/extattr.h',
  'sys/fcntl.h',
  'sys/file.h',
  'sys/filio.h',
  'sys/ioctl.h',
  'sys/mkdev.h',
  'sys/mode.h',
  'sys/param.h',
  'sys/select.h',
  'sys/socket.h',
  'sys/stat.h',
  'sys/sysmacros.h',
  'sys/time.h',
  'sys/types.h',
  'sys/un.h',
  'sys/unistd.h',
  'sys/wait.h',
  'sys/xattr.h',
  'time.h',
  'unistd.h',
  'utime.h',
]
  has_header.set(_hdr, cc.has_header(_hdr))
endforeach

foreach _type, _hdrlist : {
  'long long'              : [],
  'gid_t'                  : ['sys/types.h'],
  'id_t'                   : ['sys/types.h'],
  'mode_t'                 : ['sys/types.h'],
  'off_t'                  : ['sys/types.h'],
  'pid_t'                  : ['sys/types.h'],
  'size_t'                 : ['stdlib.h'],
  'struct addrinfo'        : ['netdb.h'],
  'struct sockaddr_storage': ['sys/socket.h'],
  'struct stat64'          : ['sys/stat.h'],
  'struct utimbuf'         : ['utime.h'],
}
  _prefix = []
  foreach _hdr : _hdrlist
    if has_header.get(_hdr)
      _prefix += '#include <@0@>'.format(_hdr)
    endif
  endforeach
  has_type.set(_type, cc.has_type(_type, args: feat_macros, prefix: _prefix))
endforeach

cdata.set('gid_t', has_type.get('gid_t') ? false : 'int')
cdata.set('GETGROUPS_T', 'gid_t')

foreach _hdr, _symlist : {
  'alloca.h'      : 'alloca',
  'arpa/inet.h'   : ['inet_ntop', 'inet_pton'],
  'errno.h'       : 'errno',
  'fcntl.h'       : [
    'linkat',
    'open64',
    'posix_fallocate',
  ],
  'grp.h'         : ['getgrouplist', 'initgroups', 'setgroups'],
  'langinfo.h'    : 'nl_langinfo',
  'libcharset.h'  : 'locale_charset',
  'linux/falloc.h': [
    'FALLOC_FL_PUNCH_HOLE',
    'FALLOC_FL_ZERO_RANGE',
  ],
  'locale.h'      : 'setlocale',
  'iconv.h'       : ['iconv_open', 'libiconv_open'],
  'malloc.h'      : ['mallinfo', 'mallinfo2'],
  'netdb.h'       : 'innetgr',
  'signal.h'      : ['sigaction', 'sigprocmask'],
  'stdarg.h'      : ['__va_copy', 'va_copy'],
  'stdio.h'       : [
    'asprintf',
    'setvbuf',
    'snprintf',
    'vasprintf',
    'vsnprintf',
  ],
  'string.h'      : [
    'memcpy',
    'memmove',
    'strchr',
    'strerror',
    'strlcat',
    'strlcpy',
    'strpbrk',
  ],
  'strings.h'     : 'strcasecmp',
  'stdlib.h'      : [
    'putenv',
    'mkstemp64',
    'setenv',
    'strtol',
    'unsetenv',
  ],
  'sys/extattr.h' : 'extattr_get_link',
  'sys/socket.h'  : [
    'connect',
    'socketpair',
  ],
  'sys/stat.h'    : [
    'chmod',
    'fchmod',
    'lchmod',
    'mkfifo',
    'mknod',
    'utimensat',
  ],
  'sys/time.h'    : ['lutimes', 'utimes'],
  'sys/wait.h'    : ['wait4', 'waitpid'],
  'time.h'        : [
    'mktime',
    'nanosleep',
    'strftime',
  ],
  'unistd.h'      : [
    'chown',
    'ftruncate',
    'getattrlist',
    'getcwd',
    'getegid',
    'geteuid',
    'getgroups',
    'getpass',
    'getpgrp',
    'lchown',
    'link',
    'linkat',
    'lseek64',
    'readlink',
    'setattrlist',
    'seteuid',
    'setmode',
    'setsid',
    'tcgetpgrp',
    'usleep',
  ],
  'utime.h'       : 'utime',
}
  foreach _sym : ' '.join(_symlist).split()
    has_symbol.set(
      _sym,
      has_symbol.get(_sym, false)
      or (
        has_header.get(_hdr)
        and cc.has_header_symbol(_hdr, _sym, args: feat_macros)
      ),
    )
  endforeach
endforeach

cdata.set(
  'HAVE_EFFICIENT_POSIX_FALLOCATE',
  host_machine.system() == 'cygwin'
  and has_symbol.get('posix_fallocate') ? 1 : false,
)

has_symbol.set('lchmod', has_symbol.get('lchmod') and cc.has_function('lchmod'))

foreach _hdr : [
  'dirent.h',
  'sys/ndir.h',
  'sys/dir.h',
  'ndir.h',
]
  assert(not has_header.has(_hdr))
  cdata.set('HAVE_' + _hdr.underscorify().to_upper(), cc.has_header_symbol(_hdr, 'DIR') ? 1 : false)
endforeach

foreach _check : [
  ['struct sockaddr'    , 'sa_len'              , 'sys/socket.h'],
  ['struct sockaddr_in' , 'sin_len'             , 'netinet/in.h'],
  ['struct sockaddr_in6', 'sin6_scope_id'       , 'netinet/in.h'],
  ['struct sockaddr_un' , 'sun_len'             , 'sys/un.h'    ],
  ['struct stat'        , 'st_mtim.tv_nsec'     , 'sys/stat.h'  ],
  ['struct stat'        , 'st_mtimensec'        , 'sys/stat.h'  ],
  ['struct stat'        , 'st_mtimespec.tv_nsec', 'sys/stat.h'  ],
  ['struct stat'        , 'st_rdev'             , 'sys/stat.h'  ],
]
  _prefix = []
  foreach _hdr : ' '.join(_check[2]).split()
    if has_header.get(_hdr)
      _prefix += '#include <@0@>'.format(_hdr)
    endif
  endforeach
  has_member.set(
    '.'.join(_check[0], _check[1]),
    cc.has_member(_check[0], _check[1], args: feat_macros, prefix: _prefix),
  )
endforeach

cdata.set('HAVE_SOCKADDR_IN6_SCOPE_ID'           , has_member.get('struct sockaddr_in6.sin6_scope_id') ? 1 : false)
cdata.set('HAVE_SOCKADDR_IN_LEN'                 , has_member.get('struct sockaddr_in.sin_len') ? 1 : false)
cdata.set('HAVE_SOCKADDR_LEN'                    , has_member.get('struct sockaddr.sa_len') ? 1 : false)
cdata.set('HAVE_SOCKADDR_UN_LEN'                 , has_member.get('struct sockaddr_un.sun_len') ? 1 : false)
cdata.set('HAVE_STRUCT_STAT_ST_MTIMENSEC'        , has_member.get('struct stat.st_mtimensec') ? 1 : false)
cdata.set('HAVE_STRUCT_STAT_ST_MTIMESPEC_TV_NSEC', has_member.get('struct stat.st_mtimespec.tv_nsec') ? 1 : false)
cdata.set('HAVE_STRUCT_STAT_ST_MTIM_TV_NSEC'     , has_member.get('struct stat.st_mtim.tv_nsec') ? 1 : false)
cdata.set('HAVE_STRUCT_STAT_ST_RDEV'             , has_member.get('struct stat.st_rdev') ? 1 : false)

foreach _hdr : ['sys/mkdev.h', 'sys/sysmacros.h']
  _has_symbols = has_header.get(_hdr)
  foreach _sym : ['major', 'minor', 'makedev']
    _has_symbols = _has_symbols and cc.has_header_symbol(_hdr, _sym)
  endforeach
  cdata.set('MAJOR_IN_' + _hdr.split('/')[1].substring(0, -2).to_upper(), _has_symbols ? 1 : false)
endforeach

cdata.set('SIGNED_CHAR_OK', cc.has_type('signed char') ? 1 : false)

_prefix = []
foreach _hdr : ['sys/types.h', 'stdint.h', 'time.h']
  if has_header.get(_hdr)
    _prefix += '#include <@0@>'.format(_hdr)
  endif
endforeach
foreach _type : [
  'char*',
  'int',
  'int16_t',
  'int32_t',
  'int64_t',
  'long',
  'long long',
  'off64_t',
  'off_t',
  'short',
  'time_t',
  'uint16_t',
  'uint32_t',
]
  cdata.set(
    'SIZEOF_' + _type.replace('*', 'p').underscorify().to_upper(),
    cc.sizeof(_type, args: feat_macros, prefix: _prefix),
  )
endforeach

_stdc_headers = true
foreach _hdr : ['float.h', 'stdarg.h', 'stdlib.h', 'string.h']
  _stdc_headers = _stdc_headers and has_header.get(_hdr)
endforeach
cdata.set('STDC_HEADERS', _stdc_headers ? 1 : false)

cdata.set10('HAVE_ERRNO_DECL', has_symbol.get('errno'))

foreach _cfg : [has_header, has_symbol, has_type]
  foreach _hdr : _cfg.keys()
    cdata.set(
      'HAVE_' + _hdr.underscorify().to_upper(),
      _cfg.get(_hdr) ? 1 : false,
    )
  endforeach
endforeach
has_header = false
has_symbol = false
has_type = false

foreach _check : [
  [
    'GETPGRP_VOID',
    [],
    '''
    #include <unistd.h>
    int main(void) {
      getpgrp();
      return 0;
    }
    '''
  ],
  [
    'HAVE_GETADDRINFO',
    ['HAVE_STDDEF_H', 'HAVE_SYS_SOCKET_H', 'HAVE_SYS_TYPES_H'],
    '''
    #ifdef HAVE_SYS_TYPES_H
    # include <sys/types.h>
    #endif
    #ifdef HAVE_SYS_SOCKET_H
    # include <sys/socket.h>
    #endif
    #ifdef HAVE_STDDEF_H
    # include <stddef.h>
    #endif
    #include <netdb.h>
    int main(void) {
      getaddrinfo(NULL, NULL, NULL, NULL);
      return 0;
    }
    ''',
  ],
  [
    'HAVE_GETTIMEOFDAY_TZ',
    ['HAVE_UNISTD_H'],
    '''
    #include <sys/time.h>
    #ifdef HAVE_UNISTD_H
    # include <unistd.h>
    #endif
    int main(void) {
      struct timeval tv;
      return gettimeofday(&tv, NULL);
    }
    ''',
  ],
  [
    'HAVE_LONG_DOUBLE',
    [],
    '''
    #include <float.h>
    long double const a[] = {
       0.0L, DBL_MIN, DBL_MAX, DBL_EPSILON,
       LDBL_MIN, LDBL_MAX, LDBL_EPSILON
    };
    long double f(long double x) {
       return ((x + (unsigned long int) 10) * (-1 / x) + a[0] + (x ? f (x) : 'c'));
    }
    int main (void)
    {
      static int test_array [
        1 - 2 * !((0 < ((DBL_MAX_EXP < LDBL_MAX_EXP)
          + (DBL_MANT_DIG < LDBL_MANT_DIG)
          - (LDBL_MAX_EXP < DBL_MAX_EXP)
          - (LDBL_MANT_DIG < DBL_MANT_DIG)))
          && (int) LDBL_EPSILON == 0
      )];
      test_array [0] = 0;
      return test_array [0];
    }
    '''
  ],
  [
    'HAVE_SYS_FALLOCATE',
    ['HAVE_SYS_TYPES_H', 'HAVE_UNISTD_H'],
    '''
    #include <sys/syscall.h>
    #ifdef HAVE_UNISTD_H
    # include <unistd.h>
    #endif
    #ifdef HAVE_SYS_TYPES_H
    # include <sys/types.h>
    #endif
    int main(void) {
      syscall(SYS_fallocate, 0, 0, (loff_t)0, (loff_t)0);
      return 0;
    }
    '''
  ],
]
  _args = feat_macros
  foreach _flag : _check[1]
    _args += '-D@0@=@1@'.format(_flag, cdata.get(_flag))
  endforeach
  cdata.set(_check[0], cc.compiles(_check[2], args: _args, name: _check[0]) ? 1 : false)
endforeach

foreach _check : [
  [
    'HAVE_FALLOCATE',
    [],
    '''
    #include <fcntl.h>
    #ifdef HAVE_SYS_TYPES_H
    # include <sys/types.h>
    #endif
    int main(void) {
      fallocate(0, 0, 0, 0);
      return 0;
    }
    '''
  ],
]
  _args = feat_macros
  foreach _flag : _check[1]
    _args += '-D@0@=@1@'.format(_flag, cdata.get(_flag))
  endforeach
  cdata.set(_check[0], cc.links(_check[2], args: _args, name: _check[0]) ? 1 : false)
endforeach

foreach _check : [
  [
    'CAN_HARDLINK_SPECIAL', false,
    ['HAVE_SYS_STAT_H', 'HAVE_UNISTD_H'],
    '''
    #if HAVE_UNISTD_H
    # include <unistd.h>
    #endif
    #ifdef HAVE_SYS_STAT_H
    # include <sys/stat.h>
    #endif
    #include <stdlib.h>
    #include <errno.h>
    #define FILENAME "conftest.fifi"
    int main(void) {
      unlink(FILENAME);
      if (mkfifo(FILENAME, 0777) < 0) abort();
      unlink(FILENAME "2");
      if (link(FILENAME, FILENAME "2") < 0) return 1;
      return 0;
    }
    ''',
  ],
  [
    'CAN_HARDLINK_SYMLINK', false,
    ['HAVE_FCNTL_H', 'HAVE_LINKAT', 'HAVE_SYS_FCNTL_H', 'HAVE_UNISTD_H'],
    '''
    #ifdef HAVE_FCNTL_H
    # include <fcntl.h>
    #elif defined HAVE_SYS_FCNTL_H
    # include <sys/fcntl.h>
    #endif
    #if HAVE_UNISTD_H
    # include <unistd.h>
    #endif
    #include <stdlib.h>
    #include <errno.h>
    #define FILENAME "conftest.dangle"
    int main(void) {
      unlink(FILENAME);
      if (symlink("conftest.no-such", FILENAME) < 0) abort();
      unlink(FILENAME "2");
    #ifdef HAVE_LINKAT
      if (linkat(AT_FDCWD, FILENAME, AT_FDCWD, FILENAME "2", 0) < 0) return 1;
    #else
      if (link(FILENAME, FILENAME "2") < 0) return 1;
    #endif
      return 0;
    }
    ''',
  ],
  [
    'GETGROUPS_GID_T', true,
    ['gid_t'],
    '''
    #include <unistd.h>
    #define NGID 256
    #undef MAX
    #define MAX(x, y) ((x) > (y) ? (x) : (y))
    int main(void) {
      gid_t gidset[NGID];
      int i, n;
      union { gid_t gval; long int lval; }  val;
      val.lval = -1;
      for (i = 0; i < NGID; i++)
        gidset[i] = val.gval;
      n = getgroups (sizeof (gidset) / MAX (sizeof (int), sizeof (gid_t)) - 1,
                     gidset);
      /* Exit non-zero if getgroups seems to require an array of ints.  This
         happens when gid_t is short int but getgroups modifies an array
         of ints.  */
      return n > 0 && gidset[n] != val.gval;
    }
    ''',
  ],
  [
    'HAVE_AF_INET6', false,
    ['HAVE_STDLIB_H', 'HAVE_SYS_TYPES_H', 'HAVE_SYS_SOCKET_H'],
    '''
    #if HAVE_STDLIB_H
    # include <stdlib.h>
    #endif
    #ifdef HAVE_SYS_TYPES_H
    # include <sys/types.h>
    #endif
    #ifdef HAVE_SYS_SOCKET_H
    # include <sys/socket.h>
    #endif
    int main(void) {
       return socket(AF_INET6, SOCK_STREAM, 0) < 0;
    }
    ''',
  ],
  [
    'HAVE_BROKEN_READDIR', false,
    ['HAVE_SYS_TYPES_H'],
    '''
    #ifdef HAVE_SYS_TYPES_H
    # include <sys/types.h>
    #endif
    #include <dirent.h>
    int main(void) {
      struct dirent *di;
      DIR *d = opendir(".");
      di = readdir(d);
      if (di && di->d_name[-2] == '.' && di->d_name[-1] == 0 && di->d_name[0] == 0)
        return 0;
      return 1;
    }
    ''',
  ],
  [
    'HAVE_C99_VSNPRINTF', false,
    ['HAVE_STDLIB_H', 'HAVE_SYS_TYPES_H'],
    '''
    #ifdef HAVE_SYS_TYPES_H
    # include <sys/types.h>
    #endif
    #include <stdarg.h>
    #include <stdio.h>
    #if HAVE_STDLIB_H
    # include <stdlib.h>
    #endif
    #include <string.h>
    void foo(const char *format, ...) {
         va_list ap;
         int len;
         static char buf[] = "12345678901234567890";
         va_start(ap, format);
         len = vsnprintf(0, 0, format, ap);
         va_end(ap);
         if (len != 5) exit(1);
         if (snprintf(buf, 3, "hello") != 5 || strcmp(buf, "he") != 0) exit(1);
    }
    int main(void) { foo("hello"); return 0; }
    ''',
  ],
  [
    'HAVE_SECURE_MKSTEMP', false,
    ['HAVE_SYS_TYPES_H', 'HAVE_UNISTD_H'],
    '''
    #include <stdlib.h>
    #ifdef HAVE_SYS_TYPES_H
    # include <sys/types.h>
    #endif
    #include <sys/stat.h>
    #ifdef HAVE_UNISTD_H
    # include <unistd.h>
    #endif
    int main(void) {
      struct stat st;
      char tpl[20]="/tmp/test.XXXXXX";
      int fd = mkstemp(tpl);
      if (fd == -1) return 1;
      unlink(tpl);
      if (fstat(fd, &st) != 0) return 1;
      if ((st.st_mode & 0777) != 0600) return 1;
      return 0;
    }
    '''
  ],
  [
    'HAVE_UTIME_ZERO', false,
    ['HAVE_SYS_STAT_H', 'HAVE_UTIME_H'],
    '''
    #ifdef HAVE_UTIME_H
    # include <utime.h>
    #endif
    #ifdef HAVE_SYS_STAT_H
    # include <sys/stat.h>
    #endif
    int main(void) {
      struct stat s, t;
      return ! (stat ("conftest.data", &s) == 0
                && utime ("conftest.data", 0) == 0
                && stat ("conftest.data", &t) == 0
                && t.st_mtime >= s.st_mtime
                && t.st_mtime - s.st_mtime < 120);
      return 0;
    }
    '''
  ],
  [
    'MKNOD_CREATES_FIFOS', false,
    ['HAVE_UNISTD_H'],
    '''
    #include <stdio.h>
    #include <sys/stat.h>
    #include <errno.h>
    #if HAVE_UNISTD_H
    # include <unistd.h>
    #endif
    int main(void) {
      const char *fn = "fifo-test";
      int rc, ec;
      unlink(fn);
      rc = mknod(fn, S_IFIFO, 0600);
      ec = errno;
      unlink(fn);
      if (rc) {
        printf("(%d %d) ", rc, ec);
        return ec;
      }
      return 0;
    }
    '''
  ],
  [
    'MKNOD_CREATES_SOCKETS', false,
    ['HAVE_UNISTD_H'],
    '''
    #include <stdio.h>
    #include <sys/stat.h>
    #include <errno.h>
    #if HAVE_UNISTD_H
    # include <unistd.h>
    #endif
    int main(void) {
      const char *fn = "sock-test";
      int rc, ec;
      unlink(fn);
      rc = mknod(fn, S_IFSOCK, 0600);
      ec = errno;
      unlink(fn);
      if (rc) {
        printf("(%d %d) ", rc, ec);
        return ec;
      }
      return 0;
    }
    '''
  ],
]
  if meson.can_run_host_binaries()
    _args = feat_macros
    foreach _flag : _check[2]
      _args += '-D@0@=@1@'.format(_flag, cdata.get(_flag))
    endforeach
    _res = cc.run(_check[3], args: _args, name: _check[0])
    _success = _res.compiled() and _res.returncode() == 0
  else
    _success = _check[1]
  endif
  cdata.set(_check[0], _success ? 1 : false)
endforeach

cdata.set(
  'INET6',
  get_option(
    'ipv6',
  ).require(
    cdata.get('HAVE_AF_INET6').to_string() == '1',
    error_message: 'AF_INET6 is not available',
  ).allowed() ? 1 : false,
)

cdata.set(
  'HAVE_UTIME_NULL',
  cdata.get('HAVE_UTIME_ZERO').to_string() != '1' ? 1 : false,
)

_asm_optim_supported = host_machine.cpu_family() == 'x86_64'
_asm_optim_unsupported_error = 'ASM optimizations are currently x86_64|amd64 only'

_support_md5_asm = get_option(
  'md5-asm',
).disable_auto_if(crypto_dep.found()).require(
  _asm_optim_supported,
  error_message: _asm_optim_unsupported_error,
).allowed()
cdata.set('USE_MD5_ASM', _support_md5_asm ? 1 : false)
sumup += {'MD5 ASM optimizations': _support_md5_asm}

_roll_simd_opt = get_option(
  'roll-simd',
).require(
  _asm_optim_supported,
  error_message: _asm_optim_unsupported_error,
).require(
  cpp_language_enabled,
  error_message: 'need a C++ compiler',
)
if _roll_simd_opt.allowed() and meson.can_run_host_binaries()
  _args = feat_macros
  foreach _flag : ['HAVE_STDLIB_H']
    _args += '-D@0@=@1@'.format(_flag, cdata.get(_flag))
  endforeach
  _res = meson.get_compiler('cpp').run(
    '''
    #include <stdio.h>
    #if HAVE_STDLIB_H
    # include <stdlib.h>
    #endif
    #include <immintrin.h>
    __attribute__ ((target("default"))) int test_ssse3(int x) { return x; }
    __attribute__ ((target("default"))) int test_sse2(int x) { return x; }
    __attribute__ ((target("default"))) int test_avx2(int x) { return x; }
    __attribute__ ((target("ssse3"))) int test_ssse3(int x) { return x; }
    __attribute__ ((target("sse2"))) int test_sse2(int x) { return x; }
    __attribute__ ((target("avx2"))) int test_avx2(int x) { return x; }
    typedef long long __m128i_u __attribute__((__vector_size__(16), __may_alias__, __aligned__(1)));
    typedef long long __m256i_u __attribute__((__vector_size__(32), __may_alias__, __aligned__(1)));
    __attribute__ ((target("default"))) void more_testing(char* buf, int len) { }
    __attribute__ ((target("ssse3"))) void more_testing(char* buf, int len) {
        int i;
        for (i = 0; i < (len-32); i+=32) {
            __m128i in8_1, in8_2;
            in8_1 = _mm_lddqu_si128((__m128i_u*)&buf[i]);
            in8_2 = _mm_lddqu_si128((__m128i_u*)&buf[i + 16]);
        }
    }
    int main(void) {
      if (test_ssse3(42) != 42 || test_sse2(42) != 42 || test_avx2(42) != 42)
        return 1;
      return 0;
    }
    ''',
    args: _args,
    name: 'SIMD_X86_64_TEST',
  )
  _roll_simd_opt = _roll_simd_opt.require(
    _res.compiled() and _res.returncode() == 0,
    error_message: 'SIMD_X86_64_TEST failed!',
  )
endif
cdata.set('USE_ROLL_SIMD', _roll_simd_opt.allowed() ? 1 : false)
sumup += {'Rolling-checksum SIMD optimizations': _roll_simd_opt.allowed()}

lib_src = [
  'lib/compat.c',
  'lib/md5-asm-x86_64.S',
  'lib/md5.c',
  'lib/mdfour.c',
  'lib/permstring.c',
  'lib/pool_alloc.c',
  'lib/snprintf.c',
  'lib/sysacls.c',
  'lib/sysxattrs.c',
  'lib/wildmatch.c',
]

rsync_src = [
  'access.c',
  'acls.c',
  'authenticate.c',
  'backup.c',
  'batch.c',
  'checksum.c',
  'chmod.c',
  'cleanup.c',
  'clientname.c',
  'clientserver.c',
  'compat.c',
  'connection.c',
  'delete.c',
  'exclude.c',
  'fileio.c',
  'flist.c',
  'generator.c',
  'hashtable.c',
  'hlink.c',
  'io.c',
  'loadparm.c',
  'log.c',
  'main.c',
  'match.c',
  'options.c',
  'params.c',
  'pipe.c',
  'progress.c',
  'receiver.c',
  'rsync.c',
  'sender.c',
  'socket.c',
  'syscall.c',
  'token.c',
  'uidlist.c',
  'usage.c',
  'util1.c',
  'util2.c',
  'xattrs.c',
]

rsync_src_simd = []
if cpp_language_enabled and host_machine.cpu_family() == 'x86_64'
  rsync_src_simd += 'simd-checksum-x86_64.cpp'
endif

config_h = configure_file(
  configuration: cdata,
  input: 'config.meson.h',
  output: 'config.h',
)

daemon_parm_h = custom_target(
  command: [
    env,
    '--chdir', '@OUTDIR@',
    awk,
    '-f', meson.current_source_dir() / 'daemon-parm.awk',
    meson.current_source_dir() / 'daemon-parm.txt',
  ],
  input: ['daemon-parm.awk', 'daemon-parm.txt'],
  output: 'daemon-parm.h',
)

git_version_h = vcs_tag(
  input: 'git-version.meson.h',
  output: 'git-version.h',
)

proto_src = []
foreach _f : rsync_src + [
  'getfsdev.c',
  'getgroups.c',
  'lib/compat.c',
  'rounding.c',
  't_stub.c',
  't_unsafe.c',
  'testrun.c',
  'tls.c',
  'trimslash.c',
  'wildtest.c',
]
  proto_src += meson.current_source_dir() / _f
endforeach

proto_h = custom_target(
  command: [
    env,
    '--chdir', '@OUTDIR@',
    awk,
    '-f', meson.current_source_dir() / 'mkproto.awk',
    daemon_parm_h.full_path(),
    proto_src,
  ],
  input: ['mkproto.awk', daemon_parm_h, proto_src],
  output: 'proto.h',
)

rounding_h = custom_target(
  command: [
    find_program('sh'),
    '-c', '''
    exec >"$1/rounding.log" 2>&1;
    set -ex;
    b="$1";
    s="$2";
    shift 2;
    cd "$b";
    for r in 0 1 3;
    do
      if "$@" -DEXTRA_ROUNDING=$r -I"$s" -I"$b" -o /dev/null "$s/rounding.c"; then
        echo "#define EXTRA_ROUNDING $r" >"$b/rounding.h";
        break;
      fi;
    done
    '''.replace('\n', ''),
    '--',
    meson.current_build_dir(),
    meson.current_source_dir(),
    cc.cmd_array(),
  ],
  input: ['rounding.c', 'rsync.h', proto_h],
  output: 'rounding.h',
)

foreach _hdr, _script : {
  'default-cvsignore.h'    : 'define-from-md.awk',
  'default-dont-compress.h': 'define-from-md.awk',
  'help-rsync.h'           : 'help-from-md.awk',
  'help-rsyncd.h'          : 'help-from-md.awk',
}
  _tgt = custom_target(
    command: [
      env,
      '--chdir', '@OUTDIR@',
      awk,
      '-f', meson.current_source_dir() / _script,
      '-v', 'hfile=' + _hdr,
      '@INPUT1@',
    ],
    input: [_script, meson.current_source_dir() / 'rsync.1.md'],
    output: [_hdr],
  )
  set_variable(_hdr.underscorify(), _tgt)
endforeach

if not popt_dep.found()
  popt_dep = declare_dependency(
    include_directories: 'popt',
    link_with: static_library(
      'rsync_popt',
      [
        config_h,
        'popt/findme.c',
        'popt/popt.c',
        'popt/poptconfig.c',
        'popt/popthelp.c',
        'popt/poptparse.c',
      ],
    ),
  )
endif

if not zlib_dep.found()
  zlib_dep = declare_dependency(
    include_directories: 'zlib',
    link_with: static_library(
      'rsync_zlib',
      [
        proto_h,
        'zlib/adler32.c',
        'zlib/compress.c',
        'zlib/crc32.c',
        'zlib/deflate.c',
        'zlib/inffast.c',
        'zlib/inflate.c',
        'zlib/inftrees.c',
        'zlib/trees.c',
        'zlib/zutil.c',
      ],
    ),
  )
endif

rsync = executable(
  'rsync',
  [
    config_h,
    default_cvsignore_h,
    default_dont_compress_h,
    git_version_h,
    help_rsync_h,
    help_rsyncd_h,
    lib_src,
    proto_h,
    rounding_h,
    rsync_src,
    rsync_src_simd,
  ],
  dependencies: [
    acls_dep,
    charset_dep,
    crypto_dep,
    lz4_dep,
    popt_dep,
    xxhash_dep,
    zlib_dep,
    zstd_dep,
  ],
  install: true,
)

if get_option('tests')
  shconfig = configure_file(
    configuration: {
      'ECHO_T'       : '',
      'ECHO_N'       : '-n',
      'ECHO_C'       : '',
      'host_os'      : host_machine.system(),
      'SHELL_PATH'   : find_program('sh').full_path(),
      'FAKEROOT_PATH': find_program('fakeroot').full_path(),
    },
    input: 'shconfig.in',
    output: 'shconfig',
  )
  test_exes = [rsync]
  test_exes += executable('getfsdev', 'getfsdev.c')
  test_exes += executable('getgroups', 'getgroups.c')
  test_exes += executable(
    't_unsafe',
    [
      't_unsafe.c',
      'syscall.c',
      'util1.c',
      'util2.c',
      't_stub.c',
      'lib/compat.c',
      'lib/snprintf.c',
      'lib/wildmatch.c',
    ],
  )
  test_exes += executable(
    'testrun',
    'testrun.c',
    # Not used, by since we can't add it
    # to `test(…, dependencies: […])`,
    # here it goes…
    link_depends: shconfig,
  )
  test_exes += executable(
    'tls',
    [
      'lib/compat.c',
      'lib/permstring.c',
      'lib/snprintf.c',
      'lib/sysxattrs.c',
      'syscall.c',
      't_stub.c',
      'tls.c',
      'util2.c',
    ],
    dependencies: popt_dep,
  )
  test_exes += executable(
    'trimslash',
    [
      'lib/compat.c',
      'lib/snprintf.c',
      'syscall.c',
      't_stub.c',
      'trimslash.c',
      'util2.c',
    ],
  )
  test_exes += executable(
    'wildtest',
    [
      'lib/compat.c',
      'lib/snprintf.c',
      'wildtest.c',
    ],
    dependencies: popt_dep,
  )
  runtests = find_program('runtests.sh')
  foreach _name, _args : {
    'check': [],
    'check29': '--protocol=29',
    'check30': '--protocol=30',
  }
    test(
      _name,
      runtests,
      args: _args,
      depends: test_exes,
      is_parallel: false,
      timeout: 0,
    )
  endforeach
endif

summary(sumup, bool_yn: true)
