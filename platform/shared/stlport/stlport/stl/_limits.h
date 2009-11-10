/*
 * Copyright (c) 1997
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

/* NOTE: This may be not portable code. Parts of numeric_limits<> are
 * inherently machine-dependent.  At present this file is suitable
 * for the MIPS, SPARC, Alpha and ia32 architectures.
 */

#ifndef _STLP_INTERNAL_LIMITS
#define _STLP_INTERNAL_LIMITS

#ifndef _STLP_CLIMITS
#  include <climits>
#endif

#ifndef _STLP_CFLOAT
#  include <cfloat>
#endif

#if defined (_STLP_HAS_WCHAR_T) && !defined (_STLP_INTERNAL_CWCHAR)
#  include <stl/_cwchar.h>
#endif

_STLP_BEGIN_NAMESPACE

enum float_round_style {
  round_indeterminate       = -1,
  round_toward_zero         =  0,
  round_to_nearest          =  1,
  round_toward_infinity     =  2,
  round_toward_neg_infinity =  3
};

enum float_denorm_style {
  denorm_indeterminate = -1,
  denorm_absent        =  0,
  denorm_present       =  1
};

_STLP_MOVE_TO_PRIV_NAMESPACE

// Base class for all specializations of numeric_limits.
template <class __number>
class _Numeric_limits_base {
public:

  static __number (_STLP_CALL min)() _STLP_NOTHROW { return __number(); }
  static __number (_STLP_CALL max)() _STLP_NOTHROW { return __number(); }

#if defined ( _STLP_STATIC_CONST_INIT_BUG)
  enum {
#else
  static const int
#endif

  digits = 0,
  digits10 = 0,
  radix = 0,
  min_exponent = 0,
  min_exponent10 = 0,
  max_exponent = 0,
  max_exponent10 = 0

#if defined ( _STLP_STATIC_CONST_INIT_BUG)
  ,
  has_denorm = denorm_absent,
  round_style = round_toward_zero,
#else
  ;
  static const float_denorm_style has_denorm =  denorm_absent;
  static const float_round_style round_style = round_toward_zero;
  static const bool
#endif

    is_specialized = false,
    is_signed  = false,
    is_integer = false,
    is_exact = false,
    has_infinity = false,
    has_quiet_NaN = false,
    has_signaling_NaN = false,
    has_denorm_loss = false,
    is_iec559 = false,
    is_bounded = false,
    is_modulo = false,
    traps = false,
    tinyness_before = false
#if defined ( _STLP_STATIC_CONST_INIT_BUG)
  }
#endif
  ;

  static __number _STLP_CALL epsilon() _STLP_NOTHROW     { return __number(); }
  static __number _STLP_CALL round_error() _STLP_NOTHROW { return __number(); }

  static __number _STLP_CALL infinity() _STLP_NOTHROW      { return __number(); }
  static __number _STLP_CALL quiet_NaN() _STLP_NOTHROW     { return __number(); }
  static __number _STLP_CALL signaling_NaN() _STLP_NOTHROW { return __number(); }
  static __number _STLP_CALL denorm_min() _STLP_NOTHROW    { return __number(); }
};

// Base class for integers.

#ifdef _STLP_LIMITED_DEFAULT_TEMPLATES
#  ifdef _STLP_LONG_LONG
#    define _STLP_LIMITS_MIN_TYPE _STLP_LONG_LONG
#    define _STLP_LIMITS_MAX_TYPE unsigned _STLP_LONG_LONG
#  else
#    define _STLP_LIMITS_MIN_TYPE long
#    define _STLP_LIMITS_MAX_TYPE unsigned long
#  endif
#else
#  define _STLP_LIMITS_MIN_TYPE _Int
#  define _STLP_LIMITS_MAX_TYPE _Int
#endif /* _STLP_LIMITED_DEFAULT_TEMPLATES */

template <class _Int,
          _STLP_LIMITS_MIN_TYPE __imin,
          _STLP_LIMITS_MAX_TYPE __imax,
          int __idigits, bool __ismod>
class _Integer_limits : public _Numeric_limits_base<_Int> {
public:

