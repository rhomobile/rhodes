/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2008-2011 Rhomobile, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "stdafx.h"
#include "common/RhoPort.h"

void UnixTimeToFileTime (const time_t unixTime, LPFILETIME pFileTime)
{
	LONGLONG ll = 0;
	ll = Int32x32To64 (unixTime, 10000000) + 116444736000000000;
	pFileTime->dwLowDateTime  = (DWORD )(ll);
	pFileTime->dwHighDateTime = (DWORD )(ll >> 32);
}


time_t SystemTimeToUnixTime (const LPSYSTEMTIME sysTime)
{
	if (NULL == sysTime)
		return -1;

	BOOL isValid = FALSE;

	if ((sysTime->wYear >= 1900) &&
		((sysTime->wMonth >= 1)  && (sysTime->wMonth <= 12))  &&
		((sysTime->wDay >= 1)    && (sysTime->wDay <= 31))    &&
		((sysTime->wHour >= 0)   && (sysTime->wHour <= 23))   &&
		((sysTime->wMinute >= 0) && (sysTime->wMinute <= 59)) &&
		((sysTime->wSecond >= 0) && (sysTime->wSecond <= 59))
		)
	{
		isValid = TRUE;
	}

	if (isValid == FALSE)
		return -1 ;

	struct tm atm;
	atm.tm_sec = sysTime->wSecond;
	atm.tm_min = sysTime->wMinute;
	atm.tm_hour = sysTime->wHour;
	atm.tm_mday = sysTime->wDay;
	atm.tm_mon = sysTime->wMonth - 1;
	atm.tm_year = sysTime->wYear - 1900;
	atm.tm_isdst = -1;

	time_t retVal = -1 ;
	retVal = mktime(&atm);

	return retVal;
}


BOOL UnixTimeToSystemTime (const time_t unixTime, LPSYSTEMTIME pSysTime)
{
	ZeroMemory (pSysTime, sizeof(SYSTEMTIME));

	FILETIME fileTime = {0};
	UnixTimeToFileTime (unixTime, &fileTime);

	BOOL retVal  = FALSE;

	FILETIME localFileTime = {0};
	retVal = FileTimeToLocalFileTime (&fileTime, &localFileTime);

	if (retVal)
		retVal = FileTimeToSystemTime (&localFileTime, pSysTime);

	return retVal;
}
