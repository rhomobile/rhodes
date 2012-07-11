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


#ifndef __wcecompat__TIME_H__
#define __wcecompat__TIME_H__

#include <stdlib.h>		// time_t defined there

extern size_t wcsftime(wchar_t *, size_t, const char *,const struct tm *);

#ifdef __cplusplus
extern "C" {
#endif


//typedef int time_t;

#ifndef _CLOCK_T_DEFINED
typedef unsigned long clock_t;
#endif
#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC (1000)
#endif

#ifndef DAYSPERWEEK
#define DAYSPERWEEK 7
#endif

time_t time(time_t* t);
clock_t __cdecl clock(void);

#ifndef _TM_DEFINED
#define _TM_DEFINED
struct tm
{
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};
#endif

struct tm* localtime(const time_t* clock);
struct tm * __cdecl gmtime(const time_t *clock);
size_t strftime(char *s, size_t maxs, const char *f, const struct tm *t);
time_t mktime(struct tm* pt);

void _tzset ();

#define tzset _tzset;
#define timegm mktime

#ifdef __cplusplus
}
#endif


#endif // __wcecompat__TIME_H__
