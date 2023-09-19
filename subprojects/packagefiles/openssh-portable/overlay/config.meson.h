/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define if building universal (internal helper macro) */
#mesondefine AC_APPLE_UNIVERSAL_BUILD

/* Define if you have a getaddrinfo that fails for the all-zeros IPv6 address
   */
#mesondefine AIX_GETNAMEINFO_HACK

/* Define if your AIX loginfailed() function takes 4 arguments (AIX >= 5.2) */
#mesondefine AIX_LOGINFAILED_4ARG

/* System only supports IPv4 audit records */
#mesondefine AU_IPv4

/* Define if your resolver libs need this for getrrsetbyname */
#mesondefine BIND_8_COMPAT

/* The system has incomplete BSM API */
#mesondefine BROKEN_BSM_API

/* Define if cmsg_type is not passed correctly */
#mesondefine BROKEN_CMSG_TYPE

/* getaddrinfo is broken (if present) */
#mesondefine BROKEN_GETADDRINFO

/* getgroups(0,NULL) will return -1 */
#mesondefine BROKEN_GETGROUPS

/* FreeBSD glob does not do what we need */
#mesondefine BROKEN_GLOB

/* Define if you system's inet_ntoa is busted (e.g. Irix gcc issue) */
#mesondefine BROKEN_INET_NTOA

/* Define if your struct dirent expects you to allocate extra space for d_name
   */
#mesondefine BROKEN_ONE_BYTE_DIRENT_D_NAME

/* Can't do comparisons on readv */
#mesondefine BROKEN_READV_COMPARISON

/* NetBSD read function is sometimes redirected, breaking atomicio comparisons
   against it */
#mesondefine BROKEN_READ_COMPARISON

/* Needed for NeXT */
#mesondefine BROKEN_SAVED_UIDS

/* Define if your setregid() is broken */
#mesondefine BROKEN_SETREGID

/* Define if your setresgid() is broken */
#mesondefine BROKEN_SETRESGID

/* Define if your setresuid() is broken */
#mesondefine BROKEN_SETRESUID

/* Define if your setreuid() is broken */
#mesondefine BROKEN_SETREUID

/* LynxOS has broken setvbuf() implementation */
#mesondefine BROKEN_SETVBUF

/* QNX shadow support is broken */
#mesondefine BROKEN_SHADOW_EXPIRE

/* Define if your snprintf is busted */
#mesondefine BROKEN_SNPRINTF

/* strndup broken, see APAR IY61211 */
#mesondefine BROKEN_STRNDUP

/* strnlen broken, see APAR IY62551 */
#mesondefine BROKEN_STRNLEN

/* strnvis detected broken */
#mesondefine BROKEN_STRNVIS

/* tcgetattr with ICANON may hang */
#mesondefine BROKEN_TCGETATTR_ICANON

/* updwtmpx is broken (if present) */
#mesondefine BROKEN_UPDWTMPX

/* Define if you have BSD auth support */
#mesondefine BSD_AUTH

/* Define if you want to specify the path to your lastlog file */
#mesondefine CONF_LASTLOG_FILE

/* Define if you want to specify the path to your utmp file */
#mesondefine CONF_UTMP_FILE

/* Define if you want to specify the path to your wtmpx file */
#mesondefine CONF_WTMPX_FILE

/* Define if you want to specify the path to your wtmp file */
#mesondefine CONF_WTMP_FILE

/* Need to call setpgrp as root */
#mesondefine DISABLE_FD_PASSING

/* Define if you don't want to use lastlog */
#mesondefine DISABLE_LASTLOG

/* Define if you don't want to use your system's login() call */
#mesondefine DISABLE_LOGIN

/* Define if you don't want to use pututline() etc. to write [uw]tmp */
#mesondefine DISABLE_PUTUTLINE

/* Define if you don't want to use pututxline() etc. to write [uw]tmpx */
#mesondefine DISABLE_PUTUTXLINE

/* Define if you want to disable shadow passwords */
#mesondefine DISABLE_SHADOW

/* Define if you don't want to use utmp */
#mesondefine DISABLE_UTMP

/* Define if you don't want to use utmpx */
#mesondefine DISABLE_UTMPX

/* Define if you don't want to use wtmp */
#mesondefine DISABLE_WTMP

/* Define if you don't want to use wtmpx */
#mesondefine DISABLE_WTMPX

/* Enable for PKCS#11 support */
#mesondefine ENABLE_PKCS11

/* Enable for U2F/FIDO support */
#mesondefine ENABLE_SK

/* Enable for built-in U2F/FIDO support */
#mesondefine ENABLE_SK_INTERNAL

/* define if fflush(NULL) does not work */
#mesondefine FFLUSH_NULL_BUG

/* File names may not contain backslash characters */
#mesondefine FILESYSTEM_NO_BACKSLASH

/* fsid_t has member val */
#mesondefine FSID_HAS_VAL

/* fsid_t has member __val */
#mesondefine FSID_HAS___VAL

/* getpgrp takes one arg */
#mesondefine GETPGRP_VOID

/* Conflicting defs for getspnam */
#mesondefine GETSPNAM_CONFLICTING_DEFS

/* Define if your system glob() function has the GLOB_ALTDIRFUNC extension */
#mesondefine GLOB_HAS_ALTDIRFUNC

/* Define if your system glob() function has gl_matchc options in glob_t */
#mesondefine GLOB_HAS_GL_MATCHC

/* Define if your system glob() function has gl_statv options in glob_t */
#mesondefine GLOB_HAS_GL_STATV

/* Define this if you want GSSAPI support in the version 2 protocol */
#mesondefine GSSAPI

/* Define if you want to use shadow password expire field */
#mesondefine HAS_SHADOW_EXPIRE

/* Define if your system uses access rights style file descriptor passing */
#mesondefine HAVE_ACCRIGHTS_IN_MSGHDR

/* Define if you have ut_addr in utmp.h */
#mesondefine HAVE_ADDR_IN_UTMP

/* Define if you have ut_addr in utmpx.h */
#mesondefine HAVE_ADDR_IN_UTMPX

/* Define if you have ut_addr_v6 in utmp.h */
#mesondefine HAVE_ADDR_V6_IN_UTMP

/* Define if you have ut_addr_v6 in utmpx.h */
#mesondefine HAVE_ADDR_V6_IN_UTMPX

/* Define to 1 if you have the `arc4random' function. */
#mesondefine HAVE_ARC4RANDOM

/* Define to 1 if you have the `arc4random_buf' function. */
#mesondefine HAVE_ARC4RANDOM_BUF

/* Define to 1 if you have the `arc4random_stir' function. */
#mesondefine HAVE_ARC4RANDOM_STIR

/* Define to 1 if you have the `arc4random_uniform' function. */
#mesondefine HAVE_ARC4RANDOM_UNIFORM

/* Define to 1 if you have the `asprintf' function. */
#mesondefine HAVE_ASPRINTF

/* OpenBSD's gcc has bounded */
#mesondefine HAVE_ATTRIBUTE__BOUNDED__

/* Have attribute nonnull */
#mesondefine HAVE_ATTRIBUTE__NONNULL__

/* OpenBSD's gcc has sentinel */
#mesondefine HAVE_ATTRIBUTE__SENTINEL__

/* Define to 1 if you have the `aug_get_machine' function. */
#mesondefine HAVE_AUG_GET_MACHINE

/* Define to 1 if you have the `b64_ntop' function. */
#mesondefine HAVE_B64_NTOP

/* Define to 1 if you have the `b64_pton' function. */
#mesondefine HAVE_B64_PTON

/* Define if you have the basename function. */
#mesondefine HAVE_BASENAME

/* Define to 1 if you have the `bcopy' function. */
#mesondefine HAVE_BCOPY

/* Define to 1 if you have the `bcrypt_pbkdf' function. */
#mesondefine HAVE_BCRYPT_PBKDF

/* Define to 1 if you have the `bindresvport_sa' function. */
#mesondefine HAVE_BINDRESVPORT_SA

/* Define to 1 if you have the `blf_enc' function. */
#mesondefine HAVE_BLF_ENC

/* Define to 1 if you have the <blf.h> header file. */
#mesondefine HAVE_BLF_H

/* Define to 1 if you have the `Blowfish_expand0state' function. */
#mesondefine HAVE_BLOWFISH_EXPAND0STATE

/* Define to 1 if you have the `Blowfish_expandstate' function. */
#mesondefine HAVE_BLOWFISH_EXPANDSTATE

/* Define to 1 if you have the `Blowfish_initstate' function. */
#mesondefine HAVE_BLOWFISH_INITSTATE

/* Define to 1 if you have the `Blowfish_stream2word' function. */
#mesondefine HAVE_BLOWFISH_STREAM2WORD

/* Define to 1 if you have the <bsd/libutil.h> header file. */
#mesondefine HAVE_BSD_LIBUTIL_H

/* Define to 1 if you have the <bsm/audit.h> header file. */
#mesondefine HAVE_BSM_AUDIT_H

/* Define to 1 if you have the <bstring.h> header file. */
#mesondefine HAVE_BSTRING_H

/* Define to 1 if you have the `bzero' function. */
#mesondefine HAVE_BZERO

