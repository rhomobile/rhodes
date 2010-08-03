package com.rho.sync;

import com.rho.IRhoRubyHelper;
import com.rho.RhoClassFactory;
import com.rho.RhoConf;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.RhoThread;
import com.rho.db.IDBResult;
import com.rho.net.NetRequest;
import com.rho.net.NetResponse;
import com.rho.db.DBAdapter;

public class ClientRegister extends RhoThread 
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("ClientRegister");
	
	private static final int WAIT_BEFOREKILL_SECONDS  = 3;
	private static final int POLL_INTERVAL_SECONDS = 60;
	private static final int POLL_INTERVAL_INFINITE = Integer.MAX_VALUE/1000;	
    public static final int DEFAULT_PUSH_PORT = 100;

	static ClientRegister m_pInstance;
	IRhoRubyHelper 	m_sysInfo;
	NetRequest      m_NetRequest;
    String          m_strDevicePin;
    int             m_nPollInterval;
    
    private NetRequest getNet() { return m_NetRequest;}
    
	public static ClientRegister Create(RhoClassFactory factory )throws Exception
	{
	    if ( m_pInstance != null) 
	        return m_pInstance;
	
	    m_pInstance = new ClientRegister(factory );
	    return m_pInstance;
	}

	public void Destroy()
	{
		m_NetRequest.cancel();
		
	    stop(WAIT_BEFOREKILL_SECONDS);
	    m_pInstance = null;
	}
    
	private ClientRegister(RhoClassFactory factory)throws Exception 
	{
		super(factory);
		
		m_sysInfo = RhoClassFactory.createRhoRubyHelper();
		m_strDevicePin = m_sysInfo.getDeviceId();
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
	
    public void run() 
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

    public String getRegisterBody(String strClientID)throws Exception
    {
		int port = RhoConf.getInstance().getInt("push_port");

        return SyncThread.getSyncEngine().getProtocol().getClientRegisterBody( strClientID, m_strDevicePin, 
            port > 0 ? port : DEFAULT_PUSH_PORT, m_sysInfo.getPlatform());
    }
    
    private boolean doRegister(	SyncEngine oSync )throws Exception
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
        if ( !res.isEnd() ) {
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
				DBAdapter.getUserDB().executeSQL("UPDATE client_info SET token_sent=?, token=?", new Integer(1), m_strDevicePin );
			} catch(Exception ex) {
				LOG.ERROR("Error saving token_sent to the DB...");
			}	
			LOG.INFO("Registered client sucessfully...");
			return true;
		} else {
			LOG.INFO("Network error POST-ing device pin to the server...");
		}
		
		return false;
    }    
}
