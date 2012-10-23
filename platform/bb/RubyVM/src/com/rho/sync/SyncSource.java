/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

package com.rho.sync;

import com.rho.RhoClassFactory;
import com.rho.RhoConf;
import com.rho.RhoEmptyLogger;
import com.rho.RhoEmptyProfiler;
import com.rho.RhoLogger;
import com.rho.RhoProfiler;
import com.rho.RhoRuby;
import com.rho.RhodesApp;
import com.rho.Tokenizer;
import com.rho.net.*;
import com.rho.db.*;

import java.util.Enumeration;
import java.util.Vector;
import java.util.Hashtable;

import com.rho.FilePath;
import com.rho.TimeInterval;
import com.rho.RhoAppAdapter;
import com.rho.net.NetRequest.MultipartItem;

public class SyncSource
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("Sync");
	
	private static final RhoProfiler PROF = RhoProfiler.RHO_STRIP_PROFILER ? new RhoEmptyProfiler() : 
		new RhoProfiler();
	
	RhoConf RHOCONF(){ return RhoConf.getInstance(); }
	
	static class CAttrValue
	{
		String m_strAttrib = "";
	    String m_strValue = "";
	    String m_strBlobSuffix = "";

	    CAttrValue(String strAttrib, String strValue)
	    {
	        m_strAttrib = strAttrib;
	        m_strValue = strValue;

	    	if ( m_strAttrib.endsWith("-rhoblob")  )
	        {
	    		m_strBlobSuffix = "-rhoblob";
	            m_strAttrib = m_strAttrib.substring(0,m_strAttrib.length()-m_strBlobSuffix.length());
	        }
	    }
	};
	
    SyncEngine m_syncEngine;
    DBAdapter  m_dbAdapter;
    
    Integer  m_nID;
    String m_strName = "";
    long m_token = 0;
    String m_strSyncType = "";
    boolean m_bTokenFromDB; 
    
    int m_nCurPageCount, m_nInserted, m_nDeleted, m_nTotalCount;
    int m_nErrCode = RhoAppAdapter.ERR_NONE;
    String m_strError = "";
    String m_strQueryParams = "";
    
    int m_nRefreshTime = 0;
    int m_nProgressStep = 0;
    boolean m_bSchemaSource;
    
    static class CAssociation
    {
        String m_strSrcName, m_strAttrib;
        CAssociation( String strSrcName, String strAttrib ){m_strSrcName = strSrcName; m_strAttrib = strAttrib; }
    };
    
    Vector/*<CAssociation>*/ m_arAssociations = new Vector();
    Vector/*Ptr<net::CMultipartItem*>*/ m_arMultipartItems = new Vector();
    Vector/*<String>*/                  m_arBlobAttrs = new Vector();
    Hashtable/*<String,int>*/           m_hashIgnorePushObjects = new Hashtable();
    Hashtable/*<String,int>*/           m_hashBelongsTo = new Hashtable();
    
    Integer getID() { return m_nID; }
    String getName() { return m_strName; }
    String getSyncType(){ return m_strSyncType; }
    int getErrorCode(){ return m_nErrCode; }    
    int getServerObjectsCount(){ return m_nInserted+m_nDeleted; }
    
    long getToken(){ return m_token; }
    boolean isTokenFromDB(){ return m_bTokenFromDB; }    
    void setToken(long token){ m_token = token; m_bTokenFromDB = false; }
    boolean isEmptyToken()
    {
    	return m_token == 0;
    }
    
    int getProgressStep(){ return m_nProgressStep; }
    void setProgressStep(int nProgressStep){ m_nProgressStep = nProgressStep; }

    int getRefreshTime(){ return m_nRefreshTime; }
    Vector/*<CAssociation>*/ getAssociations(){ return m_arAssociations; }
    
    int getInsertedCount() { return m_nInserted; }
    int getDeletedCount() { return m_nDeleted; }
    void setCurPageCount(int nCurPageCount){m_nCurPageCount = nCurPageCount;}
    void setTotalCount(int nTotalCount){m_nTotalCount = nTotalCount;}
    int  getCurPageCount(){return m_nCurPageCount;}
    int  getTotalCount(){return m_nTotalCount;}

    SyncEngine getSync(){ return m_syncEngine; }
    SyncNotify getNotify(){ return getSync().getNotify(); }
	NetRequest getNet(){ return getSync().getNet(); }
	ISyncProtocol getProtocol(){ return getSync().getProtocol(); }
	void setRefreshTime( int nRefreshTime ){ m_nRefreshTime = nRefreshTime;}
	DBAdapter getDB(){ return m_dbAdapter; }

    SyncSource(SyncEngine syncEngine, DBAdapter db)throws DBException
    {
    	m_syncEngine = syncEngine;
    	m_dbAdapter = db;
    	
        m_nID = new Integer(0);
        
        m_bTokenFromDB = true;
        m_nProgressStep = 0;
        
        m_nCurPageCount = 0;
        m_nInserted = 0;
        m_nDeleted = 0;
        m_nTotalCount = 0;

        m_nErrCode = RhoAppAdapter.ERR_NONE;
        m_bSchemaSource = db.isTableExist(m_strName);
    }
	
    SyncSource(int id, String name, String strSyncType, DBAdapter db, SyncEngine syncEngine )throws DBException
    {
    	m_syncEngine = syncEngine;
    	m_dbAdapter = db;
        m_nID = new Integer(id);
        m_strName = name;
        m_strSyncType = strSyncType;
        m_nProgressStep = 0;
        
        m_nCurPageCount = 0;
        m_nInserted = 0;
        m_nDeleted = 0;
        m_nTotalCount = 0;

        m_nErrCode = RhoAppAdapter.ERR_NONE;

        IDBResult res = db.executeSQL("SELECT token,associations from sources WHERE source_id=?", m_nID);
        if ( !res.isEnd() )
        {
        	m_token = res.getLongByIdx(0);
            m_bTokenFromDB = true;
        }else
        {
        	m_token = 0;
            m_bTokenFromDB = true;
        }
        
        m_bSchemaSource = db.isTableExist(m_strName);
        parseAssociations(res.getStringByIdx(1));
    }
	
    void parseAssociations(String strAssociations)
    {
        if (strAssociations.length() == 0 )
            return;

        Tokenizer oTokenizer = new Tokenizer( strAssociations, "," );

        String strSrcName = "";
        while (oTokenizer.hasMoreTokens()) 
        {
    	    String tok = oTokenizer.nextToken();
    	    if (tok.length() == 0)
    		    continue;
            
            if ( strSrcName.length() > 0 )
            {
            	m_arAssociations.addElement( new CAssociation(strSrcName, tok) );
                strSrcName = "";
            }else
                strSrcName = tok;
        }
    }
    
	void sync() throws Exception
	{
    	getNotify().reportSyncStatus(RhoAppAdapter.getMessageText("syncronizing") + getName() + "...", m_nErrCode, m_strError );
    	
	    TimeInterval startTime = TimeInterval.getCurrentTime();
	    //m_bIsSearch = false;
	    
	    try{
	        //if ( isTokenFromDB() && getToken() > 1 )
	        //    syncServerChanges();  //sync only server changes, which was paused before
	        //else
	        //{
		        if ( isEmptyToken() )
		            processToken(1);

		        syncClientChanges();
		        syncServerChanges();
		        /*		        
		        boolean bSyncedServer = syncClientChanges();
		        if ( !bSyncedServer )
		        	syncServerChanges();
		        */
	        //}	        
	    }catch(Exception exc)
	    {
	    	//getSync().stopSync();
	    	throw exc;
	    }finally{
			TimeInterval endTime = TimeInterval.getCurrentTime();
			   
			int nSyncSucess = m_nErrCode == RhoAppAdapter.ERR_NONE ? 1 : 0;
			if ( nSyncSucess > 0 )
				getDB().executeSQL("UPDATE sources set last_updated=?,last_inserted_size=?,last_deleted_size=?, "+
						 "last_sync_duration=?,last_sync_success=?, backend_refresh_time=? WHERE source_id=?", 
			             new Long(endTime.toULong()/1000), new Integer(getInsertedCount()), new Integer(getDeletedCount()), 
			             new Long((endTime.minus(startTime)).toULong()), 
			             new Integer(nSyncSucess), new Integer(m_nRefreshTime), getID() );
			else
				getDB().executeSQL("UPDATE sources set last_inserted_size=?,last_deleted_size=?, "+
						 "last_sync_duration=?,last_sync_success=?, backend_refresh_time=? WHERE source_id=?", 
			             new Integer(getInsertedCount()), new Integer(getDeletedCount()), 
			             new Long((endTime.minus(startTime)).toULong()), 
			             new Integer(nSyncSucess), new Integer(m_nRefreshTime), getID() );
	    }
	}
	
	void syncClientChanges()throws Exception
	{
      	PROF.START("Pull");

      	boolean bSyncClient  = haveChangedValues();
      	
        if ( bSyncClient )
            doSyncClientChanges();

        PROF.STOP("Pull");
	}
	
	boolean haveChangedValues()throws Exception
	{
		IDBResult res = getDB().executeSQL("SELECT object FROM changed_values WHERE source_id=? and sent<=1 LIMIT 1 OFFSET 0", getID());
		return !res.isEnd();
	}	
