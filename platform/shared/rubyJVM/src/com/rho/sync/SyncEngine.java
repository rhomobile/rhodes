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
import com.rho.TimeInterval;
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
	
    public static final int esNone = 0, esSyncAllSources = 1, esSyncSource = 2, esSearch=3, esStop = 4, esExit = 5;

    static class SourceID
    {
        String m_strName = "";
        int m_nID;

        SourceID(int id, String strName ){ m_nID = id; m_strName = strName; }
        SourceID(String strName ){ m_strName = strName; }
        
        public String toString()
        {
            if ( m_strName.length() > 0 )
                return "name : " + m_strName;

            return "# : " + m_nID;
        }
        boolean isEqual(SyncSource src)
        {
            if ( m_strName.length() > 0 )
                return src.getName().equals(m_strName);

            return m_nID == src.getID().intValue();
        }
    };
    
    Vector/*<SyncSource*>*/ m_sources = new Vector();
    Vector/*<String>*/      m_arPartitions = new Vector();    
    NetRequest m_NetRequest;
    ISyncProtocol m_SyncProtocol;
    int         m_syncState;
    String     m_clientID = "";
    Mutex m_mxLoadClientID = new Mutex();
    String m_strSession = "";
    SyncNotify m_oSyncNotify = new SyncNotify(this);
    boolean m_bStopByUser = false;
    int m_nSyncPageSize = 2000;
    boolean m_bNoThreaded = false;
    int m_nErrCode = RhoRuby.ERR_NONE;
    
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
    boolean isSessionExist(){ return m_strSession != null && m_strSession.length() > 0; }
    
    DBAdapter getUserDB(){ return DBAdapter.getUserDB(); }
    DBAdapter getDB(String strPartition){ return DBAdapter.getDB(strPartition); }
    
  //IRhoSession
    public String getSession(){ return m_strSession; }
    public String getContentType(){ return getProtocol().getContentType();}
    
    SyncNotify getNotify(){ return m_oSyncNotify; }
    NetRequest getNet() { return m_NetRequest;}
    ISyncProtocol getProtocol(){ return m_SyncProtocol; }
    
    SyncEngine(){
		m_NetRequest = null;
    	m_syncState = esNone;
    	
    	initProtocol();
    }

    void initProtocol()
    {
        m_SyncProtocol = new SyncProtocol_3();
    }
    
    int getSyncPageSize() { return m_nSyncPageSize; }
    void setSyncPageSize(int nPageSize){ m_nSyncPageSize = nPageSize; }
    
    void setFactory(RhoClassFactory factory)throws Exception{ 
		m_NetRequest = RhoClassFactory.createNetRequest();
    }
    
    void prepareSync(int eState)throws Exception
    {
        setState(eState);
        m_bStopByUser = false;
        m_nErrCode = RhoRuby.ERR_NONE;
        
        loadAllSources();

        m_strSession = loadSession();
        if ( isSessionExist()  )
        {
            m_clientID = loadClientID();
            if ( m_nErrCode == RhoRuby.ERR_NONE )
            {
                getNotify().cleanLastSyncObjectCount();
       	        doBulkSync();

                return;
            }
        }else
            m_nErrCode = RhoRuby.ERR_CLIENTISNOTLOGGEDIN;
        
    	if ( m_sources.size() > 0 )
        {		    	
	    	SyncSource src = (SyncSource)m_sources.elementAt(getStartSource());
	    	src.m_nErrCode = m_nErrCode;
	    	
	    	getNotify().fireSyncNotification(src, true, src.m_nErrCode, "");
        }else
        	getNotify().fireSyncNotification(null, true, m_nErrCode, "");

        stopSync();
    }
    
    void doSyncAllSources()
    {
	    try
	    {
	        prepareSync(esSyncAllSources);
	
	        if ( isContinueSync() )
	        {
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
	        }
	
	        getNotify().cleanCreateObjectErrors();
	    }catch(Exception exc)
	    {
	    	LOG.ERROR("Sync failed.", exc);
	    }

        if ( getState() != esExit )
            setState(esNone);
    }

    void doSearch(Vector/*<rho::String>*/ arSources, String strParams, String strAction, boolean bSearchSyncChanges, int nProgressStep)
    {
	    try
	    {
		    prepareSync(esSearch);
		    if ( !isContinueSync() )
		    {
		        if ( getState() != esExit )
		            setState(esNone);
		
		        return;
		    }
		
		    TimeInterval startTime = TimeInterval.getCurrentTime();
		
		    if ( bSearchSyncChanges )
		    {
		        for ( int i = 0; i < (int)arSources.size(); i++ )
		        {
		            SyncSource pSrc = findSourceByName((String)arSources.elementAt(i));
		            if ( pSrc != null )
		                pSrc.syncClientChanges();
		        }
		    }
		
		    int nErrCode = 0;
		    while( isContinueSync() )
		    {
		        int nSearchCount = 0;
		        String strUrl = getProtocol().getServerQueryUrl(strAction);
		        String strQuery = getProtocol().getServerQueryBody("", getClientID(), getSyncPageSize());
		
		        if ( strParams.length() > 0 )
		            strQuery += strParams;
		
		        for ( int i = 0; i < (int)arSources.size(); i++ )
		        {
		            SyncSource pSrc = findSourceByName((String)arSources.elementAt(i));
		            if ( pSrc != null )
		            {
		                strQuery += "&source[][name]=" + pSrc.getName();
		
		                if ( !pSrc.isTokenFromDB() && pSrc.getToken() > 1 )
		                    strQuery += "&source[][token]=" + pSrc.getToken();
		            }
		        }
		
				LOG.INFO("Call search on server. Url: " + (strUrl+strQuery) );
		        NetResponse resp = getNet().pullData(strUrl+strQuery, this);
		
		        if ( !resp.isOK() )
		        {
		            stopSync();
		            nErrCode = RhoRuby.getErrorFromResponse(resp);
		            continue;
		        }
		
		        String szData = resp.getCharData();
		
		        JSONArrayIterator oJsonArr = new JSONArrayIterator(szData);
		
		        for( ; !oJsonArr.isEnd() && isContinueSync(); oJsonArr.next() )
		        {
		            JSONArrayIterator oSrcArr = oJsonArr.getCurArrayIter();//new JSONArrayIterator(oJsonArr.getCurItem());
		
		            int nVersion = 0;
		            if ( !oSrcArr.isEnd() && oSrcArr.getCurItem().hasName("version") )
		            {
		                nVersion = oSrcArr.getCurItem().getInt("version");
		                oJsonArr.next();
		            }
		
		            if ( nVersion != getProtocol().getVersion() )
		            {
		                LOG.ERROR( "Sync server send search data with incompatible version. Client version: " + getProtocol().getVersion() +
		                    "; Server response version: " + nVersion );
		                stopSync();
		                nErrCode = RhoRuby.ERR_UNEXPECTEDSERVERRESPONSE;
		                continue;
		            }
		
		            if ( !oSrcArr.getCurItem().hasName("source") )
		            {
		                LOG.ERROR( "Sync server send search data without source name." );
		                stopSync();
		                nErrCode = RhoRuby.ERR_UNEXPECTEDSERVERRESPONSE;
		                continue;
		            }
		
		            String strSrcName = oSrcArr.getCurItem().getString("source");
		            SyncSource pSrc = findSourceByName(strSrcName);
		            if ( pSrc == null )
		            {
		                LOG.ERROR("Sync server send search data for unknown source name:" + strSrcName);
		                stopSync();
		                nErrCode = RhoRuby.ERR_UNEXPECTEDSERVERRESPONSE;
		                continue;
		            }
		
		            oSrcArr.reset(0);
		            pSrc.m_bIsSearch = true;
		            pSrc.setProgressStep(nProgressStep);
		            pSrc.processServerResponse_ver3(oSrcArr);
		
		            nSearchCount += pSrc.getCurPageCount();
		        }
		
		        if ( nSearchCount == 0 )
		            break;
		    }  
		
		    if ( isContinueSync() )
		    {
		        SyncSource src = (SyncSource)m_sources.elementAt(getStartSource());
		        src.m_bIsSearch = true;
		
		    	getNotify().fireSyncNotification(src, true, RhoRuby.ERR_NONE, RhoRuby.getMessageText("sync_completed"));
		    }
		    else if ( nErrCode != 0 )
		    {
		        SyncSource src = (SyncSource)m_sources.elementAt(getStartSource());
		        src.m_nErrCode = nErrCode;
		        src.m_bIsSearch = true;
		        getNotify().fireSyncNotification(src, true, src.m_nErrCode, "");
		    }
		
		    //update db info
		    TimeInterval endTime = TimeInterval.getCurrentTime();
		    //unsigned long timeUpdated = CLocalTime().toULong();
		    for ( int i = 0; i < (int)arSources.size(); i++ )
		    {
		        SyncSource oSrc = findSourceByName((String)arSources.elementAt(i));
		        if ( oSrc == null )
		            continue;
		        oSrc.getDB().executeSQL("UPDATE sources set last_updated=?,last_inserted_size=?,last_deleted_size=?, "+
					 "last_sync_duration=?,last_sync_success=?, backend_refresh_time=? WHERE source_id=?", 
					 new Long(endTime.toULong()/1000), new Integer(oSrc.getInsertedCount()), new Integer(oSrc.getDeletedCount()), 
					 new Long((endTime.minus(startTime)).toULong()), new Integer(oSrc.getGetAtLeastOnePage()?1:0), 
					 new Integer(oSrc.getRefreshTime()),  oSrc.getID() );
		    }
		    //
		
		    getNotify().cleanCreateObjectErrors();
		    if ( getState() != esExit )
		        setState(esNone);

	    } catch(Exception exc) {
    		LOG.ERROR("Search failed.", exc);

	        SyncSource src = (SyncSource)m_sources.elementAt(getStartSource());
	        src.m_bIsSearch = true;
    		src.m_nErrCode = RhoRuby.ERR_RUNTIME;
	    	
	    	getNotify().fireSyncNotification(src, true, src.m_nErrCode, "" ); 
	    }
		    
    }

    void doSyncSource(SourceID oSrcID)
    {
        SyncSource src = null;

	    try
	    {
	        prepareSync(esSyncSource);
	
	        if ( isContinueSync() )
	        {
	        	src = findSource(oSrcID);
	            if ( src != null )
	            {
		            LOG.INFO("Started synchronization of the data source: " + src.getName() );
	
	                src.sync();
	
				    getNotify().fireSyncNotification(src, true, src.m_nErrCode, src.m_nErrCode == RhoRuby.ERR_NONE ? RhoRuby.getMessageText("sync_completed") : "");
	            }else
	            {
//                    LOG.ERROR( "Sync one source : Unknown Source " + oSrcID.toString() );
	            
		        	src = new SyncSource(this, getUserDB());
			    	//src.m_strError = "Unknown sync source.";
			    	src.m_nErrCode = RhoRuby.ERR_RUNTIME;
		        	
	    	    	throw new RuntimeException("Sync one source : Unknown Source " + oSrcID.toString() );
	            }
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
		return findSource(new SourceID(strSrcName));		
	}
	
	void loadAllSources()throws DBException
	{
	    m_sources.removeAllElements();
	    
	    IDBResult res = getUserDB().executeSQL("SELECT source_id,sync_type,name, partition from sources ORDER BY sync_priority");
	    for ( ; !res.isEnd(); res.next() )
	    { 
	        String strShouldSync = res.getStringByIdx(1);
	        if ( strShouldSync.compareTo("none") == 0)
	            continue;

	        String strName = res.getStringByIdx(2);
	        String strPartition = res.getStringByIdx(3);
	        
	        if ( m_arPartitions.indexOf(strPartition) < 0 )
	        	m_arPartitions.addElement(strPartition);
	        
	        m_sources.addElement( new SyncSource( res.getIntByIdx(0), strName, strShouldSync, getDB(strPartition), this) );
	    }
	}

	public String readClientID()throws Exception
	{
	    String clientID = "";
		
		synchronized( m_mxLoadClientID )
		{
	        IDBResult res = getUserDB().executeSQL("SELECT client_id,reset from client_info");
	        if ( !res.isEnd() )
	            clientID = res.getStringByIdx(0);
		}
		
		return clientID;
	}
	
	public String loadClientID()throws Exception
	{
	    String clientID = "";
		
		synchronized( m_mxLoadClientID )
		{
		    boolean bResetClient = false;
		    {
		        IDBResult res = getUserDB().executeSQL("SELECT client_id,reset from client_info");
		        if ( !res.isEnd() )
		        {
		            clientID = res.getStringByIdx(0);
		            bResetClient = res.getIntByIdx(1) > 0;
		        }
		    }
		    
		    if ( clientID.length() == 0 )
		    {
		        clientID = requestClientIDByNet();
		
	            IDBResult res = getUserDB().executeSQL("SELECT * FROM client_info");
	            if ( !res.isEnd() )
	            	getUserDB().executeSQL("UPDATE client_info SET client_id=?", clientID);
	            else
	            	getUserDB().executeSQL("INSERT INTO client_info (client_id) values (?)", clientID);
	            
		    	if ( ClientRegister.getInstance() != null )
		    		ClientRegister.getInstance().startUp();	    	
	            
		    }else if ( bResetClient )
		    {
		    	if ( !resetClientIDByNet(clientID) )
		    		stopSync();
		    	else
		    		getUserDB().executeSQL("UPDATE client_info SET reset=? where client_id=?", new Integer(0), clientID );	    	
		    }
		}
		
		return clientID;
	}

	void processServerSources(String strSources)throws Exception
	{
	    if ( strSources.length() > 0 )
	    {
	        NetResponse resp = getNet().pushData( getNet().resolveUrl("/system/loadserversources"), strSources, null);
	        loadAllSources();
	        
	        DBAdapter.initAttrManager();
	    }
	}
	
	boolean resetClientIDByNet(String strClientID)throws Exception
	{
        String strBody = "";
        //TODO: send client register info in client reset 
//        if ( ClientRegister.getInstance() != null )
//            strBody += ClientRegister.getInstance().getRegisterBody();

	    NetResponse resp = getNet().pullData(getProtocol().getClientResetUrl(strClientID), this);
/*	    
	    processServerSources("{\"server_sources\":[{\"name\":\"Product\",\"partition\":\"application\",\"source_id\":\"2\",\"sync_priority\":\"0\","+
	    	    "\"schema_version\":\"7.0\",\"schema\":{"+
	    	    "\"columns\":[\'brand\',\'created_at\',\'name\',\'price\',\'quantity\',\'sku\',\'updated_at\']"+
	    	    "}}]}"); 
*/	    				
	    
	    if ( !resp.isOK() )
	    	m_nErrCode = RhoRuby.getErrorFromResponse(resp);
	    /*else
    	{
    		processServerSources(resp.getCharData());
    	}*/
	    
	    return resp.isOK();
	}

	String requestClientIDByNet()throws Exception
	{
        String strBody = "";
        //TODO: send client register info in client create 
//        if ( ClientRegister.getInstance() != null )
//            strBody += ClientRegister.getInstance().getRegisterBody();
	
	    NetResponse resp = getNet().pullData(getProtocol().getClientCreateUrl(), this);
	    if ( resp.isOK() && resp.getCharData() != null )
	    {
	    	String szData = resp.getCharData();
	    	
	        JSONEntry oJsonEntry = new JSONEntry(szData);
	
	        //if (oJsonEntry.hasName("sources") )
	        //    processServerSources(szData);
	        
	        JSONEntry oJsonObject = oJsonEntry.getEntry("client");
	        if ( !oJsonObject.isEmpty() )
	            return oJsonObject.getString("client_id");
	    }else
	    {
	    	m_nErrCode = RhoRuby.getErrorFromResponse(resp);
	    	if ( m_nErrCode == RhoRuby.ERR_NONE )
	    		m_nErrCode = RhoRuby.ERR_UNEXPECTEDSERVERRESPONSE;
	    }
	
	    return "";
	}

	void doBulkSync()throws Exception
	{
//	    processServerSources("{\"partition\":\"" + "application" + "\"}");

	    if ( !RhoConf.getInstance().isExist("bulksync_state") )
	        return;
		
	    int nBulkSyncState = RhoConf.getInstance().getInt("bulksync_state");;
	    if ( nBulkSyncState >= 1 || !isContinueSync() )
	        return;

		LOG.INFO("Bulk sync: start");
		getNotify().fireBulkSyncNotification(false, "start", "", RhoRuby.ERR_NONE);
		
	    for (int i = 0; i < (int)m_arPartitions.size() && isContinueSync(); i++)
	        loadBulkPartition( (String)m_arPartitions.elementAt(i));

	    if (isContinueSync())
	    {
	    	RhoConf.getInstance().setInt("bulksync_state", 1, true);
	        getNotify().fireBulkSyncNotification(true, "", "", RhoRuby.ERR_NONE);
	    }
	}

	void loadBulkPartition(String strPartition )throws Exception
	{
		DBAdapter dbPartition = getDB(strPartition); 		
	    String serverUrl = RhoConf.getInstance().getPath("syncserver");
	    String strUrl = serverUrl + "bulk_data";
	    String strQuery = "?client_id=" + m_clientID + "&partition=" + strPartition;
	    String strDataUrl = "", strCmd = "";

	    getNotify().fireBulkSyncNotification(false, "start", strPartition, RhoRuby.ERR_NONE);
	    
	    while(strCmd.length() == 0&&isContinueSync())
	    {	    
	        NetResponse resp = getNet().pullData(strUrl+strQuery, this);
	        if ( !resp.isOK() || resp.getCharData() == null )
	        {
	    	    LOG.ERROR( "Bulk sync failed: server return an error." );
	    	    stopSync();
	    	    getNotify().fireBulkSyncNotification(true, "", strPartition, RhoRuby.ERR_REMOTESERVER);
	    	    return;
	        }

		    LOG.INFO("Bulk sync: got response from server: " + resp.getCharData() );
	    	
	        String szData = resp.getCharData();
	        JSONEntry oJsonEntry = new JSONEntry(szData);
	        strCmd = oJsonEntry.getString("result");
	        if ( oJsonEntry.hasName("url") )
	   	        strDataUrl = oJsonEntry.getString("url");
	        
	        if ( strCmd.compareTo("wait") == 0)
	        {
	            int nTimeout = RhoConf.getInstance().getInt("bulksync_timeout_sec");
	            if ( nTimeout == 0 )
	                nTimeout = 5;

	            SyncThread.getInstance().wait(nTimeout);
	            strCmd = "";
	        }
	    }

	    if ( strCmd.compareTo("nop") == 0)
	    {
		    LOG.INFO("Bulk sync return no data.");
		    getNotify().fireBulkSyncNotification(true, "", strPartition, RhoRuby.ERR_NONE);		    
		    return;
	    }

        if ( !isContinueSync() )
            return;

	    getNotify().fireBulkSyncNotification(false, "download", strPartition, RhoRuby.ERR_NONE);
	    
	    String fDataName = makeBulkDataFileName(strDataUrl, dbPartition.getDBPath(), "_bulk.data");
	    String strHsqlDataUrl = FilePath.join(getHostFromUrl(serverUrl), strDataUrl) + ".hsqldb.data";
	    LOG.INFO("Bulk sync: download data from server: " + strHsqlDataUrl);
	    {
		    NetResponse resp1 = getNet().pullFile(strHsqlDataUrl, fDataName, this, null);
		    if ( !resp1.isOK() )
		    {
			    LOG.ERROR("Bulk sync failed: cannot download database file: " + resp1.getRespCode() );
			    stopSync();
			    getNotify().fireBulkSyncNotification(true, "", strPartition, RhoRuby.ERR_REMOTESERVER);
			    return;
		    }
	    }
	    
        if ( !isContinueSync() )
            return;
	    
	    String fScriptName = makeBulkDataFileName(strDataUrl, dbPartition.getDBPath(), "_bulk.script" );
	    String strHsqlScriptUrl = FilePath.join(getHostFromUrl(serverUrl), strDataUrl) + ".hsqldb.script";
	    LOG.INFO("Bulk sync: download script from server: " + strHsqlScriptUrl);
	    {
		    NetResponse resp1 = getNet().pullFile(strHsqlScriptUrl, fScriptName, this, null);
		    if ( !resp1.isOK() )
		    {
			    LOG.ERROR("Bulk sync failed: cannot download database file.");
			    stopSync();
			    getNotify().fireBulkSyncNotification(true, "", strPartition, RhoRuby.ERR_REMOTESERVER);
			    return;
		    }
	    }
	    
		LOG.INFO("Bulk sync: start change db");
		getNotify().fireBulkSyncNotification(false, "change_db", strPartition, RhoRuby.ERR_NONE);
		
	    dbPartition.setBulkSyncDB(fDataName, fScriptName);
	    processServerSources("{\"partition\":\"" + strPartition + "\"}");
	    
		LOG.INFO("Bulk sync: end change db");
		getNotify().fireBulkSyncNotification(false, "", strPartition, RhoRuby.ERR_NONE);
	}
	
	String makeBulkDataFileName(String strDataUrl, String strDbPath, String strExt)throws Exception
	{
	    FilePath oUrlPath = new FilePath(strDataUrl);
	    String strNewName = oUrlPath.getBaseName();
	    String strOldName = RhoConf.getInstance().getString("bulksync_filename");
	    if ( strOldName.length() > 0 && strNewName.compareTo(strOldName) != 0 )
	    {
	        FilePath oFilePath = new FilePath(strDbPath);
	        String strFToDelete = oFilePath.changeBaseName(strOldName+strExt);
	        LOG.INFO( "Bulk sync: remove old bulk file '" + strFToDelete + "'" );

	        //RhoFile.deleteFile( strFToDelete.c_str() );
	        RhoClassFactory.createFile().delete(strFToDelete);	        
	    }

	    RhoConf.getInstance().setString("bulksync_filename", strNewName, true);

	    FilePath oFilePath = new FilePath(strDbPath);
	    return oFilePath.changeBaseName(strNewName+strExt);
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
		//TODO: do not stop on error source
		boolean bError = false;
	    for( int i = getStartSource(); i < m_sources.size() && isContinueSync(); i++ )
	    {
	    	SyncSource src = null;
	    	try{
		        src = (SyncSource)m_sources.elementAt(i);
		        
		        if ( src.getSyncType().compareTo("bulk_sync_only")==0 )
		            continue;
		        
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
	
	void login(String name, String password, String callback)
	{
		try {
/*			
			processServerSources("{\"sources\":{ \"ProductEx\":{ "+
	        "\"sync_type\":\"incremental\", \"partition\":\"application\", \"source_id\":\"7\","+
	        " \"sync_priority\":\"0\", \"model_type\":\"fixed_schema\", "+
	        " \"schema\":{\"version\":\"1.1\", \"property\":{\"brand\":\"string\", \"price\":\"string\", \"quantity\":\"string\", \"name\":\"string\", "+
	        " \"image_url\":\"blob\", \"image_url_ex\":\"blob,overwrite\"}, "+
	        " \"index\":[{\"by_brand_price1\":\"brand,price\"}, {\"by_quantity1\":\"quantity\"}], \"unique_index\":[{\"by_name1\":\"name\"}]}, "+
	        " \"belongs_to\":{\"brand\":\"Customer\"}}}}");//, \"schema_version\":\"1.0\"
	        */
		    NetResponse resp = null;
		    
		    try{
				
			    resp = getNet().pullCookies( getProtocol().getLoginUrl(), getProtocol().getLoginBody(name, password), this );
			    int nErrCode = RhoRuby.getErrorFromResponse(resp);
			    if ( nErrCode != RhoRuby.ERR_NONE )
			    {
			        getNotify().callLoginCallback(callback, nErrCode, resp.getCharData());
			        return;
			    }
		    }catch(IOException exc)
		    {
				LOG.ERROR("Login failed.", exc);
		    	getNotify().callLoginCallback(callback, RhoRuby.getNetErrorCode(exc), "" );
		    	return;
		    }
		    
		    String strSession = resp.getCharData();
		    if ( strSession == null || strSession.length() == 0 )
		    {
		    	LOG.ERROR("Return empty session.");
		    	getNotify().callLoginCallback(callback, RhoRuby.ERR_UNEXPECTEDSERVERRESPONSE, "" );
		        return;
		    }
		    
		    IDBResult res = getUserDB().executeSQL("SELECT * FROM client_info");
		    if ( !res.isEnd() )
		    	getUserDB().executeSQL( "UPDATE client_info SET session=?", strSession );
		    else
		    	getUserDB().executeSQL("INSERT INTO client_info (session) values (?)", strSession);
		
	    	getNotify().callLoginCallback(callback, RhoRuby.ERR_NONE, "" );
		    
	    	if ( ClientRegister.getInstance() != null )
	    		ClientRegister.getInstance().startUp();	    	
	    	
		}catch(Exception exc)
		{
			LOG.ERROR("Login failed.", exc);
	    	getNotify().callLoginCallback(callback, RhoRuby.ERR_RUNTIME, "" );
		}
	}

	boolean isLoggedIn()throws DBException
	{
	    String strRes = "";
	    IDBResult res = getUserDB().executeSQL("SELECT session FROM client_info");
	    
	    if ( !res.isEnd() )
	    	strRes = res.getStringByIdx(0);
	    
	    return strRes.length() > 0;
	}

	String loadSession()throws DBException
	{
		m_strSession = "";
	    IDBResult res = getUserDB().executeSQL("SELECT session FROM client_info");
	    
	    if ( !res.isEnd() )
	    	m_strSession = res.getStringByIdx(0);
	    
	    return m_strSession;
	}
	
	public void logout()throws Exception
	{
		getUserDB().executeSQL( "UPDATE client_info SET session = NULL");
	    m_strSession = "";
	
	    loadAllSources();
	}
	
	public void setSyncServer(String url)throws Exception
	{
		RhoConf.getInstance().setPropertyByName("syncserver", url);
		RhoConf.getInstance().saveToFile();
		RhoConf.getInstance().loadConf();
		
		getUserDB().executeSQL("DELETE FROM client_info");
		
		logout();
	}
	
	static String getHostFromUrl( String strUrl )
	{
		URI uri = new URI(strUrl);
		return uri.getHostSpecificPart() + "/";
	}
	
}
