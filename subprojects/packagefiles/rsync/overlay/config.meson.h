/* config.h.  Generated from config.meson.h by meson.  */

/* Define to 1 if link() can hard-link special files. */
#mesondefine CAN_HARDLINK_SPECIAL

/* Define to 1 if link() can hard-link symlinks. */
#mesondefine CAN_HARDLINK_SYMLINK

/* Define to 1 if chown modifies symlinks. */
#mesondefine CHOWN_MODIFIES_SYMLINK

/* Undefine if you do not want locale features. By default this is defined. */
#mesondefine CONFIG_LOCALE

/* Define to 1 if using external zlib */
#mesondefine EXTERNAL_ZLIB

/* Used to make "checker" understand that FD_ZERO() clears memory. */
#mesondefine FORCE_FD_ZERO_MEMSET

/* Define to the type of elements in the array set by `getgroups'. Usually
   this is either `int' or `gid_t'. */
#mesondefine GETGROUPS_T

/* Define to 1 if the `getpgrp' function requires zero arguments. */
#mesondefine GETPGRP_VOID

/* true if you have acl_get_perm_np */
#mesondefine HAVE_ACL_GET_PERM_NP

/* Define to 1 if you have the <acl/libacl.h> header file. */
#mesondefine HAVE_ACL_LIBACL_H

/* true if you have AIX ACLs */
#mesondefine HAVE_AIX_ACLS

/* Define to 1 if you have 'alloca', as a function or macro. */
#mesondefine HAVE_ALLOCA

/* Define to 1 if <alloca.h> works. */
#mesondefine HAVE_ALLOCA_H

/* Define to 1 if you have the <arpa/inet.h> header file. */
#mesondefine HAVE_ARPA_INET_H

/* Define to 1 if you have the <arpa/nameser.h> header file. */
#mesondefine HAVE_ARPA_NAMESER_H

/* Define to 1 if you have the `asprintf' function. */
#mesondefine HAVE_ASPRINTF

/* Define to 1 if you have the <attr/xattr.h> header file. */
#mesondefine HAVE_ATTR_XATTR_H

/* Define to 1 if readdir() is broken */
#mesondefine HAVE_BROKEN_READDIR

/* Define to 1 if you have the <bsd/string.h> header file. */
#mesondefine HAVE_BSD_STRING_H

/* Define to 1 if vsprintf has a C99-compatible return value */
#mesondefine HAVE_C99_VSNPRINTF

/* Define to 1 if you have the `chmod' function. */
#mesondefine HAVE_CHMOD

/* Define to 1 if you have the `chown' function. */
#mesondefine HAVE_CHOWN

/* Define to 1 if you have the <compat.h> header file. */
#mesondefine HAVE_COMPAT_H

/* Define to 1 if you have the "connect" function */
#mesondefine HAVE_CONNECT

/* Define to 1 if you have the <ctype.h> header file. */
#mesondefine HAVE_CTYPE_H

/* Define to 1 if you have the <dirent.h> header file, and it defines `DIR'.
   */
#mesondefine HAVE_DIRENT_H

/* Define to 1 if you have the <dl.h> header file. */
#mesondefine HAVE_DL_H

/* Define if posix_fallocate is efficient (Cygwin) */
#mesondefine HAVE_EFFICIENT_POSIX_FALLOCATE

/* Define to 1 if errno is declared in errno.h */
#mesondefine HAVE_ERRNO_DECL

/* Define to 1 if you have the `extattr_get_link' function. */
#mesondefine HAVE_EXTATTR_GET_LINK

/* Define to 1 if you have the fallocate function and it compiles and links
   without error */
#mesondefine HAVE_FALLOCATE

/* Define if FALLOC_FL_PUNCH_HOLE is available. */
#mesondefine HAVE_FALLOC_FL_PUNCH_HOLE

/* Define if FALLOC_FL_ZERO_RANGE is available. */
#mesondefine HAVE_FALLOC_FL_ZERO_RANGE

/* Define to 1 if you have the `fchmod' function. */
#mesondefine HAVE_FCHMOD

/* Define to 1 if you have the <fcntl.h> header file. */
#mesondefine HAVE_FCNTL_H

/* Define to 1 if you have the <float.h> header file. */
#mesondefine HAVE_FLOAT_H

