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


#ifndef __wcecompat__ASSERT_H__
#define __wcecompat__ASSERT_H__

#include <ceconfig.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifdef NDEBUG
#define assert(exp) ((void)0)
#else
#ifndef COREDLL_CORESIOA
void _assert(void*, void*, unsigned);
#else
#define _assert(exp,file,line) fprintf(stderr, "Assertion failed: %s, file %s, line %d\n", (char*)exp, file, line)
#endif
#define assert(exp) (void)( (exp) || (_assert(#exp, __FILE__, __LINE__), 0) )
#endif


#ifdef __cplusplus
}
#endif


#endif // __wcecompat__ASSERT_H__
