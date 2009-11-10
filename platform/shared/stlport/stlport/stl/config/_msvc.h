/* STLport configuration file
 * It is internal STLport header - DO NOT include it directly
 * Microsoft Visual C++ 6.0, 7.0, 7.1, 8.0, ICL
 */

#if !defined (_STLP_COMPILER)
#  define _STLP_COMPILER "Microsoft Visual Studio C++"
#endif

#if !defined (__ICL) && !defined (_STLP_MSVC)
#  define _STLP_MSVC _MSC_VER
#endif

#if !defined (_STLP_MSVC_LIB)
#  define _STLP_MSVC_LIB _MSC_VER
#endif

#if defined (__BUILDING_STLPORT) && defined (_MANAGED)
/* Building a managed version of STLport is not supported because we haven't
 * found a good reason to support it. However, building a managed translation
 * unit using STLport _is_ supported.
 */
#  error Sorry but building a managed version of STLport is not supported.
#endif

#if defined (_STLP_USING_PLATFORM_SDK_COMPILER)
/* This is a specific section for compilers coming with platform SDKs. Native
 * library coming with it is different from the one coming with commercial
 * MSVC compilers so there is some specific settings.
 */
#  define _STLP_NATIVE_INCLUDE_PATH ../crt
#  define _STLP_VENDOR_GLOBAL_CSTD
#  define _STLP_VENDOR_TERMINATE_STD
#  define _STLP_GLOBAL_NEW_HANDLER
#  if (_STLP_MSVC_LIB <= 1400)
/* We hope this bug will be fixed in future versions. */
#    define _STLP_NEW_DONT_THROW_BAD_ALLOC 1
#  endif
#endif

#define _STLP_CALL __cdecl

#ifndef _STLP_LONG_LONG
#  define _STLP_LONG_LONG __int64
#endif

#define _STLP_PRAGMA_ONCE

/* These switches depend on compiler flags. We are hoping here that compilers
 * simulating MSVC behavior use identical macros to report compilation context.
 * Otherwise those macros will have to be undef in specific compiler configuration
 * files.
 */
#ifndef _CPPUNWIND
#  define _STLP_DONT_USE_EXCEPTIONS 1
#endif

#ifndef _CPPRTTI
#  define _STLP_NO_RTTI 1
#endif

#if defined (_MT) && !defined (_STLP_NO_THREADS) && !defined (_REENTRANT)
#  define _REENTRANT 1
#endif

#if !defined (_NATIVE_WCHAR_T_DEFINED)
#  define _STLP_WCHAR_T_IS_USHORT 1
#endif

#define _STLP_MINIMUM_IMPORT_STD
#define _STLP_NO_VENDOR_STDLIB_L 1

#if defined (_STLP_MSVC)

#define _STLP_NORETURN_FUNCTION __declspec(noreturn)

/* Full compiler version comes from boost library intrinsics.hpp header. */
#  if defined (_MSC_FULL_VER) && (_MSC_FULL_VER >= 140050215)
#    define _STLP_HAS_TRIVIAL_CONSTRUCTOR(T) __has_trivial_constructor(T)
#    define _STLP_HAS_TRIVIAL_COPY(T) __has_trivial_copy(T)
#    define _STLP_HAS_TRIVIAL_ASSIGN(T) __has_trivial_assign(T)
#    define _STLP_HAS_TRIVIAL_DESTRUCTOR(T) __has_trivial_destructor(T)
#    define _STLP_IS_POD(T) __is_pod(T)
#    define _STLP_HAS_TYPE_TRAITS_INTRINSICS
#  endif

#  ifndef _STLP_MSVC50_COMPATIBILITY
#    define _STLP_MSVC50_COMPATIBILITY   1
#  endif

#  define _STLP_DLLEXPORT_NEEDS_PREDECLARATION 1
#  define _STLP_HAS_SPECIFIC_PROLOG_EPILOG 1

/* # ifndef __BUILDING_STLPORT
 * #  define _STLP_USE_TEMPLATE_EXPORT 1
 * # endif
 */
#  if (_STLP_MSVC <= 1401)
#    define _STLP_STATIC_CONST_INIT_BUG   1
#  endif