/* calloc(0, x) returns NULL */
#mesondefine HAVE_CALLOC

/* Define to 1 if you have the `cap_rights_limit' function. */
#mesondefine HAVE_CAP_RIGHTS_LIMIT

/* Define to 1 if you have the `clock' function. */
#mesondefine HAVE_CLOCK

/* Have clock_gettime */
#mesondefine HAVE_CLOCK_GETTIME

/* define if you have clock_t data type */
#mesondefine HAVE_CLOCK_T

/* Define to 1 if you have the `closefrom' function. */
#mesondefine HAVE_CLOSEFROM

/* Define if gai_strerror() returns const char * */
#mesondefine HAVE_CONST_GAI_STRERROR_PROTO

/* Define if your system uses ancillary data style file descriptor passing */
#mesondefine HAVE_CONTROL_IN_MSGHDR

/* Define to 1 if you have the `crypt' function. */
#mesondefine HAVE_CRYPT

/* Define to 1 if you have the <crypto/sha2.h> header file. */
#mesondefine HAVE_CRYPTO_SHA2_H

/* Define to 1 if you have the <crypt.h> header file. */
#mesondefine HAVE_CRYPT_H

/* Define if you are on Cygwin */
#mesondefine HAVE_CYGWIN

/* Define if your libraries define daemon() */
#mesondefine HAVE_DAEMON

/* Define to 1 if you have the declaration of `AI_NUMERICSERV', and to 0 if
   you don't. */
#mesondefine HAVE_DECL_AI_NUMERICSERV

/* Define to 1 if you have the declaration of `authenticate', and to 0 if you
   don't. */
#mesondefine HAVE_DECL_AUTHENTICATE

/* Define to 1 if you have the declaration of `bzero', and to 0 if you don't.
   */
#mesondefine HAVE_DECL_BZERO

/* Define to 1 if you have the declaration of `getpeereid', and to 0 if you
   don't. */
#mesondefine HAVE_DECL_GETPEEREID

/* Define to 1 if you have the declaration of `GLOB_NOMATCH', and to 0 if you
   don't. */
#mesondefine HAVE_DECL_GLOB_NOMATCH

/* Define to 1 if you have the declaration of `GSS_C_NT_HOSTBASED_SERVICE',
   and to 0 if you don't. */
#mesondefine HAVE_DECL_GSS_C_NT_HOSTBASED_SERVICE

/* Define to 1 if you have the declaration of `howmany', and to 0 if you
   don't. */
#mesondefine HAVE_DECL_HOWMANY

/* Define to 1 if you have the declaration of `h_errno', and to 0 if you
   don't. */
#mesondefine HAVE_DECL_H_ERRNO

/* Define to 1 if you have the declaration of `loginfailed', and to 0 if you
   don't. */
#mesondefine HAVE_DECL_LOGINFAILED

/* Define to 1 if you have the declaration of `loginrestrictions', and to 0 if
   you don't. */
#mesondefine HAVE_DECL_LOGINRESTRICTIONS

/* Define to 1 if you have the declaration of `loginsuccess', and to 0 if you
   don't. */
#mesondefine HAVE_DECL_LOGINSUCCESS

/* Define to 1 if you have the declaration of `MAXSYMLINKS', and to 0 if you
   don't. */
#mesondefine HAVE_DECL_MAXSYMLINKS

/* Define to 1 if you have the declaration of `memmem', and to 0 if you don't.
   */
#mesondefine HAVE_DECL_MEMMEM

/* Define to 1 if you have the declaration of `NFDBITS', and to 0 if you
   don't. */
#mesondefine HAVE_DECL_NFDBITS

/* Define to 1 if you have the declaration of `offsetof', and to 0 if you
   don't. */
#mesondefine HAVE_DECL_OFFSETOF

/* Define to 1 if you have the declaration of `O_NONBLOCK', and to 0 if you
   don't. */
#mesondefine HAVE_DECL_O_NONBLOCK

/* Define to 1 if you have the declaration of `passwdexpired', and to 0 if you
   don't. */
#mesondefine HAVE_DECL_PASSWDEXPIRED

/* Define to 1 if you have the declaration of `readv', and to 0 if you don't.
   */
#mesondefine HAVE_DECL_READV

/* Define to 1 if you have the declaration of `setauthdb', and to 0 if you
   don't. */
#mesondefine HAVE_DECL_SETAUTHDB

/* Define to 1 if you have the declaration of `SHUT_RD', and to 0 if you
   don't. */
#mesondefine HAVE_DECL_SHUT_RD

/* Define to 1 if you have the declaration of `UINT32_MAX', and to 0 if you
   don't. */
#mesondefine HAVE_DECL_UINT32_MAX

/* Define to 1 if you have the declaration of `writev', and to 0 if you don't.
   */
#mesondefine HAVE_DECL_WRITEV

/* Define to 1 if you have the declaration of `_getlong', and to 0 if you
   don't. */
#mesondefine HAVE_DECL__GETLONG

/* Define to 1 if you have the declaration of `_getshort', and to 0 if you
   don't. */
#mesondefine HAVE_DECL__GETSHORT

/* Define to 1 if you have the `DES_crypt' function. */
#mesondefine HAVE_DES_CRYPT

/* Define if you have /dev/ptmx */
#mesondefine HAVE_DEV_PTMX

/* Define if you have /dev/ptc */
#mesondefine HAVE_DEV_PTS_AND_PTC

/* Define to 1 if you have the `DH_get0_key' function. */
#mesondefine HAVE_DH_GET0_KEY

/* Define to 1 if you have the `DH_get0_pqg' function. */
#mesondefine HAVE_DH_GET0_PQG

/* Define to 1 if you have the `DH_set0_key' function. */
#mesondefine HAVE_DH_SET0_KEY

/* Define to 1 if you have the `DH_set0_pqg' function. */
#mesondefine HAVE_DH_SET0_PQG

/* Define to 1 if you have the `DH_set_length' function. */
#mesondefine HAVE_DH_SET_LENGTH

/* Define to 1 if you have the <dirent.h> header file. */
#mesondefine HAVE_DIRENT_H

/* Define to 1 if you have the `dirfd' function. */
#mesondefine HAVE_DIRFD

/* Define to 1 if you have the `dirname' function. */
#mesondefine HAVE_DIRNAME

/* Define to 1 if you have the `dlopen' function. */
#mesondefine HAVE_DLOPEN

/* Define to 1 if you have the `DSA_generate_parameters_ex' function. */
#mesondefine HAVE_DSA_GENERATE_PARAMETERS_EX

/* Define to 1 if you have the `DSA_get0_key' function. */
#mesondefine HAVE_DSA_GET0_KEY

/* Define to 1 if you have the `DSA_get0_pqg' function. */
#mesondefine HAVE_DSA_GET0_PQG

/* Define to 1 if you have the `DSA_set0_key' function. */
#mesondefine HAVE_DSA_SET0_KEY

/* Define to 1 if you have the `DSA_set0_pqg' function. */
#mesondefine HAVE_DSA_SET0_PQG

/* Define to 1 if you have the `DSA_SIG_get0' function. */
#mesondefine HAVE_DSA_SIG_GET0

/* Define to 1 if you have the `DSA_SIG_set0' function. */
#mesondefine HAVE_DSA_SIG_SET0

/* Define to 1 if you have the `ECDSA_SIG_get0' function. */
#mesondefine HAVE_ECDSA_SIG_GET0

/* Define to 1 if you have the `ECDSA_SIG_set0' function. */
#mesondefine HAVE_ECDSA_SIG_SET0

/* Define to 1 if you have the `EC_KEY_METHOD_new' function. */
#mesondefine HAVE_EC_KEY_METHOD_NEW

/* Define to 1 if you have the <elf.h> header file. */
#mesondefine HAVE_ELF_H

/* Define to 1 if you have the `endgrent' function. */
#mesondefine HAVE_ENDGRENT

/* Define to 1 if you have the <endian.h> header file. */
#mesondefine HAVE_ENDIAN_H

/* Define to 1 if you have the `endutent' function. */
#mesondefine HAVE_ENDUTENT

/* Define to 1 if you have the `endutxent' function. */
#mesondefine HAVE_ENDUTXENT

/* Define to 1 if you have the `err' function. */
#mesondefine HAVE_ERR

/* Define to 1 if you have the `errx' function. */
#mesondefine HAVE_ERRX

/* Define to 1 if you have the <err.h> header file. */
#mesondefine HAVE_ERR_H

/* Define if your system has /etc/default/login */
#mesondefine HAVE_ETC_DEFAULT_LOGIN

/* Define to 1 if you have the `EVP_chacha20' function. */
#mesondefine HAVE_EVP_CHACHA20

/* Define to 1 if you have the `EVP_CIPHER_CTX_ctrl' function. */
#mesondefine HAVE_EVP_CIPHER_CTX_CTRL

/* Define to 1 if you have the `EVP_CIPHER_CTX_get_iv' function. */
#mesondefine HAVE_EVP_CIPHER_CTX_GET_IV

/* Define to 1 if you have the `EVP_CIPHER_CTX_get_updated_iv' function. */
#mesondefine HAVE_EVP_CIPHER_CTX_GET_UPDATED_IV

