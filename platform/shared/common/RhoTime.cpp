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
	RAWTRACE("CRhoTimer::CRhoTimer() 0 ");
	m_oFireTime = CTimeInterval::getCurrentTime();
	m_overflow = m_oFireTime.addMillis(nInterval);
}

CRhoTimer::CNativeTimerItem::CNativeTimerItem(int nInterval, CRhoTimer::ICallback* callback):
    m_nInterval(nInterval), m_pCallback(callback)
{
	RAWTRACE("CRhoTimer::CRhoTimer() 1 ");
	m_oFireTime = CTimeInterval::getCurrentTime();
    m_overflow = m_oFireTime.addMillis(nInterval);
}

CRhoTimer::CRhoTimer() : m_checkerThread(*this)
{
	RAWTRACE("CRhoTimer::CRhoTimer() ");
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
// Multiple Timer blocking case fix
#if defined(OS_MACOSX) || defined(OS_IPHONE)
    //Nothing to do
#else
      m_checkerThread.stopWait();
#endif
    }
}

void CRhoTimer::addNativeTimer(int nInterval, CRhoTimer::ICallback* callback)
{
    RAWTRACE1("CRhoTimer::addNativeTimer %d", nInterval);
	RAWTRACE1("CRhoTimer::addNativeTimer(i) m_arNativeItems size %d", m_arNativeItems.size());

    synchronized(m_mxAccess)
    {
      m_arNativeItems.addElement(CNativeTimerItem(nInterval, callback));
	  RAWTRACE1("CRhoTimer::addNativeTimer(ii) m_arNativeItems size %d", m_arNativeItems.size());
// Multiple Timer blocking case fix
#if defined(OS_MACOSX) || defined(OS_IPHONE)
        //Nothing to do
#else
      m_checkerThread.stopWait();
#endif
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
        		nInterval=nMinInterval+m_arItems.elementAt(i).m_oFireTime.toULong() - curTime.toULong();
    		}

        if ( nInterval < nMinInterval )
            nMinInterval = nInterval;
    }

	RAWTRACE1("CRhoTimer::getNextTimeout m_arNativeItems size : %d", m_arNativeItems.size());
    for( int i = 0; i < (int)m_arNativeItems.size(); i++ )
    {
        unsigned long nInterval = 0;
		RAWTRACE("getNextTimeout------------------------------------------");
		RAWTRACE1("CRhoTimer::getNextTimeout m_arNativeItems Index %d", i);
		RAWTRACE1("CRhoTimer::getNextTimeout m_arNativeItems Firetime : %d ", m_arNativeItems.elementAt(i).m_oFireTime.toULong() );
		RAWTRACE1("CRhoTimer::getNextTimeout m_arNativeItems curTime : %d ", curTime.toULong() );
		RAWTRACE1("CRhoTimer::getNextTimeout m_arNativeItems Calc : %d ", (m_arNativeItems.elementAt(i).m_oFireTime.toULong() - curTime.toULong()) );
		RAWTRACE1("CRhoTimer::getNextTimeout m_arNativeItems Index %d", i);
        if ( m_arNativeItems.elementAt(i).m_oFireTime.toULong() > curTime.toULong() )
    		{
            nInterval = m_arNativeItems.elementAt(i).m_oFireTime.toULong() - curTime.toULong();
    		}
    		else
    		{	
        		nInterval=nMinInterval+m_arNativeItems.elementAt(i).m_oFireTime.toULong() - curTime.toULong();
    		}

        if ( nInterval < nMinInterval )
            nMinInterval = nInterval;
		RAWTRACE("getNextTimeout------------------------------------------");
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
	RAWTRACE1("CRhoTimer::checkTimers m_arNativeItems size : %d", m_arNativeItems.size());
	RAWTRACE1("CRhoTimer::checkTimers m_arNativeItems size(-1) : %d", m_arNativeItems.size() -1);
	for( int i = (int)m_arNativeItems.size()-1; i >= 0; i--)
    {
		RAWTRACE("checkTimers------------------------------------------for loop entry");
		RAWTRACE1("CRhoTimer::checkTimers m_arNativeItems Index : %d", i);
        CNativeTimerItem oItem = m_arNativeItems.elementAt(i);
		RAWTRACE1("CRhoTimer::checkTimers m_arNativeItems Index Overflow : %d", oItem.m_overflow);
		RAWTRACE1("CRhoTimer::checkTimers m_arNativeItems Index curTime : %d", curTime.toULong());
		RAWTRACE1("CRhoTimer::checkTimers m_arNativeItems Index m_oFireTime : %d", oItem.m_oFireTime.toULong());
		RAWTRACE1("CRhoTimer::checkTimers m_arNativeItems Index Calcultion : %d", (oItem.m_oFireTime.toULong() - curTime.toULong()) );
		RAWTRACE1("CRhoTimer::checkTimers m_arNativeItems Index Interval : %d", oItem.m_nInterval);
        
        if(oItem.m_overflow==false)
		{
            if ( curTime.toULong() >= oItem.m_oFireTime.toULong() )
            {
                RAWTRACE("(i)CRhoTimer::checkTimers: firing native timer");
                m_arNativeItems.removeElementAt(i);
                if ( oItem.m_pCallback->onTimer() )
                    bRet = true;
            }
        }
        else
        {
            if ( curTime.toULong() >= oItem.m_oFireTime.toULong() )
			{
				if((curTime.toULong()-oItem.m_oFireTime.toULong())<=oItem.m_nInterval)
				{
                    RAWTRACE("(ii)CRhoTimer::checkTimers: firing native timer");
                    m_arNativeItems.removeElementAt(i);
                    if ( oItem.m_pCallback->onTimer() )
                        bRet = true;
				}

			}
        }
		RAWTRACE("checkTimers------------------------------------------for loop exit");
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
  
  RAWTRACE1("CRhoTimer::stopTimer m_arNativeItems(i) size : %d", m_arNativeItems.size());

  if ( 0 == callback )
  {
    m_arNativeItems.removeAllElements();
  }
	RAWTRACE1("CRhoTimer::stopTimer m_arNativeItems (ii)size : %d", m_arNativeItems.size());
  for( int i = (int)m_arNativeItems.size()-1; i >= 0; i--)
  {
	  RAWTRACE1("CRhoTimer::stopTimer m_arNativeItems Index  : %d", i);
    CNativeTimerItem oItem = m_arNativeItems.elementAt(i);
    if ( oItem.m_pCallback == callback )
    {
		RAWTRACE1("CRhoTimer::stopTimer m_arNativeItems removing item   : %d", i);
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