/** Note: the macro _STLP_NO_UNCAUGHT_EXCEPT_SUPPORT is defined
unconditionally and undef'ed here when applicable. */
#  if defined(UNDER_CE)
/* eVCx:
uncaught_exception is declared in the SDKs delivered with eVC4 (eVC3 is
unknown) and they all reside in namespace 'std' there. However, they are not
part of any lib so linking fails. When using VC8 to crosscompile for CE 5 on
an ARMV4I, the uncaught_exception test fails, the function returns the wrong
value. */
#  else
/* VCx:
These are present at least since VC6, but the uncaught_exception() of VC6 is
broken, it returns the wrong value in the unittests. 7.1 and later seem to
work, 7.0 is still unknown (we assume it works until negative report). */
#    if (_STLP_MSVC >= 1300)// VC7 and later
#      undef _STLP_NO_UNCAUGHT_EXCEPT_SUPPORT
#      if !defined (_STLP_DONT_USE_EXCEPTIONS)
#        define _STLP_NOTHROW throw()
#      endif
#    endif
#  endif

#  if (_STLP_MSVC <= 1300)
#    define _STLP_NO_CLASS_PARTIAL_SPECIALIZATION 1
#    define _STLP_NO_FUNCTION_TMPL_PARTIAL_ORDER 1
/* There is no partial spec, and MSVC breaks on simulating it for iterator_traits queries */
#    define _STLP_USE_OLD_HP_ITERATOR_QUERIES
#    define _STLP_NO_TYPENAME_IN_TEMPLATE_HEADER
#    define _STLP_NO_METHOD_SPECIALIZATION 1
#    define _STLP_DEF_CONST_PLCT_NEW_BUG 1
#    define _STLP_NO_TYPENAME_ON_RETURN_TYPE 1
/* VC++ cannot handle default allocator argument in template constructors */
#    define _STLP_NEEDS_EXTRA_TEMPLATE_CONSTRUCTORS
#    define _STLP_NO_QUALIFIED_FRIENDS    1
#    define _STLP_NO_FRIEND_TEMPLATES
/* Fails to properly resolve call to sin() from within sin() */
#  endif

#  if (_STLP_MSVC < 1300)
#    define _STLP_NO_IEC559_SUPPORT 1
#  endif

#  if (_STLP_MSVC < 1300) /* including MSVC 6.0 */
#    define _STLP_NO_MEMBER_TEMPLATE_KEYWORD 1
#    define _STLP_DONT_SUPPORT_REBIND_MEMBER_TEMPLATE 1
#  endif

#  if (_STLP_MSVC >= 1200)
#    define _STLP_HAS_NATIVE_FLOAT_ABS 1
#  endif

// TODO: some eVC4 compilers report _MSC_VER 1201 or 1202, which category do they belong to?
#  if (_STLP_MSVC > 1200) && (_STLP_MSVC < 1310)
#    define _STLP_NO_MOVE_SEMANTIC
#  endif

#  if (_STLP_MSVC < 1300)
/* TODO: remove this if it is handled and documented elsewhere
 * dums: VC6 do not handle correctly member templates of class that are explicitely
 * instanciated to be exported. There is a workaround, seperate the non template methods
 * from the template ones within 2 different classes and only export the non template one.
 * It is implemented for basic_string and locale at the writing of this note.
 * However this problem hos not  been considered as important enough to remove template member
 * methods for other classes. Moreover Boost (www.boost.org) required it to be granted.
 * The workaround is activated thanks to the _STLP_USE_MSVC6_MEM_T_BUG_WORKAROUND macro defined
 * later in this config file.
 */
/*
#    if defined (_DLL)
#      define _STLP_NO_MEMBER_TEMPLATES 1
#    endif
*/

/* Boris : not defining this macro for SP5 causes other problems */
/*#    if !defined (_MSC_FULL_VER) || (_MSC_FULL_VER < 12008804 ) */
#    define _STLP_NO_USING_FOR_GLOBAL_FUNCTIONS 1
/*#    endif */
#    define _STLP_DONT_USE_BOOL_TYPEDEF 1
#    define _STLP_DONT_RETURN_VOID 1
#  endif

