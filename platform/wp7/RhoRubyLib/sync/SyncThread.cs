using System;
using rho.common;
using rho.db;

namespace rho.sync
{
    public class SyncThread : CThreadQueue
    {
        private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		    new RhoLogger("Sync");
        public CRhoRuby RhoRuby { get { return CRhoRuby.Instance; } }

	    private static int SYNC_WAIT_BEFOREKILL_SECONDS  = 3;
	
   	    public const int scNone = 0, scSyncAll = 1, scSyncOne = 2, scLogin = 3, scSearchOne=4; 
    
   	    public class SyncCommand : CThreadQueue.IQueueCommand
   	    {
   		    public int m_nCmdCode;
   		    public int m_nCmdParam;
   		    public String m_strCmdParam;
   		    public boolean m_bShowStatus;
   		
   		    public SyncCommand(int nCode, int nParam, boolean bShowStatus)
   		    {
   			    m_nCmdCode = nCode;
   			    m_nCmdParam = nParam;
   			    m_bShowStatus = bShowStatus;
   		    }
   		    public SyncCommand(int nCode, String strParam, boolean bShowStatus)
   		    {
   			    m_nCmdCode = nCode;
   			    m_strCmdParam = strParam;
   			    m_bShowStatus = bShowStatus;
   		    }
	        public SyncCommand(int nCode, String strParam, int nCmdParam, boolean bShowStatus)
	        {
		        m_nCmdCode = nCode;
		        m_strCmdParam = strParam;
                m_nCmdParam = nCmdParam;
                m_bShowStatus = bShowStatus;
	        }
   		
   		    public SyncCommand(int nCode, boolean bShowStatus)
   		    {
   			    m_nCmdCode = nCode;
   			    m_nCmdParam = 0;
   			    m_bShowStatus = bShowStatus;
   		    }
   		
   		    public boolean equals(IQueueCommand obj)
   		    {
   			    SyncCommand oSyncCmd = (SyncCommand)obj;
   			    return m_nCmdCode == oSyncCmd.m_nCmdCode && m_nCmdParam == oSyncCmd.m_nCmdParam &&
   				    (m_strCmdParam == oSyncCmd.m_strCmdParam ||
   				    (m_strCmdParam != null && oSyncCmd.m_strCmdParam != null && m_strCmdParam.equals(oSyncCmd.m_strCmdParam)));  		
   		    }
   		
   		    public String toString()
   		    {
   		        switch(m_nCmdCode)
   		        {
   		        case scNone:
   		            return "CheckPollInterval";

   		        case scSyncAll:
   		            return "SyncAll";
   		        case scSyncOne:
   		            return "SyncOne";
   		        case scLogin:
   		            return "Login";
   		        case scSearchOne:
   		            return "Search";
   		        }

   		        return "Unknown; Code : " + m_nCmdCode;
   		    }
   		
   		    public void cancel(){}
   		
   	    };

   	    public class SyncLoginCommand : SyncCommand
   	    {
   		    public String m_strName, m_strPassword;
   		    public /*common::CAutoPtr<C*/SyncNotify.SyncNotification/*>*/ m_pNotify;
   		    public SyncLoginCommand(String name, String password, String callback, SyncNotify.SyncNotification pNotify) :
                base(scLogin,callback,false)
   		    {
   			    m_strName = name;
   			    m_strPassword = password;
   			    m_pNotify = pNotify; 
   		    }
   	    };
        public class SyncSearchCommand : SyncCommand
        {
	        public String m_strFrom;
	        public boolean   m_bSyncChanges;
	        public Vector<String> m_arSources;
	    
            public SyncSearchCommand(String from, String _params, Vector<String> arSources, boolean sync_changes, int nProgressStep) :
                base(scSearchOne,_params,nProgressStep, false)
	        {
		        m_strFrom = from;
		        m_bSyncChanges = sync_changes;
		        m_arSources = arSources;
	        }
        };

	    static SyncThread m_pInstance;
        SyncEngine  m_oSyncEngine;

        public static SyncThread getInstance(){ return m_pInstance; }
        public static SyncEngine getSyncEngine(){ return m_pInstance!= null ? m_pInstance.m_oSyncEngine : null; }
        public override boolean isSkipDuplicateCmd() { return true; }
    
	    public static SyncThread Create()
	    {
	        if ( m_pInstance != null) 
	            return m_pInstance;
	
	        m_pInstance = new SyncThread();
	        return m_pInstance;
	    }

	    public void Destroy()
	    {
	        m_oSyncEngine.exitSync();
	        stop(SYNC_WAIT_BEFOREKILL_SECONDS);
		
	        if ( ClientRegister.getInstance() != null )
	    	    ClientRegister.getInstance().Destroy();
	    
	        DBAdapter.closeAll();
	    
	        m_pInstance = null;
	    }

	    SyncThread()
	    {
		    base.setLogCategory(LOG.getLogCategory());
		
		    if( RhoConf.getInstance().isExist("sync_poll_interval") )
			    setPollInterval(RhoConf.getInstance().getInt("sync_poll_interval"));

		    m_oSyncEngine = new SyncEngine();
		    m_oSyncEngine.setFactory();

	        LOG.INFO("sync_poll_interval: " + RhoConf.getInstance().getInt("sync_poll_interval"));
	        LOG.INFO("syncserver: " + RhoConf.getInstance().getString("syncserver"));
	        LOG.INFO("bulksync_state: " + RhoConf.getInstance().getInt("bulksync_state"));
	    
	        //ClientRegister.Create();
	    
		    if ( RhoConf.getInstance().getString("syncserver").length() > 0 )
			    start(epLow);
	    }

