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
import com.rho.RhoEmptyLogger;
import com.rho.RhoEmptyProfiler;
import com.rho.RhoLogger;
import com.rho.RhoProfiler;
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

class SyncSource
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("Sync");
	
	private static final RhoProfiler PROF = RhoProfiler.RHO_STRIP_PROFILER ? new RhoEmptyProfiler() : 
		new RhoProfiler();

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
    
    int m_nCurPageCount, m_nInserted, m_nDeleted, m_nTotalCount, m_nAttribCounter=0;
    boolean m_bGetAtLeastOnePage = false;
    int m_nErrCode = RhoAppAdapter.ERR_NONE;
    String m_strError = "";
    
	//String m_strPushBody = "";
    Vector/*Ptr<CSyncBlob*>*/ m_arSyncBlobs = new Vector();
    
    int m_nRefreshTime = 0;
    int m_nProgressStep = -1;
    boolean m_bSchemaSource;
    
    static class CAssociation
    {
        String m_strSrcName, m_strAttrib;
        CAssociation( String strSrcName, String strAttrib ){m_strSrcName = strSrcName; m_strAttrib = strAttrib; }
    };
    
    Vector/*<CAssociation>*/ m_arAssociations = new Vector();
    Vector/*Ptr<net::CMultipartItem*>*/ m_arMultipartItems = new Vector();
    Vector/*<String>*/                  m_arBlobAttrs = new Vector();
    
    Integer getID() { return m_nID; }
    String getName() { return m_strName; }
    String getSyncType(){ return m_strSyncType; }
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

    boolean getGetAtLeastOnePage(){ return m_bGetAtLeastOnePage; }
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

        m_nCurPageCount = 0;
        m_nInserted = 0;
        m_nDeleted = 0;
        m_nTotalCount = 0;
        m_bGetAtLeastOnePage = false;

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
        
        m_nCurPageCount = 0;
        m_nInserted = 0;
        m_nDeleted = 0;
        m_nTotalCount = 0;
        m_bGetAtLeastOnePage = false;

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
	        if ( isTokenFromDB() && !isEmptyToken() )
	            syncServerChanges();  //sync only server changes, which was paused before
	        else
	        {
		        if ( isEmptyToken() )
		            processToken(1);
		    	
		        boolean bSyncedServer = syncClientChanges();
		        if ( !bSyncedServer )
		        	syncServerChanges();
	        }	        
	    }catch(Exception exc)
	    {
	    	getSync().stopSync();
	    	throw exc;
	    }finally{
		   TimeInterval endTime = TimeInterval.getCurrentTime();
		   getDB().executeSQL("UPDATE sources set last_updated=?,last_inserted_size=?,last_deleted_size=?, "+
			 "last_sync_duration=?,last_sync_success=?, backend_refresh_time=? WHERE source_id=?", 
             new Long(endTime.toULong()/1000), new Integer(getInsertedCount()), new Integer(getDeletedCount()), 
             new Long((endTime.minus(startTime)).toULong()), 
             new Integer(m_bGetAtLeastOnePage?1:0), new Integer(m_nRefreshTime), getID() );
	    }
	}

	boolean syncClientChanges()throws Exception
	{
	    boolean bSyncedServer = false;
	    if ( isPendingClientChanges() )
	    {
	        syncServerChanges();
	        bSyncedServer = true;
	    }

	    if ( bSyncedServer && isPendingClientChanges() )
	        getSync().setState(SyncEngine.esStop);
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
	    for( i = 0; i < 3 && getSync().isContinueSync(); i++ )
	    {
	        String strBody1;
	        strBody1 = makePushBody_Ver3(arUpdateTypes[i]);
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
	    strBody += "}";

	    if ( bSend )
	    {
	        LOG.INFO( "Push client changes to server. Source: " + getName() + "Size :" + strBody.length() );
	        LOG.TRACE("Push body: " + strBody);		

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
	                getSync().setState(SyncEngine.esStop);
	                m_nErrCode = RhoAppAdapter.ERR_REMOTESERVER;
	            }
	        }else
	        {
	            NetResponse resp = getNet().pushData( getProtocol().getClientChangesUrl(), strBody, getSync());
	            if ( !resp.isOK() )
	            {
	                getSync().setState(SyncEngine.esStop);
	                m_nErrCode = RhoAppAdapter.ERR_REMOTESERVER;
	            }
	        }
	    }

	    for( i = 0; i < 3 && getSync().isContinueSync(); i++ )
	    {
	        if ( arUpdateSent[i] )
	        {
	            //oo conflicts
	            if ( i < 1 ) //create
	                getDB().executeSQL("UPDATE changed_values SET sent=2 WHERE source_id=? and update_type=? and sent=1", getID(), arUpdateTypes[i] );
	            else
	            //
	                getDB().executeSQL("DELETE FROM changed_values WHERE source_id=? and update_type=? and sent=1", getID(), arUpdateTypes[i] );
	        }
	    }

	    m_arMultipartItems.removeAllElements();
	    m_arBlobAttrs.removeAllElements();
	}
	
	//{"source_name":"SampleAdapter","client_id":1,"create":{"1":{"brand":"Apple","name":"iPhone","price":"199.99"}}}
	//{"source_name":"SampleAdapter","client_id":1,"update":{"1":{"brand":"Apple","name":"iPhone","price":"199.99"}}}
	//{"source_name":"SampleAdapter","client_id":1,"delete":{"1":{"brand":"Apple","name":"iPhone","price":"199.99"}}}
	//{"source_name":"SampleAdapter","client_id":1,"delete":{"3":{"brand":"HTC","name":"Fuze","price":"299.99"}},"create":{"1":{"brand":"Apple","name":"iPhone","price":"199.99"}},"update":{"2":{"brand":"Android","name":"G2","price":"99.99"}}}
	String makePushBody_Ver3( String strUpdateType)throws DBException
	{
		String strBody = "";
	    getDB().Lock();
	    IDBResult res = getDB().executeSQL("SELECT attrib, object, value, attrib_type "+
	        "FROM changed_values where source_id=? and update_type =? and sent<=1 ORDER BY object", getID(), strUpdateType );

	    if ( res.isEnd() )
	    {
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

	        if ( attribType.compareTo("blob.file") == 0 )
	        {
	            MultipartItem oItem = new MultipartItem();
	            oItem.m_strFilePath = RhodesApp.getInstance().resolveDBFilesPath(value);
	            oItem.m_strContentType = "application/octet-stream";
	            oItem.m_strName = strAttrib + "-" + strObject;

	            m_arBlobAttrs.addElement(strAttrib);
	            m_arMultipartItems.addElement(oItem);
	        }

	        if ( strBody.length() == 0 )
	            strBody += "\"" + strUpdateType + "\":{";

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

	    getDB().executeSQL("UPDATE changed_values SET sent=1 WHERE source_id=? and update_type=? and sent=0", getID(), strUpdateType );
	    getDB().Unlock();
	    
	    return strBody;
	}

	void syncServerChanges()throws Exception
	{
		LOG.INFO("Sync server changes source ID :" + getID() );
		
	    while( getSync().isContinueSync() )
	    {
	        setCurPageCount(0);
	        String strUrl = getProtocol().getServerQueryUrl("");
	        String strQuery = getProtocol().getServerQueryBody(getName(), getSync().getClientID(), getSync().getSyncPageSize());

	        if ( !m_bTokenFromDB && getToken() > 1 )
	            strQuery += "&token=" + getToken();

			LOG.INFO( "Pull changes from server. Url: " + (strUrl+strQuery) );
			
			NetResponse resp = null;
			try{
		        PROF.START("Net");	    
		        resp = getNet().pullData(strUrl+strQuery, getSync());
				PROF.STOP("Net");
	
		        if ( !resp.isOK() )
		        {
		            getSync().stopSync();
		            m_nErrCode = RhoAppAdapter.getErrorFromResponse(resp);
		            m_strError = resp.getCharData();
		            continue;
		        }
		    }catch(Exception exc)
		    {
		    	m_nErrCode = RhoAppAdapter.getNetErrorCode(exc);
		    	throw exc;
		    }

	        String szData = resp.getCharData();

	        PROF.START("Parse");
	        JSONArrayIterator oJsonArr = new JSONArrayIterator(szData);
	        PROF.STOP("Parse");

	        processServerResponse_ver3(oJsonArr);

	        if ( getToken() == 0 )
	            break;
	    }
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
	   /* if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("source-error") )
	    {
	        CJSONEntry oJsonErr = oJsonArr.getCurItem().getEntry("source-error");
	        m_strError = oJsonErr.getString("message");
	        m_nErrCode = RhoAppAdapter.ERR_CUSTOMSYNCSERVER;
	        getSync().stopSync();
	        return;
	    }*/

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

	        getDB().startTransaction();
	        if ( oCmds.hasName("metadata") && getSync().isContinueSync() )
	        {
	            String strMetadata = oCmds.getString("metadata");
	            getDB().executeSQL("UPDATE sources SET metadata=? WHERE source_id=?", strMetadata, getID() );
	        }
	        if ( oCmds.hasName("links") && getSync().isContinueSync() )
	            processSyncCommand("links", oCmds.getEntry("links") );
	        if ( oCmds.hasName("delete") && getSync().isContinueSync() )
	            processSyncCommand("delete", oCmds.getEntry("delete") );
	        if ( oCmds.hasName("insert") && getSync().isContinueSync() )
	            processSyncCommand("insert", oCmds.getEntry("insert") );

	        PROF.STOP("Data");

		    PROF.START("DB");
	        getDB().endTransaction();
	        PROF.STOP("DB");

	        getNotify().fireObjectsNotification();
	    }

		PROF.START("Data1");
	    if ( getCurPageCount() > 0 )
	        getNotify().fireSyncNotification(this, false, RhoAppAdapter.ERR_NONE, "");
		PROF.STOP("Data1");
	}
	
	void processSyncCommand(String strCmd, JSONEntry oCmdEntry)throws Exception
	{
	    JSONStructIterator objIter = new JSONStructIterator(oCmdEntry);

	    for( ; !objIter.isEnd() && getSync().isContinueSync(); objIter.next() )
	    {
	        String strObject = objIter.getCurKey();
	        JSONStructIterator attrIter = new JSONStructIterator( objIter.getCurValue() );
	        if ( m_bSchemaSource )
	            processServerCmd_Ver3_Schema(strCmd,strObject,attrIter);
	        else
	        {
	            for( ; !attrIter.isEnd() && getSync().isContinueSync(); attrIter.next() )
	            {
	                String strAttrib = attrIter.getCurKey();
	                String strValue = attrIter.getCurString();

	                processServerCmd_Ver3(strCmd,strObject,strAttrib,strValue);
	            }
	        }

	        int nSyncObjectCount  = getNotify().incLastSyncObjectCount(getID());
	        if ( getProgressStep() > 0 && (nSyncObjectCount%getProgressStep() == 0) )
	            getNotify().fireSyncNotification(this, false, RhoAppAdapter.ERR_NONE, "");
	        
	        if ( getDB().isUIWaitDB() )
	        {
		        LOG.INFO("Commit transaction because of UI request.");
	            getDB().endTransaction();
	            SyncThread.getInstance().sleep(1000);
	            getDB().startTransaction();
	        }
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
	
	void processServerCmd_Ver3_Schema(String strCmd, String strObject, JSONStructIterator attrIter)throws Exception
	{
	    if ( strCmd.compareTo("insert") == 0 )
	    {
	        Vector/*<String>*/ vecValues = new Vector(), vecAttrs = new Vector();
	        String strCols = "", strQuest = "", strSet = "";
	        for( ; !attrIter.isEnd() && getSync().isContinueSync(); attrIter.next() )
	        {
	            CAttrValue oAttrValue = new CAttrValue(attrIter.getCurKey(),attrIter.getCurString());
	            if ( !processBlob(strCmd,strObject,oAttrValue) )
	                continue;
	        	
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

	            // oo conflicts
	            for( int i = 0; i < (int)vecAttrs.size(); i++ )
	            {
	                getDB().executeSQL("UPDATE changed_values SET sent=4 where object=? and attrib=? and source_id=? and sent>1", 
	                    strObject, vecAttrs.elementAt(i), getID() );
	            }
	            //
	        }

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
	        
	        if ( strSet.length() == 0 || !getSync().isContinueSync() )
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
	        
	        getNotify().onObjectChanged(getID(), strObject, SyncNotify.enDelete);
	        // oo conflicts
	        for( int i = 0; i < (int)vecAttrs.size(); i++ )
	        {
	            getDB().executeSQL("UPDATE changed_values SET sent=3 where object=? and attrib=? and source_id=?", 
	                strObject, vecAttrs.elementAt(i), getID() );
	        }
	        //

	        m_nDeleted++;
	    }else if ( strCmd.compareTo("links") == 0 )
	    {
	        String strValue = attrIter.getCurString();
	        processAssociations(strObject, strValue);
	        
	        String strSqlUpdate = "UPDATE ";
	        strSqlUpdate += getName() + " SET object=? WHERE object=?";
	        getDB().executeSQL(strSqlUpdate, strValue, strObject);

	        getDB().executeSQL("UPDATE changed_values SET object=?,sent=3 where object=? and source_id=?", strValue, strObject, getID() );
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
	    
	    String fName = makeFileName( oAttrValue );	  
	    String fOldName = RhodesApp.getInstance().resolveDBFilesPath(strDbValue);
	    RhoClassFactory.createFile().renameOverwrite(fOldName, fName); 
	    
	    oAttrValue.m_strValue = FilePath.getRelativePath( fName, RhodesApp.getInstance().getRhoRootPath());
	    return true;
	}
	
	void processServerCmd_Ver3(String strCmd, String strObject, String strAttriba, String strValuea)throws Exception
	{
	    CAttrValue oAttrValue = new CAttrValue(strAttriba,strValuea);
		
	    if ( strCmd.compareTo("insert") == 0 )
	    {
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

	            // oo conflicts
	            getDB().executeSQL("UPDATE changed_values SET sent=4 where object=? and attrib=? and source_id=? and sent>1", 
	                    strObject, oAttrValue.m_strAttrib, getID() );
	            //
	        }

	        getNotify().onObjectChanged(getID(),strObject, SyncNotify.enUpdate);
	        m_nInserted++;
	    }else if (strCmd.compareTo("delete") == 0)
	    {
	        getDB().executeSQL("DELETE FROM object_values where object=? and attrib=? and source_id=?", strObject, oAttrValue.m_strAttrib, getID() );
	        getNotify().onObjectChanged(getID(), strObject, SyncNotify.enDelete);
	        // oo conflicts
	        getDB().executeSQL("UPDATE changed_values SET sent=3 where object=? and attrib=? and source_id=?", strObject, oAttrValue.m_strAttrib, getID() );
	        //

	        m_nDeleted++;
	    }else if ( strCmd.compareTo("links") == 0 )
	    {
	        processAssociations(strObject, oAttrValue.m_strValue);

	        getDB().executeSQL("UPDATE object_values SET object=? where object=? and source_id=?", oAttrValue.m_strValue, strObject, getID() );
	        getDB().executeSQL("UPDATE changed_values SET object=?,sent=3 where object=? and source_id=?", oAttrValue.m_strValue, strObject, getID() );

	        getNotify().onObjectChanged(getID(), strObject, SyncNotify.enCreate);
	    }

	}
	
	private String makeFileName(CAttrValue value)throws Exception
	{
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
		
		return fName;
	}
	
	boolean downloadBlob(CAttrValue value)throws Exception
	{
		String fName = makeFileName( value );
		String url = value.m_strValue;
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
	        	getSync().stopSync();
	        	m_nErrCode = RhoAppAdapter.getErrorFromResponse(resp);
	        	return false;
	        }
	    }catch(Exception exc)
	    {
	    	m_nErrCode = RhoAppAdapter.getNetErrorCode(exc);
	    	throw exc;
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
