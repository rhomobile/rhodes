using System;
using rho.common;
using rho.db;
using rho.net;

namespace rho.sync
{
    public class ClientRegister : CRhoThread
    {
        private static RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		    new RhoLogger("ClientRegister");
        private static CRhodesApp RHODESAPP() { return CRhodesApp.Instance; }
	
	    private static int WAIT_BEFOREKILL_SECONDS  = 3;
	    private static int POLL_INTERVAL_SECONDS = 60;
	    private static int POLL_INTERVAL_INFINITE = int.MaxValue/1000;	
        public static  int DEFAULT_PUSH_PORT = 100;

	    static ClientRegister m_pInstance;
	    NetRequest      m_NetRequest;
        String          m_strDevicePin;
        int             m_nPollInterval;
    
        private NetRequest getNet() { return m_NetRequest;}
    
	    public static ClientRegister Create(String device_pin)
	    {
	        if ( m_pInstance != null) 
	            return m_pInstance;
	
	        m_pInstance = new ClientRegister(device_pin);
	        return m_pInstance;
	    }

	    public void Destroy()
	    {
		    m_NetRequest.cancel();
		
	        stop(WAIT_BEFOREKILL_SECONDS);
	        m_pInstance = null;
	    }
    
	    private ClientRegister(String device_pin)
	    {
		    m_strDevicePin = device_pin;
		    m_NetRequest = RhoClassFactory.createNetRequest();
		    m_nPollInterval = POLL_INTERVAL_SECONDS;
		
		    startUp();
	    }
	
	    public static ClientRegister getInstance(){ return m_pInstance; }
	
	    public void startUp() 
	    {	
		    if ( RhoConf.getInstance().getString("syncserver").length() > 0 )
		    {
			    start(epLow);	
	            stopWait();
	        }
	    }
	
        public override void run() 
        {
    	    LOG.INFO("ClientRegister start");    	
    	    while(!isStopping()) 
    	    {
    		    try
    		    {
    			    if ( SyncThread.getInstance() != null )
    			    {
	    			    if ( doRegister(SyncThread.getSyncEngine()) )
	    			    {
	    				    m_nPollInterval = POLL_INTERVAL_INFINITE;
	    				    //break;
	    			    }
    			    }
    			
    		    }catch(Exception exc)
    		    {
    			    LOG.ERROR("doRegister failed", exc);
    		    }
    		
			    LOG.INFO("Waiting for "+ m_nPollInterval+ " sec to try again to register client");
			    wait(m_nPollInterval);
		    }
            LOG.INFO( "ClientRegister thread shutdown" );
        }

        public String getRegisterBody(String strClientID)
        {
		    int port = RhoConf.getInstance().getInt("push_port");

            return SyncThread.getSyncEngine().getProtocol().getClientRegisterBody( strClientID, m_strDevicePin, 
                port > 0 ? port : DEFAULT_PUSH_PORT, RHODESAPP().getPlatform());
        }
    
        private boolean doRegister(	SyncEngine oSync )
        {
    	    String session = oSync.loadSession();
    	    if ( session == null || session.length() == 0 )
    	    {
    		    m_nPollInterval = POLL_INTERVAL_INFINITE;    		
    		    return false;
    	    }
		    m_nPollInterval = POLL_INTERVAL_SECONDS;    		
    	
		    String client_id = oSync.loadClientID();
		    if ( client_id == null || client_id.length() == 0 )
			    return false;
    	
		    IDBResult res = DBAdapter.getUserDB().executeSQL("SELECT token,token_sent from client_info");
            if ( !res.isOneEnd() ) {
			    String token = res.getStringByIdx(0); 
			    int token_sent = res.getIntByIdx(1);
			    if ( m_strDevicePin.equals(token) && token_sent > 0 ) 
			    {
				    //token in db same as new one and it was already send to the server
				    //so we do nothing
				    return true; 
			    }
            }

	        String strBody = getRegisterBody(client_id);
            NetResponse resp = getNet().pushData( oSync.getProtocol().getClientRegisterUrl(), strBody, oSync );
		    if( resp.isOK() ) 
		    {
			    try {
				    DBAdapter.getUserDB().executeSQL("UPDATE client_info SET token_sent=?, token=?", 1, m_strDevicePin );
			    } catch(Exception ex) {
				    LOG.ERROR("Error saving token_sent to the DB...", ex);
			    }	
			    LOG.INFO("Registered client sucessfully...");
			    return true;
		    } else {
			    LOG.INFO("Network error POST-ing device pin to the server...");
		    }
		
		    return false;
        }
    }
}
