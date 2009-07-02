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

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.net.*;
import com.rho.db.*;
import java.util.Vector;

import org.json.me.JSONException;

import com.rho.FilePath;
import com.rho.TimeInterval;

class SyncSource
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("Sync");

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
    String m_name;
    String m_strUrl;
    String m_token;

    int m_nCurPageCount, m_nInserted, m_nDeleted, m_nTotalCount;
    boolean m_bGetAtLeastOnePage = false;
    
    String m_strAskParams;
    Vector/*Ptr<CSyncBlob*>*/ m_arSyncBlobs = new Vector();

    String getUrl() { return m_strUrl; }
    Integer getID() { return m_nID; }
    String getName() { return m_name; }
    int getServerObjectsCount(){ return m_nInserted+m_nDeleted; }

    String getToken(){ return m_token; }
    void setToken(String token){ m_token = token; }
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
        m_name = name;
        m_token = token;

        m_nCurPageCount = 0;
        m_nInserted = 0;
        m_nDeleted = 0;
    }
	
	void sync(ISyncStatusListener statusListener) throws Exception
	{
	
		reportStatus( statusListener, "Synchronizing " + getName() + "...", 0);
		
	    TimeInterval startTime = TimeInterval.getCurrentTime();
		
	    try{
		    syncClientChanges();
		    getAndremoveAsk();
		    syncServerChanges();
	    }catch(Exception exc)
	    {
	    	reportStatus(statusListener, "Failed to synchronize " + getName(), 0);
	    	LOG.ERROR("sync failed", exc);
	    	getSync().stopSync();
	    }
	    
	    TimeInterval endTime = TimeInterval.getCurrentTime();
	    getDB().executeSQL("UPDATE sources set last_updated=?,last_inserted_size=?,last_deleted_size=?, "+
							 "last_sync_duration=?,last_sync_success=? WHERE source_id=?", 
	                         new Long(endTime.toULong()), new Integer(getInsertedCount()), new Integer(getDeletedCount()), new Long((endTime.minus(startTime)).toULong()), 
	                         new Integer(m_bGetAtLeastOnePage?1:0), getID() );
	}

	void syncClientBlobs(String strBaseQuery)throws Exception
	{
	    String strQuery;
	    for( int i = 0; i < m_arSyncBlobs.size(); i ++)
	    {
	        SyncBlob blob = (SyncBlob)m_arSyncBlobs.elementAt(i);
	
	        strQuery = strBaseQuery + "&" + blob.getBody();
	        if ( !getNet().pushFile(strQuery, blob.getFilePath(), getSync() ) )
	        {
	            getSync().setState(SyncEngine.esStop);
	            return;
	        }
	
	        getDB().executeSQL("DELETE FROM object_values WHERE source_id=? and attrib_type=? and value=?", getID(), "blob.file", blob.getFilePath() );
	    }
	
	    m_arSyncBlobs.removeAllElements();
	}

	void syncClientChanges()throws Exception
	{
		LOG.INFO("Sync client changes source ID :" + getID() );
	    String arUpdateTypes[] = {"update", "create", "delete"};
	    for( int i = 0; i < 3 && getSync().isContinueSync(); i++ )
	    {
	        String strUrl = getUrl() + "/" + arUpdateTypes[i];
	        strUrl += "objects";
	        String strQuery = SyncEngine.SYNC_SOURCE_FORMAT() + "&client_id=" + getSync().getClientID();
	
	        String strBody = "";
	        makePushBody(strBody, arUpdateTypes[i]);
	        if ( strBody.length() > 0 )
	        {
			    LOG.INFO("Push client changes to server. Source id: " + getID() + "Size :" + strBody.length());
			    LOG.TRACE( "Push body: " + strBody );		
	
	            if ( !getNet().pushData(strUrl+strQuery,strBody, getSync() ) )
	            {
	                getSync().setState(SyncEngine.esStop);
	                continue;
	            }
	        }
	
	        if ( m_arSyncBlobs.size() > 0  )
	        {
			    LOG.INFO( "Push blobs to server. Source id: " + getID() + "Count :" + m_arSyncBlobs.size() );
	
	            getDB().executeSQL("DELETE FROM object_values WHERE source_id=? and update_type=? and (attrib_type ISNULL or attrib_type!=?)", getID(), arUpdateTypes[i], "blob.file" );
	            syncClientBlobs(strUrl+strQuery);
	        }else
	            getDB().executeSQL("DELETE FROM object_values WHERE source_id=? and update_type=?", getID(), arUpdateTypes[i] );
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
	void makePushBody(String strBody, String szUpdateType)throws DBException
	{
	    //boolean bFirst = true;
	    IDBResult res = getDB().executeSQL("SELECT attrib, object, value, attrib_type "+
						 "FROM object_values where source_id=? and update_type =?", getID(), szUpdateType );
	    for( ; !res.isEnd(); res.next() )
	    {
	        String strSrcBody = "attrvals[][attrib]=" + res.getStringByIdx(0);
	
	        if ( res.getStringByIdx(1).length() > 0 ) 
	            strSrcBody += "&attrvals[][object]=" + res.getStringByIdx(1);
	
	        String value = res.getStringByIdx(2);
	        String attribType = res.getStringByIdx(3);
	
	        /*if ( bFirst )
	        {
	            value = "d:\\work\\blobtest.png";
	            attribType = "blob.file";
	            bFirst = false;
	        }*/
	
	        if ( value.length() > 0 )
	        {
	            if ( attribType == "blob.file" )
	            {
	                FilePath oBlobPath = new FilePath(value);
	                strSrcBody += "&attrvals[][value]=";
	                strSrcBody += oBlobPath.getBaseName();
	                strSrcBody += "&attrvals[][attrib_type]=blob";
	
	                m_arSyncBlobs.addElement(new SyncBlob(strSrcBody,value));
	                continue;
	            }else
	                strSrcBody += "&attrvals[][value]=" + value;
	        }
	
	        if ( strBody.length() > 0 )
	            strBody += "&";
	
	        strBody += strSrcBody;
	    }
	}

	void getAndremoveAsk()throws DBException
	{
	    String askParams = "";
	    {
	        IDBResult res = getDB().executeSQL("SELECT value from object_values where source_id=? and update_type=?", 
	            getID(), "ask" );
        	askParams = res.isEnd() ? "" : res.getStringByIdx(0);
	    }
	
	    getDB().executeSQL("DELETE FROM object_values WHERE source_id=? and update_type=?", getID(), "ask" );
	
	    setAskParams(askParams);
	}

	void syncServerChanges()throws Exception
	{
		LOG.INFO("Sync server changes source ID :" + getID() );
		
	    while( getSync().isContinueSync() )
	    {
	        setCurPageCount(0);
	        String strUrl = getUrl();
	        String strQuery = getSync().SYNC_SOURCE_FORMAT() + "&client_id=" + getSync().getClientID() + 
	                "&p_size=" + getSync().SYNC_PAGE_SIZE();
	
	        if ( getAskParams().length() > 0 )
	        {
	            strUrl +=  getSync().SYNC_ASK_ACTION();
	            strQuery += "&question=" + getAskParams();
	        }
	
	        if ( isEmptyToken() )
	            processToken("1");
	        else if ( isTokenMoreThanOne() )
	            strQuery += "&ack_token=" + getToken();
	
			LOG.INFO( "Pull changes from server. Url: " + (strUrl+strQuery) );
			
	        String szData = getNet().pullData(strUrl+strQuery, "", getSync()).getCharData();
	        if ( szData == null )
	        {
	            getSync().stopSync();
	            continue;
	        }
	
	        processServerData(szData);
	
	        m_bGetAtLeastOnePage = true;
	
	        if ( getAskParams().length() > 0 || getCurPageCount() == 0 )
	            break;
	    }
	}

	void processServerData(String szData)throws Exception
	{
	    JSONArrayIterator oJsonArr = new JSONArrayIterator(szData);
	    if ( !oJsonArr.isEnd() )
	    {
	        setCurPageCount(oJsonArr.getCurItem().getInt("count"));
	        oJsonArr.next();
	    }
	    if ( !oJsonArr.isEnd() && oJsonArr.getCurItem().hasName("total_count") )
	    {
	        setTotalCount(oJsonArr.getCurItem().getInt("total_count"));
	        oJsonArr.next();
	    }
	    if ( getServerObjectsCount() == 0 )
	    	m_syncEngine.fireNotification(this, false);
	    
	    if ( !oJsonArr.isEnd() )
	    {
	        processToken(oJsonArr.getCurItem().getUInt64("token"));
	        oJsonArr.next();
	    }else if ( getCurPageCount() == 0 )
	        processToken("0");
	
		LOG.INFO( "Got " + getCurPageCount() + " records of " + getTotalCount() + " from server. Source ID: " + getID() );
		
	    //TODO: support DBExceptions
	    getDB().startTransaction();
	    try{
		    for( ; !oJsonArr.isEnd() && getSync().isContinueSync(); oJsonArr.next() )
		    {
		        if ( getDB().isUnlockDB() )
		        {
					LOG.INFO( "Commit transaction because of UI request." );
		            getDB().endTransaction();
		            getDB().startTransaction();
		        }
		
		        JSONEntry oJsonEntry = oJsonArr.getCurItem();
		
		        JSONEntry oJsonObject = oJsonEntry.getEntry("object_value");
		        if ( !oJsonObject.isEmpty() )
		            processSyncObject(oJsonObject);
		    }
	    }finally{    
			getDB().endTransaction();
		}
	    
	    if ( getServerObjectsCount() < getTotalCount() )
	    	m_syncEngine.fireNotification(this, false);
	}

	void processSyncObject(JSONEntry oJsonEntry)throws DBException, JSONException
	{
	    String szDbOp = oJsonEntry.getString("db_operation");
	    if ( szDbOp != null && szDbOp.equals("insert") )
	    {
	        getDB().executeSQL("INSERT INTO object_values " +
	            "(id, attrib, source_id, object, value, update_type,attrib_type) VALUES(?,?,?,?,?,?,?)", 
	            new Integer(oJsonEntry.getInt("id")), oJsonEntry.getString("attrib"), getID(), oJsonEntry.getString("object"),
	            oJsonEntry.getString("value"), oJsonEntry.getString("update_type"), oJsonEntry.getString("attrib_type") );
	
	        m_nInserted++;
	    }else if ( szDbOp != null && szDbOp.equals("delete") )
	    {
	        getDB().executeSQL("DELETE FROM object_values where id=?", oJsonEntry.getUInt64("id") );
	
	        m_nDeleted++;
	    }else{
	        LOG.ERROR("Unknown DB operation: " + (szDbOp != null ? szDbOp : "") );
	    }
	}

	void processToken(String token)throws DBException
	{
	    if ( this.isTokenMoreThanOne() && getToken().equals(token) ){
			//Delete non-confirmed records
	
	        getDB().executeSQL("DELETE FROM object_values where source_id=? and token=?", getID(), token );
		}else
	    {
	        setToken( token );
	        getDB().executeSQL("UPDATE sources SET token=? where source_id=?", token, getID() );
		}
	
	}
	
    private void reportStatus(ISyncStatusListener statusListener, String status, int error) {
    	if (statusListener != null) {
    		statusListener.reportStatus(status, error);
    	}
    	LOG.INFO(status);
    }	
}