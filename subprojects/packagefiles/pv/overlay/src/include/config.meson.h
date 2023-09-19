/* config.h.  Generated from config.meson.h by meson.  */

/* Various other header files. */
#mesondefine HAVE_GETOPT_H
#mesondefine HAVE_SYS_IPC_H
#mesondefine HAVE_SYS_PARAM_H
#mesondefine HAVE_LIBGEN_H

/* Functions. */
#mesondefine HAVE_GETOPT
#mesondefine HAVE_GETOPT_LONG
#mesondefine HAVE_MEMCPY
#mesondefine HAVE_BASENAME
#mesondefine HAVE_SNPRINTF
#mesondefine HAVE_STAT64

#mesondefine HAVE_SPLICE

/* NLS stuff. */
#mesondefine ENABLE_NLS
#mesondefine LOCALEDIR
#mesondefine HAVE_LIBINTL_H
#mesondefine HAVE_LOCALE_H
#mesondefine HAVE_GETTEXT
#ifdef ENABLE_NLS
# include "library/gettext.h"
#else
# define _(String) (String)
# define N_(String) (String)
#endif

/* The name of the program. */
#mesondefine PROGRAM_NAME

/* The name of the package. */
#mesondefine PACKAGE

/* The current package version. */
#mesondefine VERSION

/* Various identification and legal stuff. */
#define COPYRIGHT_YEAR   _("2015")
#define COPYRIGHT_HOLDER _("Andrew Wood <andrew.wood@ivarch.com>")
#define PROJECT_HOMEPAGE "http://www.ivarch.com/programs/" PROGRAM_NAME ".shtml"
#define BUG_REPORTS_TO   _("<pv@ivarch.com>")

/* LFS support. */
#mesondefine ENABLE_LARGEFILE
#ifdef ENABLE_LARGEFILE
# define __USE_LARGEFILE64 1
# define _LARGEFILE64_SOURCE 1
#else
/*
 * Some Macs have stat64 despite not having open64 while others don't have
 * either, so here even if we don't have open64 or LFS is disabled, we have
 * to check whether stat64 exists and only redefine it if it doesn't
 * otherwise some Macs fail to compile.
 */
# ifdef __APPLE__
#  ifndef HAVE_STAT64
#   define stat64 stat
#   define fstat64 fstat
#   define lstat64 lstat
#  endif
# else
#  define stat64 stat
#  define fstat64 fstat
#  define lstat64 lstat
# endif
# define open64 open
# define lseek64 lseek
#endif

#mesondefine HAVE_IPC

#mesondefine CURSOR_ANSWERBACK_BYTE_BY_BYTE
#ifndef _AIX
#define CURSOR_ANSWERBACK_BYTE_BY_BYTE 1
#endif

/* Boolean type support. */
#mesondefine HAVE_STDBOOL_H
#mesondefine HAVE__BOOL
#ifdef HAVE_STDBOOL_H
# include <stdbool.h>
#else
# ifndef HAVE__BOOL
#  ifdef __cplusplus
typedef bool _Bool;
#  else
#   define _Bool signed char
#  endif
# endif
# define bool _Bool
# define false 0
# define true 1
# define __bool_true_false_are_defined 1
#endif

/* Support for debugging output. */
#mesondefine ENABLE_DEBUGGING

/* EOF */
