package com.rho;

import java.util.Calendar;

public class RhoProfiler {

	public static boolean RHO_STRIP_PROFILER = true;
	
	private static final RhoLogger LOG = new RhoLogger("PROFILER");
	
    private static class CCounter
    {
        long  m_startTime;
        boolean m_bWasStarted;
        
        CCounter(boolean bStart){
        	m_bWasStarted = false;
            if ( bStart )
                start();     
        }

        boolean isGlobal(){ return false;}

        long getCurTime(){
        	return Calendar.getInstance().getTime().getTime();
        }
        void start(){ 
        	m_startTime = getCurTime(); 
        	m_bWasStarted = true; 
        }
        
        long stop(){
            if ( m_startTime == 0 )
                return m_startTime;

            long res = getCurTime()-m_startTime;
            m_startTime = 0;
            return res;
        }
        long flush(){ return stop(); }

        boolean isWasStarted(){ return m_bWasStarted;}
    };

    private class CGlobalCounter extends CCounter
    {
    	long m_sumGlobal;
        
        CGlobalCounter() { super(false); }
        boolean isGlobal(){ return true;}

        long stop(){
            m_sumGlobal += super.stop();

            return m_sumGlobal;
        }
        long flush(){ 
        	long res = stop(); 
            m_sumGlobal = 0;
            return res;
        }

    };
	
    private static java.util.Hashtable m_mapCounters = new java.util.Hashtable();
    
	public void START(String szCounterName){
	    CCounter pCounter = (CCounter)m_mapCounters.get(szCounterName);
	    if ( pCounter==null || !pCounter.isWasStarted() )
	        LOG.INFO( szCounterName + " : START" );

	    if ( pCounter == null)
	        m_mapCounters.put( szCounterName, new CCounter(true) ); 
	    else
	        pCounter.start();
	}
	
	private String intervalToString(long nInterval){
        long nMin = nInterval/(60*1000);
        long nSec = (nInterval - nMin*(60*1000))/1000;
        long mSec = nInterval - nSec*1000 - nMin*(60*1000);

        String strTime = Long.toString(nMin) + ":" +
        	Long.toString(nSec) + ":" +
        	Long.toString(mSec);
        
		return strTime;
	}
	
	private void stopCounter(String szCounterName, boolean bDestroy){
	    CCounter pCounter = (CCounter)m_mapCounters.get(szCounterName);
	    if ( pCounter==null ){
	        LOG.ERROR( szCounterName + " : Cannot find counter." );
	        return;
	    }

	    if ( bDestroy || !pCounter.isGlobal() )
	    {
	    	long oInterval = pCounter.stop();
	        LOG.INFO( szCounterName + " (" + intervalToString(oInterval) + ") : STOP" );

	        m_mapCounters.remove(szCounterName);
	    }else
	        pCounter.stop();
		
	}

	public void STOP(String szCounterName){
	    stopCounter(szCounterName,false);
	}
	
	//Global accumulative counters
	public void CREATE_COUNTER(String szCounterName){
	    m_mapCounters.put(szCounterName, new CGlobalCounter() ); 
	}
	
	public void DESTROY_COUNTER(String szCounterName){
	    stopCounter( szCounterName, true );
	}
	
	public void FLUSH_COUNTER(String szCounterName, String msg){
	    CCounter pCounter = (CCounter)m_mapCounters.get(szCounterName);
	    if ( pCounter==null ){
	        LOG.ERROR( szCounterName + " : Cannot find counter." );
	        return;
	    }

	    long oInterval = pCounter.flush();
	    LOG.INFO( szCounterName + (msg !=null&&msg.length()>0 ? " - " : "" ) + 
	    	(msg !=null&&msg.length()>0 ? msg : "" ) +
	        " (" + intervalToString(oInterval) + ") : STOP" );
	}	
}