/* True if you have FreeBSD xattrs */
#mesondefine HAVE_FREEBSD_XATTRS

/* Define to 1 if you have the `ftruncate' function. */
#mesondefine HAVE_FTRUNCATE

/* Define to 1 if you have the "getaddrinfo" function and required types. */
#mesondefine HAVE_GETADDRINFO

/* Define to 1 if you have the `getattrlist' function. */
#mesondefine HAVE_GETATTRLIST

/* Define to 1 if you have the `getcwd' function. */
#mesondefine HAVE_GETCWD

/* Define to 1 if you have the `getegid' function. */
#mesondefine HAVE_GETEGID

/* Define to 1 if you have the `geteuid' function. */
#mesondefine HAVE_GETEUID

/* Define to 1 if you have the `getgrouplist' function. */
#mesondefine HAVE_GETGROUPLIST

/* Define to 1 if you have the `getgroups' function. */
#mesondefine HAVE_GETGROUPS

/* Define to 1 if you have the `getpass' function. */
#mesondefine HAVE_GETPASS

/* Define to 1 if you have the `getpgrp' function. */
#mesondefine HAVE_GETPGRP

/* Define to 1 if gettimeofday() takes a time-zone arg */
#mesondefine HAVE_GETTIMEOFDAY_TZ

/* Define to 1 if you have the <grp.h> header file. */
#mesondefine HAVE_GRP_H

/* true if you have HPUX ACLs */
#mesondefine HAVE_HPUX_ACLS

/* Define to 1 if you have the <iconv.h> header file. */
#mesondefine HAVE_ICONV_H

/* Define to 1 if you have the `iconv_open' function. */
#mesondefine HAVE_ICONV_OPEN

/* Define to 1 if the system has the type `id_t'. */
#mesondefine HAVE_ID_T

/* Define to 1 if you have the `inet_ntop' function. */
#mesondefine HAVE_INET_NTOP

/* Define to 1 if you have the `inet_pton' function. */
#mesondefine HAVE_INET_PTON

/* Define to 1 if you have the `initgroups' function. */
#mesondefine HAVE_INITGROUPS

/* Define to 1 if you have the `innetgr' function. */
#mesondefine HAVE_INNETGR

/* Define to 1 if you have the <inttypes.h> header file. */
#mesondefine HAVE_INTTYPES_H

/* true if you have IRIX ACLs */
#mesondefine HAVE_IRIX_ACLS

/* Define to 1 if you have the <langinfo.h> header file. */
#mesondefine HAVE_LANGINFO_H

/* Define to 1 if you have the `lchmod' function. */
#mesondefine HAVE_LCHMOD

/* Define to 1 if you have the `lchown' function. */
#mesondefine HAVE_LCHOWN

/* Define to 1 if you have the <libcharset.h> header file. */
#mesondefine HAVE_LIBCHARSET_H

/* Define to 1 if you have the <limits.h> header file. */
#mesondefine HAVE_LIMITS_H

/* Define to 1 if you have the `link' function. */
#mesondefine HAVE_LINK

/* Define to 1 if you have the `linkat' function. */
#mesondefine HAVE_LINKAT

/* Define to 1 if you have the <linux/falloc.h> header file. */
#mesondefine HAVE_LINUX_FALLOC_H

/* True if you have Linux xattrs (or equivalent) */
#mesondefine HAVE_LINUX_XATTRS

/* Define to 1 if you have the `locale_charset' function. */
#mesondefine HAVE_LOCALE_CHARSET

/* Define to 1 if you have the <locale.h> header file. */
#mesondefine HAVE_LOCALE_H

/* Define to 1 if the type `long double' works and has more range or precision
   than `double'. */
#mesondefine HAVE_LONG_DOUBLE

/* Define to 1 if the system has the type `long long'. */
#mesondefine HAVE_LONG_LONG

/* Define to 1 if you have the `lseek64' function. */
#mesondefine HAVE_LSEEK64

/* Define to 1 if you have the `lutimes' function. */
#mesondefine HAVE_LUTIMES

/* Define to 1 if you have the `mallinfo' function. */
#mesondefine HAVE_MALLINFO

/* Define to 1 if you have the `mallinfo2' function. */
#mesondefine HAVE_MALLINFO2

/* Define to 1 if you have the <malloc.h> header file. */
#mesondefine HAVE_MALLOC_H

