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


#ifndef __wcecompat__SYS__STAT_H__
#define __wcecompat__SYS__STAT_H__


#ifdef __cplusplus
extern "C" {
#endif


#include <stdlib.h>		/* time_t */


#define _S_IFMT         0170000         /* file type mask */
#define _S_IFDIR        0040000         /* directory */
#define _S_IFCHR        0020000         /* character special */
#define _S_IFIFO        0010000         /* pipe */
#define _S_IFREG        0100000         /* regular */
#define _S_IREAD        0000400         /* read permission, owner */
#define _S_IWRITE       0000200         /* write permission, owner */
#define _S_IEXEC        0000100         /* execute/search permission, owner */

#define S_IFMT			_S_IFMT
#define S_IFDIR			_S_IFDIR
#define S_IFCHR			_S_IFCHR
#define S_IFIFO			_S_IFIFO
#define S_IFREG			_S_IFREG
#define S_IREAD			_S_IREAD
#define S_IWRITE		_S_IWRITE
#define S_IEXEC			_S_IEXEC


#ifndef _DEV_T_DEFINED
typedef unsigned int _dev_t;
#define _DEV_T_DEFINED
#endif

#ifndef _INO_T_DEFINED
typedef unsigned short _ino_t;
#define _INO_T_DEFINED
#endif

#ifndef __OFF_T_DEFINED
typedef long _off_t;
#define __OFF_T_DEFINED
#endif

struct stat
{
	_dev_t			st_dev;
	_ino_t			st_ino;
	unsigned short	st_mode;
	short			st_nlink;
	short			st_uid;
	short			st_gid;
	_dev_t			st_rdev;
	_off_t			st_size;
	time_t			st_atime;
	time_t			st_mtime;
	time_t			st_ctime;
};


int __cdecl stat(const char *filename, struct stat *buf);
int fstat(int file, struct stat *sbuf);

#ifdef __cplusplus
}
#endif


#endif // __wcecompat__SYS__STAT_H__
