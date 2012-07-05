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


#ifndef __wcecompat__MALLOC_H__
#define __wcecompat__MALLOC_H__

#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

_CRTIMP void   __cdecl free(void *);
_CRTIMP void * __cdecl malloc(size_t);
_CRTIMP void * __cdecl calloc(size_t num, size_t size);
size_t __cdecl _msize(void *);
_CRTIMP void * __cdecl realloc(void *, size_t);

// implemented by compiler
void * __cdecl _alloca(size_t);

// process exit functions
void __cdecl exit (int code);
void __cdecl _exit (int code);
void __cdecl _cexit(void);
void __cdecl _c_exit (void);

#if !__STDC__
/* Non-ANSI names for compatibility */
#define alloca  _alloca
#endif  /* __STDC__*/

#if defined(_M_MRX000) || defined(_M_PPC) || defined(_M_ALPHA)
#pragma intrinsic(_alloca)
#endif

#ifdef __cplusplus
}
#endif

#endif /* __wcecompat__MALLOC_H__ */
