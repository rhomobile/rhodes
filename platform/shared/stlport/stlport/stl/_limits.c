/*
 * Copyright (c) 1998,1999
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

#ifndef _STLP_LIMITS_C
#define _STLP_LIMITS_C

#ifndef _STLP_INTERNAL_LIMITS
#  include <stl/_limits.h>
#endif

//==========================================================
//  numeric_limits static members
//==========================================================

_STLP_BEGIN_NAMESPACE

_STLP_MOVE_TO_PRIV_NAMESPACE

#if !defined (_STLP_STATIC_CONST_INIT_BUG)

#  define __declare_numeric_base_member(__type, __mem, _Init) \
template <class __number> \
  const __type _Numeric_limits_base<__number>:: __mem

__declare_numeric_base_member(bool, is_specialized, false);
__declare_numeric_base_member(int, digits, 0);
__declare_numeric_base_member(int, digits10, 0);
__declare_numeric_base_member(bool, is_signed, false);
__declare_numeric_base_member(bool, is_integer, false);
__declare_numeric_base_member(bool, is_exact, false);
__declare_numeric_base_member(int, radix, 0);
__declare_numeric_base_member(int, min_exponent, 0);
__declare_numeric_base_member(int, max_exponent, 0);
__declare_numeric_base_member(int, min_exponent10, 0);
__declare_numeric_base_member(int, max_exponent10, 0);
__declare_numeric_base_member(bool, has_infinity, false);
__declare_numeric_base_member(bool, has_quiet_NaN, false);
__declare_numeric_base_member(bool, has_signaling_NaN, false);
__declare_numeric_base_member(float_denorm_style, has_denorm, denorm_absent);
__declare_numeric_base_member(bool, has_denorm_loss, false);
__declare_numeric_base_member(bool, is_iec559, false);
__declare_numeric_base_member(bool, is_bounded, false);
__declare_numeric_base_member(bool, is_modulo, false);
__declare_numeric_base_member(bool, traps, false);
__declare_numeric_base_member(bool, tinyness_before, false);
__declare_numeric_base_member(float_round_style, round_style, round_toward_zero);

#  undef __declare_numeric_base_member

#  define __declare_integer_limits_member(__type, __mem, _Init) \
template <class _Int, _STLP_LIMITS_MIN_TYPE __imin, _STLP_LIMITS_MAX_TYPE __imax, int __idigits, bool __ismod> \
  const __type _Integer_limits<_Int, __imin, __imax, __idigits, __ismod>:: __mem

__declare_integer_limits_member(bool, is_specialized, true);
__declare_integer_limits_member(int, digits, (__idigits < 0) ? \
          ((int)((sizeof(_Int) * (CHAR_BIT))) - ((__imin == 0) ? 0 : 1)) \
                            : (__idigits) );
__declare_integer_limits_member(int, digits10, (int)(301UL * digits) /1000);
__declare_integer_limits_member(bool, is_signed, __imin != 0);
__declare_integer_limits_member(bool, is_integer, true);
__declare_integer_limits_member(bool, is_exact, true);
__declare_integer_limits_member(int, radix, 2);
__declare_integer_limits_member(bool, is_bounded, true);
__declare_integer_limits_member(bool, is_modulo, true);
#  undef __declare_integer_limits_member

#  define __declare_float_limits_member(__type, __mem, _Init) \
template <class __number,  \
         int __Digits, int __Digits10,    \
         int __MinExp, int __MaxExp,      \
         int __MinExp10, int __MaxExp10,  \
         bool __IsIEC559, \
         float_round_style __RoundStyle> \
const __type _Floating_limits< __number, __Digits, __Digits10,    \
         __MinExp, __MaxExp, __MinExp10, __MaxExp10,  \
         __IsIEC559, __RoundStyle>::\
         __mem

__declare_float_limits_member(bool, is_specialized, true);
__declare_float_limits_member(int, digits, __Digits);
__declare_float_limits_member(int, digits10, __Digits10);
__declare_float_limits_member(bool, is_signed, true);
__declare_float_limits_member(int, radix, FLT_RADIX);
__declare_float_limits_member(int, min_exponent, __MinExp);
__declare_float_limits_member(int, max_exponent, __MaxExp);
__declare_float_limits_member(int, min_exponent10, __MinExp10);
__declare_float_limits_member(int, max_exponent10, __MaxExp10);
__declare_float_limits_member(bool, has_infinity, true);
__declare_float_limits_member(bool, has_quiet_NaN, true);
__declare_float_limits_member(bool, has_signaling_NaN, true);
__declare_float_limits_member(float_denorm_style, has_denorm, denorm_indeterminate);
__declare_float_limits_member(bool, has_denorm_loss, false);
__declare_float_limits_member(bool, is_iec559, __IsIEC559);
__declare_float_limits_member(bool, is_bounded, true);
__declare_float_limits_member(bool, traps, true);
__declare_float_limits_member(bool, tinyness_before, false);
__declare_float_limits_member(float_round_style, round_style, __RoundStyle);
#  undef __declare_float_limits_member

#endif /* _STLP_STATIC_CONST_INIT_BUG */


