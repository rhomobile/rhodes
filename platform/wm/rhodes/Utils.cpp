#include "stdafx.h"

time_t SystemTimeToUnixTime (const LPSYSTEMTIME sys_time)
{
	if ( NULL == sys_time )
		return -1;
	
	bool is_valid = false; //flag for sys_time is valid

	if ((sys_time->wYear >= 1900) && 
		((sys_time->wMonth >= 1)  && (sys_time->wMonth <= 12))  && 
		((sys_time->wDay >= 1)    && (sys_time->wDay <= 31))    &&
		((sys_time->wHour >= 0)   && (sys_time->wHour <= 23))   && 
		((sys_time->wMinute >= 0) && (sys_time->wMinute <= 59)) &&
		((sys_time->wSecond >= 0) && (sys_time->wSecond <= 59))
		)
	{
		is_valid = true;
	} 
	
	if (is_valid == true)
		return -1 ;
	
	struct tm atm;
	atm.tm_sec = sys_time->wSecond;
	atm.tm_min = sys_time->wMinute;
	atm.tm_hour = sys_time->wHour;
	atm.tm_mday = sys_time->wDay;
	atm.tm_mon = sys_time->wMonth - 1;
	atm.tm_year = sys_time->wYear - 1900;
	atm.tm_isdst = -1; 
	
	time_t ret_val = -1 ;
	ret_val = ::mktime (&atm);

	return ( ret_val );
}