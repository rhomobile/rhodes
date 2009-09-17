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

    Integer  m_nID;
    String m_strName;
    String m_strUrl;
    String m_token;
    boolean m_bTokenFromDB; 
    
    int m_nCurPageCount, m_nInserted, m_nDeleted, m_nTotalCount;
    boolean m_bGetAtLeastOnePage = false;
    int m_nErrCode = RhoRuby.ERR_NONE;
    String m_strError = "";
    String m_strParams = "";
    String m_strAction = "";
    
	String m_strPushBody = "";
    Vector/*Ptr<CSyncBlob*>*/ m_arSyncBlobs = new Vector();
    
    String m_strAskParams = "";

    String getUrl() { return m_strUrl; }
    Integer getID() { return m_nID; }
    String getName() { return m_strName; }
    int getServerObjectsCount(){ return m_nInserted+m_nDeleted; }

    String getToken(){ return m_token; }
    void setToken(String token){ m_token = token; m_bTokenFromDB = false; }
    boolean isEmptyToken()
    {
        return m_token == null || m_token.length() == 0 || m_token.equals("0");
    }
    boolean isTokenMoreThanOne()
    {
    	return m_token != null && m_token.length() > 0 && !m_token.equals("1")&& !m_token.equals("0"); 
    }
    void setAskParams(String ask){ m_strAskParams = ask;}
    String getAskParams(){ return m_strAskParams;}
    int getInsertedCount() { return m_nInserted; }
    int getDeletedCount() { return m_nDeleted; }
    void setCurPageCount(int nCurPageCount){m_nCurPageCount = nCurPageCount;}
    void setTotalCount(int nTotalCount){m_nTotalCount = nTotalCount;}
    int  getCurPageCount(){return m_nCurPageCount;}
    int  getTotalCount(){return m_nTotalCount;}
    
    SyncEngine getSync(){ return m_syncEngine; }
	DBAdapter getDB(){ return getSync().getDB(); }
	NetRequest getNet(){ return getSync().getNet(); }

    SyncSource(int id, String strUrl, String name, String token, SyncEngine syncEngine )
    {
    	m_syncEngine = syncEngine;
        m_nID = new Integer(id);
        m_strUrl = strUrl;
        m_strName = name;
        m_token = token;
        m_bTokenFromDB = true;
        
        m_nCurPageCount = 0;
        m_nInserted = 0;
        m_nDeleted = 0;
    }
	
    SyncSource(SyncEngine syncEngine)
    {
    	m_syncEngine = syncEngine;
        m_nID = new Integer(0);
        m_strUrl = "";
        m_strName = "";
        m_token = "";
        m_bTokenFromDB = true;
        
        m_nCurPageCount = 0;
        m_nInserted = 0;
        m_nDeleted = 0;
    }
    
	void sync() throws Exception
	{
    	m_syncEngine.fireNotification(this, false, RhoRuby.ERR_NONE, "Synchronizing " + getName() + "...");
		
	    TimeInterval startTime = TimeInterval.getCurrentTime();
	    
	    try{
	        if ( m_strParams.length() == 0 )
	        {
			    syncClientChanges();
			    getAndremoveAsk();
	        }
		    syncServerChanges();
	    }catch(Exception exc)
	    {
	    	getSync().stopSync();
	    	throw exc;
	    }finally{
		    TimeInterval endTime = TimeInterval.getCurrentTime();
		    getDB().executeSQL("UPDATE sources set last_updated=?,last_inserted_size=?,last_deleted_size=?, "+
								 "last_sync_duration=?,last_sync_success=? WHERE source_id=?", 
		                         new Long(endTime.toULong()), new Integer(getInsertedCount()), new Integer(getDeletedCount()), new Long((endTime.minus(startTime)).toULong()), 
		                         new Integer(m_bGetAtLeastOnePage?1:0), getID() );
		    
		    String profName = "File operations: " + getName();
		    //PROF.FLUSH_COUNTER(RhoProfiler.FILE_SYNC, profName);
		    //PROF.FLUSH_COUNTER(RhoProfiler.FILE_READ, profName);
		    //PROF.FLUSH_COUNTER(RhoProfiler.FILE_WRITE, profName);
		    //PROF.FLUSH_COUNTER(RhoProfiler.FILE_SET_SIZE, profName);
		    //PROF.FLUSH_COUNTER(RhoProfiler.FILE_DELETE, profName);
		    //PROF.FLUSH_COUNTER(RhoProfiler.FILE_RENAME, profName);
	    }
	}

	void syncClientBlobs(String strBaseQuery)throws Exception
	{
	    String strQuery;
	    for( int i = 0; i < m_arSyncBlobs.size(); i ++)
	    {
	        SyncBlob blob = (SyncBlob)m_arSyncBlobs.elementAt(i);
	
	        String strFilePath = RhoClassFactory.createFile().getDirPath("");
	        strFilePath += blob.getFilePath();
	        
	        strQuery = strBaseQuery + "&" + blob.getBody();
	        try{
		        NetResponse resp = getNet().pushFile(strQuery, strFilePath, getSync() );
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

	void syncClientChanges()throws Exception
	{
	    String[] arUpdateTypes = {"create", "update", "delete"};
	    for( int i = 0; i < 3 && getSync().isContinueSync(); i++ )
	    {
	        String strUrl = getUrl() + "/" + arUpdateTypes[i];
	        strUrl += "objects";
	        String strQuery = SyncEngine.SYNC_SOURCE_FORMAT() + "&client_id=" + getSync().getClientID();

	        m_arSyncBlobs.removeAllElements();
	        String strBody = makePushBody(arUpdateTypes[i]);
	        if ( strBody.length() > 0 )
	        {
			    LOG.INFO( "Push client changes to server. Source id: " + getID() + "Size :" + strBody.length() );
			    LOG.TRACE("Push body: " + strBody);		
	 
	            NetResponse resp = getNet().pushData(strUrl+strQuery,strBody, getSync());
	            if ( !resp.isOK() )
	            {
	                getSync().setState(SyncEngine.esStop);
	                continue;
	            }
	        }
	 
	        if ( m_arSyncBlobs.size()>0 )
	        {
			    LOG.INFO( "Push blobs to server. Source id: " + getID() + "Count :" + m_arSyncBlobs.size() );

	            if ( i <= 1 ) //create, update
	            {
	                IDBResult res = getDB().executeSQL("SELECT object, attrib " +
						     "FROM changed_values WHERE source_id=? and update_type=? and (attrib_type IS NULL or attrib_type!=?)", 
	                    getID(), arUpdateTypes[i], "blob.file" );
	                if ( !res.isEnd() )
	                {
	                    getDB().startTransaction();

		                for( ; !res.isEnd(); res.next() )
		                {
		                    getDB().executeSQL("DELETE FROM object_values WHERE object=? and attrib=? and source_id=?", 
		                        res.getStringByIdx(0), res.getStringByIdx(1), getID() );
	                        RhoRuby.RhomAttribManager_delete_attrib(getID(),res.getStringByIdx(1));
		                }
		                    
	                    RhoRuby.RhomAttribManager_save(getID());
	                    getDB().endTransaction();
	                }
	            }

	            getDB().executeSQL("DELETE FROM changed_values WHERE source_id=? and update_type=? and (attrib_type IS NULL or attrib_type!=?)", 
	                getID(), arUpdateTypes[i], "blob.file" );
	            syncClientBlobs(strUrl+strQuery);
	        }else if ( strBody.length() > 0 )
	        {
	        	if ( i <= 1 ) //create, update
	            {
	                IDBResult res = getDB().executeSQL("SELECT object, attrib "+
						     "FROM changed_values where source_id=? and update_type =?", getID(), arUpdateTypes[i] );
	                if ( !res.isEnd() )
	                {
	                    getDB().startTransaction();

		                for( ; !res.isEnd(); res.next() )
		                {
		                    getDB().executeSQL("DELETE FROM object_values WHERE object=? and attrib=? and source_id=?", 
		                        res.getStringByIdx(0), res.getStringByIdx(1), getID() );
	                        RhoRuby.RhomAttribManager_delete_attrib(getID(),res.getStringByIdx(1));
		                }
	                    RhoRuby.RhomAttribManager_save(getID());
	                    getDB().endTransaction();
	                }
	            }

	            getDB().executeSQL("DELETE FROM changed_values WHERE source_id=? and update_type=?", getID(), arUpdateTypes[i] );
	        }
	    }
	}
	
	/*
	 * Construct the body of the request by filtering 
	 * the attr_filter string. The body format should
	 * look like the following:
	 * create: attrvals[][attrib]=<name|industry>&attrvals[][object]=<locallygeneratedid>&attrvals[][value]=<some value>
	 * update: attrvals[][attrib]=<name|industry>&attrvals[][object]=<remoteid>&attrvals[][value]=<some new value>
	 * delete: attrvals[][attrib]=<name|industry>&attrvals[][object]=<remoteid>
	 */
	
	String makePushBody( String szUpdateType)throws DBException
	{
		String strBody = "";
	    IDBResult res = getDB().executeSQL("SELECT attrib, object, value, attrib_type "+
						 "FROM changed_values where source_id=? and update_type =?", getID(), szUpdateType );
	    for( ; !res.isEnd(); res.next() )
	    {
	        String strSrcBody = "attrvals[][attrib]=" + res.getStringByIdx(0);
	
	        if ( res.getStringByIdx(1).length() > 0 ) 
	            strSrcBody += "&attrvals[][object]=" + res.getStringByIdx(1);
	
	        String value = res.getStringByIdx(2);
	        String attribType = res.getStringByIdx(3);
	
	
	        //if ( value.length() > 0 )
	        {
	            if ( attribType.equals("blob.file") )
	            {
	                FilePath oBlobPath = new FilePath(value);
	                strSrcBody += "&attrvals[][value]=";
	                strSrcBody += oBlobPath.getBaseName();
	                strSrcBody += "&attrvals[][attrib_type]=blob";
	
	                if ( value.length() > 0 )
	                	m_arSyncBlobs.addElement(new SyncBlob(strSrcBody,value));
	                continue;
	            }else
	                strSrcBody += "&attrvals[][value]=" + value;
	        }
	
	        if ( strBody.length() > 0 )
	            strBody += "&";
	
	        strBody += strSrcBody;
	    }
	    
	    return strBody;
	}

	void getAndremoveAsk()throws DBException
	{
	    String askParams = "";
	    {
	        IDBResult res = getDB().executeSQL("SELECT object, attrib, value "+
				     "FROM changed_values WHERE source_id=? and update_type =?", getID(), "ask" );
	        if ( !res.isEnd() )
	        {
	            askParams = res.getStringByIdx(2);

	            getDB().executeSQL("DELETE FROM object_values WHERE object=? and attrib=? and source_id=?", 
	                res.getStringByIdx(0), res.getStringByIdx(1), getID() );
	        }
	    }

	    getDB().executeSQL("DELETE FROM changed_values WHERE source_id=? and update_type=?", getID(), "ask" );

	    setAskParams(askParams);
	}
	
	void syncServerChanges()throws Exception
	{
		LOG.INFO("Sync server changes source ID :" + getID() );
		
	    while( getSync().isContinueSync() )
	    {
	        setCurPageCount(0);
	        String strUrl = getUrl();
	        if ( m_strAction.length() > 0 )
	            strUrl += '/' + m_strAction;
	        
			String strQuery = SyncEngine.SYNC_SOURCE_FORMAT() + "&client_id=" + getSync().getClientID() + 
	                "&p_size=" + SyncEngine.SYNC_PAGE_SIZE() + "&version=1";
	        if ( m_strParams.length() > 0 )
	            strQuery += m_strParams;
	
	        if ( getAskParams().length() > 0 )
	        {
	            getSync();
				strUrl +=  SyncEngine.SYNC_ASK_ACTION();
	            strQuery += "&question=" + getAskParams();
	        }
	
	        if ( isEmptyToken() )
	            processToken("1");
	        else if ( !m_bTokenFromDB && isTokenMoreThanOne() )
	            strQuery += "&ack_token=" + getToken();
	
			LOG.INFO( "Pull changes from server. Url: " + (strUrl+strQuery) );
			
			NetResponse resp = null;
			try{
			    PROF.START("Net");	    
				resp = getNet().pullData(strUrl+strQuery, "", getSync());
				PROF.STOP("Net");
				
		        if ( !resp.isOK() )
		        {
		            getSync().stopSync();
		        	m_nErrCode = RhoRuby.ERR_REMOTESERVER;
		        	//m_strError = resp.getCharData();
		            
		            continue;
		        }
		    }catch(Exception exc)
		    {
		    	m_nErrCode = RhoRuby.getNetErrorCode(exc);
		    	throw exc;
		    }
	
	        processServerData(resp.getCharData());
	        
	        /*String strData =
			"[{count: 124},{version: 1},{total_count: 5425},{token: 123}," +
	        "{s:\"AeropriseRequest\",ol:[" +
			"{o:\"2ed2e0c7-8c4c-99c6-1b37-498d250bb8e7\",av:[" +
			"{a:\"first_name\",i:47354289,v:\"Lars. \n\n Burgess\", t:\"blob\"}," +
	        "{a:\"second_name\",i:55555,v:\"Burgess\"}" +
	        "]}" +
	        "]}]";
			//u:\"query\",
	        processServerData(strData);*/
	        
	        if ( getAskParams().length() > 0 || getCurPageCount() == 0 )
	            break;
	    }
	}

	void processServerData(String szData)throws Exception
	{
		PROF.START("Parse");		
	    JSONArrayIterator oJsonArr = new JSONArrayIterator(szData);
	    PROF.STOP("Parse");
	    
	    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("error") )
	    {
	        m_strError = oJsonArr.getCurItem().getString("error");
	        m_nErrCode = RhoRuby.ERR_CUSTOMSYNCSERVER;
	        processToken("0");
	        getSync().stopSync();
	        return;
	    }
	    
	    if ( !oJsonArr.isEnd() )
	    {
	        setCurPageCount(oJsonArr.getCurItem().getInt("count"));
	        oJsonArr.next();
	    }
	    int nVersion = 0;
	    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("version") )
	    {
	        nVersion = oJsonArr.getCurItem().getInt("version");
	        oJsonArr.next();
	    }
	    
	    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("total_count") )
	    {
	        setTotalCount(oJsonArr.getCurItem().getInt("total_count"));
	        oJsonArr.next();
	    }
	    
	    if ( getServerObjectsCount() == 0 )
	    	m_syncEngine.fireNotification(this, false, RhoRuby.ERR_NONE, "");
	    
	    if ( !oJsonArr.isEnd() )
	    {
	        processToken(oJsonArr.getCurItem().getUInt64("token"));
	        oJsonArr.next();
	    }else if ( getCurPageCount() == 0 )
	        processToken("0");

		LOG.INFO( "Got " + getCurPageCount() + " records of " + getTotalCount() + " from server. Source ID: " + getID()
				+ ". Version: " + nVersion );
		
		if ( !oJsonArr.isEnd() && getSync().isContinueSync() )
		{
		    //TODO: support DBExceptions
			PROF.START("Data");
		    getDB().startTransaction();

		    try{
		        if ( nVersion == 0 )
		            processServerData_Ver0(oJsonArr);
		        else
		            processServerData_Ver1(oJsonArr);
		    }finally{
			    PROF.STOP("Data");		    
		    	
		    	PROF.START("DB");
				getDB().endTransaction();
			    PROF.STOP("DB");		    
			}
		    
		}
		
	    if ( getServerObjectsCount() < getTotalCount() )
	    	m_syncEngine.fireNotification(this, false, RhoRuby.ERR_NONE, "");
	}

	void processServerData_Ver0(JSONArrayIterator oJsonArr)throws Exception
	{
	    try{
		    for( ; !oJsonArr.isEnd() && getSync().isContinueSync(); oJsonArr.next() )
		    {
		        if ( getDB().isUnlockDB() )
		        {
					LOG.INFO( "Commit transaction because of UI request." );
					RhoRuby.RhomAttribManager_save(getID());
		            getDB().endTransaction();
		            getDB().startTransaction();
		        }
		
		        JSONEntry oJsonObject = oJsonArr.getCurItem();
		        if( !processSyncObject(oJsonObject))
	            {
		            getSync().stopSync();
		            break;
	            }
		        
		        m_bGetAtLeastOnePage = true;
		    }
	    }finally{
	    	RhoRuby.RhomAttribManager_save(getID());
		}
	}

	void processServerData_Ver1(JSONArrayIterator oJsonArr)throws Exception
	{
	    for( ; !oJsonArr.isEnd() && getSync().isContinueSync(); oJsonArr.next() )
	    {
	        JSONEntry oJsonSource = oJsonArr.getCurItem();
	        String strSrcName = oJsonSource.getString("s");
	        Integer nSrcID = getID();
	        if ( strSrcName.compareTo("RhoDeleteSource") == 0 )
	            nSrcID = new Integer(-1);
	        else if ( strSrcName.compareTo(getName()) != 0 )
	        {
	            SyncSource pSrc = getSync().findSourceByName(strSrcName);
	            if ( pSrc == null )
	            {
	                LOG.ERROR("Sync server send data for unknown source name:" + strSrcName);
	                continue;
	            }
	            nSrcID = pSrc.getID();
	        }

	        JSONArrayIterator oJsonObjList = new JSONArrayIterator(oJsonSource, "ol");
	        try{
		        for( ; !oJsonObjList.isEnd() && getSync().isContinueSync(); oJsonObjList.next() )
		        {
			        if ( getDB().isUnlockDB() )
			        {
						LOG.INFO( "Commit transaction because of UI request." );
						RhoRuby.RhomAttribManager_save(getID());
			            getDB().endTransaction();
			            getDB().startTransaction();
			        }
			
			        JSONEntry oJsonObject = oJsonObjList.getCurItem();
			        if( !processSyncObject_ver1(oJsonObject,nSrcID))
		            {
			            getSync().stopSync();
			            break;
		            }
			        
			        m_bGetAtLeastOnePage = true;
		        }
		    }finally{
		    	RhoRuby.RhomAttribManager_save(getID());
			}
	    }
	}
	
	class CValue
	{
	    String m_strValue;
		String m_strAttrType;
		long   m_nID;
		
		CValue(JSONEntry oJsonEntry)throws JSONException
		{
			m_strValue = oJsonEntry.getString("value");
			m_strAttrType = oJsonEntry.getString("attrib_type");
			m_nID = oJsonEntry.getLong("id");
		}
		CValue(JSONEntry oJsonEntry, int nVer)throws JSONException
		{
			if ( nVer == 1 )
			{
				m_strValue = oJsonEntry.getString("v");
				m_strAttrType = oJsonEntry.getString("t");
				m_nID = oJsonEntry.getLong("i");
			}
		}
		
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
		
		fName += "/id_" + Long.toString(value.m_nID) + strExt;
		
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
			NetResponse resp = getNet().pullFile(url, fName, getSync());
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
	
	boolean processSyncObject_ver1(JSONEntry oJsonObject, Integer nSrcID)throws Exception
	{
		String strObject = oJsonObject.getString("o");
		JSONArrayIterator oJsonArr = new JSONArrayIterator(oJsonObject, "av");
		
	    for( ; !oJsonArr.isEnd() && getSync().isContinueSync(); oJsonArr.next() )
		{
			JSONEntry oJsonEntry = oJsonArr.getCurItem();
	        if ( oJsonEntry.isEmpty() )
	        	continue;
	        
		    //int nDbOp = oJsonEntry.getInt("d");
		    if ( nSrcID.intValue() >= 0 ) //insert
		    {
		    	CValue value = new CValue(oJsonEntry,1);
		    	if ( !downloadBlob(value) )
		    		return false;
		    	
		    	String strAttrib = oJsonEntry.getString("a");
		    	//String strUpdateType = "query";
		    	//if( oJsonEntry.hasName("u") )
		    	//	strUpdateType = oJsonEntry.getString("u");
		    	
		        getDB().executeSQL("INSERT INTO object_values " +
		            "(id, attrib, source_id, object, value, attrib_type) VALUES(?,?,?,?,?,?)", 
		            new Long(value.m_nID), strAttrib, nSrcID, strObject,
		            value.m_strValue, value.m_strAttrType );
		        
		        RhoRuby.RhomAttribManager_add_attrib(nSrcID,strAttrib);
		        m_nInserted++;
		    }else// if ( nDbOp == 1 ) //delete
		    {
		    	long id = oJsonEntry.getLong("i");
	            IDBResult res = getDB().executeSQL("SELECT source_id FROM object_values where id=?", id );
	            if ( !res.isEnd() )
	            {
			        RhoRuby.RhomAttribManager_delete_attribs( res.getIntByIdx(0),id);
			        getDB().executeSQL("DELETE FROM object_values where id=?", id );
	            }
	            
		        m_nDeleted++;
		    }//else{
		     //   LOG.ERROR("Unknown DB operation: " + nDbOp );
		    //}
		}
		
		return true;
	}

	boolean processSyncObject(JSONEntry oJsonObject)throws Exception
	{
    	JSONEntry oJsonEntry = oJsonObject.getEntry("object_value");
        
        if ( oJsonEntry.isEmpty() )
        	return true;
        
	    String szDbOp = oJsonEntry.getString("db_operation");
	    if ( szDbOp != null && szDbOp.equals("insert") )
	    {
	    	CValue value = new CValue(oJsonEntry);
	    	//
	    	//value.m_strAttrType = "blob.url";
	    	//value.m_strValue = "http://img.gazeta.ru/files3/661/3219661/ld.jpg";
	    	//
	    	if ( !downloadBlob(value) )
	    		return false;
	    	
	    	String strAttrib = oJsonEntry.getString("attrib");
	        getDB().executeSQL("INSERT INTO object_values " +
	            "(id, attrib, source_id, object, value, attrib_type) VALUES(?,?,?,?,?,?)", 
	            new Long(value.m_nID), strAttrib, getID(), oJsonEntry.getString("object"),
	            value.m_strValue, value.m_strAttrType );
	        
	        RhoRuby.RhomAttribManager_add_attrib(getID(),strAttrib);
	        m_nInserted++;
	    }else if ( szDbOp != null && szDbOp.equals("delete") )
	    {
	    	long id = oJsonEntry.getLong("id");
	        RhoRuby.RhomAttribManager_delete_attribs(getID().longValue(),id);
	        getDB().executeSQL("DELETE FROM object_values where id=?", id );
	
	        m_nDeleted++;
	    }else{
	        LOG.ERROR("Unknown DB operation: " + (szDbOp != null ? szDbOp : "") );
	    }
	    
	    return true;
	}

	void processToken(String token)throws DBException
	{
	    if ( this.isTokenMoreThanOne() && getToken().equals(token) ){
			//Delete non-confirmed records
	    	
	        setToken( token ); //For m_bTokenFromDB = false;
	        //getDB().executeSQL("DELETE FROM object_values where source_id=? and token=?", getID(), token );
	        //TODO: add special table for id,token
		}else
	    {
	        setToken( token );
	        getDB().executeSQL("UPDATE sources SET token=? where source_id=?", token, getID() );
		}
	
	}
}

