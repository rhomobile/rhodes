/*  wcecompat: Windows CE C Runtime Library "compatibility" library.
 *
 *  Copyright (C) 2001-2002 Essemer Pty Ltd.  All rights reserved.
 *  http://www.essemer.com.au/
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef __wcecompat__FLOAT_H__
#define __wcecompat__FLOAT_H__

#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DBL_DIG        15            /* # of decimal digits of precision */
#define DBL_EPSILON    2.2204460492503131e-016 /* smallest such that 1.0+DBL_EPSILON != 1.0 */
#define DBL_MANT_DIG   53            /* # of bits in mantissa */
#define DBL_MAX        1.7976931348623158e+308 /* max value */
#define DBL_MAX_10_EXP 308            /* max decimal exponent */
#define DBL_MAX_EXP    1024            /* max binary exponent */
#define DBL_MIN        2.2250738585072014e-308 /* min positive value */
#define DBL_MIN_10_EXP (-307)            /* min decimal exponent */
#define DBL_MIN_EXP    (-1021)         /* min binary exponent */
#define _DBL_RADIX     2            /* exponent radix */
#define _DBL_ROUNDS    1            /* addition rounding: near */

#define FLT_DIG        6            /* # of decimal digits of precision */
#define FLT_EPSILON    1.192092896e-07F    /* smallest such that 1.0+FLT_EPSILON != 1.0 */
#define FLT_GUARD      0
#define FLT_MANT_DIG   24            /* # of bits in mantissa */
#define FLT_MAX        3.402823466e+38F    /* max value */
#define FLT_MAX_10_EXP 38            /* max decimal exponent */
#define FLT_MAX_EXP    128            /* max binary exponent */
#define FLT_MIN        1.175494351e-38F    /* min positive value */
#define FLT_MIN_10_EXP (-37)            /* min decimal exponent */
#define FLT_MIN_EXP    (-125)            /* min binary exponent */
#define FLT_NORMALIZE  0
#define FLT_RADIX      2            /* exponent radix */
#define FLT_ROUNDS     1            /* addition rounding: near */

#define LDBL_DIG        DBL_DIG         /* # of decimal digits of precision */
#define LDBL_EPSILON    DBL_EPSILON        /* smallest such that 1.0+LDBL_EPSILON != 1.0 */
#define LDBL_MANT_DIG   DBL_MANT_DIG        /* # of bits in mantissa */
#define LDBL_MAX        DBL_MAX         /* max value */
#define LDBL_MAX_10_EXP DBL_MAX_10_EXP        /* max decimal exponent */
#define LDBL_MAX_EXP    DBL_MAX_EXP        /* max binary exponent */
#define LDBL_MIN        DBL_MIN         /* min positive value */
#define LDBL_MIN_10_EXP DBL_MIN_10_EXP        /* min decimal exponent */
#define LDBL_MIN_EXP    DBL_MIN_EXP        /* min binary exponent */
#define _LDBL_RADIX     DBL_RADIX        /* exponent radix */
#define _LDBL_ROUNDS    DBL_ROUNDS        /* addition rounding: near */

// Abstract User Control Word Mask and bit definitions
#define _MCW_EM         0x0008001f        /* interrupt Exception Masks */
#define _EM_INEXACT     0x00000001        /*   inexact (precision) */
#define _EM_UNDERFLOW   0x00000002        /*   underflow */
#define _EM_OVERFLOW    0x00000004        /*   overflow */
#define _EM_ZERODIVIDE  0x00000008        /*   zero divide */
#define _EM_INVALID     0x00000010        /*   invalid */
#define _MCW_RC         0x00000300        /* Rounding Control */
#define _RC_NEAR        0x00000000        /*   near */
#define _RC_DOWN        0x00000100        /*   down */
#define _RC_UP          0x00000200        /*   up */
#define _RC_CHOP        0x00000300        /*   chop */

// Abstract User Status Word bit definitions
#define _SW_INEXACT     0x00000001        /* inexact (precision) */
#define _SW_UNDERFLOW   0x00000002        /* underflow */
#define _SW_OVERFLOW    0x00000004        /* overflow */
#define _SW_ZERODIVIDE  0x00000008        /* zero divide */
#define _SW_INVALID     0x00000010        /* invalid */

#if ! defined (_M_CEE)

// i386 specific definitions
#define _MCW_PC       0x00030000        /* Precision Control */
#define _PC_64        0x00000000        /*    64 bits */
#define _PC_53        0x00010000        /*    53 bits */
#define _PC_24        0x00020000        /*    24 bits */
#define _MCW_IC       0x00040000        /* Infinity Control */
#define _IC_AFFINE    0x00040000        /*   affine */
#define _IC_PROJECTIVE  0x00000000        /*   projective */
#define _EM_DENORMAL    0x00080000        /* denormal exception mask (_control87 only) */
#define _SW_DENORMAL    0x00080000        /* denormal status bit */