  static _Int (_STLP_CALL min) () _STLP_NOTHROW { return (_Int)__imin; }
  static _Int (_STLP_CALL max) () _STLP_NOTHROW { return (_Int)__imax; }

#if defined (_STLP_STATIC_CONST_INIT_BUG)
  enum {
#else
  static const int
#endif
  digits = (__idigits < 0) ?
  ((int)((sizeof(_Int) * (CHAR_BIT))) - ((__imin == 0) ? 0 : 1))
  : (__idigits),
  digits10 = (digits * 301UL) / 1000,
  radix = 2
#if !defined (_STLP_STATIC_CONST_INIT_BUG)
  ;
  static const bool
#else
  ,
#endif
  is_specialized = true,
  is_signed = (__imin != 0),
  is_integer = true,
  is_exact = true,
  is_bounded = true,
  is_modulo = __ismod
#if defined ( _STLP_STATIC_CONST_INIT_BUG)
  }
#endif
  ;
};

// Base class for floating-point numbers.
template <class __number,
         int __Digits, int __Digits10,
         int __MinExp, int __MaxExp,
         int __MinExp10, int __MaxExp10,
         bool __IsIEC559,
         float_round_style __RoundStyle>
class _Floating_limits : public _Numeric_limits_base<__number> {
public:

#if defined (_STLP_STATIC_CONST_INIT_BUG)
  enum {
#else
  static const int
#endif

  digits = __Digits,
  digits10 = __Digits10,

  radix = (  FLT_RADIX /* 2 */ ),
  min_exponent = __MinExp,
  max_exponent = __MaxExp,
  min_exponent10 = __MinExp10,
  max_exponent10 = __MaxExp10

#if defined (_STLP_STATIC_CONST_INIT_BUG)
  ,
  has_denorm = denorm_indeterminate,
  round_style = __RoundStyle,
#else
  ;
  static const float_denorm_style has_denorm = denorm_indeterminate;
  static const float_round_style round_style = __RoundStyle;
  static const bool
#endif

  is_specialized = true,
  is_signed = true,

  //IEC 559 specify the floating point representation of
  //infinity, quiet and signaling Not a Number. Not supporting
  //it is consider as not being able to grant those values.
#if (defined (_STLP_MSVC) && (_STLP_MSVC < 1300))
  //MSVC 6 do not fully support IEC 599 but grant a good infinity value.
  has_infinity      = true,
#else
  has_infinity      = __IsIEC559,
#endif
  has_quiet_NaN     = __IsIEC559,
  has_signaling_NaN = __IsIEC559,

  has_denorm_loss   =  false,
  is_iec559      =  __IsIEC559,
  is_bounded     =  true,
  traps          =  true,
  tinyness_before=  false

#if defined (_STLP_STATIC_CONST_INIT_BUG)
  }
#endif
  ;
};

_STLP_MOVE_TO_STD_NAMESPACE

// Class numeric_limits

// The unspecialized class.

template<class _Tp>
class numeric_limits : public _STLP_PRIV _Numeric_limits_base<_Tp> {};

// Specializations for all built-in integral types.

#if !defined (_STLP_NO_BOOL)
_STLP_TEMPLATE_NULL
class numeric_limits<bool>
  : public _STLP_PRIV _Integer_limits<bool, false, true, 1, false>
{};
#endif /* _STLP_NO_BOOL */

_STLP_TEMPLATE_NULL
class numeric_limits<char>
  : public _STLP_PRIV _Integer_limits<char, CHAR_MIN, CHAR_MAX, -1, true>
{};

#if !defined (_STLP_NO_SIGNED_BUILTINS)
_STLP_TEMPLATE_NULL
class numeric_limits<signed char>
  : public _STLP_PRIV _Integer_limits<signed char, SCHAR_MIN, SCHAR_MAX, -1, true>
{};
#endif

_STLP_TEMPLATE_NULL
class numeric_limits<unsigned char>
  : public _STLP_PRIV _Integer_limits<unsigned char, 0, UCHAR_MAX, -1, true>
{};

#if !(defined (_STLP_NO_WCHAR_T) || defined (_STLP_WCHAR_T_IS_USHORT))