/* Define to 1 if you have the <mcheck.h> header file. */
#mesondefine HAVE_MCHECK_H

/* Define to 1 if you have the `memcpy' function. */
#mesondefine HAVE_MEMCPY

/* Define to 1 if you have the `memmove' function. */
#mesondefine HAVE_MEMMOVE

/* Define to 1 if you have the `mkfifo' function. */
#mesondefine HAVE_MKFIFO

/* Define to 1 if you have the `mknod' function. */
#mesondefine HAVE_MKNOD

/* Define to 1 if you have the `mkstemp64' function. */
#mesondefine HAVE_MKSTEMP64

/* Define to 1 if you have the `mktime' function. */
#mesondefine HAVE_MKTIME

/* Define to 1 if the system has the type `mode_t'. */
#mesondefine HAVE_MODE_T

/* Define to 1 if you have the `nanosleep' function. */
#mesondefine HAVE_NANOSLEEP

/* Define to 1 if you have the <ndir.h> header file, and it defines `DIR'. */
#mesondefine HAVE_NDIR_H

/* Define to 1 if you have the <netdb.h> header file. */
#mesondefine HAVE_NETDB_H

/* Define to 1 if you have the <netgroup.h> header file. */
#mesondefine HAVE_NETGROUP_H

/* Define to 1 if you have the <netinet/in_systm.h> header file. */
#mesondefine HAVE_NETINET_IN_SYSTM_H

/* Define to 1 if you have the <netinet/ip.h> header file. */
#mesondefine HAVE_NETINET_IP_H

/* Define to 1 if you have the `nl_langinfo' function. */
#mesondefine HAVE_NL_LANGINFO

/* Define to 1 if the system has the type `off_t'. */
#mesondefine HAVE_OFF_T

/* Define to 1 if you have the `open64' function. */
#mesondefine HAVE_OPEN64

/* true if you have Mac OS X ACLs */
#mesondefine HAVE_OSX_ACLS

/* True if you have Mac OS X xattrs */
#mesondefine HAVE_OSX_XATTRS

/* Define to 1 if the system has the type `pid_t'. */
#mesondefine HAVE_PID_T

/* true if you have posix ACLs */
#mesondefine HAVE_POSIX_ACLS

/* Define to 1 if you have the `putenv' function. */
#mesondefine HAVE_PUTENV

/* Define to 1 if you have the `readlink' function. */
#mesondefine HAVE_READLINK

/* Define to 1 if remote shell is remsh, not rsh */
#mesondefine HAVE_REMSH

/* Define to 1 if mkstemp() is available and works right */
#mesondefine HAVE_SECURE_MKSTEMP

/* Define to 1 if you have the `setattrlist' function. */
#mesondefine HAVE_SETATTRLIST

/* Define to 1 if you have the `setenv' function. */
#mesondefine HAVE_SETENV

/* Define to 1 if you have the `seteuid' function. */
#mesondefine HAVE_SETEUID

/* Define to 1 if you have the `setgroups' function. */
#mesondefine HAVE_SETGROUPS

/* Define to 1 if you have the `setlocale' function. */
#mesondefine HAVE_SETLOCALE

/* Define to 1 if you have the `setmode' function. */
#mesondefine HAVE_SETMODE

/* Define to 1 if you have the `setsid' function. */
#mesondefine HAVE_SETSID

/* Define to 1 if you have the `setvbuf' function. */
#mesondefine HAVE_SETVBUF

/* Define to 1 if you have the `sigaction' function. */
#mesondefine HAVE_SIGACTION

/* Define to 1 if you have the `sigprocmask' function. */
#mesondefine HAVE_SIGPROCMASK

/* Define to 1 if the system has the type `size_t'. */
#mesondefine HAVE_SIZE_T

/* Define to 1 if you have the `snprintf' function. */
#mesondefine HAVE_SNPRINTF

/* Do we have sockaddr_in6.sin6_scope_id? */
#mesondefine HAVE_SOCKADDR_IN6_SCOPE_ID

/* Do we have sockaddr_in.sin_len? */
#mesondefine HAVE_SOCKADDR_IN_LEN

/* Do we have sockaddr.sa_len? */
#mesondefine HAVE_SOCKADDR_LEN

