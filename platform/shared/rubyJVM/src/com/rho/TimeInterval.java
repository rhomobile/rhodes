package com.rho;

import java.util.Calendar;

public class TimeInterval 
{
    public TimeInterval() { m_nativeTime = 0; }

    public String toString(){
        long nMin = m_nativeTime/(60*1000);
        long nSec = (m_nativeTime - nMin*(60*1000))/1000;
        long mSec = m_nativeTime - nSec*1000 - nMin*(60*1000);

        String strTime = Long.toString(nMin) + ":" +
        	Long.toString(nSec) + ":" +
        	Long.toString(mSec);
        
		return strTime;
    }

    public long toULong()
    {
        return m_nativeTime;
    }

    public TimeInterval minus( TimeInterval time)
    {
        TimeInterval res = new TimeInterval();
        res.m_nativeTime = m_nativeTime - time.m_nativeTime;
        return res;
    }

    public TimeInterval plusAssign(TimeInterval time){
        m_nativeTime += time.m_nativeTime;
        return this;
    }

    public static TimeInterval getCurrentTime(){
        TimeInterval res = new TimeInterval();
        res.m_nativeTime = Calendar.getInstance().getTime().getTime();
        return res;
    }

    public boolean isEmpty(){ return m_nativeTime == 0 ; }
    
	private long m_nativeTime;
}
