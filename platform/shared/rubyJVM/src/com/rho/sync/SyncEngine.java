/*
 *  rhodes
 *
 *  Copyright (C) 2008 Rhomobile, Inc. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package com.rho.sync;

import com.rho.RhoClassFactory;
import com.rho.RhoConf;
import com.rho.RhoEmptyLogger;
import com.rho.RhoEmptyProfiler;
import com.rho.RhoLogger;
import com.rho.RhoProfiler;
import com.rho.RhoRuby;
import com.rho.db.*;
import com.rho.net.*;
import com.rho.*;
import java.io.IOException;
import java.util.Vector;
import java.util.Hashtable;

public class SyncEngine implements NetRequest.IRhoSession
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("Sync");
	private static final RhoProfiler PROF = RhoProfiler.RHO_STRIP_PROFILER ? new RhoEmptyProfiler() : 
		new RhoProfiler();
	
    public static final int esNone = 0, esSyncAllSources = 1, esSyncSource = 2, esStop = 3, esExit = 4;

    static String SYNC_SOURCE_FORMAT() { return "?format=json"; }
    int SYNC_VERSION() { return 2; }
    static String SYNC_ASK_ACTION() { return "/ask"; }
	
    static class SourceID
    {
        String m_strName = "";
        String m_strUrl = "";
        int m_nID;

        public String toString()
        {
            if ( m_strName.length() > 0 )
                return "name : " + m_strName;
            else if ( m_strUrl.length() > 0 )
                return "url : " + m_strUrl;

            return "# : " + m_nID;
        }
        boolean isEqual(SyncSource src)
        {
            if ( m_strName.length() > 0 )
                return src.getName().equals(m_strName);
            else if ( m_strUrl.length() > 0 )
            {
            	URI uri1 = new URI(m_strUrl);
            	URI uri2 = new URI(src.getUrl());
            	
            	return uri1.getPath().compareTo(uri2.getPath()) == 0;
                //return src.getUrl().equals(m_strUrl);
            }
            
            return m_nID == src.getID().intValue();
        }
    };
    
    Vector/*<SyncSource*>*/ m_sources = new Vector();
    DBAdapter   m_dbAdapter;
    NetRequest m_NetRequest;
    int         m_syncState;
    String     m_clientID = "";
    Mutex m_mxLoadClientID = new Mutex();
    String m_strSession = "";
    SyncNotify m_oSyncNotify = new SyncNotify(this);
    boolean m_bStopByUser = false;
    int m_nSyncPageSize = 2000;
    
    void setState(int eState){ m_syncState = eState; }
    int getState(){ return m_syncState; }
    boolean isContinueSync(){ return m_syncState != esExit && m_syncState != esStop; }
	boolean isSyncing(){ return m_syncState == esSyncAllSources || m_syncState == esSyncSource; }
    void stopSync(){ if (isContinueSync()){ setState(esStop); m_NetRequest.cancel(); } }
    void stopSyncByUser(){ m_bStopByUser = true; stopSync(); }
    void exitSync(){ setState(esExit); m_NetRequest.cancel(); }
    boolean isStoppedByUser(){ return m_bStopByUser; }
    
    String getClientID(){ return m_clientID; }
    void setSession(String strSession){m_strSession=strSession;}
    public String getSession(){ return m_strSession; }
    boolean isSessionExist(){ return m_strSession != null && m_strSession.length() > 0; }
    DBAdapter getDB(){ return m_dbAdapter; }
    SyncNotify getNotify(){ return m_oSyncNotify; }
    NetRequest getNet() { return m_NetRequest;}
    
    SyncEngine(DBAdapter db){
    	m_dbAdapter = db;

		m_NetRequest = null;
    	m_syncState = esNone; 
    }

    String SYNC_PAGE_SIZE(){ return Integer.toString(m_nSyncPageSize); }
    int getSyncPageSize() { return m_nSyncPageSize; }
    void setSyncPageSize(int nPageSize){ m_nSyncPageSize = nPageSize; }
    
    void setFactory(RhoClassFactory factory)throws Exception{ 
		m_NetRequest = RhoClassFactory.createNetRequest();
    }
    
	void doSyncAllSources()
	{
		LOG.INFO("Sync all sources started.");
	    setState(esSyncAllSources);
	    m_bStopByUser = false;
	    
	    try
	    {
		    loadAllSources();
		
		    m_strSession = loadSession();
		    if ( isSessionExist()  ) {
		    	m_clientID = loadClientID();
			    getNotify().cleanLastSyncObjectCount();

			    PROF.CREATE_COUNTER("Net");	    
			    PROF.CREATE_COUNTER("Parse");
			    PROF.CREATE_COUNTER("DB");
			    PROF.CREATE_COUNTER("Data");
			    PROF.CREATE_COUNTER("Data1");
			    PROF.CREATE_COUNTER("Pull");
			    PROF.START("Sync");
		    	
			    syncAllSources();

			    PROF.DESTROY_COUNTER("Net");	    
			    PROF.DESTROY_COUNTER("Parse");
			    PROF.DESTROY_COUNTER("DB");
			    PROF.DESTROY_COUNTER("Data");
			    PROF.DESTROY_COUNTER("Data1");
			    PROF.DESTROY_COUNTER("Pull");
			    PROF.STOP("Sync");
		    } else {
		    	if ( m_sources.size() > 0 )
		        {		    	
			    	SyncSource src = (SyncSource)m_sources.elementAt(getStartSource());
			    	//src.m_strError = "Client is not logged in. No sync will be performed.";
			    	src.m_nErrCode = RhoRuby.ERR_CLIENTISNOTLOGGEDIN;
			    	
			    	getNotify().fireSyncNotification(src, true, src.m_nErrCode, "");
		        }else
		        	getNotify().fireSyncNotification(null, true, RhoRuby.ERR_CLIENTISNOTLOGGEDIN, "");
		    }
		    
	    }catch(Exception exc)
	    {
	    	LOG.ERROR("Sync failed.", exc);
	    }
	    
	    if ( getState() != esExit )
	    	setState(esNone);
	}

	void doSyncSource(SourceID oSrcID, String strParams, String strAction, boolean bSearchSyncChanges,
			int nProgressStep)
	{
	    setState(esSyncSource);
	    m_bStopByUser = false;
        SyncSource src = null;

	    try
	    {
		    loadAllSources();
		    src = findSource(oSrcID);
		    
	        if ( src != null )
	        {
	            LOG.INFO("Started synchronization of the data source: " + src.getName() );
	        	
	        	src.m_strParams = strParams;
	        	src.m_strAction = strAction;
	        	src.m_bSearchSyncChanges = bSearchSyncChanges;
	        	src.m_nProgressStep = nProgressStep;
	        	if ( oSrcID.m_strUrl.length() != 0 )
	        	{
	        		try{
		        		URI uri = new URI(oSrcID.m_strUrl);
		        		src.setUrlParams(uri.getQueryString());
		        		
		        		if (uri.getScheme()!= null && uri.getScheme().length()>0)
		        			src.setUrl(uri.getPathSpecificPart());
	        		}catch(Exception exc)
	        		{
	        			LOG.ERROR("Malformed url when sync by url.", exc);
	        		}
	        	}
	        	
			    m_strSession = loadSession();
			    if ( isSessionExist()  ) {
			    	m_clientID = loadClientID();
			        if ( getState() != esStop )
			        {
			        	getNotify().cleanLastSyncObjectCount();
			        	
					    PROF.CREATE_COUNTER("Net");	    
					    PROF.CREATE_COUNTER("Parse");
					    PROF.CREATE_COUNTER("DB");
					    PROF.CREATE_COUNTER("Data");
					    PROF.CREATE_COUNTER("Data1");
					    PROF.CREATE_COUNTER("Pull");
					    PROF.START("Sync");
			        	
			            src.sync();
			            
					    PROF.DESTROY_COUNTER("Net");	    
					    PROF.DESTROY_COUNTER("Parse");
					    PROF.DESTROY_COUNTER("DB");
					    PROF.DESTROY_COUNTER("Data");
					    PROF.DESTROY_COUNTER("Data1");
					    PROF.DESTROY_COUNTER("Pull");
					    PROF.STOP("Sync");
			        }
			    } else {
			    	//src.m_strError = "Client is not logged in. No sync will be performed.";
			    	src.m_nErrCode = RhoRuby.ERR_CLIENTISNOTLOGGEDIN;
			    }
		
			    getNotify().fireSyncNotification(src, true, src.m_nErrCode, src.m_nErrCode == RhoRuby.ERR_NONE ? RhoRuby.getMessageText("sync_completed") : "");
	        } else {
	        	src = new SyncSource(this);
		    	//src.m_strError = "Unknown sync source.";
		    	src.m_nErrCode = RhoRuby.ERR_RUNTIME;
	        	
    	    	throw new RuntimeException("Sync one source : Unknown Source " + oSrcID.toString() );
	        }
	    } catch(Exception exc) {
    		LOG.ERROR("Sync source " + oSrcID.toString() + " failed.", exc);
	    	
	    	if ( src != null && src.m_nErrCode == RhoRuby.ERR_NONE )
	    		src.m_nErrCode = RhoRuby.ERR_RUNTIME;
	    	
	    	getNotify().fireSyncNotification(src, true, src.m_nErrCode, "" ); 
	    }
        
	    getNotify().cleanCreateObjectErrors();
	    if ( getState() != esExit )
	    	setState(esNone);
	}

	SyncSource findSource(SourceID oSrcID)
	{
	    for( int i = 0; i < (int)m_sources.size(); i++ )
	    {
	        SyncSource src = (SyncSource)m_sources.elementAt(i);
	        if ( oSrcID.isEqual(src) )
	            return src;
	    }
	    
	    return null;
	}
	
	SyncSource findSourceByName(String strSrcName)
	{
	    SourceID oSrcID = new SourceID();
	    oSrcID.m_strName = strSrcName;
	    return findSource(oSrcID);
	}
	
	void loadAllSources()throws DBException
	{
	    m_sources.removeAllElements();
	    IDBResult res = getDB().executeSQL("SELECT source_id,source_url,token,name from sources ORDER BY priority");
	
	    for ( ; !res.isEnd(); res.next() )
	    { 
	        String strDbUrl = res.getStringByIdx(1);
	        if ( strDbUrl.length() == 0 )
	        	continue;
	        
	        if ( strDbUrl.charAt(0) == '/' || strDbUrl.charAt(0) == '\\' )
	        	strDbUrl = strDbUrl.substring(1);
	        
	        String strUrl = strDbUrl.startsWith("http") ? strDbUrl : (RhoConf.getInstance().getPath("syncserver") + strDbUrl);
	        if ( strUrl.charAt(strUrl.length()-1) == '/' || strUrl.charAt(strUrl.length()-1) == '\\' )
	        	strUrl = strUrl.substring(0, strUrl.length()-1);	        
	        
	        String name = res.getStringByIdx(3);
	        if ( strUrl.length() > 0 )
	            m_sources.addElement( new SyncSource( res.getIntByIdx(0), strUrl, name, res.getLongByIdx(2), this) );
	    }
	}

	public String loadClientID()throws Exception
	{
	    String clientID = "";
		
		synchronized( m_mxLoadClientID )
		{
		    boolean bResetClient = false;
		    int nInitialSyncState = 0;
	        IDBResult res = getDB().executeSQL("SELECT client_id,reset,initialsync_state from client_info");
	        if ( !res.isEnd() )
	        {
	            clientID = res.getStringByIdx(0);
	            bResetClient = res.getIntByIdx(1) > 0;
	            nInitialSyncState = res.getIntByIdx(2);
	        }
		
		    if ( clientID.length() == 0 )
		    {
		        clientID = requestClientIDByNet();
		
		        getDB().executeSQL("DELETE FROM client_info");
		        getDB().executeSQL("INSERT INTO client_info (client_id) values (?)", clientID);
		    }else if ( bResetClient )
		    {
		    	if ( !resetClientIDByNet(clientID) )
		    	{
			    	if ( m_sources.size() > 0 )
			        {		    	
				    	SyncSource src = (SyncSource)m_sources.elementAt(getStartSource());
				    	src.m_nErrCode = RhoRuby.ERR_REMOTESERVER;
				    	
				    	getNotify().fireSyncNotification(src, true, src.m_nErrCode, "");
			        }else
			        	getNotify().fireSyncNotification(null, true, RhoRuby.ERR_REMOTESERVER, "");
		    		
		    		stopSync();
		    	}
		    	else
		    		getDB().executeSQL("UPDATE client_info SET reset=? where client_id=?", new Integer(0), clientID );	    	
		    }

//TODO: doInitialSync 		    
//		    if ( nInitialSyncState == 0 && isContinueSync() )
//		    	doInitialSync(clientID);
		}
		
		return clientID;
	}

	boolean resetClientIDByNet(String strClientID)throws Exception
	{
	    String serverUrl = RhoConf.getInstance().getPath("syncserver");
	    String strUrl = serverUrl + "clientreset";
	    String strQuery = "?client_id=" + strClientID;
	    strQuery += "&" + ClientRegister.getInstance().getRegisterBody();
	    
	    NetResponse resp = getNet().pullData(strUrl+strQuery, this);
	    return resp.isOK();
	}
	
	String requestClientIDByNet()throws Exception
	{
		LOG.INFO("Request clientID by Net.");
		
	    String serverUrl = RhoConf.getInstance().getPath("syncserver");
	    String strUrl = serverUrl + "clientcreate";
	    String strQuery = SYNC_SOURCE_FORMAT();
	    strQuery += "&" + ClientRegister.getInstance().getRegisterBody();
	    
	    NetResponse resp = getNet().pullData(strUrl+strQuery, this);
	    if ( resp.isOK() && resp.getCharData() != null )
	    {
	    	String szData = resp.getCharData();
	        JSONEntry oJsonEntry = new JSONEntry(szData);
	
	        JSONEntry oJsonObject = oJsonEntry.getEntry("client");
	        if ( !oJsonObject.isEmpty() )
	            return oJsonObject.getString("client_id");
	    }
	
	    return "";
	}

	void doInitialSync(String strClientID)throws Exception
	{
		LOG.INFO("Initial sync: start");		
    	getNotify().fireInitialSyncNotification(false, RhoRuby.ERR_NONE);
		
	    String serverUrl = RhoConf.getInstance().getPath("syncserver");
	    String strUrl = serverUrl + "initialsync";
	    String strQuery = "?client_id=" + strClientID;
	    
	    /*NetResponse resp = getNet().pullData(strUrl+strQuery, this);
        if ( !resp.isOK() )
        {
        	LOG.ERROR("Initial sync failed: server return an error.");
        	stopSync();
        	getNotify().fireInitialSyncNotification(true, RhoRuby.ERR_REMOTESERVER);
        	return;
        }
        
        String strDataUrl = "", strScriptUrl="";
	    if ( resp.getCharData() != null )
	    {
			LOG.INFO("Initial sync: got response from server: " + resp.getCharData());		
	    	
	    	String szData = resp.getCharData();
	        JSONEntry oJsonEntry = new JSONEntry(szData);
	
	        JSONEntry oJsonObject = oJsonEntry.getEntry("initialsync");
	        if ( !oJsonObject.isEmpty() )
	        {
	        	strDataUrl = oJsonObject.getString("data");
	        	strScriptUrl = oJsonObject.getString("script");
	        }
	    }
	    if ( strDataUrl.length() == 0 || strScriptUrl.length() == 0)
	    {
        	LOG.ERROR("Initial sync failed: server return incorrect response.");
        	stopSync();
        	getNotify().fireInitialSyncNotification(true, RhoRuby.ERR_REMOTESERVER);
        	return;
	    }*/

	    String strPath = getDB().getDBPath();
	    String fDataName =  strPath + "_initial.data";
		/*LOG.INFO("Initial sync: download data from server: " + strDataUrl);		
	    NetResponse resp1 = getNet().pullFile(strDataUrl+strQuery, fDataName, this);
        if ( !resp1.isOK() )
        {
        	LOG.ERROR("Initial sync failed: cannot download database file.");
        	stopSync();
        	getNotify().fireInitialSyncNotification(true, RhoRuby.ERR_REMOTESERVER);
        	return;
        }*/
        
	    String fScriptName = strPath + "_initial.script";
/*		LOG.INFO("Initial sync: download script from server: " + strScriptUrl);		
	    NetResponse resp2 = getNet().pullFile(strScriptUrl+strQuery, fDataName, this);
        if ( !resp2.isOK() )
        {
        	LOG.ERROR("Initial sync failed: cannot download database script file.");
        	stopSync();
        	getNotify().fireInitialSyncNotification(true, RhoRuby.ERR_REMOTESERVER);
        	return;
        }*/
	    
		LOG.INFO("Initial sync: change db");
        
        //getDB().setInitialSyncDB(fDataName, fScriptName);
        
		getDB().executeSQL("UPDATE client_info SET initialsync_state=? where client_id=?", new Integer(1), strClientID );	    	
        
        getNotify().fireInitialSyncNotification(true, RhoRuby.ERR_NONE);        
	}
	
	int getStartSource()
	{
	    for( int i = 0; i < m_sources.size(); i++ )
	    {
	        SyncSource src = (SyncSource)m_sources.elementAt(i);
	        if ( !src.isEmptyToken() )
	            return i;
	    }
	
	    return 0;
	}

	void syncAllSources()throws Exception
	{
		boolean bError = false;
	    for( int i = getStartSource(); i < m_sources.size() && isContinueSync(); i++ )
	    {
	    	SyncSource src = null;
	    	try{
		        src = (SyncSource)m_sources.elementAt(i);
		        if ( isSessionExist() && getState() != esStop )
		            src.sync();
		
	    	}catch(Exception exc)
	    	{
		    	if ( src.m_nErrCode == RhoRuby.ERR_NONE )
		    		src.m_nErrCode = RhoRuby.ERR_RUNTIME;
		    	
		    	setState(esStop);
	    		throw exc;
	    	}finally{
	    		getNotify().onSyncSourceEnd( i, m_sources );
	    		bError = src.m_nErrCode != RhoRuby.ERR_NONE;
	    	}
	    }
	    
	    if ( !bError)
	    	getNotify().fireSyncNotification(null, true, RhoRuby.ERR_NONE, RhoRuby.getMessageText("sync_completed"));
	}
	
	void callLoginCallback(String callback, int nErrCode, String strMessage)
	{
		try{
		    String strBody = "error_code=" + nErrCode;
	        strBody += "&error_message=" + URI.urlEncode(strMessage != null? strMessage : "");
	        strBody += "&rho_callback=1";
	        
	        String strUrl = getNet().resolveUrl(callback);
	        
			LOG.INFO( "Login callback: " + callback + ". Body: "+ strBody );
	
		    NetResponse resp = getNet().pushData( strUrl, strBody, this );
		    if ( !resp.isOK() )
		        LOG.ERROR( "Call Login callback failed. Code: " + resp.getRespCode() + "; Error body: " + resp.getCharData() );
		}catch(Exception exc)
		{
			LOG.ERROR("Call Login callback failed.", exc);
		}
	}
	
	boolean checkAllSourcesFromOneDomain()throws Exception
	{
		loadAllSources();
		
	    if ( m_sources.size() == 0 )
	        return true;
	
	    //All sources should be from one domain
	    SyncSource src0 = (SyncSource)m_sources.elementAt(0);
	    String srv0 = getServerFromUrl(src0.getUrl());
	    for( int i = 1; i < m_sources.size(); i++ )
	    {
	        SyncSource src = (SyncSource)m_sources.elementAt(i);
	        String srv = getServerFromUrl(src.getUrl());
	        if ( srv.equals( srv0 ) != true )
	            return false;
	    }
		
	    return true;
	}
	
	void login(String name, String password, String callback)
	{
		try {
			if ( !checkAllSourcesFromOneDomain() )
			{
		    	callLoginCallback(callback, RhoRuby.ERR_DIFFDOMAINSINSYNCSRC, "");
		    	return;
			}

		    NetResponse resp = null;
			
		    try{
				
			    String serverUrl = RhoConf.getInstance().getPath("syncserver");
			    String strBody = "login=" + name + "&password=" + password + "&remember_me=1";
			    
			    resp = getNet().pullCookies( serverUrl+"client_login", strBody, this);
			    
			    if ( resp.isUnathorized() )
			    {
			        callLoginCallback(callback, RhoRuby.ERR_UNATHORIZED, resp.getCharData());
			    	return;
			    }
			    
			    if ( !resp.isOK() )
			    {
			    	callLoginCallback(callback, RhoRuby.ERR_REMOTESERVER, resp.getCharData());
			    	return;
			    }
		    }catch(IOException exc)
		    {
				LOG.ERROR("Login failed.", exc);
		    	callLoginCallback(callback, RhoRuby.getNetErrorCode(exc), "" );
		    	return;
		    }
		    
		    String strSession = resp.getCharData();
		    if ( strSession == null || strSession.length() == 0 )
		    {
		    	LOG.ERROR("Return empty session.");
		    	callLoginCallback(callback, RhoRuby.ERR_UNEXPECTEDSERVERRESPONSE, "" );
		        return;
		    }
		    
            //TODO: move session to client_info table
		    getDB().executeSQL( "UPDATE sources SET session=?", strSession );
		
		    //if ( ClientRegister.getInstance() != null )
		    //	ClientRegister.getInstance().stopWait();
		    
	    	callLoginCallback(callback, RhoRuby.ERR_NONE, "" );
		    
		}catch(Exception exc)
		{
			LOG.ERROR("Login failed.", exc);
	    	callLoginCallback(callback, RhoRuby.ERR_RUNTIME, "" );
		}
	}

	boolean isLoggedIn()throws DBException
	{
	    int nCount = 0;
	    IDBResult res = getDB().executeSQL("SELECT count(session) FROM sources WHERE session IS NOT NULL");
	    
	    if ( !res.isEnd() )
	        nCount = res.getIntByIdx(0);
	    
	    return nCount > 0;
	}

	String loadSession()throws DBException
	{
	    String strRes = "";
	    IDBResult res = getDB().executeSQL("SELECT session FROM sources WHERE session IS NOT NULL");
	    
	    if ( !res.isEnd() )
	    	strRes = res.getStringByIdx(0);
	    
	    return strRes;
	}
	
	public void logout()throws Exception
	{
	    getDB().executeSQL( "UPDATE sources SET session = NULL");
	    m_strSession = "";
	
	    loadAllSources();
	}
	
	public void setSyncServer(String url)throws Exception
	{
		RhoConf.getInstance().setPropertyByName("syncserver", url);
		RhoConf.getInstance().saveToFile();
		RhoConf.getInstance().loadConf();
		
		getDB().executeSQL("DELETE FROM client_info");
		
		logout();
	}
	
	static String getServerFromUrl( String strUrl )
	{
		URI uri = new URI(strUrl);
		return uri.getHost();
	}
	
}