/* Define to 1 if you have the `EVP_CIPHER_CTX_iv' function. */
#mesondefine HAVE_EVP_CIPHER_CTX_IV

/* Define to 1 if you have the `EVP_CIPHER_CTX_iv_noconst' function. */
#mesondefine HAVE_EVP_CIPHER_CTX_IV_NOCONST

/* Define to 1 if you have the `EVP_CIPHER_CTX_set_iv' function. */
#mesondefine HAVE_EVP_CIPHER_CTX_SET_IV

/* Define to 1 if you have the `EVP_DigestFinal_ex' function. */
#mesondefine HAVE_EVP_DIGESTFINAL_EX

/* Define to 1 if you have the `EVP_DigestInit_ex' function. */
#mesondefine HAVE_EVP_DIGESTINIT_EX

/* Define to 1 if you have the `EVP_MD_CTX_cleanup' function. */
#mesondefine HAVE_EVP_MD_CTX_CLEANUP

/* Define to 1 if you have the `EVP_MD_CTX_copy_ex' function. */
#mesondefine HAVE_EVP_MD_CTX_COPY_EX

/* Define to 1 if you have the `EVP_MD_CTX_free' function. */
#mesondefine HAVE_EVP_MD_CTX_FREE

/* Define to 1 if you have the `EVP_MD_CTX_init' function. */
#mesondefine HAVE_EVP_MD_CTX_INIT

/* Define to 1 if you have the `EVP_MD_CTX_new' function. */
#mesondefine HAVE_EVP_MD_CTX_NEW

/* Define to 1 if you have the `EVP_PKEY_get0_RSA' function. */
#mesondefine HAVE_EVP_PKEY_GET0_RSA

/* Define to 1 if you have the `EVP_sha256' function. */
#mesondefine HAVE_EVP_SHA256

/* Define to 1 if you have the `EVP_sha384' function. */
#mesondefine HAVE_EVP_SHA384

/* Define to 1 if you have the `EVP_sha512' function. */
#mesondefine HAVE_EVP_SHA512

/* Define if you have ut_exit in utmp.h */
#mesondefine HAVE_EXIT_IN_UTMP

/* Define to 1 if you have the `explicit_bzero' function. */
#mesondefine HAVE_EXPLICIT_BZERO

/* Define to 1 if you have the `explicit_memset' function. */
#mesondefine HAVE_EXPLICIT_MEMSET

/* Define to 1 if you have the `fchmod' function. */
#mesondefine HAVE_FCHMOD

/* Define to 1 if you have the `fchmodat' function. */
#mesondefine HAVE_FCHMODAT

/* Define to 1 if you have the `fchown' function. */
#mesondefine HAVE_FCHOWN

/* Define to 1 if you have the `fchownat' function. */
#mesondefine HAVE_FCHOWNAT

/* Use F_CLOSEM fcntl for closefrom */
#mesondefine HAVE_FCNTL_CLOSEM

/* Define to 1 if you have the <fcntl.h> header file. */
#mesondefine HAVE_FCNTL_H

/* Define to 1 if the system has the type `fd_mask'. */
#mesondefine HAVE_FD_MASK

/* Define to 1 if you have the <features.h> header file. */
#mesondefine HAVE_FEATURES_H

/* Define to 1 if you have the `fido_cred_prot' function. */
#mesondefine HAVE_FIDO_CRED_PROT

/* Define to 1 if you have the `fido_cred_set_prot' function. */
#mesondefine HAVE_FIDO_CRED_SET_PROT

/* Define to 1 if you have the `fido_dev_get_touch_begin' function. */
#mesondefine HAVE_FIDO_DEV_GET_TOUCH_BEGIN

/* Define to 1 if you have the `fido_dev_get_touch_status' function. */
#mesondefine HAVE_FIDO_DEV_GET_TOUCH_STATUS

/* Define to 1 if you have the `fido_dev_supports_cred_prot' function. */
#mesondefine HAVE_FIDO_DEV_SUPPORTS_CRED_PROT

/* Define to 1 if you have the <floatingpoint.h> header file. */
#mesondefine HAVE_FLOATINGPOINT_H

/* Define to 1 if you have the `flock' function. */
#mesondefine HAVE_FLOCK

/* Define to 1 if you have the `fmt_scaled' function. */
#mesondefine HAVE_FMT_SCALED

/* Define to 1 if you have the `fnmatch' function. */
#mesondefine HAVE_FNMATCH

/* Define to 1 if you have the <fnmatch.h> header file. */
#mesondefine HAVE_FNMATCH_H

/* Define to 1 if you have the `freeaddrinfo' function. */
#mesondefine HAVE_FREEADDRINFO

/* Define to 1 if you have the `freezero' function. */
#mesondefine HAVE_FREEZERO

/* Define to 1 if the system has the type `fsblkcnt_t'. */
#mesondefine HAVE_FSBLKCNT_T

/* Define to 1 if the system has the type `fsfilcnt_t'. */
#mesondefine HAVE_FSFILCNT_T

/* Define to 1 if you have the `fstatfs' function. */
#mesondefine HAVE_FSTATFS

/* Define to 1 if you have the `fstatvfs' function. */
#mesondefine HAVE_FSTATVFS

/* Define to 1 if you have the `futimes' function. */
#mesondefine HAVE_FUTIMES

/* Define to 1 if you have the `gai_strerror' function. */
#mesondefine HAVE_GAI_STRERROR

/* Define to 1 if you have the `getaddrinfo' function. */
#mesondefine HAVE_GETADDRINFO

/* Define to 1 if you have the `getaudit' function. */
#mesondefine HAVE_GETAUDIT

/* Define to 1 if you have the `getaudit_addr' function. */
#mesondefine HAVE_GETAUDIT_ADDR

/* Define to 1 if you have the `getcwd' function. */
#mesondefine HAVE_GETCWD

/* Define to 1 if you have the `getgrouplist' function. */
#mesondefine HAVE_GETGROUPLIST

/* Define to 1 if you have the `getgrset' function. */
#mesondefine HAVE_GETGRSET

/* Define to 1 if you have the `getlastlogxbyname' function. */
#mesondefine HAVE_GETLASTLOGXBYNAME

/* Define to 1 if you have the `getline' function. */
#mesondefine HAVE_GETLINE

/* Define to 1 if you have the `getluid' function. */
#mesondefine HAVE_GETLUID

/* Define to 1 if you have the `getnameinfo' function. */
#mesondefine HAVE_GETNAMEINFO

/* Define to 1 if you have the `getopt' function. */
#mesondefine HAVE_GETOPT

/* Define to 1 if you have the <getopt.h> header file. */
#mesondefine HAVE_GETOPT_H

/* Define if your getopt(3) defines and uses optreset */
#mesondefine HAVE_GETOPT_OPTRESET

/* Define if your libraries define getpagesize() */
#mesondefine HAVE_GETPAGESIZE

/* Define to 1 if you have the `getpeereid' function. */
#mesondefine HAVE_GETPEEREID

/* Define to 1 if you have the `getpeerucred' function. */
#mesondefine HAVE_GETPEERUCRED

/* Define to 1 if you have the `getpgid' function. */
#mesondefine HAVE_GETPGID

/* Define to 1 if you have the `getpgrp' function. */
#mesondefine HAVE_GETPGRP

/* Define to 1 if you have the `getpwanam' function. */
#mesondefine HAVE_GETPWANAM

/* Define to 1 if you have the `getrandom' function. */
#mesondefine HAVE_GETRANDOM

/* Define to 1 if you have the `getrlimit' function. */
#mesondefine HAVE_GETRLIMIT

/* Define if getrrsetbyname() exists */
#mesondefine HAVE_GETRRSETBYNAME

/* Define to 1 if you have the `getseuserbyname' function. */
#mesondefine HAVE_GETSEUSERBYNAME

/* Define to 1 if you have the `getsid' function. */
#mesondefine HAVE_GETSID

/* Define to 1 if you have the `gettimeofday' function. */
#mesondefine HAVE_GETTIMEOFDAY

/* Define to 1 if you have the `getttyent' function. */
#mesondefine HAVE_GETTTYENT

/* Define to 1 if you have the `getutent' function. */
#mesondefine HAVE_GETUTENT

/* Define to 1 if you have the `getutid' function. */
#mesondefine HAVE_GETUTID

/* Define to 1 if you have the `getutline' function. */
#mesondefine HAVE_GETUTLINE

/* Define to 1 if you have the `getutxent' function. */
#mesondefine HAVE_GETUTXENT

/* Define to 1 if you have the `getutxid' function. */
#mesondefine HAVE_GETUTXID

/* Define to 1 if you have the `getutxline' function. */
#mesondefine HAVE_GETUTXLINE

/* Define to 1 if you have the `getutxuser' function. */
#mesondefine HAVE_GETUTXUSER

/* Define to 1 if you have the `get_default_context_with_level' function. */
#mesondefine HAVE_GET_DEFAULT_CONTEXT_WITH_LEVEL

/* Define to 1 if you have the `glob' function. */
#mesondefine HAVE_GLOB

/* Define to 1 if you have the <glob.h> header file. */
#mesondefine HAVE_GLOB_H

/* Define to 1 if you have the `group_from_gid' function. */
#mesondefine HAVE_GROUP_FROM_GID

