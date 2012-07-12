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


#ifndef __wcecompat__ERRNO_H__
#define __wcecompat__ERRNO_H__


#ifdef __cplusplus
extern "C" {
#endif


extern int errno;

#ifndef EPERM
#define EPERM           1
#endif
#ifndef ENOENT
#define ENOENT          2
#endif
#ifndef ESRCH
#define ESRCH           3
#endif
#ifndef EINTR
#define EINTR           4
#endif
#ifndef EIO
#define EIO             5
#endif
#ifndef ENXIO
#define ENXIO           6
#endif
#ifndef E2BIG
#define E2BIG           7
#endif
#ifndef ENOEXEC
#define ENOEXEC         8
#endif
#ifndef EBADF
#define EBADF           9
#endif
#ifndef ECHILD
#define ECHILD          10
#endif
#ifndef EAGAIN
#define EAGAIN          11
#endif
#ifndef ENOMEM
#define ENOMEM          12
#endif
#ifndef EACCES
#define EACCES          13
#endif
#ifndef EFAULT
#define EFAULT          14
#endif
#ifndef EBUSY
#define EBUSY           16
#endif
#ifndef EEXIST
#define EEXIST          17
#endif
#ifndef EXDEV
#define EXDEV           18
#endif
#ifndef ENODEV
#define ENODEV          19
#endif
#ifndef ENOTDIR
#define ENOTDIR         20
#endif
#ifndef EISDIR
#define EISDIR          21
#endif
#ifndef ENFILE
#define ENFILE          23
#endif
#ifndef EMFILE
#define EMFILE          24
#endif
#ifndef ENOTTY
#define ENOTTY          25
#endif
#ifndef EFBIG
#define EFBIG           27
#endif
#ifndef ENOSPC
#define ENOSPC          28
#endif
#ifndef ESPIPE
#define ESPIPE          29
#endif
#ifndef EROFS
#define EROFS           30
#endif
#ifndef EMLINK
#define EMLINK          31
#endif
#ifndef EPIPE
#define EPIPE           32
#endif
#ifndef EDOM
#define EDOM            33
#endif
#ifndef EDEADLK
#define EDEADLK         36
#define EDEADLOCK EDEADLK
#endif
#ifndef ENAMETOOLONG
#define ENAMETOOLONG    38
#endif
#ifndef ENOLCK
#define ENOLCK          39
#endif
#ifndef ENOSYS
#define ENOSYS          40
#endif
#ifndef ENOTEMPTY
#define ENOTEMPTY       41
#endif

#ifndef EINVAL
#define EINVAL          22
#endif
#ifndef ERANGE
#define ERANGE          34
#endif
#ifndef EILSEQ
#define EILSEQ          42
#endif
#ifndef STRUNCATE
#define STRUNCATE       80
#endif

#ifdef __cplusplus
}
#endif


#endif // __wcecompat__ERRNO_H__
