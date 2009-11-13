/* STLport configuration file
 * It is internal STLport header - DO NOT include it directly
 */

#define _STLP_COMPILER "gcc"

/* Systems having GLIBC installed have different traits */
#if defined (__linux__)
#  ifndef _STLP_USE_GLIBC
#    define _STLP_USE_GLIBC 1
#  endif
#  if defined (__UCLIBC__) && !defined (_STLP_USE_UCLIBC)
#    define _STLP_USE_UCLIBC 1
#  endif
#endif

#if defined (__CYGWIN__) && \
     (__GNUC__ >= 3) && (__GNUC_MINOR__ >= 3) && !defined (_GLIBCPP_USE_C99)
#  define _STLP_NO_VENDOR_MATH_L
#  define _STLP_NO_VENDOR_STDLIB_L
#endif

#if (__GNUC__ < 3)
#  define _STLP_NO_VENDOR_STDLIB_L
#endif

/* We guess if we are using the cygwin distrib that has a special include schema.
 * There is no way to distinguish a cygwin distrib used in no-cygwin mode from a
 * mingw install. We are forced to use a configuration option
 */
#if !defined (_STLP_NATIVE_INCLUDE_PATH) && \
    (defined (__CYGWIN__) || defined (__MINGW32__) && defined (_STLP_NO_CYGWIN))
#  if (__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ > 3))
#    define _STLP_NATIVE_INCLUDE_PATH ../../../__GNUC__.__GNUC_MINOR__.__GNUC_PATCHLEVEL__/include/c++
#  elif defined (_STLP_NO_CYGWIN)
#    define _STLP_NATIVE_INCLUDE_PATH ../mingw
/*#  else
 * Before version gcc 3.4, the cygwin package include path was conform to the
 * GNU convention which is set later in this file.
 */
#  endif
#endif

#if (__GNUC__ < 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ < 4))
/* define for gcc versions before 3.4.0. */
#  define _STLP_NO_MEMBER_TEMPLATE_KEYWORD
#endif

/* azov: gcc on lynx have a bug that causes internal
 * compiler errors when compiling STLport with namespaces turned on.
 * When the compiler gets better - comment out _STLP_HAS_NO_NAMESPACES
 */
#if defined (__Lynx__) && (__GNUC__ < 3)
#  define _STLP_HAS_NO_NAMESPACES 1
#  define _STLP_NO_STATIC_TEMPLATE_DATA 1
/* turn off useless warning about including system headers */
#  define __NO_INCLUDE_WARN__ 1
#endif

/* Tru64 Unix, AIX, HP : gcc there by default uses native ld and hence cannot auto-instantiate
   static template data. If you are using GNU ld, please say so in user_config.h header */
#if (__GNUC__ < 3) && !defined(_STLP_GCC_USES_GNU_LD) && \
   ((defined (__osf__) && defined (__alpha__)) || defined (_AIX) || defined (__hpux) || defined(__amigaos__) )
#  define _STLP_NO_STATIC_TEMPLATE_DATA
#endif

#if !defined (_REENTRANT) && (defined (_THREAD_SAFE) || \
                             (defined (__OpenBSD__) && defined (_POSIX_THREADS)) || \
                             (defined (__MINGW32__) && defined (_MT)))
#  define _REENTRANT
#endif

#if defined (__DJGPP)
#  define _STLP_RAND48    1
#  define _NOTHREADS    1
#  undef  _PTHREADS
#  define _STLP_LITTLE_ENDIAN
#endif

#if defined (__MINGW32__)
/* Mingw32, egcs compiler using the Microsoft C runtime */
#  define _STLP_VENDOR_GLOBAL_CSTD
#  undef  _STLP_NO_DRAND48
#  define _STLP_NO_DRAND48
#  define _STLP_CALL

#  if defined (_STLP_NEW_PLATFORM_SDK)
/* For the moment the Windows SDK coming with Mingw still mimik the old platform SDK. */
#    undef _STLP_NEW_PLATFORM_SDK
#  endif
#endif /* __MINGW32__ */