/* Do we have sockaddr_un.sun_len? */
#mesondefine HAVE_SOCKADDR_UN_LEN

/* Define to 1 if you have the "socketpair" function */
#mesondefine HAVE_SOCKETPAIR

/* true if you have solaris ACLs */
#mesondefine HAVE_SOLARIS_ACLS

/* True if you have Solaris xattrs */
#mesondefine HAVE_SOLARIS_XATTRS

/* Define to 1 if you have the <stdarg.h> header file. */
#mesondefine HAVE_STDARG_H

/* Define to 1 if you have the <stdint.h> header file. */
#mesondefine HAVE_STDINT_H

/* Define to 1 if you have the <stdio.h> header file. */
#mesondefine HAVE_STDIO_H

/* Define to 1 if you have the <stdlib.h> header file. */
#mesondefine HAVE_STDLIB_H

/* Define to 1 if you have the `strcasecmp' function. */
#mesondefine HAVE_STRCASECMP

/* Define to 1 if you have the `strchr' function. */
#mesondefine HAVE_STRCHR

/* Define to 1 if you have the `strerror' function. */
#mesondefine HAVE_STRERROR

/* Define to 1 if you have the `strftime' function. */
#mesondefine HAVE_STRFTIME

/* Define to 1 if you have the <strings.h> header file. */
#mesondefine HAVE_STRINGS_H

/* Define to 1 if you have the <string.h> header file. */
#mesondefine HAVE_STRING_H

/* Define to 1 if you have the `strlcat' function. */
#mesondefine HAVE_STRLCAT

/* Define to 1 if you have the `strlcpy' function. */
#mesondefine HAVE_STRLCPY

/* Define to 1 if you have the `strpbrk' function. */
#mesondefine HAVE_STRPBRK

/* Define to 1 if you have the `strtol' function. */
#mesondefine HAVE_STRTOL

/* Define to 1 if the system has the type `struct addrinfo'. */
#mesondefine HAVE_STRUCT_ADDRINFO

/* Define to 1 if the system has the type `struct sockaddr_storage'. */
#mesondefine HAVE_STRUCT_SOCKADDR_STORAGE

/* Define to 1 if the system has the type `struct stat64'. */
#mesondefine HAVE_STRUCT_STAT64

/* Define to 1 if `st_mtimensec' is a member of `struct stat'. */
#mesondefine HAVE_STRUCT_STAT_ST_MTIMENSEC

/* Define to 1 if `st_mtimespec.tv_nsec' is a member of `struct stat'. */
#mesondefine HAVE_STRUCT_STAT_ST_MTIMESPEC_TV_NSEC

/* Define to 1 if `st_mtim.tv_nsec' is a member of `struct stat'. */
#mesondefine HAVE_STRUCT_STAT_ST_MTIM_TV_NSEC

/* Define to 1 if `st_rdev' is a member of `struct stat'. */
#mesondefine HAVE_STRUCT_STAT_ST_RDEV

/* Define to 1 if you have the "struct utimbuf" type */
#mesondefine HAVE_STRUCT_UTIMBUF

/* Define to 1 if you have the <sys/acl.h> header file. */
#mesondefine HAVE_SYS_ACL_H

/* Define to 1 if you have the <sys/attr.h> header file. */
#mesondefine HAVE_SYS_ATTR_H

/* Define to 1 if you have the <sys/dir.h> header file, and it defines `DIR'.
   */
#mesondefine HAVE_SYS_DIR_H

/* Define to 1 if you have the <sys/extattr.h> header file. */
#mesondefine HAVE_SYS_EXTATTR_H

/* Define to 1 if you have the SYS_fallocate syscall number */
#mesondefine HAVE_SYS_FALLOCATE

/* Define to 1 if you have the <sys/fcntl.h> header file. */
#mesondefine HAVE_SYS_FCNTL_H

/* Define to 1 if you have the <sys/file.h> header file. */
#mesondefine HAVE_SYS_FILE_H

/* Define to 1 if you have the <sys/filio.h> header file. */
#mesondefine HAVE_SYS_FILIO_H

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#mesondefine HAVE_SYS_IOCTL_H

/* Define to 1 if you have the <sys/mode.h> header file. */
#mesondefine HAVE_SYS_MODE_H

/* Define to 1 if you have the <sys/ndir.h> header file, and it defines `DIR'.
   */
