// STLport configuration file
// It is internal STLport header - DO NOT include it directly

//#define _STLP_VERBOSE

#define _STLP_COMPILER "Borland"

#if (__BORLANDC__ < 0x551)
#  error - Borland compilers below version 5.5.1 not supported.
#endif

#if (__BORLANDC__ >= 0x580) && (__BORLANDC__ < 0x590)
#  define _STLP_NO_NEW_C_HEADERS
#  define _STLP_NATIVE_CPP_RUNTIME_HEADER(header) <../include/dinkumware/##header>
#  define _STLP_NO_FORCE_INSTANTIATE
#endif

#if (__BORLANDC__ >= 0x560) && (__BORLANDC__ < 0x570)
#  define _USE_OLD_RW_STL
#endif

#if (__BORLANDC__ >= 0x560)
#  define NOWINBASEINTERLOCK  // src/fstream.cpp error in winbase.h
#endif

#if (__BORLANDC__ < 0x564)
#  define _STLP_QUALIFIED_SPECIALIZATION_BUG
#endif

#if (__BORLANDC__ < 0x560)
#  define _STLP_NO_CONTAINERS_EXTENSION
#endif

#define _STLP_NO_FUNCTION_TMPL_PARTIAL_ORDER
#define _STLP_DONT_USE_PRIV_NAMESPACE
#define _STLP_NO_VENDOR_STDLIB_L
#define _STLP_NO_VENDOR_MATH_F
#define _STLP_DONT_USE_SHORT_STRING_OPTIM 1
#define _STLP_NO_NATIVE_MBSTATE_T
#define _STLP_DLLEXPORT_NEEDS_PREDECLARATION
#undef _STLP_NO_UNEXPECTED_EXCEPT_SUPPORT
#if (__BORLANDC__ < 0x580) && !defined (_RTLDLL)
#  define _UNCAUGHT_EXCEPTION 1
#endif

// <limits> problem
#define _STLP_STATIC_CONST_INIT_BUG

// <bitset> problems
#define _STLP_MEMBER_SPECIALIZATION_BUG 1
#define _STLP_HAS_SPECIFIC_PROLOG_EPILOG 1

#define _STLP_LONG_LONG  __int64

// auto enable thread safety and exceptions:
#ifndef _CPPUNWIND
#  define _STLP_HAS_NO_EXCEPTIONS
#endif

#if defined (__MT__) && !defined (_NOTHREADS) && !defined (_REENTRANT)
#  if defined (_STLP_VERBOSE)
#    pragma message ("multi threaded")
#  endif
#  define _REENTRANT 1
#else
#  if defined (_STLP_VERBOSE)
#    pragma message ("single threaded")
#  endif
#endif

#define _STLP_EXPORT_DECLSPEC __declspec(dllexport)
#define _STLP_IMPORT_DECLSPEC __declspec(dllimport)

#define _STLP_CLASS_EXPORT_DECLSPEC __declspec(dllexport)
#define _STLP_CLASS_IMPORT_DECLSPEC __declspec(dllimport)

#if defined (_DLL)
#  define _STLP_DLL
#endif
#if defined (_RTLDLL)
#  define _STLP_RUNTIME_DLL
#endif
#include <stl/config/_detect_dll_or_lib.h>
#undef _STLP_RUNTIME_DLL
#undef _STLP_DLL

#if defined (_STLP_USE_DYNAMIC_LIB)
#  if defined (_STLP_VERBOSE)
#    pragma message ("Using/Building STLport dll")
#  endif
#elif defined (_STLP_USE_STATIC_LIB)
#  if defined (_STLP_VERBOSE)
#    pragma message ("Using/Building STLport lib")
#  endif
#else
#  error Unknown STLport usage config (dll/lib?)
#endif

#if defined (_STLP_USING_CROSS_NATIVE_RUNTIME_LIB)
#  if defined (_STLP_VERBOSE)
#    pragma message ("Using cross version of native runtime")
#  endif
#endif

#if !defined (_STLP_IMPORT_TEMPLATE_KEYWORD)
//#  define _STLP_IMPORT_TEMPLATE_KEYWORD __declspec(dllimport)
#endif
//#define _STLP_EXPORT_TEMPLATE_KEYWORD __declspec(dllexport)

#if defined (_STLP_USE_DYNAMIC_LIB)
#  define _STLP_USE_DECLSPEC 1
#  if (__BORLANDC__ < 0x580)
#  if defined (__BUILDING_STLPORT)
#    define _STLP_CALL __cdecl __export
#  else
#    define  _STLP_CALL __cdecl __import
#  endif
#else
#  define  _STLP_CALL __cdecl
#endif
#else
#  define  _STLP_CALL __cdecl
#endif

#include <stl/config/_auto_link.h>