#if defined (__CYGWIN__) || defined (__MINGW32__)
#  if !defined (_STLP_USE_STATIC_LIB)
#    define _STLP_USE_DECLSPEC 1
#    if !defined (_STLP_USE_DYNAMIC_LIB)
#      define _STLP_USE_DYNAMIC_LIB
#    endif
#    define _STLP_EXPORT_DECLSPEC __declspec(dllexport)
#    define _STLP_CLASS_EXPORT_DECLSPEC __declspec(dllexport)
#    define _STLP_CLASS_IMPORT_DECLSPEC __declspec(dllimport)
#  endif
/* The following is defined independently of _STLP_USE_STATIC_LIB because it is also
 * used to import symbols from PSDK under MinGW
 */
#  define _STLP_IMPORT_DECLSPEC __declspec(dllimport)
#endif

#if defined (__CYGWIN__) || defined (__MINGW32__) || !(defined (_STLP_USE_GLIBC) || defined (__sun) || defined(__APPLE__))
#  if !defined (__MINGW32__) && !defined (__CYGWIN__)
#    define _STLP_NO_NATIVE_MBSTATE_T    1
#  endif
#  if !defined (__MINGW32__) || (__GNUC__ < 3) || (__GNUC__ == 3) && (__GNUC_MINOR__ < 4)
#    define _STLP_NO_NATIVE_WIDE_FUNCTIONS 1
#  endif
#  define _STLP_NO_NATIVE_WIDE_STREAMS   1
#endif

#define _STLP_NORETURN_FUNCTION __attribute__((noreturn))

/* Mac OS X is a little different with namespaces and cannot instantiate
 * static data members in template classes */
#if defined (__APPLE__)
#  if ((__GNUC__ < 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ < 3)))
/* Mac OS X is missing a required typedef and standard macro */
typedef unsigned int wint_t;
#  endif

#  define __unix

#  if (__GNUC__ < 3)

 /* Mac OS X needs one and only one source file to initialize all static data
  * members in template classes. Only one source file in an executable or
  * library can declare instances for such data members, otherwise duplicate
  * symbols will be generated. */

#    define _STLP_NO_STATIC_TEMPLATE_DATA
#    define _STLP_STATIC_CONST_INIT_BUG 1
#    define _STLP_STATIC_TEMPLATE_DATA 0
#    define _STLP_WEAK_ATTRIBUTE 1
 /* Workaround for the broken Mac OS X C++ preprocessor which cannot handle
  * parameterized macros in #include statements */
#    define _STLP_NATIVE_HEADER(header) <../g++/##header##>
#    define _STLP_NATIVE_C_HEADER(header) <../include/##header##>
#    define _STLP_NATIVE_CPP_C_HEADER(header) <../g++/##header##>
#    define _STLP_NATIVE_OLD_STREAMS_HEADER(header) <../g++/##header##>
#    define _STLP_NATIVE_CPP_RUNTIME_HEADER(header) <../g++/##header##>
#  endif /* __GNUC__ < 3 */

#  define _STLP_NO_LONG_DOUBLE

/* Mac OS X needs all "::" scope references to be "std::" */
#  define _STLP_USE_NEW_C_HEADERS

#  define _STLP_NO_VENDOR_STDLIB_L

#endif /* __APPLE__ */


#if defined(__BEOS__) && defined(__INTEL__)
#  define _STLP_NATIVE_HEADER(header) <../stlport/beos/##header##>
#  define _STLP_NATIVE_C_HEADER(header) <../stlport/beos/##header##>
#  define _STLP_NATIVE_CPP_C_HEADER(header) <../stlport/beos/##header##>
#  define _STLP_NATIVE_OLD_STREAMS_HEADER(header) <../stlport/beos/##header##>
#  define _STLP_NATIVE_CPP_RUNTIME_HEADER(header) <../stlport/beos/##header##>
#  define _STLP_NO_NATIVE_WIDE_FUNCTIONS 1
#  define _STLP_NO_NATIVE_WIDE_STREAMS   1
/*
#  define _NOTHREADS 1
*/
#  ifdef _PTHREADS
#    undef  _PTHREADS
#  endif
#  ifdef _STLP_PTHREADS
#    undef _STLP_PTHREADS
#  endif
#  define _STLP_USE_STDIO_IO 1
#  define _STLP_USE_GLIBC 1
#endif

/* g++ 2.7.x and above */
#define _STLP_LONG_LONG long long

