#ifndef _RHOTIME_H_
#define _RHOTIME_H_

#include "RhoPort.h"

#if !defined( OS_WINDOWS ) && !defined(OS_WINCE)
#include <sys/time.h>
#endif

namespace rho{
namespace common{

class CBaseTime{
public:
    CBaseTime(){ memset( &m_nativeTime, 0, sizeof(m_nativeTime) ); }
    CBaseTime(const CBaseTime& time ){ m_nativeTime = time.m_nativeTime; }
    const CBaseTime& operator=(const CBaseTime& time ){ m_nativeTime = time.m_nativeTime; return *this; }

    void setToCurTime(){
        time( &m_nativeTime );
    }

    unsigned long toULong()
    {
        return static_cast<unsigned long>(m_nativeTime);
    }

protected:
    time_t m_nativeTime;

};

class CLocalTime : public CBaseTime{
public:
    CLocalTime(){ setToCurTime(); }

    String toString(boolean ms = false, boolean inFileFormat = false){ 
        char timeBuf[40];
        int nSize = 0;
        if ( ms )
        {
#if !defined( OS_WINDOWS ) && !defined(OS_WINCE)
            struct timeval tv;
            struct timezone tz;
            struct tm* locTime;
            gettimeofday( &tv, &tz );
            locTime = localtime(&tv.tv_sec);

            if (inFileFormat)
                nSize = sprintf(timeBuf, "%02d%02d%04d%02d%02d%02d%03d", locTime->tm_mon+1, locTime->tm_mday, locTime->tm_year + 1900,
                        locTime->tm_hour, locTime->tm_min, locTime->tm_sec, (int)tv.tv_usec/1000 );
            else
                nSize = sprintf(timeBuf, "%02d/%02d/%04d %02d:%02d:%02d:%03d", locTime->tm_mon+1, locTime->tm_mday, locTime->tm_year + 1900,
                        locTime->tm_hour, locTime->tm_min, locTime->tm_sec, (int)tv.tv_usec/1000 );
#else
            SYSTEMTIME st; 
            GetSystemTime( &st ); 

            if (inFileFormat)
                nSize = sprintf(timeBuf, "%02d%02d%04d%02d%02d%02d%03d", st.wMonth, st.wDay, st.wYear,
                    st.wHour, st.wMinute, st.wSecond, st.wMilliseconds );
            else
                nSize = sprintf(timeBuf, "%02d/%02d/%04d %02d:%02d:%02d:%03d", st.wMonth, st.wDay, st.wYear,
                    st.wHour, st.wMinute, st.wSecond, st.wMilliseconds );
#endif
        }else
        {
            struct tm* locTime = localtime(&m_nativeTime);

            if (inFileFormat)
                nSize = sprintf(timeBuf, "%02d%02d%04d%02d%02d%02d", locTime->tm_mon+1, locTime->tm_mday, locTime->tm_year + 1900,
                        locTime->tm_hour, locTime->tm_min, locTime->tm_sec );
            else
                nSize = sprintf(timeBuf, "%02d/%02d/%04d %02d:%02d:%02d", locTime->tm_mon+1, locTime->tm_mday, locTime->tm_year + 1900,
                        locTime->tm_hour, locTime->tm_min, locTime->tm_sec );
        }

        timeBuf[nSize] = 0;
        return String(timeBuf); 
    }

};

class CTimeInterval {
public:
    CTimeInterval() : m_nativeTime(0){}

    String toString(){
        char timeBuf[22];

        int nMin = m_nativeTime/(60*1000);
        int nSec = (m_nativeTime - nMin*(60*1000))/1000;
        int mSec = m_nativeTime - nSec*1000 - nMin*(60*1000);

        int nSize = sprintf(timeBuf, "%d:%02d:%03d", nMin, nSec, mSec );
        timeBuf[nSize] = 0;
        return String(timeBuf);
    }

    unsigned long toULong()
    {
        return m_nativeTime;
    }

    CTimeInterval minus(const CTimeInterval& time)const{
        CTimeInterval res;
        res.m_nativeTime = m_nativeTime - time.m_nativeTime;
        return res;
    }

    CTimeInterval& operator+=(const CTimeInterval& time){
        m_nativeTime += time.m_nativeTime;
        return *this;
    }

    static CTimeInterval getCurrentTime(){
        CTimeInterval res;

#if defined( OS_WINDOWS ) || defined(OS_WINCE)
        res.m_nativeTime = GetTickCount();
#else
        struct timeval tv;
        gettimeofday( &tv, NULL );
        res.m_nativeTime = (unsigned long)((tv.tv_sec) * 1000 + tv.tv_usec / 1000);
#endif

        return res;
    }

    bool isEmpty()const{ return m_nativeTime == 0 ; }
    
private:
	unsigned long m_nativeTime;
};

inline CTimeInterval operator-(const CTimeInterval& time1, const CTimeInterval& time2){
    return time1.minus(time2);
}

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
