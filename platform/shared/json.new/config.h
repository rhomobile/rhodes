/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.in by autoheader.  */

#ifdef __SYMBIAN32__
#undef _WIN32
#endif

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you don't have `vprintf' but do have `_doprnt.' */
/* #undef HAVE_DOPRNT */

/* Define to 1 if you have the <fcntl.h> header file. */
#if !defined(_WIN32_WCE)
#define HAVE_FCNTL_H 1
#endif

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if your system has a GNU libc compatible `malloc' function, and
   to 0 otherwise. */
#define HAVE_MALLOC 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `open' function. */
#define HAVE_OPEN 1

/* Define to 1 if your system has a GNU libc compatible `realloc' function,
   and to 0 otherwise. */
#define HAVE_REALLOC 1

/* Define to 1 if you have the <stdarg.h> header file. */
#define HAVE_STDARG_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strerror' function. */
#define HAVE_STRERROR 1

/* Define to 1 if you have the <strings.h> header file. */
#if !defined(_WIN32_WCE)&& !defined(WIN32) && !defined(_WP8_LIB)  && !defined(_UWP_LIB)
#define HAVE_STRINGS_H 1
#endif

/* Define to 1 if you have the <string.h> header file. */
//#define HAVE_STRING_H 1

/* Define to 1 if you have the `strncasecmp' function. */
#define HAVE_STRNCASECMP 1

/* Define to 1 if you have the `strndup' function. */
#if defined(OS_ANDROID)
#define HAVE_STRNDUP 1
#endif

/* Define to 1 if you have the <syslog.h> header file. */
#if !defined(_WIN32_WCE) && !defined(WIN32) && !defined(OS_UWP)
#define HAVE_SYSLOG_H 1
#endif

/* Define to 1 if you have the <sys/param.h> header file. */
#if !defined(_WIN32_WCE)&& !defined(WIN32) && !defined(OS_UWP)
#define HAVE_SYS_PARAM_H 1
#endif

/* Define to 1 if you have the <sys/stat.h> header file. */
#if !defined(_WIN32_WCE)&& !defined(WIN32) && !defined(OS_UWP)
#define HAVE_SYS_STAT_H 1
#endif

/* Define to 1 if you have the <sys/types.h> header file. */
#if !defined(_WIN32_WCE)&& !defined(WIN32) && !defined(OS_UWP)
#define HAVE_SYS_TYPES_H 1
#endif

/* Define to 1 if you have the <unistd.h> header file. */
#if !defined(_WIN32_WCE)&& !defined(WIN32) && !defined(_UWP_LIB)
#define HAVE_UNISTD_H 1
#endif

/* Define to 1 if you have the `vasprintf' function. */
#if !defined(_WIN32_WCE)&& !defined(WIN32) && !defined(_UWP_LIB)
#define HAVE_VASPRINTF 1
#endif

/* Define to 1 if you have the `vprintf' function. */
#define HAVE_VPRINTF 1

/* Define to 1 if you have the `vsnprintf' function. */
#if !defined(_WIN32_WCE) //&& !defined(WIN32)
#define HAVE_VSNPRINTF 1
#endif

/* Define to 1 if you have the `vsyslog' function. */
#if !defined(_WIN32_WCE) && !defined(WIN32) && !defined(OS_UWP)
#define HAVE_VSYSLOG 1
#endif

/* Name of package */
#define PACKAGE ""

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "michael@metaparadigm.com"

/* Define to the full name of this package. */
#define PACKAGE_NAME "JSON C Library"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "JSON C Library 0.8"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "json-c"

/* Define to the version of this package. */
#define PACKAGE_VERSION "0.8"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "0.8"

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to rpl_malloc if the replacement function should be used. */
/* #undef malloc */

/* Define to rpl_realloc if the replacement function should be used. */
/* #undef realloc */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

//#if !defined(OS_ANDROID)
//#include "tcmalloc/rhomem.h"
//#endif

#if defined(WIN32)

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#endif
