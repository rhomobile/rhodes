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


#ifndef __wcecompat__MATH_H__
#define __wcecompat__MATH_H__

#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

int       __cdecl abs(int);
long      __cdecl labs(long);
double    __cdecl acos(double);
double    __cdecl asin(double);
double    __cdecl atan(double);
double    __cdecl atan2(double, double);
double    __cdecl cos(double);
double    __cdecl cosh(double);
double    __cdecl exp(double);
double    __cdecl fabs(double);
double    __cdecl fmod(double, double);
double    __cdecl log(double);
double    __cdecl log10(double);
double    __cdecl pow(double, double);
double    __cdecl sin(double);
double    __cdecl sinh(double);
double    __cdecl sqrt(double);
double    __cdecl tan(double);
double    __cdecl tanh(double);
double    __cdecl _cabs(struct _complex);
double    __cdecl ceil(double);
double    __cdecl floor(double);
double    __cdecl frexp(double, int *);
double    __cdecl _hypot(double, double);
double    __cdecl _j0(double);
double    __cdecl _j1(double);
double    __cdecl _jn(int, double);
double    __cdecl ldexp(double, int);
_CRTIMP int       __cdecl _matherr(struct _exception *);
double    __cdecl modf(double, double *);
double    __cdecl _y0(double);
double    __cdecl _y1(double);
double    __cdecl _yn(int, double);

// macros -- oldnames
#define acosl(x)    ((long double)acos((double)(x)))
#define asinl(x)    ((long double)asin((double)(x)))
#define atanl(x)    ((long double)atan((double)(x)))
#define atan2l(x,y) ((long double)atan2((double)(x), (double)(y)))
#define ceill(x)    ((long double)ceil((double)(x)))
#define cosl(x)     ((long double)cos((double)(x)))
#define coshl(x)    ((long double)cosh((double)(x)))
#define expl(x)     ((long double)exp((double)(x)))
#define fabsl(x)    ((long double)fabs((double)(x)))
#define floorl(x)   ((long double)floor((double)(x)))
#define fmodl(x,y)  ((long double)fmod((double)(x), (double)(y)))
#define frexpl(x,y) ((long double)frexp((double)(x), (y)))
#define hypotl(x,y) ((long double)hypot((double)(x), (double)(y)))
#define ldexpl(x,y) ((long double)ldexp((double)(x), (y)))
#define logl(x)     ((long double)log((double)(x)))
#define log10l(x)   ((long double)log10((double)(x)))
#define _matherrl   _matherr
#define modfl(x,y)  ((long double)modf((double)(x), (double *)(y)))
#define powl(x,y)   ((long double)pow((double)(x), (double)(y)))
#define sinl(x)     ((long double)sin((double)(x)))
#define sinhl(x)    ((long double)sinh((double)(x)))
#define sqrtl(x)    ((long double)sqrt((double)(x)))
#define tanl(x)     ((long double)tan((double)(x)))
#define tanhl(x)    ((long double)tanh((double)(x)))

#ifdef __cplusplus
}
#endif

#endif /* __wcecompat__MATH_H__ */
