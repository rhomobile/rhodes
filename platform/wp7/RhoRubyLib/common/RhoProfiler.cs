using System;

namespace rho.common
{
    public class RhoProfiler
    {
        public static boolean RHO_STRIP_PROFILER = true;
	
	    public static String FILE_READ = "FileRead";
	    public static String FILE_WRITE = "FileWrite";
	    public static String FILE_SYNC = "FileSync";
	    public static String FILE_SET_SIZE = "FileSetSize";
	    public static String FILE_DELETE = "FileDelete";
	    public static String FILE_RENAME = "FileRename";
	
	    private static RhoLogger LOG = new RhoLogger("PROFILER");
	
        class CCounter
        {
            long  m_startTime;
            boolean m_bWasStarted;
        
            public CCounter(boolean bStart){
        	    m_bWasStarted = false;
                if ( bStart )
                    start();     
            }

            public virtual boolean isGlobal() { return false; }

            long getCurTime(){
        	    return DateTime.Now.ToFileTime();
            }
            public void start(){ 
        	    m_startTime = getCurTime(); 
        	    m_bWasStarted = true; 
            }
        
            public virtual long stop(){
                if ( m_startTime == 0 )
                    return m_startTime;

                long res = getCurTime()-m_startTime;
                m_startTime = 0;
                return res;
            }
            public virtual long flush() { return stop(); }

            public boolean isWasStarted(){ return m_bWasStarted;}
        };

        class CGlobalCounter : CCounter
        {
    	    long m_sumGlobal;

            public CGlobalCounter() : base(false)  { }
            public override boolean isGlobal() { return true; }

            public override long stop(){
                m_sumGlobal += base.stop();

                return m_sumGlobal;
            }
            public override long flush()
            { 
        	    long res = stop(); 
                m_sumGlobal = 0;
                return res;
            }

        };

        private static Hashtable<String, CCounter> m_mapCounters = new Hashtable<String, CCounter>();
    
	    public virtual void START(String szCounterName){
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

            String strTime = nMin + ":" +
        	    nSec + ":" +
        	    mSec;
        
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

	    public virtual void STOP(String szCounterName){
	        stopCounter(szCounterName,false);
	    }
	
	    //Global accumulative counters
        public virtual void CREATE_COUNTER(String szCounterName)
        {
	        m_mapCounters.put(szCounterName, new CGlobalCounter() ); 
	    }

        public virtual void DESTROY_COUNTER(String szCounterName)
        {
	        stopCounter( szCounterName, true );
	    }

        public virtual void FLUSH_COUNTER(String szCounterName, String msg)
        {
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
}
