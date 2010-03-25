#include "stdafx.h"

time_t SystemTimeToUnixTime (const LPSYSTEMTIME sysTime)
{
	if ( NULL == sysTime )
		return -1;
	
	bool isValid = false;

	if ((sysTime->wYear >= 1900) && 
		((sysTime->wMonth >= 1)  && (sysTime->wMonth <= 12))  && 
		((sysTime->wDay >= 1)    && (sysTime->wDay <= 31))    &&
		((sysTime->wHour >= 0)   && (sysTime->wHour <= 23))   && 
		((sysTime->wMinute >= 0) && (sysTime->wMinute <= 59)) &&
		((sysTime->wSecond >= 0) && (sysTime->wSecond <= 59))
		)
	{
		isValid = true;
	} 
	
	if (isValid == false)
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
	retVal = ::mktime (&atm);

	return retVal;
}