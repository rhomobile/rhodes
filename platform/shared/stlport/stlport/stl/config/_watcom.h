// STLport configuration file
// It is internal STLport header - DO NOT include it directly

#define _STLP_COMPILER "Watcom"

# ifndef _STLP_USE_NO_IOSTREAMS
#  define _STLP_USE_NO_IOSTREAMS
# endif

# define _STLP_NO_RELOPS_NAMESPACE
# define _STLP_NO_PARTIAL_SPECIALIZATION_SYNTAX

#  define _STLP_HAS_SPECIFIC_PROLOG_EPILOG
#  define _STLP_DONT_SIMULATE_PARTIAL_SPEC_FOR_TYPE_TRAITS
#  define _STLP_USE_OLD_HP_ITERATOR_QUERIES

// On QNX, headers are supposed to be found in /usr/include,
// so default "../include" should work.
# ifndef __QNX__
#  define _STLP_NATIVE_INCLUDE_PATH ../h
# endif

// Inline replacements for locking calls under Watcom
// Define _STLP_NO_WATCOM_INLINE_INTERLOCK to keep using
// standard WIN32 calls
// Define _STL_MULTIPROCESSOR to enable lock
#if !defined(_STLP_NO_WATCOM_INLINE_INTERLOCK)

long    __stl_InterlockedIncrement( long *var );
long    __stl_InterlockedDecrement( long *var );

#ifdef _STL_MULTIPROCESSOR
// Multiple Processors, add lock prefix
#pragma aux __stl_InterlockedIncrement parm [ ecx ] = \
        ".586"                  \
        "mov eax, 1"            \
        "lock xadd [ecx], eax"       \
        "inc eax"               \
        value [eax];


#pragma aux __stl_InterlockedDecrement parm [ ecx ] = \
        ".586"                  \
        "mov eax, 0FFFFFFFFh"   \
        "lock xadd [ecx], eax"       \
        "dec eax"               \
        value [eax];
#else
// Single Processor, lock prefix not needed
#pragma aux __stl_InterlockedIncrement parm [ ecx ] = \
        ".586"                  \
        "mov eax, 1"            \
        "xadd [ecx], eax"       \
        "inc eax"               \
        value [eax];

#pragma aux __stl_InterlockedDecrement parm [ ecx ] = \
        ".586"                  \
        "mov eax, 0FFFFFFFFh"   \
        "xadd [ecx], eax"       \
        "dec eax"               \
        value [eax];
#endif // _STL_MULTIPROCESSOR

long    __stl_InterlockedExchange( long *Destination, long Value );

// xchg has auto-lock
#pragma aux __stl_InterlockedExchange parm [ecx] [eax] = \
        ".586"                  \
        "xchg eax, [ecx]"       \
        value [eax];
#else

#define __stl_InterlockedIncrement      InterlockedIncrement
#define __stl_InterlockedDecrement      InterlockedDecrement
#define __stl_InterlockedExchange       InterlockedExchange
#endif /* INLINE INTERLOCK */

#define _STLP_ATOMIC_INCREMENT(__x) __stl_InterlockedIncrement((long*)__x)
#define _STLP_ATOMIC_DECREMENT(__x) __stl_InterlockedDecrement((long*)__x)
#define _STLP_ATOMIC_EXCHANGE(__x, __y) __stl_InterlockedExchange((long*)__x, (long)__y)

// boris : is this true or just the header is not in /usr/include ?
# ifdef __QNX__
#  define _STLP_NO_TYPEINFO 1
# endif

#  define _STLP_NO_FUNCTION_TMPL_PARTIAL_ORDER 1
#  define _STLP_NO_CLASS_PARTIAL_SPECIALIZATION 1
#  define _STLP_NO_MEMBER_TEMPLATE_KEYWORD 1
#  define _STLP_NO_MEMBER_TEMPLATES 1
#  define _STLP_NO_FRIEND_TEMPLATES 1
#  define _STLP_NO_MEMBER_TEMPLATE_CLASSES 1


#  define _STLP_LIMITED_DEFAULT_TEMPLATES 1
#  define _STLP_HAS_NO_NAMESPACES 1
#  define _STLP_NEED_TYPENAME 1

#  if __WATCOMC__ < 1100
#  define _STLP_NO_WCHAR_T 1
#  define _STLP_NO_PARTIAL_SPECIALIZATION_SYNTAX 1
#  endif

#  define _STLP_NO_EXPLICIT_FUNCTION_TMPL_ARGS 1

#  define _STLP_STATIC_CONST_INIT_BUG 1
// #  define _STLP_THROW_RETURN_BUG 1
#  define _STLP_NO_TEMPLATE_CONVERSIONS 1

#  define _STLP_BASE_TYPEDEF_OUTSIDE_BUG 1

#  define _STLP_HAS_NO_NEW_IOSTREAMS 1
#  define _STLP_HAS_NO_NEW_C_HEADERS 1
#  define _STLP_NO_NEW_NEW_HEADER 1
#  define _STLP_VENDOR_GLOBAL_STD

#  define _STLP_NO_DEFAULT_NON_TYPE_PARAM 1
#  define _STLP_NON_TYPE_TMPL_PARAM_BUG 1
#  define _STLP_NONTEMPL_BASE_MATCH_BUG
#  define _STLP_NO_EXCEPTION_HEADER 1
#  define _STLP_NO_BAD_ALLOC 1

#  define _STLP_NESTED_TYPE_PARAM_BUG 1

#  define _STLP_NO_USING_FOR_GLOBAL_FUNCTIONS 1

#  if (__WATCOM_CPLUSPLUS__ < 1100 )
#   define _STLP_NO_BOOL 1
#   define _STLP_NEED_EXPLICIT 1
#   define _STLP_NEED_MUTABLE 1
#   define _STLP_NO_ARROW_OPERATOR 1
#  endif
// This one is present in 11, but apparently has bugs (with auto_ptr).
#   define _STLP_NO_NEW_STYLE_CASTS 1

// Get rid of Watcom's min and max macros
#undef min
#undef max

// for switches (-xs,  -xss,  -xst)
//
#if !(defined (__SW_XS) || defined (__SW_XSS) || defined(__SW_XST))
#    define _STLP_HAS_NO_EXCEPTIONS 1
# endif

# if defined ( _MT ) && !defined (_NOTHREADS) && !defined (_REENTRANT)
# define _REENTRANT 1
# endif





