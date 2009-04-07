#ifndef _RHOTIME_H_
#define _RHOTIME_H_

#include "RhoPort.h"

namespace rho{
namespace general{

class CBaseTime{
public:
    CBaseTime(){ memset( &m_nativeTime, 0, sizeof(m_nativeTime) ); }

    void setToCurTime(){
        time( &m_nativeTime );
    }

protected:
    time_t m_nativeTime;

};

class CLocalTime : public CBaseTime{
public:
    CLocalTime(){ setToCurTime(); }

    String toString(){ 
        char timeBuf[22];

        struct tm* locTime = localtime(&m_nativeTime);

        int nSize = sprintf(timeBuf, "%02d/%02d/%04d %02d:%02d:%02d", locTime->tm_mon, locTime->tm_mday, locTime->tm_year + 1900,
            locTime->tm_hour, locTime->tm_min, locTime->tm_sec );
        timeBuf[nSize] = 0;
        return String(timeBuf); 
    }

};

#if 0
class CBaseTime{
#if defined(OS_WINDOWS) || defined(OS_WINCE) 
typedef SYSTEMTIME TimeType;
#else
#endif

public:
    CBaseTime(){ memset( &m_nativeTime, 0, sizeof(m_nativeTime) ); }
/*
    static int64 getTickCount() {
        #if defined( OS_WINDOWS ) || defined(OS_WINCE)
          SYSTEMTIME now;
          GetSystemTime(&now);
          return (static_cast<int64>(now.wSecond) * 1000000 +
                  static_cast<int64>(now.wMilliseconds) * 1000);
        #else
          struct timeval tv;
          gettimeofday(&tv, NULL);
          return static_cast<int64>(tv.tv_sec) * 1000000 + tv.tv_usec;
        #endif
    }
*/
    void setToLocalTime(){
#if defined( OS_WINDOWS ) || defined(OS_WINCE)
          GetLocalTime(&m_nativeTime);
#else
#endif
    }
    void setToUTCTime(){
#if defined( OS_WINDOWS ) || defined(OS_WINCE)
          GetSystemTime(&m_nativeTime);
#else
#endif
    }

protected:

    TimeType m_nativeTime;
};

class CLocalTime : public CBaseTime{
public:
    CLocalTime(){ setToLocalTime(); }

    String toString(){ 
        char timeBuf[100];

#if defined( OS_WINDOWS ) || defined(OS_WINCE)
        sprintf(timeBuf, "%02d/%02d/%04d %02d:%02d:%02d", m_nativeTime.wMonth, m_nativeTime.wDay, m_nativeTime.wYear,
            m_nativeTime.wHour, m_nativeTime.wMinute, m_nativeTime.wSecond );
#else
#endif

        return String(timeBuf); 
    }

};

#endif //0
}
}

#endif //_RHOFILEPATH_H_
