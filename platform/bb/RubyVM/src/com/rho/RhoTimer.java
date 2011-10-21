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

package com.rho;

import java.util.Vector;

public class RhoTimer {

	class TimerItem
    {
        int m_nInterval;
        TimeInterval m_oFireTime;
        String m_strCallback;
        String m_strCallbackData;

        TimerItem(int nInterval, String szCallback, String szCallbackData)
        {
        	m_nInterval = nInterval;
        	m_strCallback = szCallback;
        	m_strCallbackData = szCallbackData;
        	m_oFireTime = TimeInterval.getCurrentTime();
        	m_oFireTime.addMillis(nInterval);
        }
    };

    Vector/*<CTimerItem>*/ m_arItems = new Vector();

    private static RhodesApp RHODESAPP(){ return RhodesApp.getInstance(); }

    public void addTimer(int nInterval, String szCallback, String szCallbackData)
    {
        m_arItems.addElement(new TimerItem(nInterval, szCallback, szCallbackData));
    }

    public long getNextTimeout()
    {
        if ( m_arItems.size() == 0 )
            return 0;

        TimeInterval curTime = TimeInterval.getCurrentTime();
        long nMinInterval = 0xFFFFFFFF;

        for( int i = 0; i < (int)m_arItems.size(); i++ )
        {
            long nInterval = 0;
            if ( ((TimerItem)m_arItems.elementAt(i)).m_oFireTime.toULong() > curTime.toULong() )
                nInterval = ((TimerItem)m_arItems.elementAt(i)).m_oFireTime.toULong() - curTime.toULong();

            if ( nInterval < nMinInterval )
                nMinInterval = nInterval;
        }

        if ( nMinInterval < 100 )
            nMinInterval = 100;

        return nMinInterval;
    }

    public boolean checkTimers()
    {
        boolean bRet = false;
        TimeInterval curTime = TimeInterval.getCurrentTime();
        for( int i = (int)m_arItems.size()-1; i >= 0; i--)
        {
            TimerItem oItem = (TimerItem)m_arItems.elementAt(i);
            if ( curTime.toULong() >= oItem.m_oFireTime.toULong() )
            {
                m_arItems.removeElementAt(i);
                if ( RHODESAPP().callTimerCallback(oItem.m_strCallback, oItem.m_strCallbackData) )
                    bRet = true;
            }
        }

        return bRet;
    }

    public void stopTimer(String szCallback)
    {
        if ( szCallback == null || szCallback.length() == 0 )
            m_arItems.removeAllElements();

        for( int i = (int)m_arItems.size()-1; i >= 0; i--)
        {
            TimerItem oItem = (TimerItem)m_arItems.elementAt(i);
            if ( oItem.m_strCallback.compareTo(szCallback) == 0 )
            {
                m_arItems.removeElementAt(i);
            }
        }
    }
}
