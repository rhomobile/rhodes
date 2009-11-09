// STLport configuration file
// It is internal STLport header - DO NOT include it directly

#define _STLP_COMPILER "Metrowerks CodeWarrior"

// Bring in definition of __MSL__ and related items
#include <mslGlobals.h>
#include <ansi_parms.h>

// *** CodeWarrior Compiler Common Features ***
#if __option(longlong)
#  define _STLP_LONG_LONG  long long
#endif

#define _STLP_USE_UNIX_EMULATION_IO  1

#define _STLP_USE_AUTO_PTR_CONVERSIONS  1

#ifdef __INTEL__
#  define _STLP_LITTLE_ENDIAN
#else
#  define _STLP_BIG_ENDIAN
#endif

#if defined(_MSL_NO_LOCALE)
#  define _STLP_NO_IMPORT_LOCALE
#endif

#if !__option( wchar_type )
#  define _STLP_WCHAR_T_IS_USHORT
#endif

#if __MWERKS__ < 0x3000
// *** CodeWarrior Compiler Common Bugs ***
#  define __MSL_FIX_ITERATORS__(myType)    // Some MSL headers rely on this
#  define _STLP_NO_FRIEND_TEMPLATES 1  // Bug mysteriously reintroduced in this version.
#  define _STLP_THROW_RETURN_BUG  1
#endif

#if __MWERKS__ >= 0x2405
#  define _STLP_HAS_NATIVE_FLOAT_ABS
#endif

#if __MWERKS__ < 0x2405
#  define _STLP_STATIC_CONST_INIT_BUG
#endif

#if __MWERKS__ <= 0x2303
#  define _STLP_NO_TEMPLATE_CONVERSIONS  1
#  define _STLP_NO_MEMBER_TEMPLATE_KEYWORD  1
#endif

#if __MWERKS__ < 0x2301
#  define _STLP_MEMBER_SPECIALIZATION_BUG  1
#endif

#if __MWERKS__ < 0x2300    // CW Pro5 features
#  define _STLP_INLINE_MEMBER_TEMPLATES 1
#  define _STLP_RELOPS_IN_STD_BUG   1
#  define _STLP_DEF_CONST_PLCT_NEW_BUG 1
#  define _STLP_DEF_CONST_DEF_PARAM_BUG 1
#  define _STLP_NO_TYPENAME_ON_RETURN_TYPE
#endif

// fixes to native inclusion wrappers.
#if __MWERKS__ >= 0x2300  // CWPro5 changes paths - dwa 2/28/99
#  define _STLP_NATIVE_INCLUDE_PATH  ../nwsdk/include/nlm
#  define _STLP_NATIVE_C_INCLUDE_PATH  ../nwsdk/include/nlm
#  define _STLP_NATIVE_HEADER(header)     <../nwsdk/include/nlm/##header>
#  define _STLP_NATIVE_CPP_C_HEADER(header)     <../Libraries/MSL C++/Include/##header>
#  define _STLP_NATIVE_C_HEADER(header)     <../nwsdk/include/nlm/##header>
#  define _STLP_NATIVE_CPP_RUNTIME_HEADER(header) <../Libraries/MSL C++/Include/##header>

#  define _STLP_VENDOR_GLOBAL_CSTD  1
#  define _STLP_NO_VENDOR_STDLIB_L  1
#  define _STLP_NO_VENDOR_MATH_F    1
#  define _STLP_NO_VENDOR_MATH_L    1
     //#   define _MSL_NO_THROW_SPECS
     //#   define _STD
     //#   define _CSTD
#endif

// fbp
#if !defined( __MSL_CPP__ ) || __MSL_CPP__ <= 0x4105
#  define _STLP_NO_NATIVE_WIDE_STREAMS 1
# endif

#define _STLP_DLLEXPORT_NEEDS_PREDECLARATION 1