// MIPS R4000 and SH4 specific definitions
#ifdef  _M_SH
#define _MCW_DN     0x00040000      /* Denormal Control (SH4)   */
#define _DN_FLUSH   0x00040000      /*   flush to zero (SH4)    */
#else
#define _MCW_DN     0x01000000      /* Denormal Control (R4000) */
#define _DN_FLUSH   0x01000000      /*   flush to zero (R4000)  */
#endif
#define _DN_SAVE    0x00000000        /*   save */


// initial Control Word value
#ifdef    _M_IX86
#define _CW_DEFAULT ( _RC_NEAR + _PC_64 + _EM_INVALID + _EM_ZERODIVIDE + _EM_OVERFLOW + _EM_UNDERFLOW + _EM_INEXACT )
#else
#define _CW_DEFAULT ( _RC_NEAR + _PC_53 + _EM_INVALID + _EM_ZERODIVIDE + _EM_OVERFLOW + _EM_UNDERFLOW + _EM_INEXACT )
#endif
#endif	// ! _M_CEE

// invalid subconditions (_SW_INVALID also set)
#define _SW_UNEMULATED      0x0040    /* unemulated instruction */
#define _SW_SQRTNEG         0x0080    /* square root of a neg number */
#define _SW_STACKOVERFLOW   0x0200    /* FP stack overflow */
#define _SW_STACKUNDERFLOW  0x0400    /* FP stack underflow */

//  Floating point error signals and return codes
#define _FPE_INVALID        0x81
#define _FPE_DENORMAL       0x82
#define _FPE_ZERODIVIDE     0x83
#define _FPE_OVERFLOW       0x84
#define _FPE_UNDERFLOW      0x85
#define _FPE_INEXACT        0x86
#define _FPE_UNEMULATED     0x87
#define _FPE_SQRTNEG        0x88
#define _FPE_STACKOVERFLOW  0x8a
#define _FPE_STACKUNDERFLOW 0x8b
#define _FPE_EXPLICITGEN    0x8c    /* raise( SIGFPE ); */

// Floating point function prototypes
unsigned int __cdecl _clearfp(void);
unsigned int __cdecl _controlfp(unsigned int,unsigned int);
unsigned int __cdecl _statusfp(void);
void         __cdecl _fpreset(void);
#define _clear87    _clearfp
#define _status87   _statusfp

// IEEE recommended functions
double __cdecl _copysign (double, double);
double __cdecl _chgsign (double);
double __cdecl _scalb(double, long);
double __cdecl _logb(double);
double __cdecl _nextafter(double, double);
int    __cdecl _finite(double);
int    __cdecl _isnan(double);
int    __cdecl _fpclass(double);

#define _FPCLASS_SNAN    0x0001    /* signaling NaN */
#define _FPCLASS_QNAN    0x0002    /* quiet NaN */
#define _FPCLASS_NINF    0x0004    /* negative infinity */
#define _FPCLASS_NN    0x0008    /* negative normal */
#define _FPCLASS_ND    0x0010    /* negative denormal */
#define _FPCLASS_NZ    0x0020    /* -0 */
#define _FPCLASS_PZ    0x0040    /* +0 */
#define _FPCLASS_PD    0x0080    /* positive denormal */
#define _FPCLASS_PN    0x0100    /* positive normal */
#define _FPCLASS_PINF    0x0200    /* positive infinity */

typedef float _FP32;
typedef double _FP64;
typedef short _I16;
typedef int _I32;
typedef unsigned short _U16;
typedef unsigned int _U32;

typedef struct {
    unsigned long W[4];
} _U32ARRAY;

typedef struct {
    unsigned short W[5];
} _FP80;

typedef struct {
    unsigned long W[4];
} _FP128;

typedef struct {
    unsigned long W[2];
} _I64;

typedef struct {
    unsigned long W[2];
} _U64;

typedef struct {
    unsigned short W[5];
} _BCD80;

typedef struct {
    union {
        _FP32        Fp32Value;
        _FP64        Fp64Value;
        _FP80        Fp80Value;
        _FP128       Fp128Value;
        _I16         I16Value;
        _I32         I32Value;
        _I64         I64Value;
        _U16         U16Value;
        _U32         U32Value;
        _U64         U64Value;
        _BCD80       Bcd80Value;
        char         *StringValue;
        int      CompareValue;
        _U32ARRAY    U32ArrayValue;
    } Value;
    unsigned int OperandValid : 1;
    unsigned int Format : 4;
} _FPIEEE_VALUE;