/* Define to 1 if you have the <gssapi_generic.h> header file. */
#mesondefine HAVE_GSSAPI_GENERIC_H

/* Define to 1 if you have the <gssapi/gssapi_generic.h> header file. */
#mesondefine HAVE_GSSAPI_GSSAPI_GENERIC_H

/* Define to 1 if you have the <gssapi/gssapi.h> header file. */
#mesondefine HAVE_GSSAPI_GSSAPI_H

/* Define to 1 if you have the <gssapi/gssapi_krb5.h> header file. */
#mesondefine HAVE_GSSAPI_GSSAPI_KRB5_H

/* Define to 1 if you have the <gssapi.h> header file. */
#mesondefine HAVE_GSSAPI_H

/* Define to 1 if you have the <gssapi_krb5.h> header file. */
#mesondefine HAVE_GSSAPI_KRB5_H

/* Define if HEADER.ad exists in arpa/nameser.h */
#mesondefine HAVE_HEADER_AD

/* Define to 1 if you have the `HMAC_CTX_init' function. */
#mesondefine HAVE_HMAC_CTX_INIT

/* Define if you have ut_host in utmp.h */
#mesondefine HAVE_HOST_IN_UTMP

/* Define if you have ut_host in utmpx.h */
#mesondefine HAVE_HOST_IN_UTMPX

/* Define to 1 if you have the <iaf.h> header file. */
#mesondefine HAVE_IAF_H

/* Define to 1 if you have the <ia.h> header file. */
#mesondefine HAVE_IA_H

/* Define if you have ut_id in utmp.h */
#mesondefine HAVE_ID_IN_UTMP

/* Define if you have ut_id in utmpx.h */
#mesondefine HAVE_ID_IN_UTMPX

/* Define to 1 if you have the <ifaddrs.h> header file. */
#mesondefine HAVE_IFADDRS_H

/* Define to 1 if you have the `inet_aton' function. */
#mesondefine HAVE_INET_ATON

/* Define to 1 if you have the `inet_ntoa' function. */
#mesondefine HAVE_INET_NTOA

/* Define to 1 if you have the `inet_ntop' function. */
#mesondefine HAVE_INET_NTOP

/* Define to 1 if you have the `innetgr' function. */
#mesondefine HAVE_INNETGR

/* define if you have int8_t data type */
#mesondefine HAVE_INT8_T

/* define if you have int16_t data type */
#mesondefine HAVE_INT16_T

/* define if you have int32_t data type */
#mesondefine HAVE_INT32_T

/* define if you have int64_t data type */
#mesondefine HAVE_INT64_T

/* Define to 1 if the system has the type `intmax_t'. */
#mesondefine HAVE_INTMAX_T

/* Define to 1 if you have the <inttypes.h> header file. */
#mesondefine HAVE_INTTYPES_H

/* define if you have intXX_t data type */
#if defined(HAVE_INT8_T) && \
    defined(HAVE_INT16_T) && \
    defined(HAVE_INT32_T)
#  define HAVE_INTXX_T 1
#endif

/* Define to 1 if the system has the type `in_addr_t'. */
#mesondefine HAVE_IN_ADDR_T

/* Define to 1 if the system has the type `in_port_t'. */
#mesondefine HAVE_IN_PORT_T

/* Define if you have isblank(3C). */
#mesondefine HAVE_ISBLANK

/* Define to 1 if you have the `krb5_cc_new_unique' function. */
#mesondefine HAVE_KRB5_CC_NEW_UNIQUE

/* Define to 1 if you have the `krb5_free_error_message' function. */
#mesondefine HAVE_KRB5_FREE_ERROR_MESSAGE

/* Define to 1 if you have the `krb5_get_error_message' function. */
#mesondefine HAVE_KRB5_GET_ERROR_MESSAGE

/* Define to 1 if you have the <langinfo.h> header file. */
#mesondefine HAVE_LANGINFO_H

/* Define to 1 if you have the <lastlog.h> header file. */
#mesondefine HAVE_LASTLOG_H

/* Define if you want ldns support */
#mesondefine HAVE_LDNS

/* Define to 1 if you have the <libaudit.h> header file. */
#mesondefine HAVE_LIBAUDIT_H

/* Define to 1 if you have the `bsm' library (-lbsm). */
#mesondefine HAVE_LIBBSM

/* Define to 1 if you have the `crypt' library (-lcrypt). */
#mesondefine HAVE_LIBCRYPT

/* Define to 1 if you have the `dl' library (-ldl). */
#mesondefine HAVE_LIBDL

/* Define to 1 if you have the <libgen.h> header file. */
#mesondefine HAVE_LIBGEN_H

/* Define if system has libiaf that supports set_id */
#mesondefine HAVE_LIBIAF

/* Define to 1 if you have the `network' library (-lnetwork). */
#mesondefine HAVE_LIBNETWORK

/* Define to 1 if you have the `pam' library (-lpam). */
#mesondefine HAVE_LIBPAM

/* Define to 1 if you have the <libproc.h> header file. */
#mesondefine HAVE_LIBPROC_H

/* Define to 1 if you have the `socket' library (-lsocket). */
#mesondefine HAVE_LIBSOCKET

/* Define to 1 if you have the <libutil.h> header file. */
#mesondefine HAVE_LIBUTIL_H

/* Define to 1 if you have the `xnet' library (-lxnet). */
#mesondefine HAVE_LIBXNET

/* Define to 1 if you have the `z' library (-lz). */
#mesondefine HAVE_LIBZ

/* Define to 1 if you have the <limits.h> header file. */
#mesondefine HAVE_LIMITS_H

/* Define to 1 if you have the <linux/audit.h> header file. */
#mesondefine HAVE_LINUX_AUDIT_H

/* Define to 1 if you have the <linux/filter.h> header file. */
#mesondefine HAVE_LINUX_FILTER_H

/* Define to 1 if you have the <linux/if_tun.h> header file. */
#mesondefine HAVE_LINUX_IF_TUN_H

/* Define to 1 if you have the <linux/seccomp.h> header file. */
#mesondefine HAVE_LINUX_SECCOMP_H

/* Define to 1 if you have the `llabs' function. */
#mesondefine HAVE_LLABS

/* Define to 1 if you have the <locale.h> header file. */
#mesondefine HAVE_LOCALE_H

/* Define to 1 if you have the `localtime_r' function. */
#mesondefine HAVE_LOCALTIME_R

/* Define to 1 if you have the `login' function. */
#mesondefine HAVE_LOGIN

/* Define to 1 if you have the <login_cap.h> header file. */
#mesondefine HAVE_LOGIN_CAP_H

/* Define to 1 if you have the `login_getcapbool' function. */
#mesondefine HAVE_LOGIN_GETCAPBOOL

/* Define to 1 if you have the `login_getpwclass' function. */
#mesondefine HAVE_LOGIN_GETPWCLASS

/* Define to 1 if you have the <login.h> header file. */
#mesondefine HAVE_LOGIN_H

/* Define to 1 if you have the `logout' function. */
#mesondefine HAVE_LOGOUT

/* Define to 1 if you have the `logwtmp' function. */
#mesondefine HAVE_LOGWTMP

/* Define to 1 if the system has the type `long double'. */
#mesondefine HAVE_LONG_DOUBLE

/* Define to 1 if the system has the type `long long'. */
#mesondefine HAVE_LONG_LONG

/* Define to 1 if you have the <maillock.h> header file. */
#mesondefine HAVE_MAILLOCK_H

/* Define to 1 if your system has a GNU libc compatible `malloc' function, and
   to 0 otherwise. */
#mesondefine HAVE_MALLOC

/* Define to 1 if you have the `mblen' function. */
#mesondefine HAVE_MBLEN

/* Define to 1 if you have the `mbtowc' function. */
#mesondefine HAVE_MBTOWC

/* Define to 1 if you have the `md5_crypt' function. */
#mesondefine HAVE_MD5_CRYPT

/* Define if you want to allow MD5 passwords */
#mesondefine HAVE_MD5_PASSWORDS

/* Define to 1 if you have the `memmem' function. */
#mesondefine HAVE_MEMMEM

/* Define to 1 if you have the `memmove' function. */
#mesondefine HAVE_MEMMOVE

/* Define to 1 if you have the `memset_s' function. */
#mesondefine HAVE_MEMSET_S

/* Define to 1 if you have the `mkdtemp' function. */
#mesondefine HAVE_MKDTEMP

/* define if you have mode_t data type */
#mesondefine HAVE_MODE_T

/* Some systems put nanosleep outside of libc */
#mesondefine HAVE_NANOSLEEP

/* Define to 1 if you have the <ndir.h> header file. */
#mesondefine HAVE_NDIR_H

/* Define to 1 if you have the <netdb.h> header file. */
#mesondefine HAVE_NETDB_H

/* Define to 1 if you have the <netgroup.h> header file. */
#mesondefine HAVE_NETGROUP_H

/* Define to 1 if you have the <net/if_tun.h> header file. */
#mesondefine HAVE_NET_IF_TUN_H

/* Define to 1 if you have the <net/route.h> header file. */
#mesondefine HAVE_NET_ROUTE_H

/* Define if you are on NeXT */
#mesondefine HAVE_NEXT