_STLP_TEMPLATE_NULL
class numeric_limits<wchar_t>
  : public _STLP_PRIV _Integer_limits<wchar_t, WCHAR_MIN, WCHAR_MAX, -1, true>
{};

#endif

_STLP_TEMPLATE_NULL
class numeric_limits<short>
  : public _STLP_PRIV _Integer_limits<short, SHRT_MIN, SHRT_MAX, -1, true>
{};

_STLP_TEMPLATE_NULL
class numeric_limits<unsigned short>
  : public _STLP_PRIV _Integer_limits<unsigned short, 0, USHRT_MAX, -1, true>
{};

#if defined (__xlC__) && (__xlC__ == 0x500)
#  undef INT_MIN
#  define INT_MIN -2147483648
#endif

_STLP_TEMPLATE_NULL
class numeric_limits<int>
  : public _STLP_PRIV _Integer_limits<int, INT_MIN, INT_MAX, -1, true>
{};

_STLP_TEMPLATE_NULL
class numeric_limits<unsigned int>
  : public _STLP_PRIV _Integer_limits<unsigned int, 0, UINT_MAX, -1, true>
{};

_STLP_TEMPLATE_NULL
class numeric_limits<long>
  : public _STLP_PRIV _Integer_limits<long, LONG_MIN, LONG_MAX, -1, true>
{};

_STLP_TEMPLATE_NULL
class numeric_limits<unsigned long>
  : public _STLP_PRIV _Integer_limits<unsigned long, 0, ULONG_MAX, -1, true>
{};

#if defined (_STLP_LONG_LONG)

#  if defined (_STLP_MSVC) || defined (__BORLANDC__)
#    define LONGLONG_MAX     0x7fffffffffffffffi64
#    define LONGLONG_MIN     (-LONGLONG_MAX-1i64)
#    define ULONGLONG_MAX    0xffffffffffffffffUi64
#  else
#    ifndef LONGLONG_MAX
#      define LONGLONG_MAX   0x7fffffffffffffffLL
#    endif
#    ifndef LONGLONG_MIN
#      define LONGLONG_MIN   (-LONGLONG_MAX-1LL)
#    endif
#    ifndef ULONGLONG_MAX
#      define ULONGLONG_MAX  0xffffffffffffffffULL
#    endif
#  endif

#  if !defined(__GNUC__) || (__GNUC__ == 2 && __GNUC_MINOR__ <= 96) || defined(OS_ANDROID)

_STLP_TEMPLATE_NULL
class numeric_limits<_STLP_LONG_LONG>
  : public _STLP_PRIV _Integer_limits<_STLP_LONG_LONG, LONGLONG_MIN, LONGLONG_MAX, -1, true>
{};

_STLP_TEMPLATE_NULL
class numeric_limits<unsigned _STLP_LONG_LONG>
  : public _STLP_PRIV _Integer_limits<unsigned _STLP_LONG_LONG, 0, ULONGLONG_MAX, -1, true>
{};
#  else /* gcc 2.97 (after 2000-11-01), 2.98, 3.0 */
/*
 newest gcc has new mangling scheme, that has problem
 with generating name [instantiated] of template specialization like
 _Integer_limits<_STLP_LONG_LONG, LONGLONG_MIN, LONGLONG_MAX, -1, true>
                                  ~~~~~~~~~~~~  ~~~~~~~~~~~~
 Below is code that solve this problem.
   - ptr
 */
_STLP_TEMPLATE_NULL
class numeric_limits<_STLP_LONG_LONG>
  : public _STLP_PRIV _Numeric_limits_base<_STLP_LONG_LONG> {
public:

  static _STLP_LONG_LONG (_STLP_CALL min) () _STLP_NOTHROW { return LONGLONG_MIN; }
  static _STLP_LONG_LONG (_STLP_CALL max) () _STLP_NOTHROW { return LONGLONG_MAX; }

#    if defined ( _STLP_STATIC_CONST_INIT_BUG)
  enum {
#    else
  static const int
#    endif
  digits = ((int)((sizeof(_STLP_LONG_LONG) * (CHAR_BIT))) - 1),
  digits10 = (digits * 301UL) / 1000,
  radix = 2
#    if ! defined (_STLP_STATIC_CONST_INIT_BUG)
  ;
  static const bool
#    else
  ,
#    endif
  is_specialized = true,
  is_signed = true,
  is_integer = true,
  is_exact = true,
  is_bounded = true,
  is_modulo = true
#    if defined (_STLP_STATIC_CONST_INIT_BUG)
  }
#    endif
  ;
};

