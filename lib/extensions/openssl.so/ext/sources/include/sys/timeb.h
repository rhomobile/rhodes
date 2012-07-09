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


#ifndef __wcecompat__SYS__TIMEB_H__
#define __wcecompat__SYS__TIMEB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <winsock2.h>
#include <time.h>

struct timeb
{
	time_t			time;
	unsigned short	millitm;
	short			timezone;
	short			dstflag;
};
#define _timeb timeb

static __int64 _onesec_in100ns = (__int64)10000000;
int __cdecl _ftime(struct timeb *tp);
#define ftime _ftime

SYSTEMTIME wce_tm2SYSTEMTIME(struct tm *t);
FILETIME wce_int642FILETIME(__int64 t);
FILETIME wce_getFILETIMEFromYear(WORD year);
__int64 wce_FILETIME2int64(FILETIME f);
time_t wce_getYdayFromSYSTEMTIME(const SYSTEMTIME* s);
struct tm wce_SYSTEMTIME2tm(SYSTEMTIME *s);
time_t wce_FILETIME2time_t(const FILETIME* f);

#ifdef __cplusplus
}
#endif


#endif // __wcecompat__SYS__TIMEB_H__
