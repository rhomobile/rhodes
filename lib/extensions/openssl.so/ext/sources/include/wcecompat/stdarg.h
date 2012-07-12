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


#ifndef __wcecompat__STDARG_H__
#define __wcecompat__STDARG_H__

#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

#if defined (_M_CEE)
	typedef struct { void* p[4]; } va_list;
#else
	typedef char *va_list;
#endif

#ifdef MIPS

#define va_start(ap,v) ap  = (va_list)&v + sizeof(v)
#define va_end(list) ( list = (va_list)0 )
#define va_arg(list, mode) ((mode *)(list =\
 (char *) ((((int)list + (__builtin_alignof(mode)<=4?3:7)) &\
 (__builtin_alignof(mode)<=4?-4:-8))+sizeof(mode))))[-1]

#elif defined(PPC)

// PPC uses 8-byte alignment of 8-byte quantities
#define _ALIGNIT(ap,t)  ((((int)(ap))+(sizeof(t)<8?3:7)) & (sizeof(t)<8?~3:~7))

#define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )
#define va_start(ap,v)  ( ap = (va_list)&v + _INTSIZEOF(v) )
#define va_arg(ap,t)    ( *(t *)((ap = (char *) (_ALIGNIT(ap, t) + _INTSIZEOF(t))) - _INTSIZEOF(t)) )
#define va_end(ap)      ( ap = (va_list)0 )

#elif defined (_M_CEE)

extern void __cdecl   __va_start(va_list*, ...);
extern void* __cdecl  __va_arg(va_list*, ...);
extern void __cdecl   __va_end(va_list*);

#define va_start(ap,v)  (__va_start(&ap, &v, sizeof(v), __builtin_alignof(v), &v))
#define va_arg(ap,t)    (*((t*)__va_arg(&ap, sizeof(t), __builtin_alignof(t), (t*)0)))
#define va_end(ap)      (__va_end(&ap))

#else

#define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )
#define va_start(ap,v)  ( ap = (va_list)&v + _INTSIZEOF(v) )
#define va_arg(ap,t)    ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#define va_end(ap)      ( ap = (va_list)0 )

#endif

#ifdef __cplusplus
}
#endif

#endif /* __wcecompat__STDARG_H__ */
