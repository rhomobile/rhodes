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
import com.rho.RhoLogger;
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

    static class SyncNotification
    {
        String m_strUrl, m_strParams;
        SyncNotification(String strUrl, String strParams){ m_strUrl = strUrl; m_strParams = strParams; }
    };

    public static final int esNone = 0, esSyncAllSources = 1, esSyncSource = 2, esStop = 3, esExit = 4;

    static String SYNC_SOURCE_FORMAT() { return "?format=json"; }
    static String SYNC_ASK_ACTION() { return "/ask"; }
    static String SYNC_PAGE_SIZE() { return "1000"; }
	
    Vector/*<SyncSource*>*/ m_sources = new Vector();
    DBAdapter   m_dbAdapter;
    NetRequest m_NetRequest;
    int         m_syncState;
    String     m_clientID = "";
    Hashtable/*<int,SyncNotification>*/ m_mapNotifications = new Hashtable();
    Mutex m_mxNotifications = new Mutex();
    Mutex m_mxLoadClientID = new Mutex();
    String m_strSession = "";

    ISyncStatusListener m_statusListener = null;
    
    public void setStatusListener(ISyncStatusListener listener) { m_statusListener = listener; }
    private void reportStatus(String status, int error) {
    	if (m_statusListener != null) {
    		String strErrMsg = RhoRuby.getErrorText(error);
    		m_statusListener.reportStatus(status + (strErrMsg.length() > 0 ? " Details: " + strErrMsg: "") 
    				, error);
    	}
    	LOG.INFO("Status: "+status);
    }
    
    void setState(int eState){ m_syncState = eState; }
    int getState(){ return m_syncState; }
    boolean isContinueSync(){ return m_syncState != esExit && m_syncState != esStop; }
	boolean isSyncing(){ return m_syncState == esSyncAllSources || m_syncState == esSyncSource; }
    void stopSync(){ if (isContinueSync()){ setState(esStop); m_NetRequest.cancel(); } }
    void exitSync(){ setState(esExit); m_NetRequest.cancel(); }
    String getClientID(){ return m_clientID; }
    void setSession(String strSession){m_strSession=strSession;}
    public String getSession(){ return m_strSession; }
    boolean isSessionExist(){ return m_strSession != null && m_strSession.length() > 0; }
    DBAdapter getDB(){ return m_dbAdapter; }
    NetRequest getNet() { return m_NetRequest;}
    
    SyncEngine(DBAdapter db){
    	m_dbAdapter = db;
		m_dbAdapter.setDbCallback(new DBCallback());

		m_NetRequest = null;
    	m_syncState = esNone; 
    }

    void setFactory(RhoClassFactory factory)throws Exception{ 
		m_NetRequest = RhoClassFactory.createNetRequest();
    }
    
	void doSyncAllSources()
	{
	    setState(esSyncAllSources);

	    try
	    {
		    loadAllSources();
		
		    m_strSession = loadSession();
		    if ( isSessionExist()  ) {
		    	m_clientID = loadClientID();
		    	
			    syncAllSources();
			    
			    if ( getState() != esStop )
			    	fireNotification(null, true, RhoRuby.ERR_NONE, "Sync completed.");
		    } else {
		    	fireNotification(null, true, RhoRuby.ERR_CLIENTISNOTLOGGEDIN, 
		    			"Sync failed. Details: Client is not logged in. No sync will be performed." );
		    }
		    
	    }catch(Exception exc)
	    {
	    	LOG.ERROR("Sync failed.", exc);
	    }
	    
	    setState(esNone);
	}

	void doSyncSource(int nSrcId, String strSrcUrl, String strParams, String strAction)
	{
	    if ( strSrcUrl != null && strSrcUrl.length()>0 )
	    	LOG.INFO( "Started synchronization of the data source url: " + strSrcUrl );
	    else
	    	LOG.INFO( "Started synchronization of the data source #" + nSrcId );
		
	    setState(esSyncSource);
        SyncSource src = null;

	    try
	    {
		    loadAllSources();
		    if ( strSrcUrl != null && strSrcUrl.length()>0 )
		    	src = findSourceByUrl(strSrcUrl);
		    else
		    	src = findSourceByID(nSrcId);
		    
	        if ( src != null )
	        {
	        	src.m_strParams = strParams;
	        	src.m_strAction = strAction;
	        	
			    m_strSession = loadSession();
			    if ( isSessionExist()  ) {
			    	m_clientID = loadClientID();
			        if ( getState() != esStop )
			            src.sync();
			    } else {
			    	src.m_strError = "Client is not logged in. No sync will be performed.";
			    	src.m_nErrCode = RhoRuby.ERR_CLIENTISNOTLOGGEDIN;
			    }
		
		        fireNotification(src, true, src.m_nErrCode,
		        		src.m_nErrCode != RhoRuby.ERR_NONE ?
		        		"Sync failed for " + src.getName() + "." + (src.m_strError.length() > 0 ? " Details: " + src.m_strError : "") : 
		        			"Sync completed." );
	        } else {
	        	src = new SyncSource(this);
		    	src.m_strError = "Unknown sync source.";
		    	src.m_nErrCode = RhoRuby.ERR_RUNTIME;
	        	
	    	    if ( strSrcUrl != null && strSrcUrl.length()>0 )
	    	    	throw new RuntimeException("Sync one source : Unknown Source Url: " + strSrcUrl );
	    	    else
	    	    	throw new RuntimeException("Sync one source : Unknown Source ID: " + nSrcId );
	        }
	    } catch(Exception exc) {
	    	if ( strSrcUrl != null && strSrcUrl.length()>0 )	    	
	    		LOG.ERROR("Sync source: " + strSrcUrl + " failed.", exc);
	    	else
	    		LOG.ERROR("Sync source: " + nSrcId + " failed.", exc);
	    	
    		fireNotification(src, true, src.m_nErrCode != RhoRuby.ERR_NONE ? src.m_nErrCode : RhoRuby.ERR_RUNTIME, 
	    				"Sync failed for " + src.getName() + ". Details: " + 
	    				(src.m_strError.length() > 0 ? src.m_strError : exc.getMessage()) );
	    }
        
	    setState(esNone);
	    
	    if ( strSrcUrl != null && strSrcUrl.length()>0 )
	    	LOG.INFO( "End synchronization of the data source url: " + strSrcUrl );
	    else
	    	LOG.INFO( "End synchronization of the data source #" + nSrcId );
	}

	SyncSource findSourceByID(int nSrcId)
	{
	    for( int i = 0; i < m_sources.size(); i++ )
	    {
	        SyncSource src = (SyncSource)m_sources.elementAt(i);
	        if ( src.getID().intValue() == nSrcId )
	            return src;
	    }
	    
	    return null;
	}
	
	SyncSource findSourceByUrl(String strSrcUrl)
	{
	    for( int i = 0; i < m_sources.size(); i++ )
	    {
	        SyncSource src = (SyncSource)m_sources.elementAt(i);
	        if ( src.getUrl().equals(strSrcUrl) )
	            return src;
	    }
	    
	    return null;
	}
	
	void loadAllSources()throws DBException
	{
	    m_sources.removeAllElements();
	    IDBResult res = getDB().executeSQL("SELECT source_id,source_url,token,name from sources order by source_id");
	
	    for ( ; !res.isEnd(); res.next() )
	    { 
	        String strDbUrl = res.getStringByIdx(1);
	        if ( strDbUrl.length() == 0 )
	        	continue;
	        
	        String strUrl = strDbUrl.startsWith("http") ? strDbUrl : (RhoConf.getInstance().getString("syncserver") + strDbUrl);
	        String name = res.getStringByIdx(3);
	        if ( strUrl.length() > 0 )
	            m_sources.addElement( new SyncSource( res.getIntByIdx(0), strUrl, name, res.getUInt64ByIdx(2), this) );
	    }
	}

	public String loadClientID()throws Exception
	{
	    String clientID = "";
		
		synchronized( m_mxLoadClientID )
		{
		    boolean bResetClient = false;
	        IDBResult res = getDB().executeSQL("SELECT client_id,reset from client_info");
	        if ( !res.isEnd() )
	        {
	            clientID = res.getStringByIdx(0);
	            bResetClient = res.getIntByIdx(1) > 0;
	        }
		
		    if ( clientID.length() == 0 )
		    {
		        clientID = requestClientIDByNet();
		
		        getDB().executeSQL("DELETE FROM client_info");
		        getDB().executeSQL("INSERT INTO client_info (client_id) values (?)", clientID);
		    }else if ( bResetClient )
		    {
		    	if ( !resetClientIDByNet(clientID) )
		    		stopSync();
		    	else
		    		getDB().executeSQL("UPDATE client_info SET reset=? where client_id=?", new Integer(0), clientID );	    	
		    }
		}
		
		return clientID;
	}

	boolean resetClientIDByNet(String strClientID)throws Exception
	{
	    String serverUrl = RhoConf.getInstance().getString("syncserver");
	    String strUrl = serverUrl + "clientreset";
	    String strQuery = "?client_id=" + strClientID;
	    
	    NetResponse resp = getNet().pullData(strUrl+strQuery, "", this);
	    return resp.isOK();
	}
	
	String requestClientIDByNet()throws Exception
	{
	    String serverUrl = RhoConf.getInstance().getString("syncserver");
	    String strUrl = serverUrl + "clientcreate";
	    String strQuery = SYNC_SOURCE_FORMAT();
	    
	    NetResponse resp = getNet().pullData(strUrl+strQuery, "", this);
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
	    for( int i = getStartSource(); i < m_sources.size() && getState() != esExit; i++ )
	    {
	    	SyncSource src = null;
	    	try{
		        src = (SyncSource)m_sources.elementAt(i);
		        if ( isSessionExist() && getState() != esStop )
		            src.sync();
		
		        fireNotification(src, true, src.m_nErrCode,
		        		src.m_nErrCode != RhoRuby.ERR_NONE ?
		        		"Sync failed for " + src.getName() + "." + (src.m_strError.length() > 0 ? " Details: " + src.m_strError : "") : "");
	    	}catch(Exception exc)
	    	{
	    		fireNotification(src, true, src.m_nErrCode != RhoRuby.ERR_NONE ? src.m_nErrCode : RhoRuby.ERR_RUNTIME, 
	    				"Sync failed for " + src.getName() + ". Details: " + 
	    				(src.m_strError.length() > 0 ? src.m_strError : exc.getMessage()) );
	    		
	    		throw exc;
	    	}
	    }
	}
	
	void callLoginCallback(String callback, int nErrCode, String strMessage)
	{
		try{
		    String strBody = "error_code=" + nErrCode;
	        strBody += "&error_message=" + URI.urlEncode(strMessage != null? strMessage : "");
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
				
			    String serverUrl = RhoConf.getInstance().getString("syncserver");
			    String strBody = "login=" + name + "&password=" + password + "&remember_me=1&";
			    strBody += ClientRegister.getInstance().getRegisterBody(this);
			    
			    resp = getNet().pullCookies( serverUrl+"client_login", strBody, this);
			    if ( !resp.isOK() )
			    {
			    	callLoginCallback(callback, RhoRuby.ERR_REMOTESERVER, resp.getCharData());
			    	return;
			    }
		    }catch(IOException exc)
		    {
				LOG.ERROR("Login failed.", exc);
				String strMsg = exc.getMessage(); 
				
		    	callLoginCallback(callback, 
		    			strMsg.indexOf("timed out") >= 0 ? RhoRuby.ERR_NOSERVERRESPONSE : RhoRuby.ERR_NETWORK, strMsg );
		    	return;
		    }
		    
		    String strSession = resp.getCharData();
		    if ( strSession == null || strSession.length() == 0 )
		    {
		    	LOG.ERROR("Return empty session.");
		    	callLoginCallback(callback, RhoRuby.ERR_UNEXPECTEDSERVERRESPONSE, "" );
		        return;
		    }
		    
		    getDB().executeSQL( "UPDATE sources SET session=?", strSession );
		
		    if ( ClientRegister.getInstance() != null )
		    	ClientRegister.getInstance().stopWait();
		    
	    	callLoginCallback(callback, RhoRuby.ERR_NONE, "" );
		    
		}catch(Exception exc)
		{
			LOG.ERROR("Login failed.", exc);
	    	callLoginCallback(callback, RhoRuby.ERR_RUNTIME, exc.getMessage() );
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
	    getNet().deleteCookie("");
	
	    loadAllSources();
	    for( int i = 0; i < m_sources.size(); i++ )
	    {
	        SyncSource src = (SyncSource)m_sources.elementAt(i);
	        getNet().deleteCookie(src.getUrl());
	    }
	
	}

	void setNotification(int source_id, String strUrl, String strParams )throws Exception
	{
		LOG.INFO( "Set notification. Source ID: " + source_id + "; Url :" + strUrl + "; Params: " + strParams );
	    String strFullUrl = getNet().resolveUrl(strUrl);
		
		if ( source_id == -1 )
		{
			synchronized(m_mxNotifications){
				m_mapNotifications.clear();
				
				if ( strFullUrl.length() > 0 )
				{
					loadAllSources();
					
				    for( int i = 0; i < m_sources.size(); i++ )
				    {
				    	SyncSource src = (SyncSource)m_sources.elementAt(i); 
				    	m_mapNotifications.put( src.getID(),new SyncNotification( strFullUrl, strParams ) );
				    }
				}
			}
			LOG.INFO( " Done Set notification for all sources; Url :" + strFullUrl + "; Params: " + strParams );			
		}else
		{
		    clearNotification(source_id);
		    if ( strFullUrl.length() > 0 )
		    {
		        synchronized(m_mxNotifications){
		        	m_mapNotifications.put(new Integer(source_id),new SyncNotification( strFullUrl, strParams ) );
		        }
				LOG.INFO( " Done Set notification. Source ID: " + source_id + "; Url :" + strFullUrl + "; Params: " + strParams );
		    }
		}
	}

	void fireNotification( SyncSource src, boolean bFinish, int nErrCode, String strErrMessage )
	{
		if ( getState() == esExit )
			return;
		
		if( strErrMessage.length() > 0 || nErrCode != RhoRuby.ERR_NONE)
		{
			if ( !( src != null && src.m_strParams.length()>0) )
				reportStatus(strErrMessage,nErrCode);
		}
		
		if ( src == null )
			return; //TODO: implement all sources callback
		
		try{
		    String strBody = "", strUrl;
		    {
		    	synchronized(m_mxNotifications){
			        SyncNotification sn = (SyncNotification)m_mapNotifications.get(src.getID());
			        if ( sn == null )
			            return;
			
			        strUrl = sn.m_strUrl;
			        strBody += "total_count=" + src.getTotalCount();
			        strBody += "&processed_count=" + src.getCurPageCount();
			        
			        strBody += "&status=";
			        if ( bFinish )
			        {
				        if ( nErrCode == RhoRuby.ERR_NONE )
				        	strBody += "ok";
				        else
				        {
				        	strBody += "error";				        	
						    strBody += "&error_code=" + nErrCode;
					        strBody += "&error_message=" + 
					        	URI.urlEncode(strErrMessage != null? strErrMessage : "");
				        }
			        }
			        else
			        	strBody += "in_progress";
			        
			        if ( sn.m_strParams.length() > 0 )
			            strBody += "&" + sn.m_strParams;
		        }
		    }
			LOG.INFO( "Fire notification. Source ID: " + src.getID() + "; Url :" + strUrl + "; Body: " + strBody );
			
		    NetResponse resp = getNet().pushData( strUrl, strBody, this );
		    if ( !resp.isOK() )
		        LOG.ERROR( "Fire notification failed. Code: " + resp.getRespCode() + "; Error body: " + resp.getCharData() );
		
		    if ( bFinish )
		    	clearNotification(src.getID().intValue());
		}catch(Exception exc)
		{
			LOG.ERROR("Fire notification failed.", exc);
		}
	}

	void clearNotification(int source_id) 
	{
		LOG.INFO( "Clear notification. Source ID: " + source_id );
		
		synchronized(m_mxNotifications){
			m_mapNotifications.remove(new Integer(source_id));
		}
	}

	static String getServerFromUrl( String strUrl )
	{
		URI uri = new URI(strUrl);
		return uri.getHost();
	}

	public static class DBCallback implements IDBCallback{

		public void OnDeleteAll() {
			OnDeleteAllFromTable("object_values");
		}
		
		public void OnDeleteAllFromTable(String tableName) {
			if ( !tableName.equals("object_values") )
				return;

			try{
				String fName = DBAdapter.makeBlobFolderName();
				RhoClassFactory.createFile().delete(fName);
				DBAdapter.makeBlobFolderName(); //Create folder back
			}catch(Exception exc){
				LOG.ERROR("DBCallback.OnDeleteAllFromTable: Error delete files from table: " + tableName, exc);				
			}
		}

		public void OnDeleteFromTable(String tableName, IDBResult rows2Delete) 
		{
			if ( !tableName.equalsIgnoreCase("object_values") )
				return;
			
			for( ; !rows2Delete.isEnd(); rows2Delete.next() )
			{
				if ( !rows2Delete.getString("attrib_type").equals("blob.file") )
					continue;

				String url = rows2Delete.getString("value");
				if ( url == null || url.length() == 0 )
					continue;
				
				try{
				    SimpleFile oFile = RhoClassFactory.createFile();
				    
			        String strFilePath = oFile.getDirPath("");
			        strFilePath += url;
				    
				    oFile.delete(strFilePath);
				}catch(Exception exc){
					LOG.ERROR("DBCallback.OnDeleteFromTable: Error delete file: " + url, exc);				
				}
				
			}
		}
		
	}
	
}