#mesondefine HAVE_SYS_NDIR_H

/* Define to 1 if you have the <sys/param.h> header file. */
#mesondefine HAVE_SYS_PARAM_H

/* Define to 1 if you have the <sys/select.h> header file. */
#mesondefine HAVE_SYS_SELECT_H

/* Define to 1 if you have the <sys/socket.h> header file. */
#mesondefine HAVE_SYS_SOCKET_H

/* Define to 1 if you have the <sys/stat.h> header file. */
#mesondefine HAVE_SYS_STAT_H

/* Define to 1 if you have the <sys/time.h> header file. */
#mesondefine HAVE_SYS_TIME_H

/* Define to 1 if you have the <sys/types.h> header file. */
#mesondefine HAVE_SYS_TYPES_H

/* Define to 1 if you have the <sys/unistd.h> header file. */
#mesondefine HAVE_SYS_UNISTD_H

/* Define to 1 if you have the <sys/un.h> header file. */
#mesondefine HAVE_SYS_UN_H

/* Define to 1 if you have the <sys/wait.h> header file. */
#mesondefine HAVE_SYS_WAIT_H

/* Define to 1 if you have the <sys/xattr.h> header file. */
#mesondefine HAVE_SYS_XATTR_H

/* Define to 1 if you have the `tcgetpgrp' function. */
#mesondefine HAVE_TCGETPGRP

/* true if you have Tru64 ACLs */
#mesondefine HAVE_TRU64_ACLS

/* Define to 1 if you have the <unistd.h> header file. */
#mesondefine HAVE_UNISTD_H

/* true if you have UnixWare ACLs */
#mesondefine HAVE_UNIXWARE_ACLS

/* Define to 1 if you have the `unsetenv' function. */
#mesondefine HAVE_UNSETENV

/* Define to 1 if you have the `usleep' function. */
#mesondefine HAVE_USLEEP

/* Define to 1 if you have the `utime' function. */
#mesondefine HAVE_UTIME

/* Define to 1 if you have the `utimensat' function. */
#mesondefine HAVE_UTIMENSAT

/* Define to 1 if you have the `utimes' function. */
#mesondefine HAVE_UTIMES

/* Define to 1 if you have the <utime.h> header file. */
#mesondefine HAVE_UTIME_H

/* Define to 1 if `utime(file, NULL)' sets file's timestamp to the present. */
#mesondefine HAVE_UTIME_NULL

/* Define to 1 if you have the `vasprintf' function. */
#mesondefine HAVE_VASPRINTF

/* Define to 1 if you have the `va_copy' function. */
#mesondefine HAVE_VA_COPY

/* Define to 1 if you have the `vsnprintf' function. */
#mesondefine HAVE_VSNPRINTF

/* Define to 1 if you have the `wait4' function. */
#mesondefine HAVE_WAIT4

/* Define to 1 if you have the `waitpid' function. */
#mesondefine HAVE_WAITPID

/* Define to 1 if you have the `__va_copy' function. */
#mesondefine HAVE___VA_COPY

/* Define as const if the declaration of iconv() needs const. */
#mesondefine ICONV_CONST

/* Define if you want the --iconv option. Specifying a value will set the
   default iconv setting (a NULL means no --iconv processing by default). */
#mesondefine ICONV_OPTION

/* true if you have IPv6 */
#mesondefine INET6

/* Define to 1 if `major', `minor', and `makedev' are declared in <mkdev.h>.
   */
#mesondefine MAJOR_IN_MKDEV

/* Define to 1 if `major', `minor', and `makedev' are declared in
   <sysmacros.h>. */
#mesondefine MAJOR_IN_SYSMACROS

/* Define to 1 if makedev() takes 3 args */
#mesondefine MAKEDEV_TAKES_3_ARGS

/* Define to 1 if mknod() can create FIFOs. */
#mesondefine MKNOD_CREATES_FIFOS

/* Define to 1 if mknod() can create sockets. */
#mesondefine MKNOD_CREATES_SOCKETS

/* unprivileged group for unprivileged user */
#mesondefine NOBODY_GROUP

/* unprivileged user--e.g. nobody */
#mesondefine NOBODY_USER

/* True if device files do not support xattrs */
#mesondefine NO_DEVICE_XATTRS

