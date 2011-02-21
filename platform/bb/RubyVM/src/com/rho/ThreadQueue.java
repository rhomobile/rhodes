package com.rho;

import j2me.util.LinkedList;

public abstract class ThreadQueue extends RhoThread
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("ThreadQueue");
	
	public static void setLogCategory(String category)
	{
		LOG.setLogCategory(category);
	}
	
	public static final int QUEUE_POLL_INTERVAL_SECONDS  = 300;
	public static final int QUEUE_POLL_INTERVAL_INFINITE  = Integer.MAX_VALUE/1000;
	public static final int QUEUE_STARTUP_INTERVAL_SECONDS  = 10;

    public interface IQueueCommand
    {
        public abstract boolean equals(IQueueCommand cmd);
        public abstract String toString();
        
        public abstract void cancel();
    };

    private RhoClassFactory m_ptrFactory;
	private int           m_nPollInterval;
	private Object m_mxStackCommands;// = new Mutex();
   	private LinkedList	  m_stackCommands = new LinkedList();
   	private IQueueCommand m_pCurCmd;
   	
    boolean m_bNoThreaded;

    public abstract void processCommand(IQueueCommand pCmd);
    public void onTimeout(){}
    
    public int  getPollInterval(){ return m_nPollInterval;}

    public boolean isNoThreadedMode(){ return m_bNoThreaded; }
    public void setNonThreadedMode(boolean b){m_bNoThreaded = b;}

    public RhoClassFactory getFactory(){ return m_ptrFactory; }

    public int getLastPollInterval(){ return 0;}
    public boolean isSkipDuplicateCmd() { return false; }

    protected Object getCommandLock(){ return m_mxStackCommands; }
    protected IQueueCommand getCurCommand(){ return m_pCurCmd; }
    protected LinkedList/*Ptr<IQueueCommand*>&*/ getCommands(){ return m_stackCommands; }
    
    public ThreadQueue(RhoClassFactory factory)
    {
        super(factory);
        
        m_nPollInterval = QUEUE_POLL_INTERVAL_SECONDS;
        m_bNoThreaded = false;

        m_ptrFactory = factory;
        
        m_mxStackCommands = getSyncObject();
    }

    protected void addQueueCommandInt(IQueueCommand pCmd)
    {
        LOG.INFO("addCommand: " + pCmd.toString());

        synchronized(m_mxStackCommands)
        {
	    	boolean bExist = false;
	        if ( isSkipDuplicateCmd() )
	        {
	    	    for ( int i = 0; i < (int)m_stackCommands.size(); i++ )
	    	    {
	    	    	if ( m_stackCommands.get(i).equals(pCmd) )
	    		    {
	                    LOG.INFO("Command already exists in queue. Skip it.");
	    			    bExist = true;
	    			    break;
	    		    }
	    	    }
	        }
	
	    	if ( !bExist )
	    		m_stackCommands.add(pCmd);
        }
    }

    public void addQueueCommand(IQueueCommand pCmd)
    { 
        addQueueCommandInt(pCmd);

        if ( isNoThreadedMode()  )
            processCommands();
        else if ( isAlive() )
    	    stopWait(); 
    }
    
    public void stop(int nTimeoutToKill)
    {
        cancelCurrentCommand();
        super.stop(nTimeoutToKill);
    }

    void cancelCurrentCommand()
    {
        synchronized(m_mxStackCommands)
        {
	        if ( m_pCurCmd != null )
	            m_pCurCmd.cancel();
        }
    }

    protected void processCommandBase(IQueueCommand pCmd)
    {
        synchronized(m_mxStackCommands)
        {
            m_pCurCmd = pCmd;
        }

        processCommand(pCmd);

        synchronized(m_mxStackCommands)
        {
            m_pCurCmd = null;
        }
    }
    
    public void run()
    {
	    LOG.INFO("Starting main routine...");

	    int nLastPollInterval = getLastPollInterval();
	    while( !isStopping() )
	    {
            int nWait = m_nPollInterval > 0 ? m_nPollInterval : QUEUE_POLL_INTERVAL_INFINITE;

            if ( m_nPollInterval > 0 && nLastPollInterval > 0 )
            {
                int nWait2 = m_nPollInterval - nLastPollInterval;
                if ( nWait2 <= 0 )
                    nWait = QUEUE_STARTUP_INTERVAL_SECONDS;
                else
                    nWait = nWait2;
            }

            synchronized(m_mxStackCommands)
            {
	            if ( nWait >= 0 && !isStopping() && isNoCommands() )
			    {
	                LOG.INFO("ThreadQueue blocked for " + nWait + " seconds...");
	                wait(nWait);
	                
	                if ( isNoCommands() )
	                	onTimeout();
	            }
            }
            nLastPollInterval = 0;

            if ( !isStopping() )
            {
	        	try{
	        		processCommands();
	        	}catch(Exception e)
	        	{
	        		LOG.ERROR("processCommand failed", e);
	        	}
            }
	    }
	    
	    LOG.INFO("Thread shutdown");	    
    }

    public boolean isNoCommands()
    {
	    boolean bEmpty = false;
	    synchronized(m_mxStackCommands)
        {		
		    bEmpty = m_stackCommands.isEmpty();
	    }

	    return bEmpty;
    }

    void processCommands()//throws Exception
    {
	    while(!isStopping() && !isNoCommands())
	    {
		    IQueueCommand pCmd = null;
		    synchronized(m_mxStackCommands)
		    {
    		    pCmd = (IQueueCommand)m_stackCommands.removeFirst();
    	    }
    		
		    processCommandBase(pCmd);
	    }
    }

    public void setPollInterval(int nInterval)
    { 
        m_nPollInterval = nInterval;
        if ( isAlive() )        
        	stopWait();
    }
}