#if defined (_STLP_EXPOSE_GLOBALS_IMPLEMENTATION)

#  if defined (_STLP_BIG_ENDIAN)
#    if defined (__OS400__)
#      define _STLP_FLOAT_INF_REP { 0x7f80, 0 }
#      define _STLP_FLOAT_QNAN_REP { 0xffc0, 0 }
#      define _STLP_FLOAT_SNAN_REP { 0xff80, 0 }
#      define _STLP_DOUBLE_INF_REP { 0x7ff0, 0, 0, 0 }
#      define _STLP_DOUBLE_QNAN_REP { 0xfff8, 0, 0, 0 }
#      define _STLP_DOUBLE_SNAN_REP { 0xfff0, 0, 0, 0 }
#      define _STLP_LDOUBLE_INF_REP { 0x7ff0, 0, 0, 0, 0, 0, 0, 0 }
#      define _STLP_LDOUBLE_QNAN_REP { 0xfff8, 0, 0, 0, 0, 0, 0, 0 }
#      define _STLP_LDOUBLE_SNAN_REP { 0xfff0, 0, 0, 0, 0, 0, 0, 0 }
#    else /* __OS400__ */
#      define _STLP_FLOAT_INF_REP   { 0x7f80, 0 }
#      define _STLP_FLOAT_QNAN_REP  { 0x7fc1, 0 }
#      define _STLP_FLOAT_SNAN_REP  { 0x7f81, 0 }
#      define _STLP_DOUBLE_INF_REP  { 0x7ff0, 0, 0, 0 }
#      define _STLP_DOUBLE_QNAN_REP { 0x7ff9, 0, 0, 0 }
#      define _STLP_DOUBLE_SNAN_REP { 0x7ff1, 0, 0, 0 }
#      define _STLP_LDOUBLE_INF_REP { 0x7ff0, 0, 0, 0, 0, 0, 0, 0 }
#      define _STLP_LDOUBLE_QNAN_REP { 0x7ff1, 0, 0, 0, 0, 0, 0, 0 }
#      define _STLP_LDOUBLE_SNAN_REP { 0x7ff9, 0, 0, 0, 0, 0, 0, 0 }
#    endif /* __OS400__ */

#  elif defined (_STLP_LITTLE_ENDIAN)

#    if 0 /* defined(_STLP_MSVC) || defined(__linux__) */
// some IA-32 platform ??
/*
#      define _STLP_FLOAT_INF_REP { 0, 0x7f80 }
#      define _STLP_FLOAT_QNAN_REP { 0, 0xffc0 }
#      define _STLP_FLOAT_SNAN_REP { 0, 0xff80 }

#      define _STLP_DOUBLE_INF_REP { 0, 0, 0, 0x7ff0 }
#      define _STLP_DOUBLE_QNAN_REP { 0, 0, 0, 0xfff8 }
#      define _STLP_DOUBLE_SNAN_REP { 0, 0, 0, 0xfff0 }
#      define _STLP_LDOUBLE_INF_REP { 0, 0, 0, 0x7FF0, 0 } // ????
#      define _STLP_LDOUBLE_QNAN_REP { 0, 0, 0, 0xFFF8, 0 } // ????
#      define _STLP_LDOUBLE_SNAN_REP { 0, 0, 0, 0xFFF0, 0 } // ????
*/
#    elif defined(__DECCXX)