_STLP_TEMPLATE_NULL
class numeric_limits<unsigned _STLP_LONG_LONG>
  : public _STLP_PRIV _Numeric_limits_base<unsigned _STLP_LONG_LONG> {
public:

  static unsigned _STLP_LONG_LONG (_STLP_CALL min) () _STLP_NOTHROW { return 0ULL; }
  static unsigned _STLP_LONG_LONG (_STLP_CALL max) () _STLP_NOTHROW { return ULONGLONG_MAX; }

#    if defined (_STLP_STATIC_CONST_INIT_BUG)
  enum {
#    else
  static const int
#    endif
  digits = ((int)((sizeof(unsigned _STLP_LONG_LONG) * (CHAR_BIT)))),
  digits10 = (digits * 301UL) / 1000,
  radix = 2
#    if ! defined (_STLP_STATIC_CONST_INIT_BUG)
  ;
  static const bool
#    else
  ,
#    endif
  is_specialized = true,
  is_signed = false,
  is_integer = true,
  is_exact = true,
  is_bounded = true,
  is_modulo = true
#    if defined ( _STLP_STATIC_CONST_INIT_BUG)
  }
#    endif
  ;
};

#  endif /* __GNUC__ > 2000-11-01 */

#endif /* _STLP_LONG_LONG */

_STLP_MOVE_TO_PRIV_NAMESPACE

// Specializations for all built-in floating-point types.
template <class __dummy>
class _LimG {
public:
  static float _STLP_CALL get_F_inf();
  static float _STLP_CALL get_F_qNaN();
  static float _STLP_CALL get_F_sNaN();
  static double _STLP_CALL get_D_inf();
  static double _STLP_CALL get_D_qNaN();
  static double _STLP_CALL get_D_sNaN();

#if !defined (_STLP_NO_LONG_DOUBLE)
  static long double _STLP_CALL get_LD_inf();
  static long double _STLP_CALL get_LD_qNaN();
  static long double _STLP_CALL get_LD_sNaN();
#endif
};

#if defined (_STLP_USE_TEMPLATE_EXPORT)
_STLP_EXPORT_TEMPLATE_CLASS _LimG<bool>;
#endif

_STLP_MOVE_TO_STD_NAMESPACE

_STLP_TEMPLATE_NULL
class numeric_limits<float>
  : public _STLP_PRIV _Floating_limits<float,
                                        FLT_MANT_DIG,   // Binary digits of precision
                                        FLT_DIG,        // Decimal digits of precision
                                        FLT_MIN_EXP,    // Minimum exponent
                                        FLT_MAX_EXP,    // Maximum exponent
                                        FLT_MIN_10_EXP, // Minimum base 10 exponent
                                        FLT_MAX_10_EXP, // Maximum base 10 exponent
#if defined (_STLP_NO_IEC559_SUPPORT)
                                        false,          // do not conform to iec559
#else
                                        true,           // conforms to iec559
#endif
                                        round_to_nearest> {
public:
  static float (_STLP_CALL min) () _STLP_NOTHROW { return FLT_MIN; }
  static float _STLP_CALL denorm_min() _STLP_NOTHROW { return FLT_MIN; }
  static float (_STLP_CALL max) () _STLP_NOTHROW { _STLP_USING_VENDOR_CSTD return FLT_MAX; }
  static float _STLP_CALL epsilon() _STLP_NOTHROW { return FLT_EPSILON; }
  static float _STLP_CALL round_error() _STLP_NOTHROW { return 0.5f; } // Units: ulps.
  static  float _STLP_CALL infinity() _STLP_NOTHROW { return _STLP_PRIV _LimG<bool>::get_F_inf(); }
  static  float _STLP_CALL quiet_NaN() _STLP_NOTHROW { return _STLP_PRIV _LimG<bool>::get_F_qNaN(); }
  static  float _STLP_CALL signaling_NaN() _STLP_NOTHROW { return _STLP_PRIV _LimG<bool>::get_F_sNaN(); }
};

