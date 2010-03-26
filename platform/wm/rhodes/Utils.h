#ifndef __UTILS_H__
#define __UTILS_H__

void   UnixTimeToFileTime (const time_t unixTime, LPFILETIME pFileTime);

time_t SystemTimeToUnixTime (const LPSYSTEMTIME sysTime);
BOOL   UnixTimeToSystemTime (const time_t unixTime, LPSYSTEMTIME pSysTime);

#endif