/*
 * MSVC6 is known to have many trouble with namespace management but
 * MSVC .Net 2003 and 2005 also have a bug difficult to reproduce without
 * STLport:
 * namespace stlp_std {
 *   typedef int foo_int;
 * }
 * #include <map>
 * const foo_int bar = 0;
 *
 * As you can see foo is available without namespace specification as if
 * a using namespace stlp_std has been performed. Defining _STLP_USING_NAMESPACE_BUG
 * restore the expected compilation error.
 */
#  define _STLP_USING_NAMESPACE_BUG 1

#  if (_STLP_MSVC < 1300) /* MSVC 6.0 and earlier */
#    define _STLP_NO_EXPLICIT_FUNCTION_TMPL_ARGS 1
/* defined for DEBUG and NDEBUG too, to allow user mix own debug build with STLP release library */
#    define _STLP_USE_ABBREVS
#  endif

// TODO: what is the earliest version for this? If it is 1200, use _STLP_MSVC>=1200.
#  if (_STLP_MSVC > 1100) && (_STLP_MSVC < 1300)
typedef char __stl_char;
#    define _STLP_DEFAULTCHAR __stl_char
#  endif

#  if (_STLP_MSVC < 1200) /* before VC++ 6.0 */
/* #  define _STLP_NO_MEMBER_TEMPLATES 1 */
#    define _STLP_DONT_SIMULATE_PARTIAL_SPEC_FOR_TYPE_TRAITS 1
#    define _STLP_DONT_USE_PARTIAL_SPEC_WRKD 1
#    define _STLP_QUALIFIED_SPECIALIZATION_BUG 1
#    define _STLP_NON_TYPE_TMPL_PARAM_BUG 1
#    define _STLP_THROW_RETURN_BUG 1
#    define _STLP_NO_MEMBER_TEMPLATE_CLASSES 1
#    define _STLP_DEF_CONST_DEF_PARAM_BUG 1
#  endif

#  if (_STLP_MSVC < 1100 )
#    ifndef _STLP_USE_NO_IOSTREAMS
#      define _STLP_USE_NO_IOSTREAMS
#    endif
/* #  define _STLP_NESTED_TYPE_PARAM_BUG 1 */
/* Debug mode does not work for 4.2 */
#    if defined (_STLP_DEBUG)
#      pragma message ("STLport debug mode does not work for VC++ 4.2, turning _STLP_DEBUG off ...")
#      undef _STLP_DEBUG
#    endif
#    define _STLP_NO_BOOL            1
#    define _STLP_NEED_TYPENAME      1
#    define _STLP_NEED_EXPLICIT      1
#    define _STLP_NEED_MUTABLE       1
#    define _STLP_NO_PARTIAL_SPECIALIZATION_SYNTAX
#    define _STLP_LIMITED_DEFAULT_TEMPLATES 1
#    define _STLP_NONTEMPL_BASE_MATCH_BUG 1
#    define _STLP_BROKEN_USING_DIRECTIVE  1
#    define _STLP_NO_ARROW_OPERATOR 1
#    define _STLP_NO_SIGNED_BUILTINS 1
#    define _STLP_NO_EXCEPTION_SPEC 1
#    define _STLP_HAS_NO_NAMESPACES 1
#    define _STLP_NO_AT_MEMBER_FUNCTION 1
#    define _STLP_NO_MEMBER_TEMPLATES 1
#  endif /* 1100 */

#endif /* _STLP_MSVC */

/** The desktop variants starting with VC8 have a set of more secure replacements
for the error-prone string handling functions of the C standard lib. */
#if (_STLP_MSVC_LIB >= 1400) && !defined (_STLP_USING_PLATFORM_SDK_COMPILER) && !defined(UNDER_CE)
#  define _STLP_USE_SAFE_STRING_FUNCTIONS 1
#endif

#if (_STLP_MSVC_LIB <= 1310)
#  define _STLP_VENDOR_GLOBAL_CSTD
#endif

#if (_STLP_MSVC_LIB >= 1300) && !defined(UNDER_CE)
/* Starting with MSVC 7.0 and compilers simulating it,
 * we assume that the new SDK is granted:
 */
