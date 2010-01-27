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

public class ClientRegister extends RhoThread 
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("ClientRegister");
	
	private static final int WAIT_BEFOREKILL_SECONDS  = 3;
	private static final int POLL_INTERVAL_SECONDS = 30*60;
    public static final int DEFAULT_PUSH_PORT = 100;

	static ClientRegister m_pInstance;
	IRhoRubyHelper 	m_sysInfo;
	NetRequest      m_NetRequest;
    String          m_strDevicePin;
    
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
		
		//send client register request in login
		//start(epLow);
	}
	
	public static ClientRegister getInstance(){ return m_pInstance; }
	
    public void run() 
    {
    	LOG.INFO("ClientRegister start");    	
    	while(true) 
    	{
    		try
    		{
    			if ( SyncThread.getInstance() != null )
    			{
	    			if ( doRegister(SyncThread.getSyncEngine()) )
	    				break;
    			}
    			
    		}catch(Exception exc)
    		{
    			LOG.ERROR("doRegister failed", exc);
    		}
    		
			LOG.INFO("Waiting for "+ POLL_INTERVAL_SECONDS+ " sec to try again to register client");
			wait(POLL_INTERVAL_SECONDS);
		}
        LOG.INFO( "ClientRegister thread shutdown" );
    	
    }

    public String getRegisterBody()throws Exception
    {
		int port = RhoConf.getInstance().getInt("push_port");
    	
		String strBody = "device_pin=" + m_strDevicePin + 
			"&device_port=" + (port > 0 ? port : DEFAULT_PUSH_PORT) +
			"&device_type=" + m_sysInfo.getPlatform();
    	
		return strBody;
    }
    
    private boolean doRegister(	SyncEngine oSync )throws Exception
    {
    	String session = oSync.loadSession();
    	if ( session == null || session.length() == 0 )
    		return false;
    	
		String client_id = oSync.loadClientID();
		if ( client_id == null || client_id.length() == 0 )
			return false;
    	
		IDBResult res = oSync.getDB().executeSQL("SELECT token,token_sent from client_info");
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
		
		String serverUrl = RhoConf.getInstance().getPath("syncserver");
		if (serverUrl != null && serverUrl.length()>0) 
		{
			String strBody = getRegisterBody();
	    	strBody += "&client_id=" + client_id;
			
			NetResponse resp = getNet().pushData(serverUrl+"clientregister", strBody, oSync);
			if( resp.isOK() ) 
			{
				try {
					oSync.getDB().executeSQL("UPDATE client_info SET token_sent=?, token=?", new Integer(1), m_strDevicePin );
				} catch(Exception ex) {
					LOG.ERROR("Error saving token_sent to the DB...");
				}	
				LOG.INFO("Registered client sucessfully...");
				return true;
			} else {
				LOG.INFO("Network error POST-ing device pin to the server...");
			}
		} else {
			LOG.INFO("Can't register client because syncserver url is not configured...");
		}
		
		return false;
    }    
}
