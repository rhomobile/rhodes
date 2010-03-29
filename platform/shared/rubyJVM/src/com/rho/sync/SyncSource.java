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
import com.rho.net.*;
import com.rho.db.*;
import java.util.Vector;

import org.json.me.JSONException;

import com.rho.FilePath;
import com.rho.TimeInterval;
import com.rho.RhoRuby;

class SyncSource
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("Sync");
	
	private static final RhoProfiler PROF = RhoProfiler.RHO_STRIP_PROFILER ? new RhoEmptyProfiler() : 
		new RhoProfiler();

	static class SyncBlob
	{
	   String m_strBody;
	   String m_strFilePath;

	   SyncBlob(String body, String filePath ){
	      m_strBody = body; m_strFilePath = filePath;
	   }

	   String getBody(){ return m_strBody; }
	   String getFilePath(){ return m_strFilePath; }
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
    int m_nErrCode = RhoRuby.ERR_NONE;
    String m_strError = "";
    public boolean m_bIsSearch = false;
    
	//String m_strPushBody = "";
    Vector/*Ptr<CSyncBlob*>*/ m_arSyncBlobs = new Vector();
    
    int m_nRefreshTime = 0;
    int m_nProgressStep = -1;
    
    Integer getID() { return m_nID; }
    String getName() { return m_strName; }
    String getSyncType(){ return m_strSyncType; }
    int getServerObjectsCount(){ return m_nInserted+m_nDeleted; }
    boolean isSearch(){ return m_bIsSearch;}
    
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

    SyncSource(SyncEngine syncEngine, DBAdapter db)
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

        m_nErrCode = RhoRuby.ERR_NONE;
        m_bIsSearch = false;
    }
	
    SyncSource(int id, String name, long token, String strSyncType, DBAdapter db, SyncEngine syncEngine )
    {
    	m_syncEngine = syncEngine;
    	m_dbAdapter = db;
        m_nID = new Integer(id);
        m_strName = name;
        m_token = token;
        m_strSyncType = strSyncType;
        m_bTokenFromDB = true;
        
        m_nCurPageCount = 0;
        m_nInserted = 0;
        m_nDeleted = 0;
        m_nTotalCount = 0;
        m_bGetAtLeastOnePage = false;

        m_nErrCode = RhoRuby.ERR_NONE;
        m_bIsSearch = false;
    }
	
	void sync() throws Exception
	{
    	getNotify().fireSyncNotification(null, false, RhoRuby.ERR_NONE, RhoRuby.getMessageText("syncronizing") + getName() + "...");
		
	    TimeInterval startTime = TimeInterval.getCurrentTime();
	    m_bIsSearch = false;
	    
	    try{
	        if ( isEmptyToken() )
	            processToken(1);
	    	
	        boolean bSyncedServer = syncClientChanges();
	        if ( !bSyncedServer )
	        	syncServerChanges();
	        
	    }catch(Exception exc)
	    {
	    	getSync().stopSync();
	    	throw exc;
	    }finally{
		   TimeInterval endTime = TimeInterval.getCurrentTime();
		   getDB().executeSQL("UPDATE sources set last_updated=?,last_inserted_size=?,last_deleted_size=?, "+
								 "last_sync_duration=?,last_sync_success=?, backend_refresh_time=? WHERE source_id=?", 
		                         new Long(endTime.toULong()/1000), new Integer(getInsertedCount()), new Integer(getDeletedCount()), new Long((endTime.minus(startTime)).toULong()), 
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
	
	void syncClientBlobs(String strBaseQuery)throws Exception
	{
	    for( int i = 0; i < m_arSyncBlobs.size(); i ++)
	    {
	        SyncBlob blob = (SyncBlob)m_arSyncBlobs.elementAt(i);
	
	        String strFilePath = "";
	        if (blob.getFilePath()!=null && blob.getFilePath().length()>0)
	        {
	        	RhoClassFactory.createFile().getDirPath("");
	        	strFilePath += blob.getFilePath();
	        }

            String strQuery = strBaseQuery + "&" + blob.getBody();
	        try{
	            NetResponse resp = getNet().pushFile( strQuery, strFilePath, getSync(), null) ;
		        if ( !resp.isOK() )
		        {
		            getSync().setState(SyncEngine.esStop);
		        	m_nErrCode = RhoRuby.ERR_REMOTESERVER;
		        	//m_strError = resp.getCharData();
		            return;
		        }
		    }catch(Exception exc)
		    {
		    	m_nErrCode = RhoRuby.getNetErrorCode(exc);
		    	throw exc;
		    }
	
	        getDB().executeSQL("DELETE FROM object_values WHERE source_id=? and attrib_type=? and value=?", getID(), "blob.file", blob.getFilePath() );
	    }
	
	    m_arSyncBlobs.removeAllElements();
	}

	void doSyncClientChanges()throws Exception
	{
	    String[] arUpdateTypes = {"create", "update", "delete"};
	    boolean arUpdateSent[] = {false, false, false};

	    m_arSyncBlobs.removeAllElements();
	    String strBody = "{\"source_name\":\"" + getName() + "\",\"client_id\":\"" + getSync().getClientID() + "\"";
	    boolean bSend = false;
	    for( int i = 0; i < 3 && getSync().isContinueSync(); i++ )
	    {
	        String strBody1 = makePushBody_Ver3(arUpdateTypes[i]);
	        if (strBody1.length() > 0)
	        {
	            strBody += "," + strBody1;
	            arUpdateSent[i] = true;
	            bSend = true;
	        }
	    }
	    strBody += "}";

	    if ( bSend )
	    {
	        LOG.INFO("Push client changes to server. Source: " + getName() + "Size :" + strBody.length() );
	        LOG.TRACE("Push body: " + strBody);		

	        NetResponse resp = getNet().pushData(
	            getProtocol().getClientChangesUrl(getName(), "", getSync().getClientID()),strBody, getSync());
	        if ( !resp.isOK() )
	        {
	            getSync().setState(SyncEngine.esStop);
	            m_nErrCode = RhoRuby.ERR_REMOTESERVER;
	        }
	    }

	    afterSyncClientChanges(arUpdateSent);
	}
	
	void afterSyncClientChanges(boolean arUpdateSent[])throws Exception
	{
	    String arUpdateTypes[] = {"create", "update", "delete"};
	    for( int i = 0; i < 3 && getSync().isContinueSync(); i++ )
	    {
	        if ( m_arSyncBlobs.size() > 0 )
	        {
			    LOG.INFO( "Push blobs to server. Source: " + getName() + "Count :" + m_arSyncBlobs.size() );
	            //oo conflicts
	            if ( i < 1 ) //create
	                getDB().executeSQL("UPDATE changed_values SET sent=2 WHERE source_id=? and update_type=? and (attrib_type IS NULL or attrib_type!=?) and sent=1", 
	                    getID(), arUpdateTypes[i], "blob.file" );
	            else
	            //
	                getDB().executeSQL("DELETE FROM changed_values WHERE source_id=? and update_type=? and (attrib_type IS NULL or attrib_type!=? and sent=1)", 
	                    getID(), arUpdateTypes[i], "blob.file" );

	            //TODO: sync blobs ver3
	            syncClientBlobs(getProtocol().getClientChangesUrl(getName(), arUpdateTypes[i], getSync().getClientID()));
	        }else if ( arUpdateSent[i] )
	        {
	            //oo conflicts
	            if ( i < 1 ) //create
	                getDB().executeSQL("UPDATE changed_values SET sent=2 WHERE source_id=? and update_type=? and sent=1", getID(), arUpdateTypes[i] );
	            else
	            //
	                getDB().executeSQL("DELETE FROM changed_values WHERE source_id=? and update_type=? and sent=1", getID(), arUpdateTypes[i] );
	        }
	    }
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
	            FilePath oBlobPath = new FilePath(value);

	            String strBlobBody = "{\"source_name\":\"" + getName() + "\",\"client_id\":\"" + getSync().getClientID() + "\",";
	            strBlobBody += "\"" + strUpdateType + "\":{";
	            strBlobBody += "\"" + strObject + "\":{";
	            strBlobBody += "\"" + strAttrib + "\":\"" + oBlobPath.getBaseName() + "\"";
	            //TODO: attrib_type set to blob?
	            //strSrcBody += "&attrvals[][attrib_type]=blob";

	            strBlobBody += "}}}";

	            m_arSyncBlobs.addElement(new SyncBlob(strBlobBody,value));

	            continue;
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
	            strBody += "\"" + strObject + "\"";
	            strCurObject = strObject;
	        }
	            
	        if (!bFirst)
	            strBody += ",";

	        if ( strAttrib.length() > 0  )
	        {
	            if ( bFirst )
	                strBody += ":{";

	            strBody += "\"" + strAttrib + "\":\"" + value + "\"";
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
					m_nErrCode = RhoRuby.ERR_NETWORK;
		            continue;
		        }
		    }catch(Exception exc)
		    {
		    	m_nErrCode = RhoRuby.getNetErrorCode(exc);
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
	        m_nErrCode = RhoRuby.ERR_UNEXPECTEDSERVERRESPONSE;
	        return;
	    }

	    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("source") )
	    {
	        //skip it. it uses in search only
	        oJsonArr.next();
	    }

	    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("token"))
	    {
	        processToken(oJsonArr.getCurItem().getUInt64("token"));
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
	        m_nErrCode = RhoRuby.ERR_CUSTOMSYNCSERVER;
	        getSync().stopSync();
	        return;
	    }*/

	    //if ( getServerObjectsCount() == 0 )
	    //    getNotify().fireSyncNotification(this, false, RhoRuby.ERR_NONE, "");

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
	        JSONStructIterator iterCmds = new JSONStructIterator(oJsonArr.getCurItem());
	        if ( !iterCmds.isEnd() )
	        {
	            PROF.START("Data");
	            //TODO: support DBExceptions
	            getDB().startTransaction();

	            for( ; !iterCmds.isEnd() && getSync().isContinueSync(); iterCmds.next() )
	            {
	                String strCmd = iterCmds.getCurKey();
	                if ( strCmd.compareTo("metadata") == 0 )
                    {
                        String strMetadata = iterCmds.getCurValue().getString();
                        getDB().executeSQL("UPDATE sources SET metadata=? WHERE source_id=?", strMetadata, getID() );
                    }else if ( strCmd.compareTo("links") == 0 || strCmd.compareTo("delete") == 0 || strCmd.compareTo("insert") == 0)
	                {
	                    JSONStructIterator objIter = new JSONStructIterator(iterCmds.getCurValue());

	                    for( ; !objIter.isEnd() && getSync().isContinueSync(); objIter.next() )
	                    {
	                        String strObject = objIter.getCurKey();
	                        JSONStructIterator attrIter = new JSONStructIterator( objIter.getCurValue() );
	                        for( ; !attrIter.isEnd() && getSync().isContinueSync(); attrIter.next() )
	                        {
	                            String strAttrib = attrIter.getCurKey();
	                            String strValue = attrIter.getCurString();

	                            processServerCmd_Ver3(strCmd,strObject,strAttrib,strValue);
	                        }

	                        int nSyncObjectCount  = getNotify().incLastSyncObjectCount(getID());
	                        if ( getProgressStep() > 0 && (nSyncObjectCount%getProgressStep() == 0) )
	                            getNotify().fireSyncNotification(this, false, RhoRuby.ERR_NONE, "");
	                    }
	                }
	            }

		        PROF.STOP("Data");		    
	    	    PROF.START("DB");
	            getDB().endTransaction();
		        PROF.STOP("DB");

	            getNotify().fireObjectsNotification();
	        }
	    }

		PROF.START("Data1");
	    if ( getCurPageCount() > 0 )
	        getNotify().fireSyncNotification(this, false, RhoRuby.ERR_NONE, "");
		PROF.STOP("Data1");
	}
	
	void processServerCmd_Ver3(String strCmd, String strObject, String strAttrib, String strValue)throws Exception
	{
	    if ( strCmd.compareTo("insert") == 0 )
	    {
	        //TODO: blobs
	        //CValue value(oJsonEntry,1);
	        //if ( !downloadBlob(value) )
		    //    return false;
	        
	        IDBResult resInsert = getDB().executeSQLReportNonUnique("INSERT INTO object_values "+
	            "(attrib, source_id, object, value, attrib_type) VALUES(?,?,?,?,?)", 
	             strAttrib, getID(), strObject, strValue, "" );
	        if ( resInsert.isNonUnique() )
	        {
	            getDB().executeSQL("UPDATE object_values " +
	                "SET value=?, attrib_type=? WHERE object=? and attrib=? and source_id=?", 
	                strValue, "", strObject, strAttrib, getID() );

	            // oo conflicts
	            getDB().executeSQL("UPDATE changed_values SET sent=4 where object=? and attrib=? and source_id=? and sent>1", strObject, strAttrib, getID() );
	            //
	        }

	        getNotify().onObjectChanged(getID(),strObject, SyncNotify.enUpdate);
	        m_nInserted++;
	    }else if (strCmd.compareTo("delete") == 0)
	    {
	        getDB().executeSQL("DELETE FROM object_values where object=? and attrib=? and source_id=?", strObject, strAttrib, getID() );
	        getNotify().onObjectChanged(getID(), strObject, SyncNotify.enDelete);
	        // oo conflicts
	        getDB().executeSQL("UPDATE changed_values SET sent=3 where object=? and attrib=? and source_id=?", strObject, strAttrib, getID() );
	        //

	        m_nDeleted++;
	    }else if ( strCmd.compareTo("links") == 0 )
	    {
	        getDB().executeSQL("UPDATE object_values SET object=? where object=? and source_id=?", strValue, strObject, getID() );
	        getDB().executeSQL("UPDATE changed_values SET object=?,sent=3 where object=? and source_id=?", strValue, strObject, getID() );

	        getNotify().onObjectChanged(getID(), strObject, SyncNotify.enCreate);
	    }

	}
	
	class CValue
	{
	    String m_strValue;
		String m_strAttrType;
		Long   m_nID;
		
/*		CValue(JSONEntry oJsonEntry)throws JSONException
		{
			m_strValue = oJsonEntry.getString("value");
			m_strAttrType = oJsonEntry.getString("attrib_type");
			m_nID = new Long(oJsonEntry.getLong("id"));
		}
		CValue(JSONEntry oJsonEntry, int nVer)throws JSONException
		{
			if ( nVer == 1 )
			{
				m_strValue = oJsonEntry.getString("v");
				m_strAttrType = oJsonEntry.getString("t");
				m_nID = new Long(oJsonEntry.getLong("i"));
			}
		}*/
		
	}
	
	private String makeFileName(CValue value)throws Exception
	{
		String fName = DBAdapter.makeBlobFolderName();
		
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
		
		fName += "/id_" + value.m_nID.toString() + strExt;
		
		return fName;
	}
	
	boolean downloadBlob(CValue value)throws Exception
	{
		if ( value.m_strAttrType == null || !value.m_strAttrType.equals("blob.url")  )
			return true;
		
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
	        	m_nErrCode = RhoRuby.ERR_REMOTESERVER;
	        	//m_strError = resp.getCharData();
	        	return false;
	        }
	    }catch(Exception exc)
	    {
	    	m_nErrCode = RhoRuby.getNetErrorCode(exc);
	    	throw exc;
	    }
        
        value.m_strAttrType = "blob.file";
        
    	String root = RhoClassFactory.createFile().getDirPath("");
        
        value.m_strValue = "/" + fName.substring(root.length());;
        
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
