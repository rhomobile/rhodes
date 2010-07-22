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
