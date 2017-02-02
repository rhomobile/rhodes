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

#include "common/RhoConf.h"


#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoTimer"


static bool is_net_trace() {
    static int res = -1;
    if (res == -1) {
        if (rho_conf_getBool("net_trace") ) {
            res = 1;
        }
        else {
            res = 0;
        }
    }
    return res == 1;
}





namespace rho{
namespace common{

CRhoTimer::CTimerItem::CTimerItem(int nInterval, const char* szCallback, const char* szCallbackData): 
    m_nInterval(nInterval), m_strCallback(szCallback), m_strCallbackData(szCallbackData)
{
	m_oFireTime = CTimeInterval::getCurrentTime();
	m_overflow = m_oFireTime.addMillis(nInterval);
}

CRhoTimer::CNativeTimerItem::CNativeTimerItem(int nInterval, CRhoTimer::ICallback* callback):
    m_nInterval(nInterval), m_pCallback(callback)
{
	m_oFireTime = CTimeInterval::getCurrentTime();
    m_overflow = m_oFireTime.addMillis(nInterval);
}

CRhoTimer::CRhoTimer() : m_checkerThread(*this)
{
  m_checkerThread.start(IRhoRunnable::epNormal);
}

CRhoTimer::~CRhoTimer()
{
  m_checkerThread.stop(1000);
}

void CRhoTimer::addTimer(int nInterval, const char* szCallback, const char* szCallbackData)
{
    RAWTRACE2("CRhoTimer::addTimer %d, %s", nInterval, szCallback);

    synchronized(m_mxAccess)
    {
      m_arItems.addElement(CTimerItem(nInterval, szCallback, szCallbackData));

      m_checkerThread.stopWait();

    }
}

void CRhoTimer::addNativeTimer(int nInterval, CRhoTimer::ICallback* callback)
{
    RAWTRACE1("CRhoTimer::addNativeTimer %d", nInterval);

    synchronized(m_mxAccess)
    {
      m_arNativeItems.addElement(CNativeTimerItem(nInterval, callback));

      m_checkerThread.stopWait();

    }
}


unsigned long CRhoTimer::getNextTimeout()
{
    synchronized(m_mxAccess);

    if ( (m_arItems.size() == 0) && (m_arNativeItems.size() == 0) )
        return 0;

    CTimeInterval curTime = CTimeInterval::getCurrentTime();
    unsigned long nMinInterval = ((unsigned long)-1);

    for( int i = 0; i < (int)m_arItems.size(); i++ )
    {
        unsigned long nInterval = 0;
        if ( m_arItems.elementAt(i).m_oFireTime.toULong() > curTime.toULong() )
    		{
            nInterval = m_arItems.elementAt(i).m_oFireTime.toULong() - curTime.toULong();
    		}
    		else
    		{	
                nInterval=0;
    		}

        if ( nInterval < nMinInterval )
            nMinInterval = nInterval;
    }

    for( int i = 0; i < (int)m_arNativeItems.size(); i++ )
    {
        unsigned long nInterval = 0;
        if ( m_arNativeItems.elementAt(i).m_oFireTime.toULong() > curTime.toULong() )
    		{
            nInterval = m_arNativeItems.elementAt(i).m_oFireTime.toULong() - curTime.toULong();
    		}
    		else
    		{	
                nInterval=0;
    		}

        if ( nInterval < nMinInterval )
            nMinInterval = nInterval;
    }


    if ( nMinInterval < 100 )
        nMinInterval = 100;

    RAWTRACE1("CRhoTimer::getNextTimeout: %d",nMinInterval);

    return nMinInterval;
}

boolean CRhoTimer::checkTimers()
{
    RAWTRACE("CRhoTimer::checkTimers");

    synchronized(m_mxAccess);

    boolean bRet = false;
    CTimeInterval curTime = CTimeInterval::getCurrentTime();
    for( int i = (int)m_arItems.size()-1; i >= 0; i--)
    {
        CTimerItem oItem = m_arItems.elementAt(i);
        
            if(oItem.m_overflow==false)
            {
                
                if ( curTime.toULong() >= oItem.m_oFireTime.toULong() )
                {
                    RAWTRACE("CRhoTimer::checkTimers: firing timer");
                    m_arItems.removeElementAt(i);
                    if ( RHODESAPP().callTimerCallback(oItem.m_strCallback, oItem.m_strCallbackData) )
                        bRet = true;
                }
                
            }
            else
            {
                if ( curTime.toULong() >= oItem.m_oFireTime.toULong() )
                {
                    if((curTime.toULong()-oItem.m_oFireTime.toULong())<=oItem.m_nInterval)
                    {
                        RAWTRACE("CRhoTimer::checkTimers: firing timer");
                        m_arItems.removeElementAt(i);
                        if ( RHODESAPP().callTimerCallback(oItem.m_strCallback, oItem.m_strCallbackData) )
                            bRet = true;
                    }
                    
                }
                
            }
    }

    for( int i = (int)m_arNativeItems.size()-1; i >= 0; i--)
    {
        CNativeTimerItem oItem = m_arNativeItems.elementAt(i);
        
        if(oItem.m_overflow==false)
        {
            if ( curTime.toULong() >= oItem.m_oFireTime.toULong() )
            {
                RAWTRACE("CRhoTimer::checkTimers: firing native timer");
                m_arNativeItems.removeElementAt(i);
                if (is_net_trace()) {
                    RAWTRACE("$NetRequestProcess$ PRE ONTIMER CRhoTimer::checkTimers() 1 TIMER");
                }
                if ( oItem.m_pCallback->onTimer() ) {
                    bRet = true;
                }
                if (is_net_trace()) {
                    RAWTRACE("$NetRequestProcess$ POST ONTIMER CRhoTimer::checkTimers() 1 TIMER");
                }
            }
        }
        else
        {
            if ( curTime.toULong() >= oItem.m_oFireTime.toULong() )
            {
                if((curTime.toULong()-oItem.m_oFireTime.toULong())<=oItem.m_nInterval)
                {
                    RAWTRACE("CRhoTimer::checkTimers: firing native timer");
                    m_arNativeItems.removeElementAt(i);
                    if (is_net_trace()) {
                        RAWTRACE("$NetRequestProcess$ PRE ONTIMER CRhoTimer::checkTimers() 2 TIMER");
                    }
                    if ( oItem.m_pCallback->onTimer() ) {
                        bRet = true;
                    }
                    if (is_net_trace()) {
                        RAWTRACE("$NetRequestProcess$ POST ONTIMER CRhoTimer::checkTimers() 2 TIMER");
                    }
                }
                
            }
        }
    }


    return bRet;
}

void CRhoTimer::stopTimer(const char* szCallback)
{
    RAWTRACE1("CRhoTimer::stopTimer: %s",szCallback);

    synchronized(m_mxAccess);

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

void CRhoTimer::stopNativeTimer(CRhoTimer::ICallback* callback)
{
  RAWTRACE("CRhoTimer::stopNativeTimer");

  synchronized(m_mxAccess);

  if ( 0 == callback )
  {
    m_arNativeItems.removeAllElements();
  }

  for( int i = (int)m_arNativeItems.size()-1; i >= 0; i--)
  {
    CNativeTimerItem oItem = m_arNativeItems.elementAt(i);
    if ( oItem.m_pCallback == callback )
    {
      m_arNativeItems.removeElementAt(i);
    }
  }

}

bool CRhoTimer::isNativeTimerExist(CRhoTimer::ICallback* callback)
{
	if (callback)
	{
		for (int i = (int)m_arNativeItems.size() - 1; i >= 0; i--)
		{
			CNativeTimerItem oItem = m_arNativeItems.elementAt(i);
			if (oItem.m_pCallback == callback)
			{
				return true;
			}
		}
	}

	return false;
}

}
}