typedef struct {
    unsigned int Inexact : 1;
    unsigned int Underflow : 1;
    unsigned int Overflow : 1;
    unsigned int ZeroDivide : 1;
    unsigned int InvalidOperation : 1;
} _FPIEEE_EXCEPTION_FLAGS;

typedef struct {
    unsigned int RoundingMode : 2;
    unsigned int Precision : 3;
    unsigned int Operation :12;
    _FPIEEE_EXCEPTION_FLAGS Cause;
    _FPIEEE_EXCEPTION_FLAGS Enable;
    _FPIEEE_EXCEPTION_FLAGS Status;
    _FPIEEE_VALUE Operand1;
    _FPIEEE_VALUE Operand2;
    _FPIEEE_VALUE Result;
} _FPIEEE_RECORD;

typedef struct _EXCEPTION_POINTERS *PEXCEPTION_POINTERS;

#ifdef _WIN32_WCE_EMULATION
int __cdecl _fpieee_flt(unsigned long exc_code, PEXCEPTION_POINTERS p, int handler(_FPIEEE_RECORD *));
#else
int _fpieee_flt(unsigned long exc_code, PEXCEPTION_POINTERS p, int handler(_FPIEEE_RECORD *));
#endif

// Non-ANSI names for compatibility
#define DBL_RADIX        _DBL_RADIX
#define DBL_ROUNDS       _DBL_ROUNDS
#define LDBL_RADIX       _LDBL_RADIX
#define LDBL_ROUNDS      _LDBL_ROUNDS
#define MCW_EM           _MCW_EM
#define EM_INVALID       _EM_INVALID
#define EM_ZERODIVIDE    _EM_ZERODIVIDE
#define EM_OVERFLOW      _EM_OVERFLOW
#define EM_UNDERFLOW     _EM_UNDERFLOW
#define EM_INEXACT       _EM_INEXACT

#if ! defined (_M_CEE)

#define EM_DENORMAL      _EM_DENORMAL
#define SW_DENORMAL      _SW_DENORMAL
#define MCW_IC           _MCW_IC
#define IC_AFFINE        _IC_AFFINE
#define IC_PROJECTIVE    _IC_PROJECTIVE
#define MCW_PC           _MCW_PC
#define PC_24            _PC_24
#define PC_53            _PC_53
#define PC_64            _PC_64

#endif // ! _M_CEE

#define MCW_RC           _MCW_RC
#define RC_CHOP          _RC_CHOP
#define RC_UP            _RC_UP
#define RC_DOWN          _RC_DOWN
#define RC_NEAR          _RC_NEAR
#define CW_DEFAULT       _CW_DEFAULT
#define SW_INVALID       _SW_INVALID
#define SW_ZERODIVIDE    _SW_ZERODIVIDE
#define SW_OVERFLOW      _SW_OVERFLOW
#define SW_UNDERFLOW     _SW_UNDERFLOW
#define SW_INEXACT       _SW_INEXACT
#define SW_UNEMULATED    _SW_UNEMULATED
#define SW_SQRTNEG       _SW_SQRTNEG
#define SW_STACKOVERFLOW   _SW_STACKOVERFLOW
#define SW_STACKUNDERFLOW  _SW_STACKUNDERFLOW
#define FPE_INVALID        _FPE_INVALID
#define FPE_DENORMAL     _FPE_DENORMAL
#define FPE_ZERODIVIDE   _FPE_ZERODIVIDE
#define FPE_OVERFLOW     _FPE_OVERFLOW
#define FPE_UNDERFLOW    _FPE_UNDERFLOW
#define FPE_INEXACT      _FPE_INEXACT
#define FPE_UNEMULATED   _FPE_UNEMULATED
#define FPE_SQRTNEG        _FPE_SQRTNEG
#define FPE_STACKOVERFLOW  _FPE_STACKOVERFLOW
#define FPE_STACKUNDERFLOW _FPE_STACKUNDERFLOW
#define FPE_EXPLICITGEN    _FPE_EXPLICITGEN

#ifdef _WIN32_WCE_EMULATION
double __cdecl _frnd(double);
double __cdecl _fsqrt(double);
#else
double _frnd(double);
double _fsqrt(double);
#endif

// definition of a _complex struct to be used by those who use cabs and
// want type checking on their argument
struct _complex {
    double x,y;    /* real and imaginary parts */
    } ;

// Non-ANSI name for compatibility
#define complex _complex

// definitions of _HUGE and HUGE_VAL - respectively the XENIX and ANSI names
// for a value returned in case of error by a number of the floating point
// math routines
_CRTIMP
extern double _HUGE;
#define HUGE_VAL _HUGE

#ifdef __cplusplus
}
#endif

#endif /* __wcecompat__FLOAT_H__ */
