// STLport configuration file
// It is internal STLport header - DO NOT include it directly

#define _STLP_COMPILER "Acc"

// system C-library dependent
#if defined(_XOPEN_SOURCE) && (_XOPEN_VERSION - 0 >= 4)
#  define _STLP_RAND48 1
#endif
// #  define _STLP_RAND48 1
#define _STLP_NO_NATIVE_MBSTATE_T      1
#define _STLP_HPACC_BROKEN_BUFEND       1
#define _STLP_WCHAR_HPACC_EXCLUDE      1

// this was reported to help, just as with SUN CC 4.2
#define _STLP_INLINE_STRING_LITERAL_BUG

// specific prolog is needed to select correct threads impl
#define _STLP_HAS_SPECIFIC_PROLOG_EPILOG

// HP aCC with +noeh
#ifdef __HPACC_NOEH
#  define _STLP_HAS_NO_EXCEPTIONS 1
#endif

// HP compilers
// At that point, we only know we are on HP (and _not_ using gcc,
// according to "stlcomp.h"

// __HP_aCC indicate HP ANSI C++, but not always (03.xx does not
// define it before 3.13, for example).
//
#if defined (__HP_aCC)

#  define _STLP_NO_FORCE_INSTANTIATE
#  define _STLP_LONG_LONG long long
#  define _STLP_NO_VENDOR_STDLIB_L

#  if (__HP_aCC <= 30000 && __HP_aCC >= 12100)

//Special kludge to workaround bug in aCC A.01.23, CR JAGac40634
#    ifdef _STLP_DEBUG
static void _STLP_dummy_literal() { const char *p = "x";}
static void _STLP_dummy_literal_2() { const char *p = "123456789"; }
static void _STLP_dummy_literal_3() { const char *p = "123456700000000000000089";}
#    endif

#    define _STLP_HP_ACC                   0123
#    define _STLP_NATIVE_INCLUDE_PATH       ../include
#    define _STLP_VENDOR_GLOBAL_STD         1
#    define _STLP_VENDOR_GLOBAL_CSTD        1
#    define _STLP_DONT_THROW_RANGE_ERRORS   1
#    define _STLP_STATIC_CONST_INIT_BUG 1
#    if (__HP_aCC  < 12700)
//new flag: on most HP compilers cwchar is missing
#      define _STLP_NO_CWCHAR
#    endif

#    define _STLP_FORCE_ALLOCATORS(t,a) \
  typedef typename _Alloc_traits<t,a>::_Orig _STLP_dummy_type1;\
  typedef typename _STLP_dummy_type1:: _STLP_TEMPLATE rebind<t>::other _STLP_dummy_type2;

#  endif /* 123 */

// latest version and up
#  if (__HP_aCC >= 32500 )
#    define _STLP_HP_ACC                   0325

#    define _STLP_USE_NEW_C_HEADERS

#    define _STLP_FORCE_ALLOCATORS(t,a) \
  typedef typename _Alloc_traits<t,a>::_Orig _STLP_dummy_type1;\
  typedef typename _STLP_dummy_type1:: _STLP_TEMPLATE rebind<t>::other _STLP_dummy_type2;

#    if !defined ( _INCLUDE__STDC_A1_SOURCE ) // HP-UX 11i only
#      define _STLP_HAS_NO_UNIX98_WCHAR_EXTENSIONS
#    endif

#    if defined (_HP_NAMESPACE_STD) // option -AA
// from now, we have a full standard lib in namespace std
//
// -AA indicates that we are compiling against Rogue Wave 2.2.1
// STL shipped with the HP aCC compiler. -AA tells the compiler
// to use the STL defined in the include_std directory.
//
#      define _STLP_NATIVE_INCLUDE_PATH ../include_std

// #      define _STLP_HPACC_ONLY_NATIVE_STRING 1 // STLPort _string.c includes <locale>
#      define _STLP_HP_ACC_COMPAT            -1
#    else // option -Aa
#      define _STLP_NATIVE_INCLUDE_PATH       ../include
#      define _STLP_VENDOR_GLOBAL_STD         1
#      define _STLP_VENDOR_GLOBAL_CSTD        1
#      define _STLP_DONT_THROW_RANGE_ERRORS   1
#    endif /* _NAMESPACE_STD */
#  endif

#  if (__HP_aCC >= 31400 && __HP_aCC < 32500)
#    define _STLP_HP_ACC 0314

#    define _STLP_FORCE_ALLOCATORS(t,a) \
typedef typename _Alloc_traits<t,a>::_Orig _STLP_dummy_type1;\
typedef typename _STLP_dummy_type1:: _STLP_TEMPLATE rebind<t>::other _STLP_dummy_type2;
#    define _STLP_NO_CWCHAR
#    if defined (_NAMESPACE_STD) // option -AA
// from now, we have a full standard lib in namespace std
#      define _STLP_NATIVE_INCLUDE_PATH       ../include_std
//#      define _STLP_HPACC_ONLY_NATIVE_STRING 1 // STLPort _string.c includes <locale>
#      define _STLP_HP_ACC_COMPAT            -1
#    else // kind of compatibility mode
#       define _STLP_NATIVE_INCLUDE_PATH       ../include
#      define _STLP_VENDOR_GLOBAL_STD         1
#      define _STLP_VENDOR_GLOBAL_CSTD        1
#      define _STLP_DONT_THROW_RANGE_ERRORS   1
#      define _STLP_NO_ROPE                   1
#    endif /* _NAMESPACE_STD */
#  endif /* 314 */

