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

using System;
using System.Threading;

namespace rho.common
{
    public abstract class CRhoThread
    {
        private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() :
		    new RhoLogger("RhoThread");

	    public static int epNormal = 0, epHigh = 1, epLow = 2;
        static int TS_NONE = 0;
        static int TS_WAIT = 1;
        static int TS_STOPPING = 2;
        //static final int TS_RUNNING = 4;

        int m_nState;
        private Thread m_thread = new Thread(runProc);
	    private Mutex m_syncObj = new Mutex();

        public Thread CurrentThread { get { return m_thread; } }

        public CRhoThread()
        {
    	    m_nState = TS_NONE;
        }

        public Object getSyncObject(){ return m_syncObj; }
        public boolean isStopping(){return (m_nState&TS_STOPPING) != 0;}
        public boolean isWaiting(){return (m_nState&TS_WAIT) != 0;}
        //public boolean isRunning(){return (m_nState&TS_RUNNING) != 0;}

        public boolean isAlive() { return m_thread.IsAlive;  }

        public void start(int ePriority)
        {
    	    if ( isAlive() )
    		    return;

            if ( ePriority == epLow )
    		    m_thread.IsBackground = true;

            m_thread.Start(this);
        }

        public abstract void run();

        public static void runProc(object obj)
        {
            ((CRhoThread)obj).run();
        }

        public virtual void stop(int nTimeoutToKill)
        {
            if (!isAlive())
                return;

    	    m_nState |= TS_STOPPING;
    	    stopWait();

		    try{
                if (!m_thread.Join(nTimeoutToKill * 1000))
                    m_thread.Abort();
		    }catch(Exception e){
			    LOG.ERROR("stop failed", e);
		    }finally
		    {
			    m_nState &= ~TS_STOPPING;
		    }
        }

        public void waitMs(int nTimeoutMs)
        {
            try{
                lock (m_syncObj)
                {
                    m_nState |= TS_WAIT;
                    Monitor.Wait(m_syncObj, nTimeoutMs);
                }
		    }finally{
			    m_nState &= ~TS_WAIT;
		    }
        }

        public void wait(int nTimeout)
        {
            try
            {
                lock (m_syncObj)
                {
                    m_nState |= TS_WAIT;
                    Monitor.Wait(m_syncObj, nTimeout * 1000);
                }
            }
            finally
            {
                m_nState &= ~TS_WAIT;
            }
        }

        public void stopWait()
        {
            lock (m_syncObj)
            {
                Monitor.PulseAll(m_syncObj);
            }
        }

        public static void sleep(int nMS)
        {
            Thread.Sleep(nMS);
        }
    }
}