_STLP_TEMPLATE_NULL
class numeric_limits<double>
  : public _STLP_PRIV _Floating_limits<double,
                                        DBL_MANT_DIG,   // Binary digits of precision
                                        DBL_DIG,        // Decimal digits of precision
                                        DBL_MIN_EXP,    // Minimum exponent
                                        DBL_MAX_EXP,    // Maximum exponent
                                        DBL_MIN_10_EXP, // Minimum base 10 exponent
                                        DBL_MAX_10_EXP, // Maximum base 10 exponent
#if defined (_STLP_NO_IEC559_SUPPORT)
                                        false,          // do not conform to iec559
#else
                                        true,           // conforms to iec559
#endif
                                        round_to_nearest> {
public:
  static double (_STLP_CALL min)() _STLP_NOTHROW { return DBL_MIN; }
  static double _STLP_CALL denorm_min() _STLP_NOTHROW { return DBL_MIN; }
  static double (_STLP_CALL max)() _STLP_NOTHROW { _STLP_USING_VENDOR_CSTD return DBL_MAX; }
  static double _STLP_CALL epsilon() _STLP_NOTHROW { return DBL_EPSILON; }
  static double _STLP_CALL round_error() _STLP_NOTHROW { return 0.5; } // Units: ulps.
  static  double _STLP_CALL infinity() _STLP_NOTHROW { return _STLP_PRIV _LimG<bool>::get_D_inf(); }
  static  double _STLP_CALL quiet_NaN() _STLP_NOTHROW { return _STLP_PRIV _LimG<bool>::get_D_qNaN(); }
  static  double _STLP_CALL signaling_NaN() _STLP_NOTHROW { return _STLP_PRIV _LimG<bool>::get_D_sNaN(); }
};

#if !defined (_STLP_NO_LONG_DOUBLE)

_STLP_TEMPLATE_NULL
class numeric_limits<long double>
  : public _STLP_PRIV _Floating_limits<long double,
                                        LDBL_MANT_DIG,  // Binary digits of precision
                                        LDBL_DIG,       // Decimal digits of precision
                                        LDBL_MIN_EXP,   // Minimum exponent
                                        LDBL_MAX_EXP,   // Maximum exponent
                                        LDBL_MIN_10_EXP,// Minimum base 10 exponent
                                        LDBL_MAX_10_EXP,// Maximum base 10 exponent
                                        false,          // do not conform to iec559
                                        round_to_nearest> {
public:
  static long double (_STLP_CALL min) () _STLP_NOTHROW { _STLP_USING_VENDOR_CSTD return LDBL_MIN; }
  static long double _STLP_CALL denorm_min() _STLP_NOTHROW { _STLP_USING_VENDOR_CSTD return LDBL_MIN; }
  static long double (_STLP_CALL max) () _STLP_NOTHROW { _STLP_USING_VENDOR_CSTD return LDBL_MAX; }
  static long double _STLP_CALL epsilon() _STLP_NOTHROW { return LDBL_EPSILON; }
  static long double _STLP_CALL round_error() _STLP_NOTHROW { return 4; } // Units: ulps.
  static long double _STLP_CALL infinity() _STLP_NOTHROW { return _STLP_PRIV _LimG<bool>::get_LD_inf(); }
  static long double _STLP_CALL quiet_NaN() _STLP_NOTHROW { return _STLP_PRIV _LimG<bool>::get_LD_qNaN(); }
  static long double _STLP_CALL signaling_NaN() _STLP_NOTHROW { return _STLP_PRIV _LimG<bool>::get_LD_sNaN(); }
};

#endif

// We write special values (Inf and NaN) as bit patterns and
// cast the the appropriate floating-point types.
_STLP_END_NAMESPACE

#if !defined (_STLP_LINK_TIME_INSTANTIATION)
#  include <stl/_limits.c>
#endif

#endif

// Local Variables:
// mode:C++
// End:
