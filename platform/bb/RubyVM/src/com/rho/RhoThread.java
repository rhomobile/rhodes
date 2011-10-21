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

public class RhoThread extends Thread
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() :
		new RhoLogger("RhoThread");

	public final static int epNormal = 0, epHigh = 1, epLow = 2;
    static final int TS_NONE = 0;
    static final int TS_WAIT = 1;
    static final int TS_STOPPING = 2;
    //static final int TS_RUNNING = 4;

    int m_nState;

	private Object m_syncObj = new Object();

    public RhoThread(RhoClassFactory factory)
    {
    	m_nState = TS_NONE;
    }

    public Object getSyncObject(){ return m_syncObj; }
    public boolean isStopping(){return (m_nState&TS_STOPPING) != 0;}
    public boolean isWaiting(){return (m_nState&TS_WAIT) != 0;}
    //public boolean isRunning(){return (m_nState&TS_RUNNING) != 0;}

    public void start(int ePriority)
    {
    	if ( super.isAlive() )
    		return;

    	super.start();

    	int nThreadPriority = Thread.NORM_PRIORITY;
    	if ( ePriority == epHigh )
    		nThreadPriority = Thread.MAX_PRIORITY;
    	else if ( ePriority == epLow )
    		nThreadPriority = Thread.MIN_PRIORITY;

    	super.setPriority(nThreadPriority);
    }

    public void stop(int nTimeoutToKill)
    {
    	m_nState |= TS_STOPPING;
    	stopWait();

		try{
			int nTry = 0;

			while( nTry < nTimeoutToKill*10 && super.isAlive() ){
				Thread.sleep(100);
				nTry++;
			}

			if ( super.isAlive() )
				super.interrupt();

		}catch(Exception e){
			LOG.ERROR("stop failed", e);
		}finally
		{
			m_nState &= ~TS_STOPPING;
		}
    }

    public void waitMs(int nTimeoutMs)
    {
		synchronized (m_syncObj) {
			try{
				m_nState |= TS_WAIT;
				m_syncObj.wait(nTimeoutMs);
			}catch(Exception e)
			{
				LOG.ERROR("waitMs failed", e);
			}finally{
				m_nState &= ~TS_WAIT;
			}
		}

    }

    public void wait(int nTimeout)
    {
		synchronized (m_syncObj) {
			try{
				m_nState |= TS_WAIT;
				m_syncObj.wait(nTimeout*1000);
			}catch(Exception e)
			{
				LOG.ERROR("wait failed", e);
			}finally{
				m_nState &= ~TS_WAIT;
			}
		}

    }

    public void stopWait()
    {
		synchronized (m_syncObj) {
	    	if ( isWaiting() )
	    		m_syncObj.notifyAll();
    	}
    }
};