        public object getRetValue()
        {
            if ( isNoThreadedMode()  )
            {
                object res = RhoRuby.createString(getSyncEngine().getNotify().getNotifyBody());
                getSyncEngine().getNotify().cleanNotifyBody();

                return res;
            }

            return null;
        }
	
        public override int getLastPollInterval()
        {
    	    try{
	    	    long nowTime = (TimeInterval.getCurrentTime().toULong())/1000;
	    	    long latestTimeUpdated = 0;
	    	
	    	    Vector<String> arPartNames = DBAdapter.getDBAllPartitionNames();
	    	    for( int i = 0; i < (int)arPartNames.size(); i++ )
	    	    {
	    		    DBAdapter dbPart = DBAdapter.getDB((String)arPartNames.elementAt(i));
			        IDBResult res = dbPart.executeSQL("SELECT last_updated from sources");
			        for ( ; !res.isEnd(); res.next() )
			        { 
			            long timeUpdated = res.getLongByIdx(0);
			            if ( latestTimeUpdated < timeUpdated )
			        	    latestTimeUpdated = timeUpdated;
			        }
	    	    }
	    	
	    	    return latestTimeUpdated > 0 ? (int)(nowTime-latestTimeUpdated) : 0;
    	    }catch(Exception exc)
    	    {
    		    LOG.ERROR("isStartSyncNow failed.", exc);
    	    }
    	    return 0;
        }

	    public override void onTimeout()//throws Exception
	    {
	        if ( isNoCommands() && getPollInterval()>0 )
	            addQueueCommandInt(new SyncCommand(scSyncAll,false));
	    }
	
	    void checkShowStatus(SyncCommand oSyncCmd)
	    {
		    boolean bShowStatus = oSyncCmd.m_bShowStatus && !this.isNoThreadedMode();
		    m_oSyncEngine.getNotify().enableReporting(bShowStatus);
		    if (m_oSyncEngine.getNotify().isReportingEnabled())
			    m_statusListener.createStatusPopup(RhoAppAdapter.getMessageText("syncronizing_data"));
	    }	
	
	    public override void processCommand(IQueueCommand pCmd)
	    {
		    SyncCommand oSyncCmd = (SyncCommand)pCmd;
	        switch(oSyncCmd.m_nCmdCode)
	        {
	        case scSyncAll:
	    	    checkShowStatus(oSyncCmd);
	            m_oSyncEngine.doSyncAllSources();
	            break;
            case scSyncOne:
	            {
				    checkShowStatus(oSyncCmd);
	                m_oSyncEngine.doSyncSource(new SyncEngine.SourceID(oSyncCmd.m_nCmdParam,oSyncCmd.m_strCmdParam));
	            }
	            break;
	        
	        case scSearchOne:
		        {
				    checkShowStatus(oSyncCmd);
	                m_oSyncEngine.doSearch( ((SyncSearchCommand)oSyncCmd).m_arSources, oSyncCmd.m_strCmdParam, 
	                        ((SyncSearchCommand)oSyncCmd).m_strFrom, ((SyncSearchCommand)oSyncCmd).m_bSyncChanges,
	                        oSyncCmd.m_nCmdParam);
		        }
	            break;
	        
	        case scLogin:
	    	    {
	    		    SyncLoginCommand oLoginCmd = (SyncLoginCommand)oSyncCmd;
	    		    checkShowStatus(oSyncCmd);
	    		    m_oSyncEngine.login(oLoginCmd.m_strName, oLoginCmd.m_strPassword, oLoginCmd.m_pNotify );
	    	    }
	            break;
	        
	        }
	    }

	    static ISyncStatusListener m_statusListener = null;
	    public boolean setStatusListener(ISyncStatusListener listener) {
		    m_statusListener = listener;
		    if (m_oSyncEngine != null) {
			    m_oSyncEngine.getNotify().setSyncStatusListener(listener);
			    return true;
		    }
		    return false;
	    }
	
	    public override void setPollInterval(int nInterval)
	    { 
	        //if ( m_nPollInterval == 0 )
	        //    m_oSyncEngine.stopSync();
	    
	        base.setPollInterval(nInterval);	    
	    }
	
	    public static void doSyncAllSources(boolean bShowStatus)
	    {
		    getInstance().addQueueCommand(new SyncCommand(SyncThread.scSyncAll,bShowStatus));
	    }
	
	    public static void doSyncSourceByName(String strSrcName, boolean bShowStatus)
	    {
		    if (bShowStatus&&(m_statusListener != null)) {
			    m_statusListener.createStatusPopup(RhoAppAdapter.getMessageText("syncronizing_data"));
		    }
	        getInstance().addQueueCommand(new SyncCommand(SyncThread.scSyncOne, strSrcName, (int)0, bShowStatus ) );		
	    }
	
	    public static void stopSync()
	    {
		    LOG.INFO("STOP sync");
		
		    if ( getSyncEngine().isSyncing() )
		    {
                LOG.INFO("STOP sync in progress.");

			    getSyncEngine().stopSyncByUser();
			    getInstance().stopWait();

                while (DBAdapter.isAnyInsideTransaction())
			        CRhoThread.sleep(100);
		    }
	    }
	
	    public void addobjectnotify_bysrcname(String strSrcName, String strObject)
	    {
		    getSyncEngine().getNotify().addObjectNotify(strSrcName, strObject);
	    }
    }
}
