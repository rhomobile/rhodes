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

#ifdef OS_MACOSX
#include <TargetConditionals.h>
#endif //OS_MACOSX

#if defined( _DEBUG ) || defined (TARGET_IPHONE_SIMULATOR)
#define RHO_DEBUG 1
#endif

#define RHO_STRIP_LOG 0
#define RHO_STRIP_PROFILER 0

#endif //_RHODEFS_H_