/* Define to 1 if you have the `ngetaddrinfo' function. */
#mesondefine HAVE_NGETADDRINFO

/* Define to 1 if you have the `nl_langinfo' function. */
#mesondefine HAVE_NL_LANGINFO

/* Define to 1 if you have the `nsleep' function. */
#mesondefine HAVE_NSLEEP

/* Define to 1 if you have the `ogetaddrinfo' function. */
#mesondefine HAVE_OGETADDRINFO

/* Define if you have an old version of PAM which takes only one argument to
   pam_strerror */
#mesondefine HAVE_OLD_PAM

/* Define to 1 if you have the `openlog_r' function. */
#mesondefine HAVE_OPENLOG_R

/* Define to 1 if you have the `openpty' function. */
#mesondefine HAVE_OPENPTY

/* as a macro */
#mesondefine HAVE_OPENSSL_ADD_ALL_ALGORITHMS

/* Define to 1 if you have the `OPENSSL_init_crypto' function. */
#mesondefine HAVE_OPENSSL_INIT_CRYPTO

/* Define to 1 if you have the `OpenSSL_version' function. */
#mesondefine HAVE_OPENSSL_VERSION

/* Define to 1 if you have the `OpenSSL_version_num' function. */
#mesondefine HAVE_OPENSSL_VERSION_NUM

/* Define if you have Digital Unix Security Integration Architecture */
#mesondefine HAVE_OSF_SIA

/* Define to 1 if you have the `pam_getenvlist' function. */
#mesondefine HAVE_PAM_GETENVLIST

/* Define to 1 if you have the <pam/pam_appl.h> header file. */
#mesondefine HAVE_PAM_PAM_APPL_H

/* Define to 1 if you have the `pam_putenv' function. */
#mesondefine HAVE_PAM_PUTENV

/* Define to 1 if you have the <paths.h> header file. */
#mesondefine HAVE_PATHS_H

/* Define if you have ut_pid in utmp.h */
#mesondefine HAVE_PID_IN_UTMP

/* define if you have pid_t data type */
#mesondefine HAVE_PID_T

/* Define to 1 if you have the `pledge' function. */
#mesondefine HAVE_PLEDGE

/* Define to 1 if you have the `poll' function. */
#mesondefine HAVE_POLL

/* Define to 1 if you have the <poll.h> header file. */
#mesondefine HAVE_POLL_H

/* Define to 1 if you have the `prctl' function. */
#mesondefine HAVE_PRCTL

/* Define to 1 if you have the `priv_basicset' function. */
#mesondefine HAVE_PRIV_BASICSET

/* Define to 1 if you have the <priv.h> header file. */
#mesondefine HAVE_PRIV_H

/* Define if you have /proc/$pid/fd */
#mesondefine HAVE_PROC_PID

/* Define to 1 if you have the `proc_pidinfo' function. */
#mesondefine HAVE_PROC_PIDINFO

/* Define to 1 if you have the `pstat' function. */
#mesondefine HAVE_PSTAT

/* Define to 1 if you have the <pty.h> header file. */
#mesondefine HAVE_PTY_H

/* Define to 1 if you have the `pututline' function. */
#mesondefine HAVE_PUTUTLINE

/* Define to 1 if you have the `pututxline' function. */
#mesondefine HAVE_PUTUTXLINE

/* Define to 1 if you have the `raise' function. */
#mesondefine HAVE_RAISE

/* Define to 1 if you have the `readpassphrase' function. */
#mesondefine HAVE_READPASSPHRASE

/* Define to 1 if you have the <readpassphrase.h> header file. */
#mesondefine HAVE_READPASSPHRASE_H

/* Define to 1 if your system has a GNU libc compatible `realloc' function,
   and to 0 otherwise. */
#mesondefine HAVE_REALLOC

/* Define to 1 if you have the `reallocarray' function. */
#mesondefine HAVE_REALLOCARRAY

/* Define to 1 if you have the `realpath' function. */
#mesondefine HAVE_REALPATH

/* Define to 1 if you have the `recallocarray' function. */
#mesondefine HAVE_RECALLOCARRAY

/* Define to 1 if you have the `recvmsg' function. */
#mesondefine HAVE_RECVMSG

/* sys/resource.h has RLIMIT_NPROC */
#mesondefine HAVE_RLIMIT_NPROC

/* Define to 1 if you have the <rpc/types.h> header file. */
#mesondefine HAVE_RPC_TYPES_H

/* Define to 1 if you have the `rresvport_af' function. */
#mesondefine HAVE_RRESVPORT_AF

/* Define to 1 if you have the `RSA_generate_key_ex' function. */
#mesondefine HAVE_RSA_GENERATE_KEY_EX

/* Define to 1 if you have the `RSA_get0_crt_params' function. */
#mesondefine HAVE_RSA_GET0_CRT_PARAMS

/* Define to 1 if you have the `RSA_get0_factors' function. */
#mesondefine HAVE_RSA_GET0_FACTORS

/* Define to 1 if you have the `RSA_get0_key' function. */
#mesondefine HAVE_RSA_GET0_KEY

/* Define to 1 if you have the `RSA_get_default_method' function. */
#mesondefine HAVE_RSA_GET_DEFAULT_METHOD

/* Define to 1 if you have the `RSA_meth_dup' function. */
#mesondefine HAVE_RSA_METH_DUP

/* Define to 1 if you have the `RSA_meth_free' function. */
#mesondefine HAVE_RSA_METH_FREE

/* Define to 1 if you have the `RSA_meth_get_finish' function. */
#mesondefine HAVE_RSA_METH_GET_FINISH

/* Define to 1 if you have the `RSA_meth_set1_name' function. */
#mesondefine HAVE_RSA_METH_SET1_NAME

/* Define to 1 if you have the `RSA_meth_set_finish' function. */
#mesondefine HAVE_RSA_METH_SET_FINISH

/* Define to 1 if you have the `RSA_meth_set_priv_dec' function. */
#mesondefine HAVE_RSA_METH_SET_PRIV_DEC

/* Define to 1 if you have the `RSA_meth_set_priv_enc' function. */
#mesondefine HAVE_RSA_METH_SET_PRIV_ENC

/* Define to 1 if you have the `RSA_set0_crt_params' function. */
#mesondefine HAVE_RSA_SET0_CRT_PARAMS

/* Define to 1 if you have the `RSA_set0_factors' function. */
#mesondefine HAVE_RSA_SET0_FACTORS

/* Define to 1 if you have the `RSA_set0_key' function. */
#mesondefine HAVE_RSA_SET0_KEY

/* Define to 1 if you have the <sandbox.h> header file. */
#mesondefine HAVE_SANDBOX_H

/* Define to 1 if you have the `sandbox_init' function. */
#mesondefine HAVE_SANDBOX_INIT

/* define if you have sa_family_t data type */
#mesondefine HAVE_SA_FAMILY_T

/* Define to 1 if you have the `scan_scaled' function. */
#mesondefine HAVE_SCAN_SCALED

/* Define if you have SecureWare-based protected password database */
#mesondefine HAVE_SECUREWARE

/* Define to 1 if you have the <security/pam_appl.h> header file. */
#mesondefine HAVE_SECURITY_PAM_APPL_H

/* Define to 1 if you have the `sendmsg' function. */
#mesondefine HAVE_SENDMSG

/* Define to 1 if you have the `setauthdb' function. */
#mesondefine HAVE_SETAUTHDB

/* Define to 1 if you have the `setdtablesize' function. */
#mesondefine HAVE_SETDTABLESIZE

/* Define to 1 if you have the `setegid' function. */
#mesondefine HAVE_SETEGID

/* Define to 1 if you have the `setenv' function. */
#mesondefine HAVE_SETENV

/* Define to 1 if you have the `seteuid' function. */
#mesondefine HAVE_SETEUID

/* Define to 1 if you have the `setgroupent' function. */
#mesondefine HAVE_SETGROUPENT

/* Define to 1 if you have the `setgroups' function. */
#mesondefine HAVE_SETGROUPS

/* Define to 1 if you have the `setlinebuf' function. */
#mesondefine HAVE_SETLINEBUF

/* Define to 1 if you have the `setlogin' function. */
#mesondefine HAVE_SETLOGIN

/* Define to 1 if you have the `setluid' function. */
#mesondefine HAVE_SETLUID

/* Define to 1 if you have the `setpassent' function. */
#mesondefine HAVE_SETPASSENT

/* Define to 1 if you have the `setpcred' function. */
#mesondefine HAVE_SETPCRED

/* Define to 1 if you have the `setpflags' function. */
#mesondefine HAVE_SETPFLAGS

/* Define to 1 if you have the `setppriv' function. */
#mesondefine HAVE_SETPPRIV

/* Define to 1 if you have the `setproctitle' function. */
#mesondefine HAVE_SETPROCTITLE

/* Define to 1 if you have the `setregid' function. */
#mesondefine HAVE_SETREGID

/* Define to 1 if you have the `setresgid' function. */
#mesondefine HAVE_SETRESGID

/* Define to 1 if you have the `setresuid' function. */
#mesondefine HAVE_SETRESUID

/* Define to 1 if you have the `setreuid' function. */
#mesondefine HAVE_SETREUID

