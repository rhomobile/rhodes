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


#ifndef __wcecompat__IO_H__
#define __wcecompat__IO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

int __cdecl _wceaccess(const char *pathname, int mode);
int __cdecl _wceunlink(const char *pathname);
int __cdecl _wcerename(const char *oldname, const char *newname);
int _wceopen(const char *file, int mode, int pmode);
int _wceread(int fd, void *buffer, int length);
int _wcewrite(int fd, const void *buffer, unsigned count);
long _wcelseek(int handle, long offset, int origin);
int _wceclose (int fd);
FILE* _wcefdopen( int handle, const char *mode );

#define access _wceaccess
#define open _wceopen
#define _open _wceopen
#define close _wceclose
#define read _wceread
#define write _wcewrite
#define lseek _wcelseek
#define _lseek _wcelseek
#define unlink _wceunlink
#ifdef rename
#undef rename
#endif
#define rename _wcerename
#define _rename _wcerename
#define fdopen _wcefdopen
#define _fdopen _wcefdopen

#ifdef __cplusplus
}
#endif


#endif // __wcecompat__IO_H__