#  define _STLP_NEW_PLATFORM_SDK 1
#endif

#if (_STLP_MSVC_LIB < 1300) /* including MSVC 6.0 */
#  define _STLP_GLOBAL_NEW_HANDLER 1
#  define _STLP_VENDOR_UNEXPECTED_STD
#  define _STLP_NEW_DONT_THROW_BAD_ALLOC 1
#endif

#if (_STLP_MSVC_LIB < 1100)
/* up to 4.2, library is in global namespace */
#  define _STLP_VENDOR_GLOBAL_STD
#endif

#if (_STLP_MSVC_LIB <= 1010)
/* "bool" is reserved in MSVC 4.1 while <yvals.h> absent, so : */
#  define _STLP_NO_BAD_ALLOC
#  define _STLP_HAS_NO_NEW_C_HEADERS 1
#  define _STLP_NO_NEW_NEW_HEADER 1
#elif (_STLP_MSVC_LIB < 1100)
/* VC++ 4.2 and higher */
#  define _STLP_YVALS_H 1
#  define _STLP_USE_NO_IOSTREAMS 1
#endif

#define _STLP_EXPORT_DECLSPEC __declspec(dllexport)
#define _STLP_IMPORT_DECLSPEC __declspec(dllimport)

#if !defined (_STLP_MSVC) || (_STLP_MSVC >= 1100)
#  define _STLP_CLASS_EXPORT_DECLSPEC __declspec(dllexport)
#  define _STLP_CLASS_IMPORT_DECLSPEC __declspec(dllimport)
#endif

#if defined (__DLL) || defined (_DLL) || defined (_RTLDLL) || defined (_AFXDLL)
#  define _STLP_RUNTIME_DLL
#endif
#if defined (__BUILDING_STLPORT) && \
   (defined (_STLP_USE_DYNAMIC_LIB) || \
    defined (_STLP_RUNTIME_DLL) && !defined (_STLP_USE_STATIC_LIB))
#  define _STLP_DLL
#endif
#include <stl/config/_detect_dll_or_lib.h>
#undef _STLP_RUNTIME_DLL
#undef _STLP_DLL

#if defined (_STLP_USE_DYNAMIC_LIB)
#  undef  _STLP_USE_DECLSPEC
#  define _STLP_USE_DECLSPEC 1
#  if (_STLP_MSVC >= 1200) && (_STLP_MSVC < 1300)
#    define _STLP_USE_MSVC6_MEM_T_BUG_WORKAROUND 1
#  endif
#endif

#if !defined (_STLP_IMPORT_TEMPLATE_KEYWORD)
#  if !defined (_MSC_EXTENSIONS) || defined(_STLP_MSVC) && _STLP_MSVC >= 1300
#    define _STLP_IMPORT_TEMPLATE_KEYWORD
#  else
#    define _STLP_IMPORT_TEMPLATE_KEYWORD extern
#  endif
#endif
#define _STLP_EXPORT_TEMPLATE_KEYWORD

#if defined (_STLP_MSVC) && (_STLP_MSVC < 1200)
/*    only static STLport lib now works for VC 5.0 */
#  undef  _STLP_USE_STATIC_LIB
#  undef  _STLP_USE_DYNAMIC_LIB
#  define _STLP_USE_STATIC_LIB
/*    disable hook which makes template symbols to be searched for in the library */
#  undef _STLP_NO_CUSTOM_IO
#endif

#include <stl/config/_auto_link.h>

#if defined (_STLP_USING_PLATFORM_SDK_COMPILER)
/* The Windows 64 bits SDK required for the moment link to bufferoverflowU.lib for
 * additional buffer overrun checks. Rather than require the STLport build system and
 * users to explicitely link with it we use the MSVC auto link feature.
 */
#  if !defined (_STLP_DONT_USE_AUTO_LINK) || defined (__BUILDING_STLPORT)
#    pragma comment (lib, "bufferoverflowU.lib")
#    if defined (_STLP_VERBOSE_AUTO_LINK)
#      pragma message ("STLport: Auto linking to bufferoverflowU.lib")
#    endif
#  endif
#endif

/* Local Variables:
 * mode:C++
 * End:
 */