/* Define to 1 if you have the `setrlimit' function. */
#mesondefine HAVE_SETRLIMIT

/* Define to 1 if you have the `setsid' function. */
#mesondefine HAVE_SETSID

/* Define to 1 if you have the `setutent' function. */
#mesondefine HAVE_SETUTENT

/* Define to 1 if you have the `setutxdb' function. */
#mesondefine HAVE_SETUTXDB

/* Define to 1 if you have the `setutxent' function. */
#mesondefine HAVE_SETUTXENT

/* Define to 1 if you have the `setvbuf' function. */
#mesondefine HAVE_SETVBUF

/* Define to 1 if you have the `set_id' function. */
#mesondefine HAVE_SET_ID

/* Define to 1 if you have the `SHA256Update' function. */
#mesondefine HAVE_SHA256UPDATE

/* Define to 1 if you have the <sha2.h> header file. */
#mesondefine HAVE_SHA2_H

/* Define to 1 if you have the `SHA384Update' function. */
#mesondefine HAVE_SHA384UPDATE

/* Define to 1 if you have the `SHA512Update' function. */
#mesondefine HAVE_SHA512UPDATE

/* Define to 1 if you have the <shadow.h> header file. */
#mesondefine HAVE_SHADOW_H

/* Define to 1 if you have the `sigaction' function. */
#mesondefine HAVE_SIGACTION

/* Define to 1 if you have the `sigvec' function. */
#mesondefine HAVE_SIGVEC

/* Define to 1 if the system has the type `sig_atomic_t'. */
#mesondefine HAVE_SIG_ATOMIC_T

/* define if you have size_t data type */
#mesondefine HAVE_SIZE_T

/* Define to 1 if you have the `snprintf' function. */
#mesondefine HAVE_SNPRINTF

/* Define to 1 if you have the `socketpair' function. */
#mesondefine HAVE_SOCKETPAIR

/* Have PEERCRED socket option */
#mesondefine HAVE_SO_PEERCRED

/* define if you have ssize_t data type */
#mesondefine HAVE_SSIZE_T

/* Fields in struct sockaddr_storage */
#mesondefine HAVE_SS_FAMILY_IN_SS

/* Define if you have ut_ss in utmpx.h */
#mesondefine HAVE_SS_IN_UTMPX

/* Define to 1 if you have the `statfs' function. */
#mesondefine HAVE_STATFS

/* Define to 1 if you have the `statvfs' function. */
#mesondefine HAVE_STATVFS

/* Define to 1 if you have the <stddef.h> header file. */
#mesondefine HAVE_STDDEF_H

/* Define to 1 if you have the <stdint.h> header file. */
#mesondefine HAVE_STDINT_H

/* Define to 1 if you have the <stdio.h> header file. */
#mesondefine HAVE_STDIO_H

/* Define to 1 if you have the <stdlib.h> header file. */
#mesondefine HAVE_STDLIB_H

/* Define to 1 if you have the `strcasestr' function. */
#mesondefine HAVE_STRCASESTR

/* Define to 1 if you have the `strdup' function. */
#mesondefine HAVE_STRDUP

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

/* Define to 1 if you have the `strmode' function. */
#mesondefine HAVE_STRMODE

/* Define to 1 if you have the `strndup' function. */
#mesondefine HAVE_STRNDUP

/* Define to 1 if you have the `strnlen' function. */
#mesondefine HAVE_STRNLEN

/* Define to 1 if you have the `strnvis' function. */
#mesondefine HAVE_STRNVIS

/* Define to 1 if you have the `strptime' function. */
#mesondefine HAVE_STRPTIME

/* Define to 1 if you have the `strsep' function. */
#mesondefine HAVE_STRSEP

/* Define to 1 if you have the `strsignal' function. */
#mesondefine HAVE_STRSIGNAL

/* Define to 1 if you have the `strtoll' function. */
#mesondefine HAVE_STRTOLL

/* Define to 1 if you have the `strtonum' function. */
#mesondefine HAVE_STRTONUM

/* Define to 1 if you have the `strtoul' function. */
#mesondefine HAVE_STRTOUL

/* Define to 1 if you have the `strtoull' function. */
#mesondefine HAVE_STRTOULL

/* define if you have struct addrinfo data type */
#mesondefine HAVE_STRUCT_ADDRINFO

/* define if you have struct in6_addr data type */
#mesondefine HAVE_STRUCT_IN6_ADDR

/* Define to 1 if `pw_change' is a member of `struct passwd'. */
#mesondefine HAVE_STRUCT_PASSWD_PW_CHANGE

/* Define to 1 if `pw_class' is a member of `struct passwd'. */
#mesondefine HAVE_STRUCT_PASSWD_PW_CLASS

/* Define to 1 if `pw_expire' is a member of `struct passwd'. */
#mesondefine HAVE_STRUCT_PASSWD_PW_EXPIRE

/* Define to 1 if `pw_gecos' is a member of `struct passwd'. */
#mesondefine HAVE_STRUCT_PASSWD_PW_GECOS

/* define if you have struct sockaddr_in6 data type */
#mesondefine HAVE_STRUCT_SOCKADDR_IN6

/* Define to 1 if `sin6_scope_id' is a member of `struct sockaddr_in6'. */
#mesondefine HAVE_STRUCT_SOCKADDR_IN6_SIN6_SCOPE_ID

/* define if you have struct sockaddr_storage data type */
#mesondefine HAVE_STRUCT_SOCKADDR_STORAGE

/* Define to 1 if `f_files' is a member of `struct statfs'. */
#mesondefine HAVE_STRUCT_STATFS_F_FILES

/* Define to 1 if `f_flags' is a member of `struct statfs'. */
#mesondefine HAVE_STRUCT_STATFS_F_FLAGS

/* Define to 1 if `st_blksize' is a member of `struct stat'. */
#mesondefine HAVE_STRUCT_STAT_ST_BLKSIZE

/* Define to 1 if `st_mtim' is a member of `struct stat'. */
#mesondefine HAVE_STRUCT_STAT_ST_MTIM

/* Define to 1 if `st_mtime' is a member of `struct stat'. */
#mesondefine HAVE_STRUCT_STAT_ST_MTIME

/* define if you have struct timespec */
#mesondefine HAVE_STRUCT_TIMESPEC

/* define if you have struct timeval */
#mesondefine HAVE_STRUCT_TIMEVAL

/* Define to 1 if you have the `swap32' function. */
#mesondefine HAVE_SWAP32

/* Define to 1 if you have the `sysconf' function. */
#mesondefine HAVE_SYSCONF

/* Define if you have syslen in utmpx.h */
#mesondefine HAVE_SYSLEN_IN_UTMPX

/* Define to 1 if you have the <sys/audit.h> header file. */
#mesondefine HAVE_SYS_AUDIT_H

/* Define to 1 if you have the <sys/bitypes.h> header file. */
#mesondefine HAVE_SYS_BITYPES_H

/* Define to 1 if you have the <sys/bsdtty.h> header file. */
#mesondefine HAVE_SYS_BSDTTY_H

/* Define to 1 if you have the <sys/byteorder.h> header file. */
#mesondefine HAVE_SYS_BYTEORDER_H

/* Define to 1 if you have the <sys/capsicum.h> header file. */
#mesondefine HAVE_SYS_CAPSICUM_H

/* Define to 1 if you have the <sys/cdefs.h> header file. */
#mesondefine HAVE_SYS_CDEFS_H

/* Define to 1 if you have the <sys/dir.h> header file. */
#mesondefine HAVE_SYS_DIR_H

/* Define if your system defines sys_errlist[] */
#mesondefine HAVE_SYS_ERRLIST

/* Define to 1 if you have the <sys/file.h> header file. */
#mesondefine HAVE_SYS_FILE_H

/* Define to 1 if you have the <sys/label.h> header file. */
#mesondefine HAVE_SYS_LABEL_H

/* Define to 1 if you have the <sys/mman.h> header file. */
#mesondefine HAVE_SYS_MMAN_H

/* Define to 1 if you have the <sys/mount.h> header file. */
#mesondefine HAVE_SYS_MOUNT_H

/* Define to 1 if you have the <sys/ndir.h> header file. */
#mesondefine HAVE_SYS_NDIR_H

/* Define if your system defines sys_nerr */
#mesondefine HAVE_SYS_NERR

/* Define to 1 if you have the <sys/poll.h> header file. */
#mesondefine HAVE_SYS_POLL_H

/* Define to 1 if you have the <sys/prctl.h> header file. */
#mesondefine HAVE_SYS_PRCTL_H

/* Define to 1 if you have the <sys/pstat.h> header file. */
#mesondefine HAVE_SYS_PSTAT_H

/* Define to 1 if you have the <sys/ptms.h> header file. */
#mesondefine HAVE_SYS_PTMS_H

/* Define to 1 if you have the <sys/ptrace.h> header file. */
#mesondefine HAVE_SYS_PTRACE_H

/* Define to 1 if you have the <sys/random.h> header file. */
#mesondefine HAVE_SYS_RANDOM_H

/* Define to 1 if you have the <sys/select.h> header file. */
#mesondefine HAVE_SYS_SELECT_H

/* Define to 1 if you have the <sys/statvfs.h> header file. */
#mesondefine HAVE_SYS_STATVFS_H

