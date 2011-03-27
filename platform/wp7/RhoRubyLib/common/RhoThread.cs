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
                if (!m_thread.Join(nTimeoutToKill * 100))
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
