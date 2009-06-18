package com.rho;

public class RhoThread extends Thread
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("RhoThread");
	
	public final static int epNormal = 0, epHigh = 1, epLow = 2;
	private Object m_syncObj = new Object();
	
    public RhoThread(RhoClassFactory factory)
    {
    	
    }

    public void start(int ePriority)
    {
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
    	stopWait();
    	
		try{
			int nTry = 0;
			
			while( nTry < nTimeoutToKill/100 && super.isAlive() ){
				Thread.sleep(100);
				nTry++;
			}
			
			if ( super.isAlive() )
				super.interrupt();
			
		}catch(Exception e){
			LOG.ERROR("stop failed", e);
		}
    	
    }
    
    public void wait(int nTimeout)
    {
		synchronized (m_syncObj) {
			try{
				m_syncObj.wait(nTimeout*1000);
			}catch(Exception e)
			{
				LOG.ERROR("wait failed", e);
			}
		}
    	
    }
    
    public void stopWait()
    {
		synchronized (m_syncObj) {
			m_syncObj.notify();
		}
    	
    }
};