/* Define to 1 if you have the <sys/stat.h> header file. */
#mesondefine HAVE_SYS_STAT_H

/* Define to 1 if you have the <sys/stream.h> header file. */
#mesondefine HAVE_SYS_STREAM_H

/* Define to 1 if you have the <sys/stropts.h> header file. */
#mesondefine HAVE_SYS_STROPTS_H

/* Define to 1 if you have the <sys/strtio.h> header file. */
#mesondefine HAVE_SYS_STRTIO_H

/* Define to 1 if you have the <sys/sysctl.h> header file. */
#mesondefine HAVE_SYS_SYSCTL_H

/* Force use of sys/syslog.h on Ultrix */
#mesondefine HAVE_SYS_SYSLOG_H

/* Define to 1 if you have the <sys/sysmacros.h> header file. */
#mesondefine HAVE_SYS_SYSMACROS_H

/* Define to 1 if you have the <sys/timers.h> header file. */
#mesondefine HAVE_SYS_TIMERS_H

/* Define to 1 if you have the <sys/time.h> header file. */
#mesondefine HAVE_SYS_TIME_H

/* Define to 1 if you have the <sys/un.h> header file. */
#mesondefine HAVE_SYS_UN_H

/* Define to 1 if you have the <sys/vfs.h> header file. */
#mesondefine HAVE_SYS_VFS_H

/* Define to 1 if you have the `tcgetpgrp' function. */
#mesondefine HAVE_TCGETPGRP

/* Define to 1 if you have the `tcsendbreak' function. */
#mesondefine HAVE_TCSENDBREAK

/* Define to 1 if you have the `time' function. */
#mesondefine HAVE_TIME

/* Define to 1 if you have the <time.h> header file. */
#mesondefine HAVE_TIME_H

/* Define if you have ut_time in utmp.h */
#mesondefine HAVE_TIME_IN_UTMP

/* Define if you have ut_time in utmpx.h */
#mesondefine HAVE_TIME_IN_UTMPX

/* Define to 1 if you have the `timingsafe_bcmp' function. */
#mesondefine HAVE_TIMINGSAFE_BCMP

/* Define to 1 if you have the <tmpdir.h> header file. */
#mesondefine HAVE_TMPDIR_H

/* Define to 1 if you have the `truncate' function. */
#mesondefine HAVE_TRUNCATE

/* Define to 1 if you have the <ttyent.h> header file. */
#mesondefine HAVE_TTYENT_H

/* Define if you have ut_tv in utmp.h */
#mesondefine HAVE_TV_IN_UTMP

/* Define if you have ut_tv in utmpx.h */
#mesondefine HAVE_TV_IN_UTMPX

/* Define if you have ut_type in utmp.h */
#mesondefine HAVE_TYPE_IN_UTMP

/* Define if you have ut_type in utmpx.h */
#mesondefine HAVE_TYPE_IN_UTMPX

/* Define to 1 if you have the <ucred.h> header file. */
#mesondefine HAVE_UCRED_H

/* define if you have uint8_t data type */
#mesondefine HAVE_UINT8_T

/* define if you have uint16_t data type */
#mesondefine HAVE_UINT16_T

/* define if you have uint32_t data type */
#mesondefine HAVE_UINT32_T

/* Define to 1 if the system has the type `uintmax_t'. */
#mesondefine HAVE_UINTMAX_T

/* define if you have uintxx_t data type */
#if defined(HAVE_UINT8_T) && \
    defined(HAVE_UINT16_T) && \
    defined(HAVE_UINT32_T)
#  define HAVE_UINTXX_T 1
#endif

/* Define to 1 if you have the <unistd.h> header file. */
#mesondefine HAVE_UNISTD_H

/* Define to 1 if you have the `unsetenv' function. */
#mesondefine HAVE_UNSETENV

/* Define to 1 if the system has the type `unsigned long long'. */
#mesondefine HAVE_UNSIGNED_LONG_LONG

/* Define to 1 if you have the `updwtmp' function. */
#mesondefine HAVE_UPDWTMP

/* Define to 1 if you have the `updwtmpx' function. */
#mesondefine HAVE_UPDWTMPX

/* Define to 1 if you have the <usersec.h> header file. */
#mesondefine HAVE_USERSEC_H

/* Define to 1 if you have the `user_from_uid' function. */
#mesondefine HAVE_USER_FROM_UID

/* Define to 1 if you have the `usleep' function. */
#mesondefine HAVE_USLEEP

/* Define to 1 if you have the <util.h> header file. */
#mesondefine HAVE_UTIL_H

/* Define to 1 if you have the `utimensat' function. */
#mesondefine HAVE_UTIMENSAT

/* Define to 1 if you have the `utimes' function. */
#mesondefine HAVE_UTIMES

/* Define to 1 if you have the <utime.h> header file. */
#mesondefine HAVE_UTIME_H

/* Define to 1 if you have the `utmpname' function. */
#mesondefine HAVE_UTMPNAME

/* Define to 1 if you have the `utmpxname' function. */
#mesondefine HAVE_UTMPXNAME

/* Define to 1 if you have the <utmpx.h> header file. */
#mesondefine HAVE_UTMPX_H

/* Define to 1 if you have the <utmp.h> header file. */
#mesondefine HAVE_UTMP_H

/* define if you have u_char data type */
#mesondefine HAVE_U_CHAR

/* define if you have u_int data type */
#mesondefine HAVE_U_INT

/* define if you have u_int8_t data type */
#mesondefine HAVE_U_INT8_T

/* define if you have u_int16_t data type */
#mesondefine HAVE_U_INT16_T

/* define if you have u_int32_t data type */
#mesondefine HAVE_U_INT32_T

/* define if you have u_int64_t data type */
#mesondefine HAVE_U_INT64_T

/* define if you have u_intxx_t data type */
#if defined(HAVE_U_INT8_T) && \
    defined(HAVE_U_INT16_T) && \
    defined(HAVE_U_INT32_T)
#  define HAVE_U_INTXX_T 1
#endif

/* Define to 1 if you have the `vasprintf' function. */
#mesondefine HAVE_VASPRINTF

/* Define if va_copy exists */
#mesondefine HAVE_VA_COPY

/* Define to 1 if you have the <vis.h> header file. */
#mesondefine HAVE_VIS_H

/* Define to 1 if you have the `vsnprintf' function. */
#mesondefine HAVE_VSNPRINTF

/* Define to 1 if you have the `waitpid' function. */
#mesondefine HAVE_WAITPID

/* Define to 1 if you have the `warn' function. */
#mesondefine HAVE_WARN

/* Define to 1 if you have the <wchar.h> header file. */
#mesondefine HAVE_WCHAR_H

/* Define to 1 if you have the `wcwidth' function. */
#mesondefine HAVE_WCWIDTH

/* Define to 1 if you have the `_getlong' function. */
#mesondefine HAVE__GETLONG

/* Define to 1 if you have the `_getpty' function. */
#mesondefine HAVE__GETPTY

/* Define to 1 if you have the `_getshort' function. */
#mesondefine HAVE__GETSHORT

/* Define if you have struct __res_state _res as an extern */
#mesondefine HAVE__RES_EXTERN

/* Define to 1 if you have the `__b64_ntop' function. */
#mesondefine HAVE___B64_NTOP

/* Define to 1 if you have the `__b64_pton' function. */
#mesondefine HAVE___B64_PTON

/* Define if compiler implements __FUNCTION__ */
#mesondefine HAVE___FUNCTION__

/* Define if libc defines __progname */
#mesondefine HAVE___PROGNAME

/* Fields in struct sockaddr_storage */
#mesondefine HAVE___SS_FAMILY_IN_SS

/* Define if __va_copy exists */
#mesondefine HAVE___VA_COPY

/* Define if compiler implements __func__ */
#mesondefine HAVE___func__

/* Define this if you are using the Heimdal version of Kerberos V5 */
#mesondefine HEIMDAL

/* Define if you need to use IP address instead of hostname in $DISPLAY */
#mesondefine IPADDR_IN_DISPLAY

/* Detect IPv4 in IPv6 mapped addresses and treat as IPv4 */
#mesondefine IPV4_IN_IPV6

/* Define if your system choked on IP TOS setting */
#mesondefine IP_TOS_IS_BROKEN

/* Define if you want Kerberos 5 support */
#mesondefine KRB5

/* Define if pututxline updates lastlog too */
#mesondefine LASTLOG_WRITE_PUTUTXLINE

/* Define to whatever link() returns for "not supported" if it doesn't return
   EOPNOTSUPP. */
#mesondefine LINK_OPNOTSUPP_ERRNO

/* Adjust Linux out-of-memory killer */
#mesondefine LINUX_OOM_ADJUST

/* max value of long long calculated by configure */
#mesondefine LLONG_MAX

/* min value of long long calculated by configure */
#mesondefine LLONG_MIN

/* Account locked with pw(1) */
#mesondefine LOCKED_PASSWD_PREFIX

/* String used in /etc/passwd to denote locked account */
#mesondefine LOCKED_PASSWD_STRING

/* String used in /etc/passwd to denote locked account */
#mesondefine LOCKED_PASSWD_SUBSTR