#ifdef _STLP_USE_UCLIBC
/*
#  ifndef __DO_C99_MATH__
*/
  /* No *f math fuctions variants (i.e. sqrtf, fabsf, etc.) */
#  define _STLP_NO_VENDOR_MATH_F
  /* No *l math fuctions variants (i.e. sqrtl, fabsl, etc.) */
#  define _STLP_NO_VENDOR_MATH_L
#  define _STLP_NO_LONG_DOUBLE
/*
#  endif
*/
#endif

#if defined (__OpenBSD__) || defined (__FreeBSD__)
#  define _STLP_NO_VENDOR_MATH_L
#  define _STLP_NO_VENDOR_STDLIB_L /* no llabs */
#  ifndef __unix
#    define __unix
#  endif
#endif

#if defined (__alpha__)
#  define _STLP_NO_VENDOR_MATH_L
#  define _STLP_NO_IEC559_SUPPORT
#endif

#if defined (__hpux)
#  define _STLP_NO_VENDOR_STDLIB_L /* no llabs */
  /* No *f math fuctions variants (i.e. sqrtf, fabsf, etc.) */
#  define _STLP_NO_VENDOR_MATH_F
#endif

#if (__GNUC__ >= 3)
#  ifndef _STLP_HAS_NO_NEW_C_HEADERS
/*
#    ifndef _STLP_USE_UCLIBC
*/
#    define _STLP_HAS_NATIVE_FLOAT_ABS
/*
#    endif
*/
#  else
#    ifdef _STLP_USE_GLIBC
#      define _STLP_VENDOR_LONG_DOUBLE_MATH  1
#    endif
#  endif
#endif

#if (__GNUC__ < 3)
#  define _STLP_HAS_NO_NEW_C_HEADERS     1
#  define _STLP_VENDOR_GLOBAL_CSTD       1
#  define _STLP_DONT_USE_PTHREAD_SPINLOCK 1
#  ifndef __HONOR_STD
#    define _STLP_VENDOR_GLOBAL_EXCEPT_STD 1
#  endif
/* egcs fails to initialize builtin types in expr. like this : new(p) char();  */
#  define _STLP_DEF_CONST_PLCT_NEW_BUG 1
#endif

/*
#define _STLP_VENDOR_GLOBAL_CSTD 1
*/

#if (__GNUC__ == 2) && (__GNUC_MINOR__ < 95)
#  define _STLP_NO_UNCAUGHT_EXCEPT_SUPPORT
#  define _STLP_NO_UNEXPECTED_EXCEPT_SUPPORT
#  define _STLP_DEF_CONST_DEF_PARAM_BUG 1
#endif

#if (__GNUC_MINOR__ < 9)  && (__GNUC__ < 3) /* gcc 2.8 */
#  define _STLP_NO_TEMPLATE_CONVERSIONS
#  define _STLP_NO_MEMBER_TEMPLATE_CLASSES 1
#  define _STLP_NO_FUNCTION_TMPL_PARTIAL_ORDER 1
#  define _STLP_NO_FRIEND_TEMPLATES 1
#  define _STLP_HAS_NO_NAMESPACES 1
#  define _STLP_NO_METHOD_SPECIALIZATION 1
#  define _STLP_NO_MEMBER_TEMPLATES 1
#  define _STLP_NO_CLASS_PARTIAL_SPECIALIZATION 1
#  define _STLP_DONT_SIMULATE_PARTIAL_SPEC_FOR_TYPE_TRAITS
/*  DJGPP doesn't seem to implement it in 2.8.x */
#  ifdef DJGPP
#    define  _STLP_NO_STATIC_TEMPLATE_DATA 1
#  endif
#endif

#if __GNUC__ <= 2 && __GNUC_MINOR__ <= 7 && !defined (__CYGWIN32__)
/* Will it work with 2.6 ? I doubt it. */
#  if ( __GNUC_MINOR__ < 6 )
__GIVE_UP_WITH_STL(GCC_272);
#  endif

#  define  _STLP_NO_RELOPS_NAMESPACE
#  define  _STLP_NON_TYPE_TMPL_PARAM_BUG
#  define  _STLP_LIMITED_DEFAULT_TEMPLATES 1
#  define  _STLP_DEFAULT_TYPE_PARAM 1
#  define  _STLP_NO_BAD_ALLOC
#  define  _STLP_NO_ARROW_OPERATOR 1
#  ifndef _STLP_NO_STATIC_TEMPLATE_DATA
#    define  _STLP_NO_STATIC_TEMPLATE_DATA
#  endif
#  define  _STLP_STATIC_CONST_INIT_BUG 1
#  define  _STLP_NO_METHOD_SPECIALIZATION 1