/*
	boolean syncClientChanges()throws Exception
	{
	    boolean bSyncedServer = false;
	    if ( isPendingClientChanges() )
	    {
	    	LOG.INFO( "Client has unconfirmed created items. Call server to update them." );	    	    	
	        syncServerChanges();
	        bSyncedServer = true;
	    }

	    if ( bSyncedServer && isPendingClientChanges() )
	    {
            LOG.INFO( "Server does not sent created items. Stop sync." );	    	    	
	        getSync().setState(SyncEngine.esStop);
	    }
	    else
	    {   
	      	PROF.START("Pull");

	        boolean bSyncClient = false;
	        {
	            IDBResult res = getDB().executeSQL("SELECT object FROM changed_values WHERE source_id=? LIMIT 1 OFFSET 0", getID());
	            bSyncClient = !res.isEnd();
	        }
	        if ( bSyncClient )
	        {
	            doSyncClientChanges();
	            bSyncedServer = false;
	        }

	        PROF.STOP("Pull");
	    }

	    return bSyncedServer;
	}
	
	boolean isPendingClientChanges()throws DBException
	{
	    IDBResult res = getDB().executeSQL("SELECT object FROM changed_values WHERE source_id=? and update_type='create' and sent>1  LIMIT 1 OFFSET 0", getID());
	    return !res.isEnd();
	}*/

	void addBelongsTo(String strAttrib, Integer nSrcID)
	{
	    m_hashBelongsTo.put(strAttrib, nSrcID);
	}

	Integer getBelongsToSrcID(String strAttrib)
	{
	    if ( m_hashBelongsTo.containsKey(strAttrib) )
	        return (Integer)m_hashBelongsTo.get(strAttrib);

	    return new Integer(-1);
	}

	void checkIgnorePushObjects()throws Exception
	{
	    // ignore changes in pending creates
	    {
	        IDBResult res = getDB().executeSQL("SELECT distinct(object) FROM changed_values where source_id=? and sent>=2", getID() );
	        for( ; !res.isEnd(); res.next() )
	        {
	            String strObject = res.getStringByIdx(0);
	            m_hashIgnorePushObjects.put(strObject, new Integer(1));
	        }
	    }

	    //check for belongs_to
	    String strAttribQuests = "";
	    Vector/*<String>*/ arValues = new Vector();
	    arValues.addElement(getID());
    	Enumeration keys = m_hashBelongsTo.keys();
		while (keys.hasMoreElements()) 
		{
	        if ( strAttribQuests.length() > 0 )
	            strAttribQuests += ",";

	        strAttribQuests += "?";
	        arValues.addElement(keys.nextElement());
	    }

	    if ( strAttribQuests.length() > 0 )
	    {
	        IDBResult res = getDB().executeSQLEx( "SELECT object, attrib, value FROM changed_values where source_id=? and sent<=1 and attrib IN ( " + strAttribQuests + " )",  
	        		arValues );

	        for( ; !res.isEnd(); res.next() )
	        {
	            String strObject = res.getStringByIdx(0);
	            String strAttrib = res.getStringByIdx(1);
	            String strValue = res.getStringByIdx(2);

	            IDBResult res2 = getDB().executeSQL(
	                "SELECT object FROM changed_values where source_id=? and sent>=2 and object=? LIMIT 1 OFFSET 0", 
	                getBelongsToSrcID(strAttrib), strValue );
	            
	            if (!res2.isEnd())
	                m_hashIgnorePushObjects.put(strObject, new Integer(1) );

	        }
	    }
	}
	
	void doSyncClientChanges()throws Exception
	{
	    String arUpdateTypes[] = {"create", "update", "delete"};
	    boolean arUpdateSent[] = {false, false, false};

	    m_arMultipartItems.removeAllElements();
	    m_arBlobAttrs.removeAllElements();
	    String strBody = "{\"source_name\":" + JSONEntry.quoteValue(getName()) + ",\"client_id\":" + JSONEntry.quoteValue(getSync().getClientID());
	    boolean bSend = false;
	    int i = 0;
	    
	    getDB().Lock();
	    try{
		    getDB().updateAllAttribChanges();
		    checkIgnorePushObjects();
		    
		    if ( getSync().getSourceOptions().getBoolProperty(getID(), "full_update") )
		    	getDB().updateFullUpdateChanges( getID() );
		    
		    for( i = 0; i < 3; i++ )
		    {
		        String strBody1;
		        strBody1 = makePushBody_Ver3(arUpdateTypes[i], true);
		        if (strBody1.length() > 0)
		        {
		            strBody += "," + strBody1;
	
		            String strBlobAttrs = "";
		            for ( int j = 0; j < (int)m_arBlobAttrs.size(); j++)
		            {
		                if ( strBlobAttrs.length() > 0 )   
		                    strBlobAttrs += ",";
	
		                strBlobAttrs += JSONEntry.quoteValue((String)m_arBlobAttrs.elementAt(j));
		            }
	
		            if ( strBlobAttrs.length() > 0 )
		                strBody += ",\"blob_fields\":[" + strBlobAttrs + "]";
	
		            arUpdateSent[i] = true;
		            bSend = true;
		        }
		    }
		    
		    if (!bSend)
		    {
		        //check for push_changes
		        IDBResult res = getDB().executeSQL("SELECT attrib_type FROM changed_values where source_id=? and update_type =?", getID(), "push_changes" );
		        bSend = !res.isEnd();
		    }
		    
		    strBody += "}";
	    }finally
	    {
	    	getDB().Unlock();
	    }
	    
	    if ( bSend && getSync().isContinueSync() )
	    {
	        LOG.INFO( "Push client changes to server. Source: " + getName() + "Size :" + strBody.length() );
	        if ( !RHOCONF().getBool("log_skip_post") )	        
	        	LOG.TRACE("Push body: " + strBody);		

	        if ( getSync().getSourceOptions().getBoolProperty(getID(), "set_sync_push_body" ) )
	            getSync().getSourceOptions().setProperty(getID(), "sync_push_body", strBody );
	        
	        try{
	            if ( getSync().getSourceOptions().getIntProperty(getID(), "sync_push_error" ) > 0 )
	            {
	                m_nErrCode = getSync().getSourceOptions().getIntProperty(getID(), "sync_push_error" );
	                m_strError = getSync().getSourceOptions().getProperty(getID(), "sync_push_error_text" );
	            }else
	            {
			        if ( m_arMultipartItems.size() > 0 )
			        {
			            MultipartItem oItem = new MultipartItem();
			            oItem.m_strBody = strBody;
			            //oItem.m_strContentType = getProtocol().getContentType();
			            oItem.m_strName = "cud";
			            m_arMultipartItems.addElement(oItem);
		
			            NetResponse resp = getNet().pushMultipartData( getProtocol().getClientChangesUrl(), m_arMultipartItems, getSync(), null );
			            if ( !resp.isOK() )
			            {
			            	//getSync().stopSync();
			                m_nErrCode = RhoAppAdapter.ERR_REMOTESERVER;
			                m_strError = resp.getCharData();
			            }
			        }else
			        {
			            NetResponse resp = getNet().pushData( getProtocol().getClientChangesUrl(), strBody, getSync());
			            if ( !resp.isOK() )
			            {
			            	//getSync().stopSync();
			                m_nErrCode = RhoAppAdapter.ERR_REMOTESERVER;
			                m_strError = resp.getCharData();
			            }
			        }
	            }
		    }catch(Exception exc)
		    {
		    	m_nErrCode = RhoAppAdapter.getNetErrorCode(exc);
		    	m_strError = exc.getMessage();
		    	//throw exc;
		    }

	        if ( getSync().isNoThreadedMode() && getSync().getSourceOptions().getProperty(getID(), "sync_push_callback" ).length() > 0 )
	        	RhoRuby.rho_ruby_callmethod( getSync().getSourceOptions().getProperty( getID(), "sync_push_callback" ) );
		    
		    getDB().Lock();
		    try{

		        for( i = 0; i < 3; i++ )
		        {
		            if ( arUpdateSent[i] )
		            {
		                if ( m_nErrCode == RhoAppAdapter.ERR_NONE )
		                {
		                    //oo conflicts
		                    if ( i < 1 &&  !getSync().getSourceOptions().getBoolProperty(getID(), "pass_through") ) //create
		                        getDB().executeSQL("UPDATE changed_values SET sent=2 WHERE source_id=? and update_type=? and sent=1", getID(), arUpdateTypes[i] );
		                    else
		                    //
		                        getDB().executeSQL("DELETE FROM changed_values WHERE source_id=? and update_type=? and sent=1", getID(), arUpdateTypes[i] );
		                }else
		                {
		                    if ( i == 0 ) //create
		                    {
		                        //remove deleted while sync objects
		                        Hashtable/*<String,int>*/ hashDeletes = new Hashtable();
		                        {
		                            IDBResult res = getDB().executeSQL("SELECT distinct(object) FROM changed_values where source_id=? and update_type=? and sent=0", getID(), "delete" );
		                            for( ; !res.isEnd(); res.next() )
		                                hashDeletes.put( res.getStringByIdx(0), new Integer(1) );
		                        }

		                        Vector/*<String>*/ arObjs = new Vector();
		                        IDBResult res = getDB().executeSQL("SELECT distinct(object) FROM changed_values where source_id=? and update_type=? and sent=1", getID(), arUpdateTypes[i] );
		                        for( ; !res.isEnd(); res.next() )
		                            arObjs.addElement( res.getStringByIdx(0) );

		                        for ( int j = 0; j < (int)arObjs.size(); j++ )
		                        {
		                            if ( !hashDeletes.containsKey( arObjs.elementAt(j) ) )
		                                getDB().executeSQL("INSERT INTO changed_values(update_type, attrib, source_id, object) VALUES(?,?,?,?)", arUpdateTypes[i], "object", getID(), arObjs.elementAt(j) );
		                        }

		                        for ( int j = 0; j < (int)arObjs.size(); j++ )
		                            getDB().executeSQL("DELETE FROM changed_values WHERE source_id=? and object=? and attrib<>?", getID(), arObjs.elementAt(j), "object" );

		                    }
		                    else if ( i == 1 )//update
		                    {
		                        //remove updates with sent=1 if sent=0 exists
		                        Vector/*<String>*/ arObjs = new Vector(), arAttribs = new Vector();
		                        IDBResult res = getDB().executeSQL("SELECT object, attrib FROM changed_values where source_id=? and update_type=? and sent=0", getID(), arUpdateTypes[i] );
		                        for( ; !res.isEnd(); res.next() )
		                        {
		                            arObjs.addElement( res.getStringByIdx(0) );
		                            arAttribs.addElement( res.getStringByIdx(1) );
		                        }
		                        for ( int j = 0; j < (int)arObjs.size(); j++ )
		                        {
		                            getDB().executeSQL("DELETE FROM changed_values WHERE source_id=? and object=? and attrib=? and update_type = ? and sent=1", getID(), arObjs.elementAt(j), arAttribs.elementAt(j), arUpdateTypes[i] );
		                        }

		                        getDB().executeSQL("UPDATE changed_values SET sent=0 WHERE source_id=? and update_type=? and sent=1", getID(), arUpdateTypes[i] );
		                    }
		                    else
		                        getDB().executeSQL("UPDATE changed_values SET sent=0 WHERE source_id=? and update_type=? and sent=1", getID(), arUpdateTypes[i] );
		                }
		            }
		        }
		    	
		        if (m_nErrCode == RhoAppAdapter.ERR_NONE)
		            getDB().executeSQL("DELETE FROM changed_values WHERE source_id=? and update_type=?", getID(), "push_changes" );
		    	
		    }finally
		    {
		    	getDB().Unlock();
		    }
	    }

	    m_arMultipartItems.removeAllElements();
	    m_arBlobAttrs.removeAllElements();
	}
	
	//{"source_name":"SampleAdapter","client_id":1,"create":{"1":{"brand":"Apple","name":"iPhone","price":"199.99"}}}
	//{"source_name":"SampleAdapter","client_id":1,"update":{"1":{"brand":"Apple","name":"iPhone","price":"199.99"}}}
	//{"source_name":"SampleAdapter","client_id":1,"delete":{"1":{"brand":"Apple","name":"iPhone","price":"199.99"}}}
	//{"source_name":"SampleAdapter","client_id":1,"delete":{"3":{"brand":"HTC","name":"Fuze","price":"299.99"}},"create":{"1":{"brand":"Apple","name":"iPhone","price":"199.99"}},"update":{"2":{"brand":"Android","name":"G2","price":"99.99"}}}
	String makePushBody_Ver3( String strUpdateType, boolean isSync)throws DBException
	{
		String strBody = "";
	    getDB().Lock();
	    
	    //if ( isSync )
	    //	getDB().updateAllAttribChanges();
	    
	    IDBResult res = getDB().executeSQL("SELECT attrib, object, value, attrib_type "+
	        "FROM changed_values where source_id=? and update_type =? and sent<=1 ORDER BY object", getID(), strUpdateType );

	    if ( res.isEnd() )
	    {
	    	res.close();
	        getDB().Unlock();
	        return strBody;
	    }

	    String strCurObject = "";
	    boolean bFirst = true;
	    for( ; !res.isEnd(); res.next() )
	    {
	        String strAttrib = res.getStringByIdx(0);
	        String strObject = res.getStringByIdx(1);
	        String value = res.getStringByIdx(2);
	        String attribType = res.getStringByIdx(3);

	        if ( m_hashIgnorePushObjects.containsKey(strObject) || strObject.length() == 0 )
	        	continue;
	        
	        if ( attribType.compareTo("blob.file") == 0 && value.length() > 0 )
	        {
	            MultipartItem oItem = new MultipartItem();
	            oItem.m_strFilePath = RhodesApp.getInstance().resolveDBFilesPath(value);
	            oItem.m_strContentType = "application/octet-stream";
	            oItem.m_strName = strAttrib + "-" + strObject;

	            if ( m_arBlobAttrs.indexOf(strAttrib) == -1 )
	            	m_arBlobAttrs.addElement(strAttrib);
	            
	            m_arMultipartItems.addElement(oItem);
	        }

	        if ( strBody.length() == 0 )
	        {
	            if ( !isSync )
	                strBody += "{";
	            else
	                strBody += "\"" + strUpdateType + "\":{";
	        }

	        if ( strObject.compareTo(strCurObject) != 0 )
	        {
	            if ( strCurObject.length() > 0 )
	            {
	                if ( !bFirst )
	                    strBody += "}";
	                strBody += ",";
	            }

	            bFirst = true;
	            strBody += JSONEntry.quoteValue(strObject);
	            strCurObject = strObject;
	        }
	            
	        if (!bFirst)
	            strBody += ",";

	        if ( strAttrib.length() > 0  )
	        {
	            if ( bFirst )
	                strBody += ":{";

	            strBody += JSONEntry.quoteValue(strAttrib) + ":" + JSONEntry.quoteValue(value);
	            bFirst = false;
	        }
	    }

	    if ( strBody.length() > 0 )
	    {
	        if ( !bFirst )
	            strBody += "}";

	        strBody += "}";
	    }

	    if ( isSync )	    
	    	getDB().executeSQL("UPDATE changed_values SET sent=1 WHERE source_id=? and update_type=? and sent=0", getID(), strUpdateType );
	    
	    getDB().Unlock();
	    
	    return strBody;
	}

	void applyChangedValues()throws Exception
	{
	    String strBody = makePushBody_Ver3("create", false);
	    if ( strBody != null && strBody.length() > 0 )
	    {
	        JSONEntry oEntry = new JSONEntry(strBody);
	        processSyncCommand("insert", oEntry, false );
	    }

	    strBody = makePushBody_Ver3("delete", false);
	    if ( strBody != null && strBody.length() > 0 )
	    {
	        JSONEntry oEntry = new JSONEntry(strBody);
	        processSyncCommand("delete", oEntry, false );
	    }

	    strBody = makePushBody_Ver3("update", false);
	    if ( strBody != null && strBody.length() > 0 )
	    {
	        JSONEntry oEntry = new JSONEntry(strBody);
	        processSyncCommand("insert", oEntry, false );
	    }
	}
	
	void syncServerChanges()throws Exception
	{
		LOG.INFO("Sync server changes source ID :" + getID() );
		
	    while( getSync().isContinueSync()&& 
	    	   ( m_nErrCode == RhoAppAdapter.ERR_NONE || m_nErrCode == RhoAppAdapter.ERR_CUSTOMSYNCSERVER) )
	    {
	        setCurPageCount(0);
	        String strUrl = getProtocol().getServerQueryUrl("");
	        String strQuery = getProtocol().getServerQueryBody(getName(), getSync().getClientID(), getSync().getSyncPageSize());

	        if ( !m_bTokenFromDB && getToken() > 1 )
	            strQuery += "&token=" + getToken();

	        if ( m_strQueryParams.length() > 0 )
	        	strQuery += "&" + m_strQueryParams;
	        
			LOG.INFO( "Pull changes from server. Url: " + (strUrl+strQuery) );
			
			NetResponse resp = null;
			try{
		        PROF.START("Net");	    
		        resp = getNet().pullData(strUrl+strQuery, getSync());
				PROF.STOP("Net");
	
		        if ( !resp.isOK() )
		        {
		            //getSync().stopSync();
		            m_nErrCode = RhoAppAdapter.getErrorFromResponse(resp);
		            m_strError = resp.getCharData();
		            continue;
		        }
		    }catch(Exception exc)
		    {
		    	m_nErrCode = RhoAppAdapter.getNetErrorCode(exc);
		    	m_strError = exc.getMessage();
		    	continue;
		    	//throw exc;
		    }

		    String szData = null;
		    String strTestResp = getSync().getSourceOptions().getProperty(getID(), "rho_server_response");
		    if ( strTestResp != null && strTestResp.length() > 0 )
		    {
		    	szData = strTestResp;
		    	getNotify().setFakeServerResponse(true);
		    }
		    else
		    	szData = resp.getCharData();		    

	        PROF.START("Parse");
	        JSONArrayIterator oJsonArr = new JSONArrayIterator(szData);
	        PROF.STOP("Parse");

	        processServerResponse_ver3(oJsonArr);

	        if (getSync().getSourceOptions().getBoolProperty(getID(), "pass_through"))
	        	processToken(0);
	        
	        if ( getToken() == 0 )
	            break;
	        
	        if ( strTestResp != null && strTestResp.length() > 0 )
	            break;	        
	    }
	    
	    if ( getSync().isSchemaChanged() )
	    	getSync().stopSync();	    
	}

	//{"create-error":{"0_broken_object_id":{"name":"wrongname","an_attribute":"error create"},"0_broken_object_id-error":{"message":"error create"}}}
	void processServerErrors(JSONEntry oCmds)throws Exception
	{
		String strServerError = "";
	    String arErrTypes[] = {"source-error", "search-error", "create-error", "update-error", "delete-error", "update-rollback", null};
	    for( int i = 0; ; i++ )
	    {
	        if ( arErrTypes[i] == null )
	            break;
	        if ( !oCmds.hasName(arErrTypes[i]) )
	            continue;

	        JSONEntry errSrc = oCmds.getEntry(arErrTypes[i]);
	        JSONStructIterator errIter = new JSONStructIterator(errSrc);
	        for( ; !errIter.isEnd(); errIter.next() )
	        {
	            String strKey = errIter.getCurKey();

	            if ( i == 0 || i == 1 )//"source-error", "search-error" 
	            {
	                if ( errIter.getCurValue().hasName("message") )
	                {
	                	if ( strServerError.length() > 0 )
	                		strServerError += "&";
	                	
	                    strServerError += "server_errors[" + URI.urlEncode(strKey) + "][message]=" + URI.urlEncode(errIter.getCurValue().getString("message"));
	                }
	            }
	            else
	            {
	                //"create-error", "update-error", "delete-error", "update-rollback"
	                String strObject = strKey;

	                if ( strObject.endsWith("-error") )
	                {
	                    strObject = strObject.substring(0, strKey.length()-6);
	                	if ( strServerError.length() > 0 )
	                		strServerError += "&";
	                    
	                    strServerError += "server_errors[" + arErrTypes[i] + "][" + URI.urlEncode(strObject) + "][message]=" + URI.urlEncode(errIter.getCurValue().getString("message"));
	                }else
	                {
		                JSONStructIterator attrIter = new JSONStructIterator(errIter.getCurValue());
		                for( ; !attrIter.isEnd(); attrIter.next() )
		                {
			                String strAttrName = attrIter.getCurKey();
			                String strAttrValue = attrIter.getCurString();
			                
			                if ( strServerError.length() > 0 )
			                	strServerError += "&";
			                
			                strServerError += "server_errors[" + arErrTypes[i] + "][" + URI.urlEncode(strObject) + "][attributes][" + URI.urlEncode(strAttrName) + "]=" + URI.urlEncode(strAttrValue);
		                }
	                }
	            }
	        }
	    }

	    if ( strServerError.length() > 0 )
	    	getNotify().fireSyncNotification2(this, true, RhoAppAdapter.ERR_CUSTOMSYNCSERVER, strServerError);
	}
	
	void processServerResponse_ver3(JSONArrayIterator oJsonArr)throws Exception
	{
	    PROF.START("Data1");

	    int nVersion = 0;
	    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("version") )
	    {
	        nVersion = oJsonArr.getCurItem().getInt("version");
	        oJsonArr.next();
	    }

	    if ( nVersion != getProtocol().getVersion() )
	    {
	        LOG.ERROR("Sync server send data with incompatible version. Client version: " + getProtocol().getVersion() +
	            "; Server response version: " + nVersion + ". Source name: " + getName() );
	        getSync().stopSync();
	        m_nErrCode = RhoAppAdapter.ERR_UNEXPECTEDSERVERRESPONSE;
	        return;
	    }

	    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("token"))
	    {
	        processToken(oJsonArr.getCurItem().getUInt64("token"));
	        oJsonArr.next();
	    }

	    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("source") )
	    {
	        //skip it. it uses in search only
	        oJsonArr.next();
	    }

	    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("count") )
	    {
	        setCurPageCount(oJsonArr.getCurItem().getInt("count"));
	        oJsonArr.next();
	    }

	    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("refresh_time") )
	    {
	        setRefreshTime(oJsonArr.getCurItem().getInt("refresh_time"));
	        oJsonArr.next();
	    }

	    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("progress_count") )
	    {
	        //TODO: progress_count
	        //setTotalCount(oJsonArr.getCurItem().getInt("progress_count"));
	        oJsonArr.next();
	    }

	    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("total_count") )
	    {
	        setTotalCount(oJsonArr.getCurItem().getInt("total_count"));
	        oJsonArr.next();
	    }

	    //if ( getServerObjectsCount() == 0 )
	    //    getNotify().fireSyncNotification(this, false, RhoAppAdapter.ERR_NONE, "");

	    if ( getToken() == 0 )
	    {
	        //oo conflicts
	        getDB().executeSQL("DELETE FROM changed_values where source_id=? and sent>=3", getID() );
	        //

	    }

		LOG.INFO("Got " + getCurPageCount() + "(Processed: " +  getServerObjectsCount() + ") records of " + getTotalCount() + " from server. Source: " + getName()
	         + ". Version: " + nVersion );

	    PROF.STOP("Data1");
	    if ( !oJsonArr.isEnd() && getSync().isContinueSync() )
	    {
	        JSONEntry oCmds = oJsonArr.getCurItem();
	        PROF.START("Data");

	        if ( oCmds.hasName("schema-changed") )
	        {
	            getSync().setSchemaChanged(true);
	        }else
	        {
	        	processServerErrors(oCmds);
		        getDB().startTransaction();
		        
	            if (getSync().getSourceOptions().getBoolProperty(getID(), "pass_through"))
	            {
	                if ( m_bSchemaSource )
	                    getDB().executeSQL( "DELETE FROM " + getName() );
	                else
	                    getDB().executeSQL( "DELETE FROM object_values WHERE source_id=?", getID() );
	            }
		        
		        if ( oCmds.hasName("metadata") && getSync().isContinueSync() )
		        {
		            String strMetadata = oCmds.getString("metadata");
		            getDB().executeSQL("UPDATE sources SET metadata=? WHERE source_id=?", strMetadata, getID() );
		        }
		        if ( oCmds.hasName("links") && getSync().isContinueSync() )
		            processSyncCommand("links", oCmds.getEntry("links"), true );
		        if ( oCmds.hasName("delete") && getSync().isContinueSync() )
		            processSyncCommand("delete", oCmds.getEntry("delete"), true );
		        if ( oCmds.hasName("insert") && getSync().isContinueSync() )
		            processSyncCommand("insert", oCmds.getEntry("insert"), true );
	
		        PROF.STOP("Data");
	
			    PROF.START("DB");
		        getDB().endTransaction();
		        PROF.STOP("DB");
	
		        getNotify().fireObjectsNotification();
	        }
	    }

		PROF.START("Data1");
	    if ( getCurPageCount() > 0 )
	        getNotify().fireSyncNotification(this, false, RhoAppAdapter.ERR_NONE, "");
		PROF.STOP("Data1");
	}
	
	void processSyncCommand(String strCmd, JSONEntry oCmdEntry, boolean bCheckUIRequest)throws Exception
	{
	    JSONStructIterator objIter = new JSONStructIterator(oCmdEntry);

	    for( ; !objIter.isEnd() && getSync().isContinueSync(); objIter.next() )
	    {
	        String strObject = objIter.getCurKey();
	        JSONStructIterator attrIter = new JSONStructIterator( objIter.getCurValue() );
	        
	        try
	        {
		        if ( m_bSchemaSource )
		            processServerCmd_Ver3_Schema(strCmd,strObject,attrIter,bCheckUIRequest);
		        else
		        {
		            for( ; !attrIter.isEnd(); attrIter.next() )
		            {
		                String strAttrib = attrIter.getCurKey();
		                String strValue = attrIter.getCurString();
	
		                processServerCmd_Ver3(strCmd,strObject,strAttrib,strValue,bCheckUIRequest);
		            }
		        }
	        }catch(DBException exc)
		    {
		    	LOG.ERROR("Sync of server changes failed for " + getName() + ";object: " + strObject, exc);
		    }
		    
	        if ( getSyncType().compareTo("none") == 0 )
	        	continue;
	        
	        if ( bCheckUIRequest )
	        {
		        if ( getDB().isUIWaitDB() )
		        {
			        LOG.INFO("Commit transaction because of UI request.");
		            getDB().endTransaction();
		            
		            checkProgressStepNotify(false);
		            
		            SyncThread.getInstance().sleep(1000);
		            getDB().startTransaction();
		        }else
		        	checkProgressStepNotify(true);
	        }
	    }
	}
	
	void checkProgressStepNotify(boolean bEndTransaction)throws Exception
	{
	    int nSyncObjectCount  = getNotify().incLastSyncObjectCount(getID());
	    if ( getProgressStep() > 0 && (nSyncObjectCount%getProgressStep() == 0) )
	    {
	        if ( bEndTransaction )
	        {
	            LOG.INFO("Commit transaction because of Sync Progress notification.");
	            getDB().endTransaction();
	        }

	        getNotify().fireSyncNotification(this, false, RhoAppAdapter.ERR_NONE, "");

	        if ( bEndTransaction )
	            getDB().startTransaction();
	    }
	}

	void processAssociations(String strOldObject, String strNewObject)throws Exception
	{
        for ( int i = 0; i < m_arAssociations.size(); i++ )
        {
            SyncSource pSrc = getSync().findSourceByName( ((CAssociation)m_arAssociations.elementAt(i)).m_strSrcName);
	        if ( pSrc != null )
	            pSrc.updateAssociation(strOldObject, strNewObject, ((CAssociation)m_arAssociations.elementAt(i)).m_strAttrib);
	    }
	}

	void updateAssociation(String strOldObject, String strNewObject, String strAttrib)throws Exception
	{
	    if ( m_bSchemaSource )
	    {
	        String strSqlUpdate = "UPDATE ";
	        strSqlUpdate += getName() + " SET " + strAttrib + "=? where " + strAttrib + "=?";

	        getDB().executeSQL(strSqlUpdate, strNewObject, strOldObject );
	    }
	    else
	        getDB().executeSQL("UPDATE object_values SET value=? where attrib=? and source_id=? and value=?", 
	            strNewObject, strAttrib, getID(), strOldObject );

	    getDB().executeSQL("UPDATE changed_values SET value=? where attrib=? and source_id=? and value=?", 
	        strNewObject, strAttrib, getID(), strOldObject );
	}
	
	void processServerCmd_Ver3_Schema(String strCmd, String strObject, JSONStructIterator attrIter, boolean bCheckUIRequest)throws Exception
	{
	    if ( strCmd.compareTo("insert") == 0 )
	    {
	        Vector/*<String>*/ vecValues = new Vector(), vecAttrs = new Vector();
	        String strCols = "", strQuest = "", strSet = "";
	        for( ; !attrIter.isEnd(); attrIter.next() )
	        {
	            CAttrValue oAttrValue = new CAttrValue(attrIter.getCurKey(),attrIter.getCurString());
	            
		    	if ( bCheckUIRequest && !checkFreezedProps(oAttrValue.m_strAttrib))
		    		continue;
	            
	            if ( !processBlob(strCmd,strObject,oAttrValue) )
	                break;
	        	
	            if ( strCols.length() > 0 )
	                strCols += ",";
	            if ( strQuest.length() > 0)
	                strQuest += ",";
	            if ( strSet.length() > 0)
	                strSet += ",";

	            strCols += oAttrValue.m_strAttrib;
	            strQuest += "?";
	            strSet += oAttrValue.m_strAttrib + "=?";
	            vecAttrs.addElement(oAttrValue.m_strAttrib);
	            vecValues.addElement(oAttrValue.m_strValue);
	        }
	        vecValues.addElement(strObject);
	        if ( strCols.length() > 0 )
	            strCols += ",";
	        if ( strQuest.length() > 0)
	            strQuest += ",";
	        
	        strCols += "object";
	        strQuest += "?";

	        String strSqlInsert = "INSERT INTO ";
	        strSqlInsert += getName() + " (";
	        strSqlInsert += strCols + ") VALUES(" + strQuest + ")";

	        if ( !getSync().isContinueSync() )
	            return;

	        IDBResult resInsert = getDB().executeSQLReportNonUniqueEx(strSqlInsert, vecValues );
	        if ( resInsert.isNonUnique() )
	        {
	            String strSqlUpdate = "UPDATE ";
	            strSqlUpdate += getName() + " SET " + strSet + " WHERE object=?";
	            getDB().executeSQLEx(strSqlUpdate, vecValues);

	            if ( getSyncType().compareTo("none") != 0 )
	            {
		            // oo conflicts
		            for( int i = 0; i < (int)vecAttrs.size(); i++ )
		            {
		                getDB().executeSQL("UPDATE changed_values SET sent=4 where object=? and attrib=? and source_id=? and update_type=? and sent>1", 
		                	strObject, vecAttrs.elementAt(i), getID(), "create" );		                
		            }
		            //
	            }
	        }

	        if ( getSyncType().compareTo("none") != 0 )	        
	        	getNotify().onObjectChanged(getID(),strObject, SyncNotify.enUpdate);
	        
	        m_nInserted++;
	    }else if (strCmd.compareTo("delete") == 0)
	    {
	    	Vector/*<String>*/ vecAttrs = new Vector();
	        String strSet = "";
	        for( ; !attrIter.isEnd() && getSync().isContinueSync(); attrIter.next() )
	        {
	        	CAttrValue oAttrValue = new CAttrValue(attrIter.getCurKey(),attrIter.getCurString());
	        	
	            if ( strSet.length() > 0 )
	                strSet += ",";

	            vecAttrs.addElement(oAttrValue.m_strAttrib);
	            strSet += oAttrValue.m_strAttrib + "=NULL";
	        }

	        String strSqlUpdate = "UPDATE ";
	        strSqlUpdate += getName() + " SET " + strSet + " WHERE object=?";
	        
	        if ( strSet.length() == 0 )
	            return;
	        
	        getDB().executeSQL(strSqlUpdate, strObject);
	        //Remove item if all nulls
	        String strSelect = "SELECT * FROM " + getName() + " WHERE object=?";
	        IDBResult res = getDB().executeSQL( strSelect, strObject );
	        if ( !res.isEnd() )
	        {
	            boolean bAllNulls = true;
	            for( int i = 0; i < res.getColCount(); i ++)
	            {
	                if ( !res.isNullByIdx(i) && res.getColName(i).compareTo("object")!=0 )
	                {
	                    bAllNulls = false;
	                    break;
	                }
	            }

	            if (bAllNulls)
	            {
	                String strDelete = "DELETE FROM " + getName() + " WHERE object=?";
	                getDB().executeSQL( strDelete, strObject);
	            }
	        }
	        
	        if ( getSyncType().compareTo("none") != 0 )
	        {
		        getNotify().onObjectChanged(getID(), strObject, SyncNotify.enDelete);
		        // oo conflicts
		        for( int i = 0; i < (int)vecAttrs.size(); i++ )
		        {
		            getDB().executeSQL("UPDATE changed_values SET sent=3 where object=? and attrib=? and source_id=? and update_type=?", 
		            	strObject, vecAttrs.elementAt(i), getID(), "create" );		            
		        }
		        //
	        }
	        
	        m_nDeleted++;
	    }else if ( strCmd.compareTo("links") == 0 )
	    {
	        String strValue = attrIter.getCurString();
	        processAssociations(strObject, strValue);
	        
	        String strSqlUpdate = "UPDATE ";
	        strSqlUpdate += getName() + " SET object=? WHERE object=?";
	        getDB().executeSQL(strSqlUpdate, strValue, strObject);

	        getDB().executeSQL("UPDATE changed_values SET object=?,sent=3 where object=? and source_id=? and update_type=?", strValue, strObject, getID(), "create" );	        
	        getNotify().onObjectChanged(getID(), strObject, SyncNotify.enCreate);
	    }

	}
	
	boolean processBlob( String strCmd, String strObject, CAttrValue oAttrValue )throws Exception
	{
	    //TODO: when server return delete with rhoblob postfix - delete isBlobAttr
	    if ( !(oAttrValue.m_strBlobSuffix.length() > 0 || getDB().getAttrMgr().isBlobAttr(getID(), oAttrValue.m_strAttrib)) )
	        return true;

	    boolean bDownload = true;
	    String strDbValue = "";
	    if ( !getDB().getAttrMgr().isOverwriteBlobFromServer(getID(), oAttrValue.m_strAttrib) )
	    {
	        if ( m_bSchemaSource )
	        {
	            String strSelect = "SELECT " + oAttrValue.m_strAttrib + " FROM " + getName() + " WHERE object=?";
	            IDBResult res = getDB().executeSQL( strSelect, strObject);
	            if (!res.isEnd())
	            {
	                strDbValue = res.getStringByIdx(0);
	                bDownload = strDbValue == null || strDbValue.length() == 0;
	            }
	        }else
	        {
	            IDBResult res = getDB().executeSQL(
	                "SELECT value FROM object_values WHERE object=? and attrib=? and source_id=?",
	                strObject, oAttrValue.m_strAttrib, getID() );
	            if (!res.isEnd())
	            {
	                strDbValue = res.getStringByIdx(0);
	                bDownload = strDbValue == null || strDbValue.length() == 0;
	            }
	        }
	    }

	    if ( bDownload )
	    {
	    	boolean bRes = false;
	        getDB().endTransaction();
	        try{
	        	bRes = downloadBlob(oAttrValue);
	        }finally
	        {
	        	getDB().startTransaction();
	        }
	        
	        return bRes;
	    }
/*	    
	    String fName = makeFileName( oAttrValue );	  
	    String fOldName = RhodesApp.getInstance().resolveDBFilesPath(strDbValue);
	    RhoClassFactory.createFile().renameOverwrite(fOldName, fName); 
	    
	    oAttrValue.m_strValue = FilePath.getRelativePath( fName, RhodesApp.getInstance().getRhoRootPath());
*/	    
	    oAttrValue.m_strValue = strDbValue;
	    return true;
	}
	
	boolean processServerBlobAttrs()throws Exception 
	{
		if ( m_bSchemaSource ) 
		{
			return true;
		}else 
		{
			final String selAttrsRequest = "SELECT source_attribs,blob_attribs from sources WHERE source_id=?";
			final String updAttrsRequest = "UPDATE sources SET source_attribs=?,blob_attribs=? WHERE source_id=?";
			final String updObjectsRequest = "UPDATE object_values SET attrib=? WHERE attrib=? and source_id=?";
			final String blobSfx = "-rhoblob";
			
			getDB().startTransaction();
			IDBResult res = getDB().executeSQL( selAttrsRequest,getID() );
			
			Vector/*<String>*/ attrsToRename = new Vector();
			
			String newAttrs = "";
			
			for ( ; !res.isEnd(); res.next() ) {
				String attrs = res.getStringByIdx(0);
				String blobs = res.getStringByIdx(1);
				
				Tokenizer tokenizer = new Tokenizer(attrs,",");
				while (tokenizer.hasMoreTokens()) {
					String attrName = tokenizer.nextToken();
					if (!tokenizer.hasMoreTokens()) {
						break;
					}
					String attrVal = tokenizer.nextToken();
					
					
					if ( attrName.endsWith(blobSfx ) ) {
						if ( blobs.length() > 0 ) {
							blobs += ",";
						}
						blobs += attrName.substring(0,attrName.length()-blobSfx.length()) + "," + attrVal;
						attrsToRename.addElement(attrName);
					} else {
						if (newAttrs.length() > 0 ) {
							newAttrs += ",";
						}
						newAttrs += attrName + "," + attrVal;
					}
				}
				
				LOG.TRACE("Updating attributes for source " + getName() + ". Old attribs=" + attrs + ", new attribs=" + newAttrs + ", blob attribs=" + blobs);
				
				getDB().executeSQL( updAttrsRequest, newAttrs, blobs, getID() );
			}
			
			for ( int i = 0; i < attrsToRename.size(); i++ ) 
			{
				String strAttr = (String)attrsToRename.elementAt(i);
				LOG.TRACE( "Updating objects with blob attribute " + strAttr + " for source " + getName() );
				getDB().executeSQL( updObjectsRequest, strAttr.substring( 0, strAttr.length()-blobSfx.length()), strAttr, getID() );
			}
			
			getDB().endTransaction();
		}
		return true;
	}

		
	boolean processAllBlobs()throws Exception 
	{
		if (m_bSchemaSource) 
		{
			Vector/*<String>*/ blobAttrs = getDB().getAttrMgr().getBlobAttrs( getID() );
			
			for ( int i = 0; i <  blobAttrs.size(); i ++ ) 
			{
				String strAttr = (String)blobAttrs.elementAt(i);
				String sql = "SELECT object," + strAttr + " FROM " + getName();
				
				IDBResult res = getDB().executeSQL(sql);
				
				LOG.TRACE( "Processing blobs for source " + getName() + ", attribute " + strAttr);
				
				for ( ; !res.isEnd(); res.next() )
				{ 
					String object = res.getStringByIdx(0);
					String value = res.getStringByIdx(1);
					
					if ( value.indexOf("://") >=0 ) 
					{
						LOG.TRACE( "Processing remote blob: " + value );
						CAttrValue attr = new CAttrValue( strAttr, value );
						if ( !downloadBlob( attr ) ) {
							return false;
						}
						
						sql = "UPDATE " + getName() + " SET " + strAttr + "=? WHERE object=?";
						
						getDB().executeSQL( sql, attr.m_strValue, object );
					}
				}
			}
		} else 
		{
			Vector/*<String>*/ blobAttrs = getDB().getAttrMgr().getBlobAttrs( getID() );
		
			for ( int i = 0; i <  blobAttrs.size(); i ++ ) 
			{
				String strAttr = (String)blobAttrs.elementAt(i);
				IDBResult res = getDB().executeSQL("SELECT object,value FROM object_values WHERE attrib=? and source_id=?",strAttr,getID());
			
				LOG.TRACE( "Processing blobs for source " + getName() + ", attribute " + strAttr);
			
				for ( ; !res.isEnd(); res.next() )
				{ 
					String object = res.getStringByIdx(0);
					String value = res.getStringByIdx(1);
				
					if ( value.indexOf("://") >=0 ) 
					{
						LOG.TRACE( "Processing remote blob: " + value );
						CAttrValue attr = new CAttrValue( strAttr, value );
						if ( !downloadBlob( attr ) ) {
							return false;
						}
					
						getDB().executeSQL("UPDATE object_values SET value=? where object=? and source_id=? and attrib=?", attr.m_strValue, object, getID(), strAttr);
					}
				}
			}
		}
		
		return true;
	}
	
	boolean checkFreezedProps(String strProp)
	{
		String strFreezedProps = getSync().getSourceOptions().getProperty(getID(), "freezed");
		
		if ( strFreezedProps.length() > 0 )
		{
			Tokenizer oTokenizer = new Tokenizer( strFreezedProps, "," );
			boolean bFound =false;
			while (oTokenizer.hasMoreTokens() && (!bFound) ) 
			{
				String tok = oTokenizer.nextToken();
				if (tok.length() == 0)
					continue;
			
				if (tok.compareTo(strProp)==0)
				{
					bFound = true;
				}
			}
		
			if (!bFound)
			{
				LOG.INFO("Skip Non-exist property : " + strProp + ". For model : " + getName());
				return false;				
			}
		}
		
		return true;
	}
	
	void processServerCmd_Ver3(String strCmd, String strObject, String strAttriba, String strValuea, boolean bCheckUIRequest)throws Exception
	{
	    CAttrValue oAttrValue = new CAttrValue(strAttriba,strValuea);
		
	    if ( strCmd.compareTo("insert") == 0 )
	    {
	    	if ( bCheckUIRequest && !checkFreezedProps(oAttrValue.m_strAttrib))
	    		return;
	    	
	        if ( !processBlob(strCmd,strObject,oAttrValue) )
	            return;

	        IDBResult resInsert = getDB().executeSQLReportNonUnique("INSERT INTO object_values "+
	                "(attrib, source_id, object, value) VALUES(?,?,?,?)", 
	                oAttrValue.m_strAttrib, getID(), strObject, oAttrValue.m_strValue );
	        
	        if ( resInsert.isNonUnique() )
	        {
	            getDB().executeSQL("UPDATE object_values " +
                    "SET value=? WHERE object=? and attrib=? and source_id=?", 
                     oAttrValue.m_strValue, strObject, oAttrValue.m_strAttrib, getID() );

	            if ( getSyncType().compareTo("none") != 0 )
	            {
		            // oo conflicts
		            getDB().executeSQL("UPDATE changed_values SET sent=4 where object=? and attrib=? and source_id=? and update_type=? and sent>1", 
		            	    strObject, oAttrValue.m_strAttrib, getID(), "create" );		            
		            //
	            }
	        }

	        if ( getSyncType().compareTo("none") != 0 )	        
	        	getNotify().onObjectChanged(getID(),strObject, SyncNotify.enUpdate);
	        
	        m_nInserted++;
	    }else if (strCmd.compareTo("delete") == 0)
	    {
	        getDB().executeSQL("DELETE FROM object_values where object=? and attrib=? and source_id=?", strObject, oAttrValue.m_strAttrib, getID() );
	        
	        if ( getSyncType().compareTo("none") != 0 )
	        {
		        getNotify().onObjectChanged(getID(), strObject, SyncNotify.enDelete);
		        // oo conflicts
		        getDB().executeSQL("UPDATE changed_values SET sent=3 where object=? and attrib=? and source_id=? and update_type=?", strObject, oAttrValue.m_strAttrib, getID(), "create" );		        
		        //
	        }
	        m_nDeleted++;
	    }else if ( strCmd.compareTo("links") == 0 )
	    {
	        processAssociations(strObject, oAttrValue.m_strValue);

	        getDB().executeSQL("UPDATE object_values SET object=? where object=? and source_id=?", oAttrValue.m_strValue, strObject, getID() );
	        getDB().executeSQL("UPDATE changed_values SET object=?,sent=3 where object=? and source_id=? and update_type=?", oAttrValue.m_strValue, strObject, getID(), "create" );	        

	        getNotify().onObjectChanged(getID(), strObject, SyncNotify.enCreate);
	    }

	}
	
	private String makeFileName(CAttrValue value)throws Exception
	{
		String strExt = "";

	    URI uri = new URI(value.m_strValue);    
	    String strQuest = uri.getQueryString();

	    if (strQuest != null && strQuest.length() > 0)
	    {
			int nExt = strQuest.indexOf("extension=");
			if ( nExt >= 0 )
	        {
				int nExtEnd = strQuest.indexOf("&", nExt);
				if (nExtEnd < 0 )
					nExtEnd = strQuest.length();
				
				strExt = strQuest.substring(nExt+10, nExtEnd);
			}
	    }

	    if ( strExt.length() == 0 )
	    {
	        String strFileName = uri.getLastNamePart();
	        int nExt = strFileName != null ? strFileName.lastIndexOf('.') : -1;
			if ( nExt >= 0 )
	            strExt = strFileName.substring(nExt);
	    }

	    if ( strExt.length() == 0 )
	        strExt = ".bin";
	    else if ( strExt.charAt(0) != '.' )    
	        strExt = "." + strExt;

		String fName = RhodesApp.getInstance().getBlobsDirPath() + "/id_" + TimeInterval.getCurrentTime().toULong() + strExt;
		
		return  fName;
		
/*		
		String strExt = ".bin";
		URI uri = new URI(value.m_strValue);
		int nDot = uri.getPath().lastIndexOf('.');
		if ( nDot >= 0 )
			strExt = uri.getPath().substring(nDot);
		else{
			int nExt = uri.getQueryString().indexOf("extension=");
			if ( nExt >= 0 ){
				int nExtEnd = uri.getQueryString().indexOf("&", nExt);
				if (nExtEnd < 0 )
					nExtEnd = uri.getQueryString().length();
				
				strExt = uri.getQueryString().substring(nExt+10, nExtEnd);
			}
		}
		
		String fName = RhodesApp.getInstance().getBlobsDirPath() + "/id_" + TimeInterval.getCurrentTime().toULong() + strExt;
		
		return fName;*/
	}
	
	boolean downloadBlob(CAttrValue value)throws Exception
	{
		String fName = makeFileName( value );
		String url = value.m_strValue;
		
		LOG.TRACE( "Download blob: " + url + " => " + fName );
		
		int nQuest = url.lastIndexOf('?');
		if ( nQuest > 0 )
			url += "&";
		else
			url += "?";
		url += "client_id=" + getSync().getClientID();
		
		try{
			NetResponse resp = getNet().pullFile(url, fName, getSync(), null);
	        if ( !resp.isOK() )
	        {
	        	//getSync().stopSync();
	        	com.rho.file.RhoFile.deleteFile(fName);
	        	m_nErrCode = RhoAppAdapter.getErrorFromResponse(resp);
	        	return false;
	        }
	    }catch(Exception exc)
	    {
	    	m_nErrCode = RhoAppAdapter.getNetErrorCode(exc);
	    	m_strError = exc.getMessage();
	    	return false;
	    	//throw exc;
	    }
        
	    value.m_strValue = FilePath.getRelativePath( fName, RhodesApp.getInstance().getRhoRootPath());
        
        return true;
	}
	
	void processToken(long token)throws DBException
	{
		if ( token > 1 && getToken() == token ){
			//Delete non-confirmed records
	    	
	        setToken( token ); //For m_bTokenFromDB = false;
	        //getDB().executeSQL("DELETE FROM object_values where source_id=? and token=?", getID(), token );
	        //TODO: add special table for id,token
		}else
	    {
	        setToken( token );
	        getDB().executeSQL("UPDATE sources SET token=? where source_id=?", new Long(token), getID() );
		}
	
	}
}
