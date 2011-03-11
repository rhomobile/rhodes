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