#  if !defined (__CYGWIN32__)
#    define _STLP_NESTED_TYPE_PARAM_BUG   1
#    define _STLP_BASE_MATCH_BUG       1
/*  unused operators are required (forward) */
#    define  _STLP_CONST_CONSTRUCTOR_BUG
#    define _STLP_NO_DEFAULT_NON_TYPE_PARAM
#  endif
#  define _STLP_NO_PARTIAL_SPECIALIZATION_SYNTAX 1
#  define _STLP_NO_EXPLICIT_FUNCTION_TMPL_ARGS 1
#  define _STLP_NO_EXCEPTION_HEADER 1
#else /* ! <= 2.7.* */
#endif /* ! <= 2.7.* */

/* static template data members workaround strategy for gcc tries
 * to use weak symbols.
 * if you don't want to use that, #define _STLP_WEAK_ATTRIBUTE=0 ( you'll
 * have to put "#define __PUT_STATIC_DATA_MEMBERS_HERE" line in one of your
 * compilation unit ( or CFLAGS for it ) _before_ including any STL header ).
 */
#if defined (_STLP_NO_STATIC_TEMPLATE_DATA) && ! defined (_STLP_WEAK_ATTRIBUTE )
/* systems using GNU ld or format that supports weak symbols
   may use "weak" attribute
   Linux & Solaris ( x86 & SPARC ) are being auto-recognized here */
#  if defined(_STLP_GNU_LD) || defined(__ELF__) || defined (__CYGWIN__) || \
     (( defined (__SVR4) || defined ( __svr4__ )) && \
      ( defined (sun) || defined ( __sun__ )))
#    define _STLP_WEAK_ATTRIBUTE 1
#  endif
#endif /* _STLP_WEAK_ATTRIBUTE */


/* strict ANSI prohibits "long long" ( gcc) */
#if defined ( __STRICT_ANSI__ )
#  undef _STLP_LONG_LONG
/*
#    define _STLP_STRICT_ANSI 1
*/
#endif

/*
#if !defined (__STRICT_ANSI__) || defined (__BUILDING_STLPORT)
#  define _STLP_USE_TEMPLATE_EXPORT
#  define _STLP_EXPORT_TEMPLATE_KEYWORD extern
#  define _STLP_IMPORT_TEMPLATE_KEYWORD extern
#endif
*/

#ifndef __EXCEPTIONS
#  undef  _STLP_DONT_USE_EXCEPTIONS
#  define _STLP_DONT_USE_EXCEPTIONS 1
#endif

#if (__GNUC__ >= 3)

#  if !defined (_STLP_NATIVE_INCLUDE_PATH)
#    if ( (__GNUC__ == 3 ) && ((__GNUC_MINOR__ == 0) || ((__GNUC_MINOR__ < 3) && defined(__APPLE_CC__))))
#      define _STLP_NATIVE_INCLUDE_PATH ../g++-v3
#    else
#      if ( ((__GNUC__ == 4 ) || (__GNUC_MINOR__ >= 3)) && defined(__APPLE_CC__))
#        define _STLP_NATIVE_INCLUDE_PATH ../c++
/*
* Before version 3.4.0 the 0 patch level was not part of the include path:
*/
#      elif defined (__GNUC_PATCHLEVEL__) && ((__GNUC_PATCHLEVEL__ > 0) || \
                                              (__GNUC__ == 3 && __GNUC_MINOR__ >= 4) || \
                                              (__GNUC__ > 3))
#        define _STLP_NATIVE_INCLUDE_PATH ../__GNUC__.__GNUC_MINOR__.__GNUC_PATCHLEVEL__
#      else
#        define _STLP_NATIVE_INCLUDE_PATH ../__GNUC__.__GNUC_MINOR__
#      endif
#    endif
#  endif

/* Instantiation scheme that used (default) in gcc 3 made void of sense explicit
   instantiation within library: nothing except increased library size. - ptr
 */
#  define _STLP_NO_FORCE_INSTANTIATE

#elif (__GNUC_MINOR__ < 8)

#  if !defined (_STLP_NATIVE_INCLUDE_PATH)
#    define _STLP_NATIVE_INCLUDE_PATH ../g++-include
#  endif

/* tuning of static template data members workaround */
#  if ( _STLP_STATIC_TEMPLATE_DATA < 1 )
#    if ( _STLP_WEAK_ATTRIBUTE > 0 )
#      define _STLP_WEAK __attribute__ (( weak ))
#    else
#      define _STLP_WEAK
#    endif /* _STLP_WEAK_ATTRIBUTE */

#    ifdef __PUT_STATIC_DATA_MEMBERS_HERE
#      define __DECLARE_INSTANCE(type,item,init) type item _STLP_WEAK init
#    else
#      define __DECLARE_INSTANCE(type,item,init)
#    endif /* __PUT_STATIC_DATA_MEMBERS_HERE */
#  endif /* _STLP_STATIC_TEMPLATE_DATA */

#else

/* gcc-2.95.0 used to use "g++-3" directory which has been changed to "g++" in
 * system-dependent "include" for 2.95.2 except for Cygwin and Mingw packages.
 * I expect "g++-3" not being used in later releases.
 * If your installation use "g++-3" include directory for any reason (pre-2.95.2 or Win binary kit),
 * please change the macro below to point to your directory.
 */

#  if !defined (_STLP_NATIVE_INCLUDE_PATH)
#    if defined(__DJGPP)
#      define _STLP_NATIVE_INCLUDE_PATH ../lang/cxx
#    elif (__GNUC__ >= 3) || (__GNUC_MINOR__ >= 97)
#      define _STLP_NATIVE_INCLUDE_PATH ../include/g++-v3
#    elif ((__GNUC_MINOR__ >= 95 && __GNUC_MINOR__ < 97) && \
          !( defined (__FreeBSD__) || defined (__NetBSD__) || defined(__sgi) || defined (__OS2__) ) )
#      define _STLP_NATIVE_INCLUDE_PATH ../g++-3
#    elif (__GNUC_MINOR__ > 8) && (__GNUC_MINOR__ < 95) && (__GNUC__ < 3) && !defined( __Lynx__ )
/* this really sucks, as GNUpro does not really identifies itself, so we have to guess
 * depending on a platform
 */
#      ifdef __hpux
#        define _STLP_NATIVE_INCLUDE_PATH ../g++-3
#      else
#        define _STLP_NATIVE_INCLUDE_PATH ../g++-2
#      endif
#    else
#      define _STLP_NATIVE_INCLUDE_PATH g++
#    endif
#  endif

/* <exception> et al */
#  ifdef __FreeBSD__
#    if (__GNUC__ > 2) || (__GNUC__ == 2 && __GNUC_MINOR__ > 95)
#      define _STLP_NATIVE_CPP_RUNTIME_INCLUDE_PATH ../include
#    endif
#  else
/* azov */
#    ifndef __Lynx__
#      if (__GNUC__ > 2) || (__GNUC__ == 2 && __GNUC_MINOR__ >= 97)
/*
#     define _STLP_NATIVE_CPP_RUNTIME_INCLUDE_PATH ../g++-v3
*/
#      else
#        define _STLP_NATIVE_CPP_RUNTIME_INCLUDE_PATH ../include
#      endif
#    endif
#  endif

#endif /* GNUC_MINOR < 8 */

#if !defined (_STLP_NATIVE_C_INCLUDE_PATH)
#  define _STLP_NATIVE_C_INCLUDE_PATH ../include
#endif

/* Tune settings for the case where static template data members are not
 * instaniated by default
 */
#if defined ( _STLP_NO_STATIC_TEMPLATE_DATA )
#  define _STLP_STATIC_TEMPLATE_DATA 0
#  if !defined ( _STLP_WEAK_ATTRIBUTE )
#    define _STLP_WEAK_ATTRIBUTE 0
#  endif
#  ifdef __PUT_STATIC_DATA_MEMBERS_HERE
#    define __DECLARE_INSTANCE(type,item,init) type item init
#  else
#    define __DECLARE_INSTANCE(type,item,init)
#  endif
#else
#  define _STLP_STATIC_TEMPLATE_DATA 1
#endif