/*
void syncClientChanges()throws Exception
{
	LOG.INFO("Sync client changes source ID :" + getID() );
	
    IDBResult res = getDB().executeSQL("SELECT attrib, object, value, attrib_type, update_type "+
			 "FROM object_values where source_id=? and (update_type = 'update' or update_type = 'create' or update_type = 'delete') order by update_type", getID() );
    
    String strUpdateType = "";
	m_arSyncBlobs.removeAllElements();
	m_strPushBody = "";
	
    for( ; !res.isEnd()&& getSync().isContinueSync(); res.next() )
    {
    	String strTemp  = res.getStringByIdx(4);
    	if ( !strUpdateType.equals(strTemp) )
    	{
    		if ( strUpdateType.length() > 0 )
    		{
    			if ( !sendClientChanges(strUpdateType) )
    			{
    				getSync().setState(SyncEngine.esStop);
                	continue;
    			}	
    		}
    		
    		m_strPushBody = "";
    		strUpdateType = strTemp;
    	}
    	
    	makePushBody( res );
    }
    
    if ( getSync().isContinueSync() && strUpdateType.length() > 0 )
    	sendClientChanges(strUpdateType);
}

boolean sendClientChanges(String strUpdateType)throws Exception
{
    String strUrl = getUrl() + "/" + strUpdateType;
    strUrl += "objects";
    String strQuery = SyncEngine.SYNC_SOURCE_FORMAT() + "&client_id=" + getSync().getClientID();

    if ( m_strPushBody.length() > 0 )
    {
	    LOG.INFO("Push client changes to server. Source id: " + getID() + "Size :" + m_strPushBody.length());
	    LOG.TRACE( "Push body: " + m_strPushBody );		

	    try{
	    	NetResponse resp = getNet().pushData(strUrl+strQuery,m_strPushBody, getSync() );
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
    }

    if ( m_arSyncBlobs.size() > 0  )
    {
	    LOG.INFO( "Push blobs to server. Source id: " + getID() + "Count :" + m_arSyncBlobs.size() );

        getDB().executeSQL("DELETE FROM object_values WHERE source_id=? and update_type=? and (attrib_type IS NULL or attrib_type!=?)", getID(), strUpdateType, "blob.file" );
        syncClientBlobs(strUrl+strQuery);
    }else if ( m_strPushBody.length() > 0 )
        getDB().executeSQL("DELETE FROM object_values WHERE source_id=? and update_type=?", getID(), strUpdateType );
    
    return true;
}
*/	
/*	void makePushBody( IDBResult res )throws DBException
{
    String strSrcBody = "attrvals[][attrib]=" + res.getStringByIdx(0);

    if ( res.getStringByIdx(1).length() > 0 ) 
        strSrcBody += "&attrvals[][object]=" + res.getStringByIdx(1);

    String value = res.getStringByIdx(2);
    String attribType = res.getStringByIdx(3);

    //if ( value.length() > 0 )
    {
        if ( attribType.equals("blob.file") )
        {
            FilePath oBlobPath = new FilePath(value);
            strSrcBody += "&attrvals[][value]=";
            strSrcBody += oBlobPath.getBaseName();
            strSrcBody += "&attrvals[][attrib_type]=blob";

            if ( value.length() > 0 )
            	m_arSyncBlobs.addElement(new SyncBlob(strSrcBody,value));
            return;
        }else
            strSrcBody += "&attrvals[][value]=" + value;
    }

    if ( m_strPushBody.length() > 0 )
    	m_strPushBody += "&";

    m_strPushBody += strSrcBody;
}
*/
