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


#ifndef __wcecompat__FCNTL_H__
#define __wcecompat__FCNTL_H__


#define _O_RDONLY		0x0000
#define _O_WRONLY		0x0001
#define _O_RDWR			0x0002
#define _O_APPEND		0x0008
#define _O_CREAT		0x0100
#define _O_TRUNC		0x0200
#define _O_EXCL			0x0400
#define _O_TEXT			0x4000
#define _O_BINARY		0x8000
#define _O_NOINHERIT	0x0080 
#define _O_TEMPORARY	0x0040 
#define _O_SHORT_LIVED 0x1000 
#define _O_RANDOM 0x0010 
#define _O_SEQUENTIAL 0x0020 

#define O_RDONLY		_O_RDONLY
#define O_WRONLY		_O_WRONLY
#define O_RDWR			_O_RDWR
#define O_APPEND		_O_APPEND
#define O_CREAT			_O_CREAT
#define O_TRUNC			_O_TRUNC
#define O_EXCL			_O_EXCL
#define O_TEXT			_O_TEXT
#define O_BINARY		_O_BINARY
#define O_NOINHERIT     _O_NOINHERIT
#define O_TEMPORARY		_O_TEMPORARY
#define O_SHORT_LIVED	_O_SHORT_LIVED
#define O_RANDOM		_O_RANDOM
#define O_SEQUENTIAL	_O_SEQUENTIAL

#endif // __wcecompat__FCNTL_H__