#      define _STLP_FLOAT_INF_REP { 0, 0x7f80 }
#      define _STLP_FLOAT_QNAN_REP { 0, 0xffc0 }
#      define _STLP_FLOAT_SNAN_REP { 0x5555, 0x7f85 }

#      define _STLP_DOUBLE_INF_REP { 0, 0, 0, 0x7ff0 }
#      define _STLP_DOUBLE_QNAN_REP { 0, 0, 0, 0xfff8 }
#      define _STLP_DOUBLE_SNAN_REP { 0x5555, 0x5555, 0x5555, 0x7ff5 }

#      define _STLP_LDOUBLE_INF_REP { 0, 0, 0, 0, 0, 0, 0, 0x7fff }
#      define _STLP_LDOUBLE_QNAN_REP { 0, 0, 0, 0, 0, 0, 0x8000, 0xffff }
#      define _STLP_LDOUBLE_SNAN_REP { 0x5555, 0x5555, 0x5555, 0x5555, 0x5555, 0x5555, 0x5555, 0x7fff}
#    else
#      define _STLP_FLOAT_INF_REP { 0, 0x7f80 }
#      define _STLP_FLOAT_QNAN_REP { 0, 0x7fc0 }
#      define _STLP_FLOAT_SNAN_REP { 0, 0x7fa0 }
#      define _STLP_DOUBLE_INF_REP { 0, 0, 0, 0x7ff0 }
#      define _STLP_DOUBLE_QNAN_REP { 0, 0, 0, 0x7ff8 }
#      define _STLP_DOUBLE_SNAN_REP { 0, 0, 0, 0x7ff4 }
#      if defined (_STLP_MSVC) || defined (__ICL)
#        define _STLP_LDOUBLE_INF_REP { 0, 0, 0, 0x7FF0, 0 }
#        define _STLP_LDOUBLE_QNAN_REP { 0, 0, 0, 0xFFF8, 0 }
#        define _STLP_LDOUBLE_SNAN_REP { 0, 0, 0, 0xFFF8, 0 }
#      elif defined (__BORLANDC__)
#        define _STLP_LDOUBLE_INF_REP { 0, 0, 0, 0x8000, 0x7fff }
#        define _STLP_LDOUBLE_QNAN_REP { 0, 0, 0, 0xc000, 0x7fff }
#        define _STLP_LDOUBLE_SNAN_REP { 0, 0, 0, 0xa000, 0x7fff }
#      else
#        define _STLP_LDOUBLE_INF_REP { 0, 0, 0, 0x8000, 0x7fff, 0 }
#        define _STLP_LDOUBLE_QNAN_REP { 0, 0, 0, 0xa000, 0x7fff, 0 }
#        define _STLP_LDOUBLE_SNAN_REP { 0, 0, 0, 0xc000, 0x7fff, 0 }
#      endif
#    endif
#  else
/* This is an architecture we don't know how to handle. Return some
obviously wrong values. */
#    define _STLP_FLOAT_INF_REP { 0, 0 }
#    define _STLP_FLOAT_QNAN_REP { 0, 0 }
#    define _STLP_FLOAT_SNAN_REP { 0, 0 }
#    define _STLP_DOUBLE_INF_REP { 0, 0 }
#    define _STLP_DOUBLE_QNAN_REP { 0, 0 }
#    define _STLP_DOUBLE_SNAN_REP { 0, 0 }
#    define _STLP_LDOUBLE_INF_REP { 0 }
#    define _STLP_LDOUBLE_QNAN_REP { 0 }
#    define _STLP_LDOUBLE_SNAN_REP { 0 }

#  endif

