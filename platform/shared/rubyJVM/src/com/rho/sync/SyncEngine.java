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

import com.rho.IRhoRubyHelper;
import com.rho.RhoClassFactory;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.db.*;
import com.rho.net.*;
import com.rho.*;

import java.util.Vector;
import java.util.Hashtable;

import org.json.me.JSONException;

class SyncEngine implements NetRequest.IRhoSession
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("Sync");

    class SyncNotification
    {
        String m_strUrl, m_strParams;
        SyncNotification(String strUrl, String strParams){ m_strUrl = strUrl; m_strParams = strParams; }
    };

    public static final int esNone = 0, esSyncAllSources = 1, esSyncSource = 2, esStop = 3, esExit = 4;

    public static final int DEFAULT_SYNC_PORT = 100;
    static String SYNC_SOURCE_FORMAT() { return "?format=json"; }
    static String SYNC_ASK_ACTION() { return "/ask"; }
    static String SYNC_PAGE_SIZE() { return "200"; }
	
    Vector/*<SyncSource*>*/ m_sources = new Vector();
    DBAdapter   m_dbAdapter;
    NetRequest m_NetRequest;
    int         m_syncState;
    String     m_clientID = "";
    Hashtable/*<int,SyncNotification>*/ m_mapNotifications = new Hashtable();
    Mutex m_mxNotifications = new Mutex();
    String m_strSession = "";

    ISyncStatusListener m_statusListener = null;
    
    public void setStatusListener(ISyncStatusListener listener) { m_statusListener = listener; }
    private void reportStatus(String status, int error) {
    	if (m_statusListener != null) {
    		m_statusListener.reportStatus(status, error);
    	}
    	LOG.INFO(status);
    }
    
    void setState(int eState){ m_syncState = eState; }
    int getState(){ return m_syncState; }
    boolean isContinueSync(){ return m_syncState != esExit && m_syncState != esStop; }
	boolean isSyncing(){ return m_syncState == esSyncAllSources || m_syncState == esSyncSource; }
    void stopSync(){ if (isContinueSync()){ setState(esStop); getNet().cancelAll();} }
    void exitSync(){ setState(esExit); getNet().cancelAll(); }
    String getClientID(){ return m_clientID; }
    void setSession(String strSession){m_strSession=strSession;}
    public String getSession(){ return m_strSession; }
    boolean isSessionExist(){ return m_strSession != null && m_strSession.length() > 0; }
    DBAdapter getDB(){ return m_dbAdapter; }
    NetRequest getNet(){ return m_NetRequest;}
    
    SyncEngine(DBAdapter db){
    	m_dbAdapter = db;
		m_dbAdapter.setDbCallback(new DBCallback());

    	m_NetRequest = null; 
    	m_syncState = esNone; 
    }

    void setFactory(RhoClassFactory factory)throws Exception{ 
        m_NetRequest = factory.createNetRequest();
    }
    
	void doSyncAllSources()
	{
		String status_report = "Sync complete.";
	    int error = 0;
	    
	    setState(esSyncAllSources);

	    try
	    {
		    loadAllSources();
		
		    m_strSession = loadSession();
		    if ( isSessionExist()  ) {
		        loadClientID();
		    } else {
		    	status_report = "Client is not logged in. No sync will be performed.";
		    	error = 1;
		    }
		    
		    syncAllSources();
	    }catch(Exception exc)
	    {
	    	LOG.ERROR("Sync failed.", exc);
	    	status_report = "Sync failed.";
	    }
	    
	    setState(esNone);
		
	    reportStatus( status_report, error );
	}

	void doSyncSource(int nSrcId)
	{
		String status_report = null;
		int error = 0;
		
		LOG.INFO( "Started synchronization of the data source #" + nSrcId );
		
	    setState(esSyncSource);
        SyncSource src = null;

	    try
	    {
		    loadAllSources();
		
		    m_strSession = loadSession();
		    if ( isSessionExist()  ) {
		        loadClientID();
		    } else {
		    	status_report = "Client is not logged in. No sync will be performed.";
		    	error = 1;
		    }
		    
		    src = findSourceByID(nSrcId);
	        if ( src != null )
	        {
		        if ( isSessionExist() && getState() != esStop )
		            src.sync(m_statusListener);
		
		        fireNotification(src, true);
	        } else {
	        	throw new RuntimeException("Sync one source : Unknown Source ID: " + nSrcId );
	        }
	    } catch(Exception exc) {
	    	LOG.ERROR("Sync source: " + nSrcId + " failed.", exc);
	    	status_report = "Sync failed for " + src.getName() + ".";
	    }
        
	    setState(esNone);
		
	    if(status_report != null) {
	    	reportStatus(status_report, error);
	    }
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
	
	void loadAllSources()throws DBException
	{
	    m_sources.removeAllElements();
	    IDBResult res = getDB().executeSQL("SELECT source_id,source_url,token,name from sources order by source_id");
	
	    for ( ; !res.isEnd(); res.next() )
	    { 
	        String strUrl = res.getStringByIdx(1);
	        String name = res.getStringByIdx(3);
	        if ( strUrl.length() > 0 )
	            m_sources.addElement( new SyncSource( res.getIntByIdx(0), strUrl, name, res.getUInt64ByIdx(2), this) );
	    }
	}

	void loadClientID()throws Exception
	{
	    m_clientID = "";
	    boolean bResetClient = false;
	    {
	        IDBResult res = getDB().executeSQL("SELECT client_id,reset from client_info");
	        if ( !res.isEnd() )
	        {
	            m_clientID = res.getStringByIdx(0);
	            bResetClient = res.getIntByIdx(1) > 0;
	        }
	    }
	
	    if ( m_clientID.length() == 0 )
	    {
	        m_clientID = requestClientIDByNet();
	
	        getDB().executeSQL("DELETE FROM client_info");
	        getDB().executeSQL("INSERT INTO client_info (client_id) values (?)", m_clientID);
	    }else if ( bResetClient )
	    {
	    	if ( !resetClientIDByNet() )
	    		stopSync();
	    	else
	    		getDB().executeSQL("UPDATE client_info SET reset=? where client_id=?", new Integer(0), m_clientID );	    	
	    }
	    	
	}

	boolean resetClientIDByNet()throws Exception
	{
	    if ( m_sources.size() == 0 )
	        return true;
	
	    SyncSource src = (SyncSource)m_sources.elementAt(0);
	    String strUrl = src.getUrl() + "/clientreset";
	    String strQuery = "?client_id=" + getClientID();
/*	    String strBody = "";
	    try{
			IRhoRubyHelper helper = RhoClassFactory.createRhoRubyHelper();
			strBody += "&device_pin=" + helper.getDeviceId() + "&device_type=" + helper.getPlatform();
	    }catch(Exception e)
		{
			LOG.ERROR("getDeviceId or getPlatform failed", e);
		}
	    
	    int port = RhoConf.getInstance().getInt("push_port");
	    strBody += "&device_port=" + (port > 0 ? port : DEFAULT_SYNC_PORT);*/
	    
	    String szData = getNet().pullData(strUrl+strQuery, "", this).getCharData();
	    return szData != null;
	}
	
	String requestClientIDByNet()throws Exception
	{
	    if ( m_sources.size() == 0 )
	        return "";
	
	    SyncSource src = (SyncSource)m_sources.elementAt(0);
	    String strUrl = src.getUrl() + "/clientcreate";
	    String strQuery = SYNC_SOURCE_FORMAT();
	    /*String strBody = "";
	    try{
			IRhoRubyHelper helper = RhoClassFactory.createRhoRubyHelper();
			strBody += "&device_pin=" + helper.getDeviceId() + "&device_type=" + helper.getPlatform();
	    }catch(Exception e)
		{
			LOG.ERROR("getDeviceId or getPlatform failed", e);
		}
	    
	    int port = RhoConf.getInstance().getInt("push_port");
	    strBody += "&device_port=" + (port > 0 ? port : DEFAULT_SYNC_PORT);*/
	    
	    String szData = getNet().pullData(strUrl+strQuery, "", this).getCharData();
	    if ( szData != null )
	    {
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
	        SyncSource src = (SyncSource)m_sources.elementAt(i);
	        if ( isSessionExist() && getState() != esStop )
	            src.sync(m_statusListener);
	
	        fireNotification(src, true);
	    }
	}

	boolean login(String name, String password)throws Exception
	{
	    loadAllSources();
	    return doLogin(name,password);
	}

	boolean doLogin(String name, String password)throws Exception
	{
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
	
	    String strBody = "login=" + name + "&password=" + password + "&remember_me=1";

/*	    try{
			IRhoRubyHelper helper = RhoClassFactory.createRhoRubyHelper();
			strBody += "&device_pin=" + helper.getDeviceId() + "&device_type=" + helper.getPlatform();
	    }catch(Exception e)
		{
			LOG.ERROR("getDeviceId or getPlatform failed", e);
		}
		
	    int port = RhoConf.getInstance().getInt("push_port");
	    strBody += "&device_port=" + (port > 0 ? port : DEFAULT_SYNC_PORT);*/
	    
	    String strSession = getNet().pullCookies( src0.getUrl()+"/client_login", strBody, this);
	    if ( strSession == null || strSession.length() == 0 )
	        return false;
	
	    getDB().executeSQL( "UPDATE sources SET session=?", strSession );
	
	    return true;
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
	    getNet().deleteCookie("");
	
	    loadAllSources();
	    for( int i = 0; i < m_sources.size(); i++ )
	    {
	        SyncSource src = (SyncSource)m_sources.elementAt(i);
	        getNet().deleteCookie(src.getUrl());
	    }
	
	}

	void resetSyncDB()throws DBException
	{
	    getDB().executeSQL( "DELETE from object_values" );
	    getDB().executeSQL( "DELETE from client_info" );
	    getDB().executeSQL( "UPDATE sources SET token=?", "" );
	    //getDB().executeSQL( "VACUUM" );
	
	    m_clientID = "";
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

	void fireNotification( SyncSource src, boolean bFinish)throws Exception
	{
	    String strBody = "", strUrl;
	    {
	    	synchronized(m_mxNotifications){
		        SyncNotification sn = (SyncNotification)m_mapNotifications.get(src.getID());
		        if ( sn == null )
		            return;
		
		        strUrl = sn.m_strUrl;
		        strBody += "total_count=" + src.getTotalCount();
		        strBody += "&processed_count=" + src.getCurPageCount();
		        
		        strBody = "&status=";
		        if ( bFinish )
		        	strBody += (src.getServerObjectsCount() > 0 ?"ok":"error");
		        else
		        	strBody += "in_progress";
		        
		        if ( sn.m_strParams.length() > 0 )
		            strBody += "&" + sn.m_strParams;
	        }
	    }
		LOG.INFO( "Fire notification. Source ID: " + src.getID() + "; Url :" + strUrl + "; Body: " + strBody );
		
	    getNet().pushData( strUrl, strBody, this );
	
	    clearNotification(src.getID().intValue());
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
				String strUpdateType = rows2Delete.getString("update_type");
				if ( strUpdateType.equals("create") || strUpdateType.equals("update") )
					continue;

				String url = rows2Delete.getString("value");
				if ( url == null || url.length() == 0 )
					continue;
				
				try{
				    SimpleFile oFile = RhoClassFactory.createFile();
				    oFile.delete(url);
				}catch(Exception exc){
					LOG.ERROR("DBCallback.OnDeleteFromTable: Error delete file: " + url, exc);				
				}
				
			}
		}
		
	}
	
}
