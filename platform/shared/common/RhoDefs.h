#ifndef _RHODEFS_H_
#define _RHODEFS_H_

#if defined(__SYMBIAN32__)
# define OS_SYMBIAN
#elif defined(_WIN32_WCE)
# define OS_WINCE _WIN32_WINCE
#elif defined(WIN32)
# define OS_WINDOWS
#elif defined(__CYGWIN__) || defined(__CYGWIN32__)
# define OS_CYGWIN
#elif defined(linux) || defined(__linux) || defined(__linux__)
# define OS_LINUX
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
# define OS_MACOSX
#elif defined(__FreeBSD__)
# define OS_FREEBSD
#else
#endif

#ifdef __cplusplus
# if defined(__GNUC__) && __GNUC__ >= 4
#  define RHO_GLOBAL extern "C" __attribute__ ((visibility ("default")))
#  define RHO_LOCAL  __attribute__ ((visibility ("hidden")))
# else
#  define RHO_GLOBAL extern "C"
#  define RHO_LOCAL
# endif
#else
# if defined(__GNUC__) && __GNUC__ >= 4
#  define RHO_GLOBAL extern __attribute__ ((visibility ("default")))
#  define RHO_LOCAL  __attribute__ ((visibility ("hidden")))
# else
#  define RHO_GLOBAL extern
#  define RHO_LOCAL
# endif
#endif

#define RHO_ABORT(x) \
  do { \
    assert(!(x)); \
    abort(); \
  } while( __LINE__ != -1 )

#define RHO_USE_OWN_HTTPD
#define RHO_HTTPD_COMMON_IMPL

#ifdef OS_MACOSX
#include <TargetConditionals.h>
#endif //OS_MACOSX

#if defined( _DEBUG ) || defined (TARGET_IPHONE_SIMULATOR)
#define RHO_DEBUG 1
#endif

#define L_TRACE   0
#define L_INFO    1
#define L_WARNING 2
#define L_ERROR   3
#define L_FATAL   4
#define L_NUM_SEVERITIES  5

#define RHO_STRIP_LOG 0
#define RHO_STRIP_PROFILER 1

typedef int LogSeverity;

#if defined( OS_WINDOWS ) || defined( OS_WINCE )
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif //_CRT_SECURE_NO_WARNINGS

#ifndef _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_NON_CONFORMING_SWPRINTFS 1
#endif //_CRT_NON_CONFORMING_SWPRINTFS

#ifndef _CRT_NONSTDC_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS 1
#endif //_CRT_NONSTDC_NO_WARNINGS

#endif

#if !defined(OS_ANDROID)
#include "tcmalloc/rhomem.h"
#endif

#endif //_RHODEFS_H_

