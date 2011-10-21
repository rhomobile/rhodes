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

#ifndef _RHOTIME_H_
#define _RHOTIME_H_

#include "RhoStd.h"

#if !defined( WINDOWS_PLATFORM )
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
#if !defined( WINDOWS_PLATFORM )
            struct timeval tv;
            struct timezone tz;
            struct tm* locTime;
            gettimeofday( &tv, &tz );
#ifdef OS_SYMBIAN
            locTime = localtime((time_t*)&tv.tv_sec);
#else
            locTime = localtime(&tv.tv_sec);
#endif
            if (inFileFormat)
                nSize = sprintf(timeBuf, "%02d%02d%04d%02d%02d%02d%03d", locTime->tm_mon+1, locTime->tm_mday, locTime->tm_year + 1900,
                        locTime->tm_hour, locTime->tm_min, locTime->tm_sec, (int)tv.tv_usec/1000 );
            else
                nSize = sprintf(timeBuf, "%02d/%02d/%04d %02d:%02d:%02d:%03d", locTime->tm_mon+1, locTime->tm_mday, locTime->tm_year + 1900,
                        locTime->tm_hour, locTime->tm_min, locTime->tm_sec, (int)tv.tv_usec/1000 );
#else
            SYSTEMTIME st;
            GetLocalTime( &st );

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

    void addMillis(int nMs)
    {
        m_nativeTime += nMs;
    }

    static CTimeInterval getCurrentTime(){
        CTimeInterval res;

#if defined( WINDOWS_PLATFORM )
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

class CRhoTimer
{
    struct CTimerItem
    {
        int m_nInterval;
        CTimeInterval m_oFireTime;
        String m_strCallback;
        String m_strCallbackData;

        CTimerItem(int nInterval, const char* szCallback, const char* szCallbackData);
    };

    Vector<CTimerItem> m_arItems;

    void callTimerCallback(CTimerItem& oItem);
public:
    CRhoTimer(){}

    void addTimer(int nInterval, const char* szCallback, const char* szCallbackData)
    {
        m_arItems.addElement(CTimerItem(nInterval, szCallback, szCallbackData));
    }

    unsigned long getNextTimeout();
    boolean checkTimers();
    void stopTimer(const char* szCallback);
};

}
}

#endif //_RHOTIME_H_