#  if ((__HP_aCC >= 30000 && __HP_aCC < 31400) || (__HP_aCC == 1)) // A.03.13: __HP_aCC == 1

#    if (__HP_aCC != 1)
#      define _STLP_HAS_NO_NEW_C_HEADERS 1
#    endif

#    define _STLP_NO_QUALIFIED_FRIENDS       1
// aCC bug ? need explicit args on constructors of partial specialized
// classes
#    define _STLP_PARTIAL_SPEC_NEEDS_TEMPLATE_ARGS 1
// ?? fbp : maybe present in some versions ?
#    define _STLP_NO_MEMBER_TEMPLATE_CLASSES 1
#    define _STLP_NO_MEMBER_TEMPLATE_KEYWORD 1
// <exception> and stuff is in global namespace
#    define _STLP_VENDOR_GLOBAL_EXCEPT_STD
// fbp : moved here
#    define _STLP_VENDOR_GLOBAL_CSTD        1
// #     define _INCLUDE_HPUX_SOURCE
#    define _XPG4
#    define _INCLUDE_XOPEN_SOURCE
#    define _INCLUDE_AES_SOURCE
#  endif /* < 314 */
#  if __HP_aCC == 1
#    define _STLP_BROKEN_USING_IN_CLASS
#    define _STLP_USING_BASE_MEMBER
#    define _STLP_NO_CWCHAR
// #     define _STLP_NO_WCHAR_T 1
#  endif
#endif /* HP_ACC */

//
#ifndef __HP_aCC
#  define _STLP_NATIVE_INCLUDE_PATH ../CC
#  define _STLP_NATIVE_C_INCLUDE_PATH ../include
// it is HP's old cfront-based compiler.
#  define _STLP_NO_BOOL 1
// #  define _STLP_DONT_USE_BOOL_TYPEDEF 1
#  define _STLP_NO_NEW_NEW_HEADER 1
#  define _STLP_LIMITED_DEFAULT_TEMPLATES 1
#  define _STLP_NO_SIGNED_BUILTINS
#  define _STLP_HAS_NO_NAMESPACES 1
#  define _STLP_NEED_TYPENAME 1
#  define _STLP_NEED_EXPLICIT 1
#  define _STLP_NO_EXCEPTION_SPEC 1
#  define _STLP_NONTEMPL_BASE_MATCH_BUG 1
#  define _STLP_NO_ARROW_OPERATOR 1
#  define _STLP_BASE_MATCH_BUG
#  define _STLP_BASE_TYPEDEF_OUTSIDE_BUG 1
#  define _STLP_NO_NEW_STYLE_CASTS 1
// #  define _STLP_NO_WCHAR_T 1
// #  define _STLP_LONG_LONG long long
#  define _STLP_NEED_MUTABLE 1
#  define _STLP_NO_PARTIAL_SPECIALIZATION_SYNTAX 1
#  define _STLP_NO_BAD_ALLOC 1
#  define _STLP_NO_MEMBER_TEMPLATES 1
#  define _STLP_NO_MEMBER_TEMPLATE_CLASSES 1
#  define _STLP_NO_MEMBER_TEMPLATE_KEYWORD 1
#  define _STLP_NO_FRIEND_TEMPLATES 1
#  define _STLP_NO_QUALIFIED_FRIENDS 1
#  define _STLP_NO_CLASS_PARTIAL_SPECIALIZATION 1
#  define _STLP_NO_FUNCTION_TMPL_PARTIAL_ORDER 1
#  define _STLP_MEMBER_POINTER_PARAM_BUG 1
#  define _STLP_NON_TYPE_TMPL_PARAM_BUG 1
#  define _STLP_NO_DEFAULT_NON_TYPE_PARAM 1
// #  define _STLP_NO_METHOD_SPECIALIZATION 1
#  define _STLP_NO_EXPLICIT_FUNCTION_TMPL_ARGS 1
#  define _STLP_NO_EXCEPTION_HEADER 1
#  define _STLP_DEF_CONST_PLCT_NEW_BUG 1
#  define _STLP_DEF_CONST_DEF_PARAM_BUG 1
#  define _STLP_HAS_NO_NEW_C_HEADERS 1
// #  define _STLP_STATIC_CONST_INIT_BUG 1
// #  define _STLP_THROW_RETURN_BUG 1
// #  define _STLP_LINK_TIME_INSTANTIATION 1
// #  define _STLP_NO_TEMPLATE_CONVERSIONS 1
#  define _STLP_NO_TYPEINFO 1
#  define _STLP_WCHAR_T_IS_USHORT 1

#endif /* cfront */