/* Some systems need a utmpx entry for /bin/login to work */
#mesondefine LOGIN_NEEDS_UTMPX

/* Set this to your mail directory if you do not have _PATH_MAILDIR */
#mesondefine MAIL_DIRECTORY

/* Need setpgrp to for controlling tty */
#mesondefine NEED_SETPGRP

/* compiler does not accept __attribute__ on prototype args */
#mesondefine NO_ATTRIBUTE_ON_PROTOTYPE_ARGS

/* compiler does not accept __attribute__ on return types */
#mesondefine NO_ATTRIBUTE_ON_RETURN_TYPE

/* SA_RESTARTed signals do no interrupt select */
#mesondefine NO_SA_RESTART

/* Define to disable UID restoration test */
#mesondefine NO_UID_RESTORATION_TEST

/* Define if X11 doesn't support AF_UNIX sockets on that system */
#mesondefine NO_X11_UNIX_SOCKETS

/* Define if EVP_DigestUpdate returns void */
#mesondefine OPENSSL_EVP_DIGESTUPDATE_VOID

/* libcrypto has NID_X9_62_prime256v1 */
#mesondefine OPENSSL_HAS_NISTP256

/* libcrypto has NID_secp384r1 */
#mesondefine OPENSSL_HAS_NISTP384

/* libcrypto has NID_secp521r1 */
#mesondefine OPENSSL_HAS_NISTP521

/* libcrypto has EVP AES CTR */
#mesondefine OPENSSL_HAVE_EVPCTR

/* libcrypto has EVP AES GCM */
#mesondefine OPENSSL_HAVE_EVPGCM

/* libcrypto is missing AES 192 and 256 bit functions */
#mesondefine OPENSSL_LOBOTOMISED_AES

/* Define if you want the OpenSSL internally seeded PRNG only */
#mesondefine OPENSSL_PRNG_ONLY

/* OpenSSL has ECC */
#mesondefine OPENSSL_HAS_ECC

/* Define if you are using Solaris-derived PAM which passes pam_messages to
   the conversation function with an extra level of indirection */
#mesondefine PAM_SUN_CODEBASE

/* Work around problematic Linux PAM modules handling of PAM_TTY */
#mesondefine PAM_TTY_KLUDGE

/* must supply username to passwd */
#mesondefine PASSWD_NEEDS_USERNAME

/* System dirs owned by bin (uid 2) */
#mesondefine PLATFORM_SYS_DIR_UID

/* Port number of PRNGD/EGD random number socket */
#mesondefine PRNGD_PORT

/* Location of PRNGD/EGD random number socket */
#mesondefine PRNGD_SOCKET

/* read(1) can return 0 for a non-closed fd */
#mesondefine PTY_ZEROREAD

/* Sandbox using capsicum */
#mesondefine SANDBOX_CAPSICUM

/* Sandbox using Darwin sandbox_init(3) */
#mesondefine SANDBOX_DARWIN

/* no privsep sandboxing */
#mesondefine SANDBOX_NULL

/* Sandbox using pledge(2) */
#mesondefine SANDBOX_PLEDGE

/* Sandbox using setrlimit(2) */
#mesondefine SANDBOX_RLIMIT

/* Sandbox using seccomp filter */
#mesondefine SANDBOX_SECCOMP_FILTER

/* setrlimit RLIMIT_FSIZE works */
#mesondefine SANDBOX_SKIP_RLIMIT_FSIZE

/* define if setrlimit RLIMIT_NOFILE breaks things */
#mesondefine SANDBOX_SKIP_RLIMIT_NOFILE

/* Sandbox using Solaris/Illumos privileges */
#mesondefine SANDBOX_SOLARIS

/* Sandbox using systrace(4) */
#mesondefine SANDBOX_SYSTRACE

/* Specify the system call convention in use */
#mesondefine SECCOMP_AUDIT_ARCH

/* Define if your platform breaks doing a seteuid before a setuid */
#mesondefine SETEUID_BREAKS_SETUID

/* The size of `int', as computed by sizeof. */
#mesondefine SIZEOF_INT

/* The size of `long int', as computed by sizeof. */
#mesondefine SIZEOF_LONG_INT

/* The size of `long long int', as computed by sizeof. */
#mesondefine SIZEOF_LONG_LONG_INT

/* The size of `short int', as computed by sizeof. */
#mesondefine SIZEOF_SHORT_INT

/* Define as const if snprintf() can declare const char *fmt */
#mesondefine SNPRINTF_CONST

/* Define to a Set Process Title type if your system is supported by
   bsd-setproctitle.c */
#mesondefine SPT_TYPE

/* Define if sshd somehow reacquires a controlling TTY after setsid() */
#mesondefine SSHD_ACQUIRES_CTTY

/* sshd PAM service name */
#mesondefine SSHD_PAM_SERVICE

/* Define if pam_chauthtok wants real uid set to the unpriv'ed user */
#mesondefine SSHPAM_CHAUTHTOK_NEEDS_RUID

/* Use audit debugging module */
#mesondefine SSH_AUDIT_EVENTS

/* Windows is sensitive to read buffer size */
#mesondefine SSH_IOBUFSZ

/* non-privileged user for privilege separation */
#mesondefine SSH_PRIVSEP_USER

/* Use tunnel device compatibility to OpenBSD */
#mesondefine SSH_TUN_COMPAT_AF

/* Open tunnel devices the FreeBSD way */
#mesondefine SSH_TUN_FREEBSD

/* Open tunnel devices the Linux tun/tap way */
#mesondefine SSH_TUN_LINUX

/* No layer 2 tunnel support */
#mesondefine SSH_TUN_NO_L2

/* Open tunnel devices the OpenBSD way */
#mesondefine SSH_TUN_OPENBSD

/* Prepend the address family to IP tunnel traffic */
#mesondefine SSH_TUN_PREPEND_AF

/* Define if you want a different $PATH for the superuser */
#mesondefine SUPERUSER_PATH

/* syslog_r function is safe to use in in a signal handler */
#mesondefine SYSLOG_R_SAFE_IN_SIGHAND

/* Support routing domains using Linux VRF */
#mesondefine SYS_RDOMAIN_LINUX

/* Support passwords > 8 chars */
#mesondefine UNIXWARE_LONG_PASSWORDS

/* Specify default $PATH */
#mesondefine USER_PATH

/* Define this if you want to use libkafs' AFS support */
#mesondefine USE_AFS

/* Use BSM audit module */
#mesondefine USE_BSM_AUDIT

/* Use btmp to log bad logins */
#mesondefine USE_BTMP

/* Use libedit for sftp */
#mesondefine USE_LIBEDIT

/* Use Linux audit module */
#mesondefine USE_LINUX_AUDIT

/* Enable OpenSSL engine support */
#mesondefine USE_OPENSSL_ENGINE

/* Define if you want to enable PAM support */
#mesondefine USE_PAM

/* Use PIPES instead of a socketpair() */
#mesondefine USE_PIPES

/* Define if you have Solaris privileges */
#mesondefine USE_SOLARIS_PRIVS

/* Define if you have Solaris process contracts */
#mesondefine USE_SOLARIS_PROCESS_CONTRACTS

/* Define if you have Solaris projects */
#mesondefine USE_SOLARIS_PROJECTS

/* compiler supports variable length arrays */
#mesondefine VARIABLE_LENGTH_ARRAYS

/* Define if you shouldn't strip 'tty' from your ttyname in [uw]tmp */
#mesondefine WITH_ABBREV_NO_TTY

/* Define if you want to enable AIX4's authenticate function */
#mesondefine WITH_AIXAUTHENTICATE

/* Define if you have/want arrays (cluster-wide session management, not C
   arrays) */
#mesondefine WITH_IRIX_ARRAY

/* Define if you want IRIX audit trails */
#mesondefine WITH_IRIX_AUDIT

/* Define if you want IRIX kernel jobs */
#mesondefine WITH_IRIX_JOBS

/* Define if you want IRIX project management */
#mesondefine WITH_IRIX_PROJECT

/* use libcrypto for cryptography */
#mesondefine WITH_OPENSSL

/* Define if you want SELinux support. */
#mesondefine WITH_SELINUX

/* Enable zlib */
#mesondefine WITH_ZLIB

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#mesondefine WORDS_BIGENDIAN

/* Define if xauth is found in your path */
#mesondefine XAUTH_PATH

/* Number of bits in a file offset, on hosts where this is settable. */
#mesondefine _FILE_OFFSET_BITS

/* Define for large files, on AIX-style hosts. */
#mesondefine _LARGE_FILES

/* log for bad login attempts */
#mesondefine _PATH_BTMP

/* Full path of your "passwd" program */
#mesondefine _PATH_PASSWD_PROG

/* Specify location of ssh.pid */
#mesondefine _PATH_SSH_PIDDIR

/* Define if we don't have struct __res_state in resolv.h */
#mesondefine __res_state

/* Define to rpl_calloc if the replacement function should be used. */
#mesondefine calloc

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
#mesondefine inline
#endif

/* Define to rpl_malloc if the replacement function should be used. */
#mesondefine malloc

/* Define to rpl_realloc if the replacement function should be used. */
#mesondefine realloc

/* type to use in place of socklen_t if not defined */
#mesondefine socklen_t
