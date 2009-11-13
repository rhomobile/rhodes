#ifndef __stlp_config_symbian_h
#define __stlp_config_symbian_h

//==========================================================

#if defined(_WIN32)
#  undef _WIN32
#endif

//==========================================================

#ifdef __cplusplus
#  include <e32cmn.h>
#endif

//==========================================================

#if defined(__MARM__)
#  if !defined(_M_ARM)
#    define _M_ARM  1
#  endif
#else
#  if !defined(_M_IX86)
#    define _M_IX86 1
#  endif
#endif

//==========================================================

#define __PIPS__

//==========================================================

#define _STLP_HAS_NO_NEW_C_HEADERS          1
#define _STLP_NO_NEW_NEW_HEADER             1
#define _STLP_NO_NEW_HEADER                 1
#define _STLP_NEW_DONT_THROW_BAD_ALLOC      1
#define _STLP_NO_EXCEPTION_HEADER           1
#define _STLP_NO_TYPEINFO                   1
#define _STLP_NO_VENDOR_STDLIB_L            1
#define _STLP_USE_NOT_INIT_SEGMENT          1
#define _STLP_NO_LONG_DOUBLE                1
#define _STLP_NO_OWN_NAMESPACE              1
#define _STLP_USE_MALLOC                    1
#define _STLP_USE_NO_EXTERN_RANGE_ERRORS    1
#define _STLP_USE_SIMPLE_NODE_ALLOC         1
#define _STLP_DONT_THROW_RANGE_ERRORS       1
#define _STLP_EXPOSE_GLOBALS_IMPLEMENTATION 1
#define _STLP_USE_NEWALLOC                  1
#define _STLP_INLINE_EXCEPTION_CLASS        1
#define _STLP_USE_PARTIAL_SPEC_WORKAROUND   1
#define _STLP_NEEDS_EXTRA_TEMPLATE_CONSTRUCTORS 1
//==========================================================

#ifdef _STLP_REAL_LOCALE_IMPLEMENTED
#  undef _STLP_REAL_LOCALE_IMPLEMENTED
#endif

//==========================================================

#define _STLP_NATIVE_C_INCLUDE_PATH   stdapis
//==========================================================

#define _Locale_SPACE   _CTYPE_S
#define _Locale_PRINT   _CTYPE_R
#define _Locale_CNTRL   _CTYPE_C
#define _Locale_UPPER   _CTYPE_U
#define _Locale_LOWER   _CTYPE_L
#define _Locale_ALPHA   _CTYPE_A
#define _Locale_DIGIT   _CTYPE_D
#define _Locale_PUNCT   _CTYPE_P
#define _Locale_XDIGIT  _CTYPE_X

//==========================================================

#endif