/* True if special files do not support xattrs */
#mesondefine NO_SPECIAL_XATTRS

/* True if symlinks do not support user xattrs */
#mesondefine NO_SYMLINK_USER_XATTRS

/* True if symlinks do not support xattrs */
#mesondefine NO_SYMLINK_XATTRS

/* location of configuration file for rsync server */
#mesondefine RSYNCD_SYSCONF

/* location of rsync on remote machine */
#mesondefine RSYNC_PATH

/* default -e command */
#mesondefine RSYNC_RSH

/* Define to 1 if --secluded-args should be the default */
#mesondefine RSYNC_USE_SECLUDED_ARGS

/* Define to 1 if sockets need to be shutdown */
#mesondefine SHUTDOWN_ALL_SOCKETS

/* Define to 1 if "signed char" is a valid type */
#mesondefine SIGNED_CHAR_OK

/* The size of `char*', as computed by sizeof. */
#mesondefine SIZEOF_CHARP

/* The size of `int', as computed by sizeof. */
#mesondefine SIZEOF_INT

/* The size of `int16_t', as computed by sizeof. */
#mesondefine SIZEOF_INT16_T

/* The size of `int32_t', as computed by sizeof. */
#mesondefine SIZEOF_INT32_T

/* The size of `int64_t', as computed by sizeof. */
#mesondefine SIZEOF_INT64_T

/* The size of `long', as computed by sizeof. */
#mesondefine SIZEOF_LONG

/* The size of `long long', as computed by sizeof. */
#mesondefine SIZEOF_LONG_LONG

/* The size of `off64_t', as computed by sizeof. */
#mesondefine SIZEOF_OFF64_T

/* The size of `off_t', as computed by sizeof. */
#mesondefine SIZEOF_OFF_T

/* The size of `short', as computed by sizeof. */
#mesondefine SIZEOF_SHORT

/* The size of `time_t', as computed by sizeof. */
#mesondefine SIZEOF_TIME_T

/* The size of `uint16_t', as computed by sizeof. */
#mesondefine SIZEOF_UINT16_T

/* The size of `uint32_t', as computed by sizeof. */
#mesondefine SIZEOF_UINT32_T

/* Define to 1 if all of the C90 standard headers exist (not just the ones
   required in a freestanding environment). This macro is provided for
   backward compatibility; new code need not use it. */
#mesondefine STDC_HEADERS

/* Define to 1 to add support for ACLs */
#mesondefine SUPPORT_ACLS

/* Undefine if you do not want LZ4 compression. By default this is defined. */
#mesondefine SUPPORT_LZ4

/* Define to 1 to add support for extended attributes */
#mesondefine SUPPORT_XATTRS

/* Undefine if you do not want xxhash checksums. By default this is defined.
   */
#mesondefine SUPPORT_XXHASH

/* Undefine if you do not want zstd compression. By default this is defined.
   */
#mesondefine SUPPORT_ZSTD

/* Define to 1 if you want rsync to make use of iconv_open() */
#mesondefine USE_ICONV_OPEN

/* Define to 1 to enable MD5 ASM optimizations */
#mesondefine USE_MD5_ASM

/* Undefine if you do not want to use openssl crypto library. By default this
   is defined. */
#mesondefine USE_OPENSSL

/* Define to 1 to enable rolling-checksum ASM optimizations (requires
   --enable-roll-simd) */
#mesondefine USE_ROLL_ASM

/* Define to 1 to enable rolling-checksum SIMD optimizations */
#mesondefine USE_ROLL_SIMD

/* String to pass to iconv() for the UTF-8 charset. */
#mesondefine UTF8_CHARSET

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#mesondefine WORDS_BIGENDIAN

/* Define _GNU_SOURCE so that we get all necessary prototypes */
#mesondefine _GNU_SOURCE

/* Define for large files, on AIX-style hosts. */
#mesondefine _LARGE_FILES

/* Define to `int' if <sys/types.h> doesn't define. */
#mesondefine gid_t

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
#mesondefine inline
#endif

/* Define to `unsigned int' if <sys/types.h> does not define. */
#mesondefine size_t

/* type to use in place of socklen_t if not defined */
#mesondefine socklen_t

/* Define to `int' if <sys/types.h> doesn't define. */
#mesondefine uid_t