#  if 0
/*
#    if defined(_STLP_BIG_ENDIAN)

#    elif defined (_STLP_LITTLE_ENDIAN)
#    else

//This is an architecture we don't know how to handle.  Return some
//obviously wrong values.
#      define _STLP_FLOAT_INF_REP  { 0, 0 }
#      define _STLP_FLOAT_QNAN_REP { 0, 0 }
#      define _STLP_FLOAT_SNAN_REP { 0, 0 }
#      define _STLP_DOUBLE_INF_REP  { 0, 0 }
#      define _STLP_DOUBLE_QNAN_REP { 0, 0 }
#      define _STLP_DOUBLE_SNAN_REP { 0, 0 }
#      define _STLP_LDOUBLE_INF_REP  { 0 }
#      define _STLP_LDOUBLE_QNAN_REP { 0 }
#      define _STLP_LDOUBLE_SNAN_REP { 0 }
#    endif
*/
#  endif

union _F_rep {
  unsigned short rep[2];
  float val;
};
union _D_rep {
  unsigned short rep[4];
  double val;
};

#  ifndef _STLP_NO_LONG_DOUBLE
union _LD_rep {
  unsigned short rep[8];
  long double val;
};
#  endif

template <class __dummy>
float _STLP_CALL _LimG<__dummy>::get_F_inf() {
  _F_rep _F_inf = {_STLP_FLOAT_INF_REP};
  return _F_inf.val;
}
template <class __dummy>
float _STLP_CALL _LimG<__dummy>::get_F_qNaN() {
  _F_rep _F_qNaN = {_STLP_FLOAT_QNAN_REP};
  return _F_qNaN.val;
}
template <class __dummy>
float _STLP_CALL _LimG<__dummy>::get_F_sNaN() {
  _F_rep _F_sNaN = {_STLP_FLOAT_SNAN_REP};
  return _F_sNaN.val;
}

template <class __dummy>
double _STLP_CALL _LimG<__dummy>::get_D_inf() {
  _D_rep _D_inf = {_STLP_DOUBLE_INF_REP};
  return _D_inf.val;
}
template <class __dummy>
double _STLP_CALL _LimG<__dummy>::get_D_qNaN() {
  _D_rep _D_qNaN = {_STLP_DOUBLE_QNAN_REP};
  return _D_qNaN.val;
}
template <class __dummy>
double _STLP_CALL _LimG<__dummy>::get_D_sNaN() {
  _D_rep _D_sNaN = {_STLP_DOUBLE_SNAN_REP};
  return _D_sNaN.val;
}

#  if !defined (_STLP_NO_LONG_DOUBLE)
template <class __dummy>
long double _STLP_CALL _LimG<__dummy>::get_LD_inf() {
  _LD_rep _LD_inf = {_STLP_LDOUBLE_INF_REP};
  return _LD_inf.val;
}
template <class __dummy>
long double _STLP_CALL _LimG<__dummy>::get_LD_qNaN() {
  _LD_rep _LD_qNaN = {_STLP_LDOUBLE_QNAN_REP};
  return _LD_qNaN.val;
}
template <class __dummy>
long double _STLP_CALL _LimG<__dummy>::get_LD_sNaN() {
  _LD_rep _LD_sNaN = {_STLP_LDOUBLE_SNAN_REP};
  return _LD_sNaN.val;
}
#  endif /* _STLP_NO_LONG_DOUBLE */

#endif /* _STLP_EXPOSE_GLOBALS_IMPLEMENTATION */

#undef _STLP_LIMITS_MIN_TYPE
#undef _STLP_LIMITS_MAX_TYPE

#undef _STLP_FLOAT_INF_REP
#undef _STLP_FLOAT_QNAN_REP
#undef _STLP_FLOAT_SNAN_REP
#undef _STLP_DOUBLE_INF_REP
#undef _STLP_DOUBLE_QNAN_REP
#undef _STLP_DOUBLE_SNAN_REP
#undef _STLP_LDOUBLE_INF_REP
#undef _STLP_LDOUBLE_QNAN_REP
#undef _STLP_LDOUBLE_SNAN_REP

_STLP_MOVE_TO_STD_NAMESPACE

_STLP_END_NAMESPACE

#endif /* _STLP_LIMITS_C_INCLUDED */
