/*
 * Copyright (c) 1999
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1999
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */


#ifndef _STLP_C_LOCALE_H
#define _STLP_C_LOCALE_H

/*
 * Implementation dependent definitions.
 * Beware: This header is not a purely internal header, it is also included
 * from the outside world when building the STLport library. So this header
 * should not reference internal headers (stlport/stl/_*.h) directly.
 */
#if defined (__sgi)
#  if defined (ROOT_65) /* IRIX 6.5.x */
#    include <sgidefs.h>
#    include <standards.h>
#    include <wchar.h>
#    include <ctype.h>
#  else /* IRIX pre-6.5 */
#    include <sgidefs.h>
#    include <standards.h>
#    if !defined(_SIZE_T) && !defined(_SIZE_T_)
#      define _SIZE_T
#      if (_MIPS_SZLONG == 32)
typedef unsigned int size_t;
#      endif
#      if (_MIPS_SZLONG == 64)
typedef unsigned long size_t;
#      endif
#    endif
#    if !defined (_WCHAR_T)
#      define _WCHAR_T
#      if (_MIPS_SZLONG == 32)
typedef long wchar_t;
#      endif
#      if (_MIPS_SZLONG == 64)
typedef __int32_t wchar_t;
#      endif
#    endif /* _WCHAR_T */
#    if !defined (_WINT_T)
#      define _WINT_T
#      if (_MIPS_SZLONG == 32)
typedef long wint_t;
#      endif
#      if (_MIPS_SZLONG == 64)
typedef __int32_t wint_t;
#      endif
#    endif /* _WINT_T */
#    if !defined (_MBSTATE_T)
#      define _MBSTATE_T
/* _MSC_VER check is here for historical reason and seems wrong as it is the macro defined
 * by Microsoft compilers to give their version and we are currently in a SGI platform scope.
 * However _MSC_VER might also be a SGI compiler macro so we keep it this way.*/
#      if defined (_MSC_VER)
typedef int mbstate_t;
#      else
typedef char mbstate_t;
#      endif
#    endif /* _MBSTATE_T */
#  endif /* ROOT65 */
#else /* __sgi */
#  include <stddef.h>
#  if !defined (ANDROID)
/* The Android wchar.h header is broken, and tries to redefine wchar_t. */
#    include <wchar.h>
#  endif
#  include <ctype.h>
#endif /* __sgi */

/*
 * GENERAL FRAMEWORK
 */

/*
 * Opaque types, implementation (if there is one) depends
 * on platform locale API.
 */
struct _Locale_ctype;
struct _Locale_numeric;
struct _Locale_time;
struct _Locale_collate;
struct _Locale_monetary;
struct _Locale_messages;
struct _Locale_name_hint;

/*
  Bitmask macros.
*/

/*
 * For narrow characters, we expose the lookup table interface.
 */

/* Internal bitmask macros, os-specific. */

#if defined (__sgi)              /* IRIX */

#  define _Locale_S      0x00000008      /* Spacing character */
#  define _Locale_A      0x00004000      /* Alphabetical characters only */
#  define _Locale_B      0x00000040      /* Obsolete: was space char only */
#  define _Locale_PR     0x00008000      /* Printable characters only */
#  define _Locale_G      0x40000000      /* Graphic characters only */
#  define _Locale_BL     0x80000000      /* The blank character class */

/* Public bitmask macros, must be defined for every OS. These values, of
 * course, are specific to IRIX. */

#  define _Locale_CNTRL  0x00000020      /* Control character */
#  define _Locale_UPPER  0x00000001      /* Upper case */
#  define _Locale_LOWER  0x00000002      /* Lower case */
#  define _Locale_DIGIT  0x00000004      /* Numeral (digit) */
#  define _Locale_XDIGIT 0x00000080      /* heXadecimal digit */
#  define _Locale_PUNCT  0x00000010      /* Punctuation */
#  define _Locale_SPACE  (_Locale_S | _Locale_BL)
#  define _Locale_PRINT  (_Locale_PUNCT | _Locale_UPPER | _Locale_LOWER | \
                        _Locale_DIGIT | _Locale_A | _Locale_XDIGIT | \
                        _Locale_PR)
#  define _Locale_ALPHA  _Locale_A

/*
* All of these except for graph and blank are from the C standard;
* graph and blank are XPG4.  (graph in XPG4 doesn't mean quite the
* same thing as graph in the C++ library)
*/

#endif /* IRIX */


#if defined (__Lynx__)
 /* azov: On Lynx isalpha defined as (_U | _L), which gives us a mask
  * unusable in ctype_table. So we have to redefine it and use hard-coded
  * numbers (to avoid potential clashes if system headers change).
  *
  * P.S. Actually, I see no reason in using platform-specific masks -
  * having just one set of masks for all platforms should work just as
  * well - we only use them internally and they don't have to be equal
  * to whatever defined in local ctype.h
  *
  */
#  define _Locale_CNTRL  040     /* _C, Control character */
#  define _Locale_UPPER  01      /* _U, Upper case */
#  define _Locale_LOWER  02      /* _L, Lower case */
#  define _Locale_DIGIT  04      /* _N, Numeral (digit) */
#  define _Locale_XDIGIT 0200    /* _X, heXadecimal digit */
#  define _Locale_PUNCT  020     /* _P, Punctuation */
#  define _Locale_SPACE  010     /* _S, Spacing */
#  define _Locale_ALPHA  040000  /* none, Alphanumerical */
#  define _Locale_PRINT  (_Locale_PUNCT | _Locale_UPPER | _Locale_LOWER | \
                          _Locale_DIGIT | _Locale_ALPHA | _Locale_XDIGIT |\
                          _Locale_SPACE ) /* Printable */
#endif /* __Lynx__ */

#if defined (__GNUC__) || defined (__BORLANDC__) || defined (__COMO__)

#  if defined (__CYGWIN__)

#    define _Locale_CNTRL  040
#    define _Locale_UPPER  02
#    define _Locale_LOWER  01
#    define _Locale_DIGIT  04
#    define _Locale_XDIGIT ( 0100 | _Locale_DIGIT )
#    define _Locale_PUNCT  020
#    define _Locale_SPACE  010
#    define _Locale_ALPHA  0200
#    define _Locale_PRINT  (_Locale_ALPHA | _Locale_DIGIT | _Locale_PUNCT | 0400 )

#  elif defined (__FreeBSD__) || ( defined (__APPLE__) && defined (__GNUC__) && (__GNUC__ > 3) )

#    define _Locale_CNTRL _CTYPE_C
#    define _Locale_UPPER _CTYPE_U
#    define _Locale_LOWER _CTYPE_L
#    define _Locale_DIGIT _CTYPE_D
#    define _Locale_XDIGIT _CTYPE_X
#    define _Locale_PUNCT _CTYPE_P
#    define _Locale_SPACE _CTYPE_S
#    define _Locale_PRINT _CTYPE_R
#    define _Locale_ALPHA _CTYPE_A

#  elif defined (__NetBSD__) || defined (__OpenBSD__) || defined (__amigaos__) || defined(ANDROID)

#    define _Locale_CNTRL _C
#    define _Locale_UPPER _U
#    define _Locale_LOWER _L
#    define _Locale_DIGIT _N
#    define _Locale_XDIGIT (_N|_X)
#    define _Locale_PUNCT _P
#    define _Locale_SPACE _S
#    define _Locale_PRINT (_P|_U|_L|_N|_B)
#    define _Locale_ALPHA (_U|_L)
#  elif defined (__EMX__) /* OS/2 with emx runtime */
#    define _Locale_CNTRL _CNTRL
#    define _Locale_UPPER _UPPER
#    define _Locale_LOWER _LOWER
#    define _Locale_DIGIT _DIGIT
#    define _Locale_XDIGIT _XDIGIT
#    define _Locale_PUNCT _PUNCT
#    define _Locale_SPACE _SPACE
#    define _Locale_PRINT _PRINT
#    define _Locale_ALPHA (_UPPER|_LOWER)

#  elif defined (_STLP_USE_GLIBC) /* linux, using the gnu compiler */

/* This section uses macros defined in the gnu libc ctype.h header */

#    define _Locale_CNTRL  _IScntrl
#    define _Locale_UPPER  _ISupper
#    define _Locale_LOWER  _ISlower
#    define _Locale_DIGIT  _ISdigit
#    define _Locale_XDIGIT _ISxdigit
#    define _Locale_PUNCT  _ISpunct
#    define _Locale_SPACE  _ISspace
#    define _Locale_PRINT  _ISprint
#    define _Locale_ALPHA  _ISalpha

#  endif /* GLIBC */

#endif /* gnu */

#if (defined (__sun) && defined (__SVR4)) || \
    (defined (__digital__) && defined (__unix__)) || \
     defined (_AIX)
/* fbp : condition from AT&T code*/
#  if !(defined (__XPG4_CHAR_CLASS__) || defined (_XPG4_2) || \
       (defined (_XOPEN_SOURCE) && (_XOPEN_VERSION - 0 == 4))) && ! defined (_ISCNTRL)
  /* fbp : on 2.5.1, the defines are different ;( */
  /* # if ( defined (__sun) && defined (__SVR4) && ! defined (_ISCNTRL) ) */
#    define _ISCNTRL _C
#    define _ISUPPER _U
#    define _ISLOWER _L
#    define _ISDIGIT _N
#    define _ISXDIGIT _X
#    define _ISPUNCT _P
#    define _ISSPACE _S
#    define _ISPRINT (_P | _U | _L | _N | _B)
#    define _ISALPHA (_U | _L)
#  endif

#  define _Locale_CNTRL  _ISCNTRL
#  define _Locale_UPPER  _ISUPPER
#  define _Locale_LOWER  _ISLOWER
#  define _Locale_DIGIT  _ISDIGIT
#  define _Locale_XDIGIT _ISXDIGIT
#  define _Locale_PUNCT  _ISPUNCT
#  define _Locale_SPACE  _ISSPACE
#  define _Locale_PRINT  _ISPRINT
#  define _Locale_ALPHA  _ISALPHA
#elif defined (__MWERKS__) && defined (N_PLAT_NLM)
#  define _Locale_CNTRL  _CNTRL_
#  define _Locale_UPPER  _UPPER_
#  define _Locale_LOWER  _LOWER_
#  define _Locale_DIGIT  _DIGIT_
#  define _Locale_XDIGIT _XDIGIT_
#  define _Locale_PUNCT  _PUNCT_
#  define _Locale_SPACE  _SPACE_
#  define _Locale_PRINT  (_PUNCT_|_UPPER_|_LOWER_|_DIGIT_|_BLANK_)
#  define _Locale_ALPHA  (_UPPER_|_LOWER_)
#elif defined (__MWERKS__)
#  define _Locale_CNTRL  __control_char
#  define _Locale_UPPER  __upper_case
#  define _Locale_LOWER  __lower_case
#  define _Locale_DIGIT  __digit
#  define _Locale_XDIGIT __hex_digit
#  define _Locale_PUNCT  __punctuation
#  define _Locale_SPACE  __space_char
#  define _Locale_PRINT  __printable
#  define _Locale_ALPHA  __alphanumeric
#elif defined (__BORLANDC__)
#  define _Locale_CNTRL  _IS_CTL
#  define _Locale_UPPER  _IS_UPP
#  define _Locale_LOWER  _IS_LOW
#  define _Locale_DIGIT  _IS_DIG
#  define _Locale_XDIGIT _IS_HEX
#  define _Locale_PUNCT  _IS_PUN
#  define _Locale_SPACE  _IS_SP
#  define _Locale_PRINT  (_IS_SP|_IS_PUN|_IS_UPP|_IS_LOW|_IS_DIG)
#  define _Locale_ALPHA  _IS_ALPHA
#elif defined (_STLP_MSVC_LIB) || defined (__MINGW32__)
#  define _Locale_CNTRL  _CONTROL
#  define _Locale_UPPER  _UPPER
#  define _Locale_LOWER  _LOWER
#  define _Locale_DIGIT  _DIGIT
#  define _Locale_XDIGIT _HEX
#  define _Locale_PUNCT  _PUNCT
#  define _Locale_SPACE  _SPACE
#  define _Locale_PRINT  (_ALPHA | _DIGIT | _BLANK | _PUNCT)
#  define _Locale_ALPHA  ( _ALPHA & ~ (_UPPER | _LOWER ))
#elif defined (__DMC__)
#  define _Locale_CNTRL  _CONTROL
#  define _Locale_UPPER  _UPPER
#  define _Locale_LOWER  _LOWER
#  define _Locale_DIGIT  _DIGIT
#  define _Locale_XDIGIT _HEX
#  define _Locale_PUNCT  _PUNCT
#  define _Locale_SPACE  _SPACE
#  define _Locale_PRINT  (_UPPER | _LOWER | _DIGIT | _PUNCT | _SPACE)
#  define _Locale_ALPHA  _ALPHA & ~(_UPPER | _LOWER)
#elif defined (__MRC__) || defined (__SC__)    /* *TY 02/24/2000 - added support for MPW */
#  define _Locale_CNTRL  _CTL
#  define _Locale_UPPER  _UPP
#  define _Locale_LOWER  _LOW
#  define _Locale_DIGIT  _DIG
#  define _Locale_XDIGIT _HEX
#  define _Locale_PUNCT  _PUN
#  define _Locale_SPACE  _BLA
#  define _Locale_PRINT  (_UPP | _LOW | _DIG | _PUN | _BLA)
#  define _Locale_ALPHA  (_UPP | _LOW)
#elif defined (__MLCCPP__)
#  define _Locale_CNTRL    1
#  define _Locale_UPPER    2
#  define _Locale_LOWER    4
#  define _Locale_DIGIT    8
#  define _Locale_XDIGIT  16
#  define _Locale_PUNCT   32
#  define _Locale_SPACE   64
#  define _Locale_PRINT  128
#  define _Locale_ALPHA  256
#elif defined (__GNUC__) && (__GNUC__ == 3) && defined (__APPLE__)
#  define _Locale_CNTRL _C
#  define _Locale_UPPER _U
#  define _Locale_LOWER _L
#  define _Locale_DIGIT _D
#  define _Locale_XDIGIT _X
#  define _Locale_PUNCT _P
#  define _Locale_SPACE _S
#  define _Locale_PRINT _R
#  define _Locale_ALPHA _A
#elif defined (__hpux) || defined (__osf__)
#  if defined (__HP_aCC) && !defined (_INCLUDE_HPUX_SOURCE)
#    define _ISALPHA      0x001
#    define _ISALNUM      0x002
#    define _ISBLANK      0x004
#    define _ISCNTRL      0x008
#    define _ISDIGIT      0x010
#    define _ISGRAPH      0x020
#    define _ISLOWER      0x040
#    define _ISPRINT      0x080
#    define _ISPUNCT      0x100
#    define _ISSPACE      0x200
#    define _ISUPPER      0x400
#    define _ISXDIGIT     0x800
#  endif
#  define _Locale_CNTRL  _ISCNTRL
#  define _Locale_UPPER  _ISUPPER
#  define _Locale_LOWER  _ISLOWER
#  define _Locale_DIGIT  _ISDIGIT
#  define _Locale_XDIGIT _ISXDIGIT
#  define _Locale_PUNCT  _ISPUNCT
#  define _Locale_SPACE  _ISSPACE
#  define _Locale_PRINT  _ISPRINT
#  define _Locale_ALPHA  _ISALPHA
#elif defined (__MVS__) || defined (__OS400__)
#  define _Locale_CNTRL __ISCNTRL
#  define _Locale_UPPER __ISUPPER
#  define _Locale_LOWER __ISLOWER
#  define _Locale_DIGIT __ISDIGIT
#  define _Locale_XDIGIT __ISXDIGIT
#  define _Locale_PUNCT __ISPUNCT
#  define _Locale_SPACE __ISSPACE
#  define _Locale_PRINT __ISPRINT
#  define _Locale_ALPHA __ISALPHA
#elif defined (__QNXNTO__)  || defined (__WATCOMC__)
#  define _Locale_CNTRL _CNTRL
#  define _Locale_UPPER _UPPER
#  define _Locale_LOWER _LOWER
#  define _Locale_DIGIT _DIGIT
#  define _Locale_XDIGIT _XDIGT
#  define _Locale_PUNCT _PUNCT
#  define _Locale_SPACE _SPACE
#  define _Locale_PRINT _PRINT
#  define _Locale_ALPHA (_UPPER | _LOWER)
#elif defined (__DJGPP)
#  define _Locale_CNTRL  __dj_ISCNTRL
#  define _Locale_UPPER  __dj_ISUPPER
#  define _Locale_LOWER  __dj_ISLOWER
#  define _Locale_DIGIT  __dj_ISDIGIT
#  define _Locale_XDIGIT __dj_ISXDIGIT
#  define _Locale_PUNCT  __dj_ISPUNCT
#  define _Locale_SPACE  __dj_ISSPACE
#  define _Locale_PRINT  __dj_ISPRINT
#  define _Locale_ALPHA  __dj_ISALPHA
#elif defined (_STLP_SCO_OPENSERVER)
#  define _Locale_CNTRL _C
#  define _Locale_UPPER _U
#  define _Locale_LOWER _L
#  define _Locale_DIGIT _N
#  define _Locale_XDIGIT _X
#  define _Locale_PUNCT _P
#  define _Locale_SPACE _S
#  define _Locale_PRINT _R
#  define _Locale_ALPHA _A
#elif defined (__NCR_SVR)
#  define _Locale_CNTRL _C
#  define _Locale_UPPER _U
#  define _Locale_LOWER _L
#  define _Locale_DIGIT _N
#  define _Locale_XDIGIT _X
#  define _Locale_PUNCT _P
#  define _Locale_SPACE _S
#  define _Locale_PRINT (_P | _U | _L | _N | _B)
#  define _Locale_ALPHA (_U | _L)
#elif defined (_CRAY)
#  define _Locale_CNTRL  _CNTRL
#  define _Locale_UPPER  _UPPER
#  define _Locale_LOWER  _LOWER
#  define _Locale_DIGIT  _DIGIT
#  define _Locale_XDIGIT _XDIGIT
#  define _Locale_PUNCT  _PUNCT
#  define _Locale_SPACE  _SPACE
#  define _Locale_PRINT  _PRINT
#  define _Locale_ALPHA  _ALPHA
#endif

/* We arbitrarily consider _Locale_CNTRL macro to check locale facet numeric
 * identifier has been defined for the platform/compiler:
 */
#if !defined (_Locale_CNTRL)
#  error Unable to find your platform locale facets definitions, please grant them.
#endif

#endif /* _STLP_C_LOCALE_H */
