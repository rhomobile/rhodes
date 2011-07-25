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

    public void addMillis(int nMs)
    {
    	m_nativeTime += nMs;
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
