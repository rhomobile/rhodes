#include "common/RhoTime.h"
#include "common/RhodesApp.h"

namespace rho{
namespace common{

CRhoTimer::CTimerItem::CTimerItem(int nInterval, const char* szCallback, const char* szCallbackData): 
    m_nInterval(nInterval), m_strCallback(szCallback), m_strCallbackData(szCallbackData)
{
    m_oFireTime = CTimeInterval::getCurrentTime();
    m_oFireTime.addMillis(nInterval);
}

timeval CRhoTimer::getNextTimeout()
{
    timeval tv = {0};

    if ( m_arItems.size() == 0 )
        return tv;

    CTimeInterval curTime = CTimeInterval::getCurrentTime();
    unsigned long nMinInterval = ((unsigned long)-1);

    for( int i = 0; i < (int)m_arItems.size(); i++ )
    {
        unsigned long nInterval = 0;
        if ( m_arItems.elementAt(i).m_oFireTime.toULong() > curTime.toULong() )
            nInterval = m_arItems.elementAt(i).m_oFireTime.toULong() - curTime.toULong();

        if ( nInterval < nMinInterval )
            nMinInterval = nInterval;
    }

    if ( nMinInterval < 100 )
        nMinInterval = 100;

    tv.tv_sec = nMinInterval/1000;
    tv.tv_usec = (nMinInterval - tv.tv_sec*1000)*1000;
    return tv;
}

boolean CRhoTimer::checkTimers()
{
    boolean bRet = false;
    CTimeInterval curTime = CTimeInterval::getCurrentTime();
    for( int i = (int)m_arItems.size()-1; i >= 0; i--)
    {
        CTimerItem oItem = m_arItems.elementAt(i);
        if ( curTime.toULong() >= oItem.m_oFireTime.toULong() )
        {
            m_arItems.removeElementAt(i);
            if ( RHODESAPP().callTimerCallback(oItem.m_strCallback, oItem.m_strCallbackData) )
                bRet = true;
        }
    }

    return bRet;
}

void CRhoTimer::stopTimer(const char* szCallback)
{
    if ( !szCallback || !*szCallback)
        m_arItems.removeAllElements();

    for( int i = (int)m_arItems.size()-1; i >= 0; i--)
    {
        CTimerItem oItem = m_arItems.elementAt(i);
        if ( oItem.m_strCallback.compare(szCallback) == 0 )
        {
            m_arItems.removeElementAt(i);
        }
    }
}

}
}

