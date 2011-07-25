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

unsigned long CRhoTimer::getNextTimeout()
{
    if ( m_arItems.size() == 0 )
        return 0;

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

    return nMinInterval;
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

