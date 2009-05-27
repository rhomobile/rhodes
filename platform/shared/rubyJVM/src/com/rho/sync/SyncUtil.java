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

import java.io.IOException;
import java.io.ByteArrayInputStream;
import com.rho.net.IHttpConnection;
import com.rho.net.URI;
import j2me.util.ArrayList;
import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.Tokenizer;
import com.rho.db.*;

/**
 * The Class SyncUtil.
 */
public class SyncUtil {
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("SyncUtil");

	/** The adapter. */
	public static DBAdapter adapter = null;
    public static byte[]  m_byteBuffer = new byte[4096];

    public static void init(){
		SyncUtil.adapter = DBAdapter.getInstance();
		SyncBlob.DBCallback callback = new SyncBlob.DBCallback();
		SyncUtil.adapter.setDbCallback(callback);
    }

	/**
	 * Fetch the changes for a given source
	 * 
	 * @param source
	 * @param client_id
	 * @param params
	 * @return
	 */
	public static SyncFetchResult fetchRemoteChanges(SyncSource source, String client_id, String params,SyncThread thread) {
		int success = 0, deleted = 0, inserted = 0;
		long start = 0, duration = 0;
		String data = null;
		SyncJSONParser.SyncHeader header = new SyncJSONParser.SyncHeader();
		int nTry = 0, nTotal = -1;
		boolean repeat = true;

		start = System.currentTimeMillis();
		String session = get_session(source);

		do {
			String fetch_url = source.get_sourceUrl() +
					((params != null && params.length() > 0) ? SyncConstants.SYNC_ASK_ACTION : "") +
					SyncConstants.SYNC_FORMAT +
					"&client_id=" + client_id
					+ "&p_size=" + SyncConstants.SYNC_PAGE_SIZE;
			if (params != null && params.length() > 0) {
				fetch_url += "&question=" + params;
				// Don't repeat if we're calling ask method
				repeat = false;
			}
			if (header._token.length() > 0)
				fetch_url += "&ack_token=" + header._token;

			if ( source.get_token().length() == 0 || source.get_token().equals("0") )
				processToken("1", source );

			header = new SyncJSONParser.SyncHeader();
			
			try {
				data = SyncManager.fetchRemoteData(fetch_url, session, true);
			} catch (IOException e) {
				LOG.ERROR("There was an error fetching data from the sync source", e);
			}
			
			if (data != null) {
		        LOG.INFO("Start parsing data." );
				
				ArrayList list = SyncJSONParser.parseObjectValues(data, header, source.get_sourceId());
				int count = list.size();
				
		        LOG.INFO("Parsed " + count + " records from sync source..." );
				
				processToken(header._token, source);
				
				if ( nTotal < 0 )
					nTotal = 0;
				nTotal += count;
				if (count > 0) {

					try{
						LOG.INFO("Start write data to DB" );
						adapter.startTransaction();
						for (int i = 0; !thread.isStop() && i < count; i++) {
							SyncObject syncObj = ((SyncObject) list.get(i)); 
							String dbOp = syncObj.getDbOperation();
							if (dbOp != null) {
								
								if (dbOp.equalsIgnoreCase("insert")) {
	//								SyncBlob.insertOp(syncObj, client_id, SyncBlob.SYNC_STAGE);
									
									syncObj.insertIntoDatabase();
									/*try {
										Thread.currentThread().sleep(1000L);
									}catch(Exception e){
									}*/
									
									inserted++;
								} else if (dbOp.equalsIgnoreCase("delete")) {
									syncObj.deleteFromDatabase();
									deleted++;
								}
							}
						}
						adapter.commit();
						LOG.INFO("Finish write data to DB" );
					}catch(DBException exc){
						LOG.ERROR("Failed write to database", exc);
						header._count = -1;
						success = 0;
						break;
					}
				}
				success = 1;
			} else {
		        LOG.INFO("Recieved null data from sync server. Try again :" + nTry );
				
				nTry++;
			}
		} while (!thread.isStop() && header._count != 0 && nTry < SyncConstants.MAX_SYNC_TRY_COUNT && repeat);

		duration = (System.currentTimeMillis() - start) / 1000L;
		updateSourceSyncStatus(source, inserted, deleted, duration, success);

        LOG.INFO("Fetch finish for source:" + source.get_sourceUrl() );
		
		return new SyncFetchResult(nTotal,header._count == -1);
	}

	private static void processToken(String token, SyncSource source) {
		if ( token.length() > 0 && !token.equals("0") && !token.equals("1") &&  
			 source.get_token().equals(token)) {
			// Delete non-confirmed records

			IDBResult result = null;
			try {
				Object[] values = {new Integer(source.get_sourceId()), token};
				result = SyncUtil.adapter.executeSQL("DELETE FROM object_values where source_id=? and token=?", values);
			}catch (DBException e) {
				LOG.ERROR("There was an error delete object_values record by token", e);
			}finally{
				if ( result != null )
					result.close();
			}
			
			/*RubyHash where = createHash();
			where.add(createString("source_id"), createInteger(source
					.get_sourceId()));
			where.add(PerstLiteAdapter.TOKEN, createString(token));

			adapter.deleteFromTable(createString("object_values"), where);*/
		} else //if (token.length() > 0) 
		{
			source.set_token(token);
			
			IDBResult result = null;
			try {
				Object[] values = {token, new Integer(source.get_sourceId())};
				result = adapter.executeSQL("UPDATE sources SET token=? where source_id=?", values);
			}catch (DBException e) {
				LOG.ERROR("There was an error update token in sources record", e);
			}finally{
				if ( result != null )
					result.close();
			}
			
			/*RubyHash values = SyncUtil.createHash();
			values.add(PerstLiteAdapter.TOKEN, createString(token));
			RubyHash where = SyncUtil.createHash();
			where.add(PerstLiteAdapter.SOURCE_ID, createInteger(source
					.get_sourceId()));
			adapter.updateIntoTable(createString(SyncConstants.SOURCES_TABLE),
					values, where);*/
		}
	}

	/**
	 * Update the sync source status after each sync run
	 * 
	 * @param source
	 * @param inserted
	 * @param deleted
	 * @param duration
	 * @param success
	 */
	private static void updateSourceSyncStatus(SyncSource source, int inserted,
			int deleted, long duration, int success) {
		
		IDBResult result = null;
		try {
			long now = System.currentTimeMillis() / 1000;
			Object[] values = {new Long(now), new Integer(inserted), new Integer(deleted), new Long(duration), new Integer(success),
					new Integer(source.get_sourceId()) };
			result = SyncUtil.adapter.executeSQL("UPDATE sources set last_updated=?,last_inserted_size=?,last_deleted_size=?,"+
					 "last_sync_duration=?,last_sync_success=? WHERE source_id=?", values);
		}catch (DBException e) {
			LOG.ERROR("There was an error update token in sources record", e);
		}finally{
			if ( result != null )
				result.close();
		}
		
		/*RubyHash values = SyncUtil.createHash();
		long now = System.currentTimeMillis() / 1000;
		values.add(PerstLiteAdapter.Table_sources.LAST_UPDATED,
				createInteger(now));
		values.add(PerstLiteAdapter.Table_sources.LAST_INSERTED_SIZE,
				createInteger(inserted));
		values.add(PerstLiteAdapter.Table_sources.LAST_DELETED_SIZE,
				createInteger(deleted));
		values.add(PerstLiteAdapter.Table_sources.LAST_SYNC_DURATION,
				createInteger(duration));
		values.add(PerstLiteAdapter.Table_sources.LAST_SYNC_SUCCESS,
				createInteger(success));
		RubyHash where = SyncUtil.createHash();
		where.add(PerstLiteAdapter.SOURCE_ID, createInteger(source
				.get_sourceId()));
		adapter.updateIntoTable(createString(SyncConstants.SOURCES_TABLE),
				values, where);*/
	}

	/**
	 * Gets the object value list.
	 * 
	 * @param id
	 *            the id
	 * 
	 * @return the object value list
	 */
	public static IDBResult getObjectValueList(int id) {
		
		try {
			Object[] values = { new Integer(id) };
			return adapter.executeSQL("SELECT * FROM object_values WHERE source_id=?", values);
		}catch (DBException e) {
			LOG.ERROR("There was an error update token in sources record", e);
		}
		
		//TODO: getObjectValueList
		return DBAdapter.createResult();
		/*RubyArray arr = createArray();
		RubyHash where = createHash();
		arr.add(createString("object_values"));
		arr.add(createString("*"));
		where.add(createString("source_id"), createInteger(id));
		arr.add(where);
		return (RubyArray) adapter.selectFromTable(arr);*/
	}
	
	/**
	 * Gets the op list from database.
	 * 
	 * @param type
	 *            the type
	 * @param source
	 *            the source
	 * 
	 * @return the op list from database
	 */
	public static ArrayList getOpListFromDatabase(String type, SyncSource source) {
		LOG.INFO("Checking database for " + type + " operations...");
		
		/*RubyArray arr = createArray();
		RubyHash where = createHash();
		arr.add(createString("object_values"));
		arr.add(createString("*"));
		where.add(createString("update_type"), createString(type));
		where.add(createString("source_id"), createInteger(source
				.get_sourceId()));
		arr.add(where);
		RubyArray rows = (RubyArray) adapter.selectFromTable(arr);*/
		
		ArrayList results = new ArrayList();
		IDBResult rows = null;
		ArrayList objects = null;
		try {
			try {
				Object[] values = { new Integer(source.get_sourceId()), type };
				rows = SyncUtil.adapter.executeSQL("SELECT * FROM object_values WHERE source_id=? AND update_type=?", values);
			}catch (DBException e) {
				LOG.ERROR("There was an error update token in sources record", e);
				return results;
			}
		
			objects = getSyncObjectList(rows);
		}finally{
			if ( rows != null )
				rows.close();
		}
		LOG.INFO("Found " + objects.size() + " records for " + type	+ " processing...");

		String operation = null;
		
		if (type != null) {
			if (type.equalsIgnoreCase("create")) {
				operation = SyncConstants.UPDATE_TYPE_CREATE;
			} else if (type.equalsIgnoreCase("update")) {
				operation = SyncConstants.UPDATE_TYPE_UPDATE;
			} else if (type.equalsIgnoreCase("delete")) {
				operation = SyncConstants.UPDATE_TYPE_DELETE;
			}
		}

		for (int i = 0; i < objects.size(); i++) {
			SyncObject current = (SyncObject) objects.get(i);
			SyncOperation newOperation = new SyncOperation(operation, current);
			results.add(newOperation);
			LOG.INFO("Adding sync operation (attrib, source_id, object, value, update_type, uri): "
							+ current.getAttrib()
							+ ", "
							+ current.getSourceId()
							+ ", "
							+ current.getObject()
							+ ", "
							+ (current.getValue() == null ? "null" : current
									.getValue())
							+ ", "
							+ operation
							+ ", "
							+ source.get_sourceUrl());
		}
		return results;
	}

	public static void removeOpListFromDatabase(String type, SyncSource source) {
		IDBResult result = null;
		try {
			Object[] values = { type, new Integer(source.get_sourceId()) };
			result = SyncUtil.adapter.executeSQL("DELETE FROM object_values WHERE update_type=? and source_id=?", values);
		}catch (DBException e) {
			LOG.ERROR("There was an error update token in sources record", e);
		}finally{
			if ( result != null )
				result.close();
		}
		
		/*RubyHash where = createHash();
		where.add(createString("update_type"), createString(type));
		where.add(createString("source_id"), createInteger(source
				.get_sourceId()));
		adapter.deleteFromTable(createString("object_values"), where);*/
	}

	/**
	 * Returns the parameter string for a source
	 * 
	 * @param sourceId
	 * @return
	 */
	public static String getParamsForSource(SyncSource source) {
		
		String askType = "ask";
		String params = "";
		try {
			
			IDBResult rows = null;
			try
			{
				Object[] values = { new Integer(source.get_sourceId()), askType };
				rows = SyncUtil.adapter.executeSQL(
						"SELECT value FROM object_values WHERE source_id=? AND update_type=?", values);
			
				if ( rows.getCount() > 0 ){
					params = rows.getStringByIdx(0,0);
					removeOpListFromDatabase(askType, source);
				}
			}
			finally {
				if ( rows != null )
					rows.close();
			}
			
		}catch (DBException e) {
			LOG.ERROR("There was an error update token in sources record", e);
		}

		return params;
		
/*		String askType = "ask";
		RubyHash where = createHash();
		RubyArray arr = createArray();
		arr.add(createString("object_values"));
		arr.add(createString("*"));
		where.add(createString("source_id"), createInteger(source.get_sourceId()));
		where.add(createString("update_type"), createString(askType));
		arr.add(where);
		RubyArray list = (RubyArray) adapter.selectFromTable(arr);
		if ( list.size() == 0 )
			return "";
		
		// There should only be one result
		RubyHash element = (RubyHash) list.at(createInteger(0));
		String params = element.get(createString("value")).asString();
		removeOpListFromDatabase(askType, source);
		return params;*/
	}

	/**
	 * Gets the source list.
	 * 
	 * @return the source list
	 */
	public static IDBResult getSourceList() {
		try {
			 return adapter.executeSQL( "SELECT * FROM sources ORDER BY source_id", null);
			
		}catch (DBException e) {
			LOG.ERROR("There was an error update token in sources record", e);
		}

		return DBAdapter.createResult();
		/*RubyArray arr = createArray();
		if (adapter == null)
			return arr;

		arr.add(createString("sources"));
		arr.add(createString("*"));

		RubyHash order = createHash();
		order.add(createString("order by"), createString("source_id"));
		arr.add(RubyConstant.QNIL); // where
		arr.add(order);

		return (RubyArray) adapter.selectFromTable(arr);*/
	}

	/**
	 * Gets the sync object list.
	 * 
	 * @param list
	 *            the list
	 * 
	 * @return the sync object list
	 */
	public static ArrayList getSyncObjectList(IDBResult list) {
		ArrayList results = new ArrayList();
		for( int i = 0; i < list.getCount(); i++)
			results.add(new SyncObject( list, i ));
		
		/*for (int i = 0; i < list.size(); i++) {
			RubyHash element = (RubyHash) list.at(createInteger(i));
			String attrib = element.get(createString("attrib")).asString();
			RubyValue val = element.get(createString("value"));
			String value = val == null ? null : val.asString();
			String object = element.get(createString("object")).asString();
			String updateType = element.get(createString("update_type"))
					.asString();
			
			String type = "";
			val = element.get(createString("attrib_type"));
			if ( val != null && val != RubyConstant.QNIL)
				type = val.asString();
			
			int sourceId = element.get(createString("source_id")).toInt();
			results.add(new SyncObject(attrib, sourceId, object, value,
					updateType, type));
		}*/
		return results;
	}

	/**
	 * Prints the results.
	 * 
	 * @param objects
	 *            the objects
	 */
	public static void printResults(IDBResult objects) 
	{
		// Debug code to print results
		for (int j = 0; j < objects.getCount(); j++) 
		{
			String value = objects.getString(j, SyncConstants.COL_VALUE);
			String attrib = objects.getString(j, SyncConstants.COL_ATTRIB);
			
			System.out.println("value[" + j + "][" + attrib + "]: " + value);
		}
	}

	private static int get_start_source( IDBResult sources )
	{
		for (int i = 0; i < sources.getCount(); i++) {
			String strToken = sources.getString(i, SyncConstants.COL_TOKEN);
			if ( strToken.length() > 0 && !strToken.equals("0") )
				return i;
		}
		
		return 0;
	}
	
	static class SyncFetchResult {
		int available = 0;
		boolean stopSync = false;
		
		SyncFetchResult() {}
		SyncFetchResult( int avail, boolean bStop ) {
			available = avail;
			stopSync = bStop;
		}
	};
	
	/**
	 * Process local changes.
	 * 
	 * @return the int
	 */
	public static int processLocalChanges(SyncThread thread) {
		IDBResult sources = SyncUtil.getSourceList();
		
		try {
			String client_id = null;
			int nStartSrc = get_start_source(sources);
			SyncFetchResult syncResult = new SyncFetchResult();
			
			for (int i = nStartSrc; i < sources.getCount() && !thread.isStop(); i++) 
			{
				int id = sources.getInt(i, SyncConstants.COL_SOURCEID);
				int success = 0;
				
				if ( !syncResult.stopSync ){
					SyncSource current = new SyncSource(sources, i);
		
					if ( current.get_sourceUrl().length() == 0 )
						continue;
					
					if (client_id == null || "".equals(client_id))
						client_id = get_client_id(current);
		
					if (thread.isStop())
						break;
		
					LOG.INFO("URL: " + current.get_sourceUrl());
					success += processOpList(current, "create", client_id);
					if (thread.isStop())
						break;
					success += processOpList(current, "update", client_id);
					if (thread.isStop())
						break;
					success += processOpList(current, "delete", client_id);
					if (thread.isStop())
						break;
		
					if (success > 0) {
						LOG.ERROR("Remote update failed, not continuing with sync...");
					} else {
						String askParams = SyncUtil.getParamsForSource(current);
						syncResult = SyncUtil.fetchRemoteChanges(current, client_id, askParams, thread);
						LOG.INFO("Successfully processed " + syncResult.available
								+ " records...");
						if (SyncConstants.DEBUG) {
							IDBResult objects = null;
							try {
								objects = SyncUtil.getObjectValueList(current.get_sourceId());
								SyncUtil.printResults(objects);
							}finally{
								objects.close();
							}
						}
						
						success = syncResult.available; 
					}
				}
				
				if (!thread.isStop())
					SyncEngine.getNotificationImpl().fireNotification(id, success);
				
			}
			return SyncConstants.SYNC_PROCESS_CHANGES_OK;
		}finally{
			sources.close();
		}
		
	}

	/**
	 * Process op list.
	 * 
	 * @param source
	 *            the source
	 * @param type
	 *            the type
	 * 
	 * @return the int
	 */
	private static int processOpList(SyncSource source, String type,
			String clientId) {
		int success = SyncConstants.SYNC_PUSH_CHANGES_OK;
		ArrayList list = getOpListFromDatabase(type, source);
		if (list.size() == 0) {
			return success;
		}
		LOG.INFO("Found " + list.size()	+ " available records for processing...");
		
		ArrayList listBlobs = SyncBlob.extractBlobs(list);
		
		if (pushRemoteChanges(source, list, clientId) != SyncConstants.SYNC_PUSH_CHANGES_OK) {
			success = SyncConstants.SYNC_PUSH_CHANGES_ERROR;
		} else {
			if ( SyncBlob.pushRemoteBlobs(source, listBlobs, clientId) == SyncConstants.SYNC_PUSH_CHANGES_OK )
			{
				// We're done processsing, remove from database so we
				// don't process again
				removeOpListFromDatabase(type, source);
			}
			else
				success = SyncConstants.SYNC_PUSH_CHANGES_ERROR;
		}
		return success;
	}

	/**
	 * Push remote changes.
	 * 
	 * @param source
	 *            the source
	 * @param list
	 *            the list
	 * 
	 * @return the int
	 */
	public static int pushRemoteChanges(SyncSource source, ArrayList list,
			String clientId) {
		int success = 0;
		StringBuffer data = new StringBuffer();
		String url = null;
		if (list.size() == 0) {
			return SyncConstants.SYNC_PUSH_CHANGES_OK;
		}

		for (int i = 0; i < list.size(); i++) {
			data.append(((SyncOperation) list.get(i)).get_postBody());
			if (i != (list.size() - 1)) {
				data.append("&");
			}
		}
		ByteArrayInputStream dataStream = null;
		try {
			// Construct the post url
			url = source.get_sourceUrl() + "/"
					+ ((SyncOperation) list.get(0)).get_operation()
					+ "?client_id=" + clientId;
			String session = get_session(source);
			
			dataStream = new ByteArrayInputStream(data.toString().getBytes()); 
			success = SyncManager.pushRemoteData(url, dataStream, session,true,
					"application/x-www-form-urlencoded");
		} catch (IOException e) {
			LOG.ERROR("There was an error pushing changes", e );
			success = SyncConstants.SYNC_PUSH_CHANGES_ERROR;
		}

		if ( dataStream != null ){
			try{dataStream.close();}catch(IOException exc){}
			dataStream = null;
		}
		
		return success == SyncConstants.SYNC_PUSH_CHANGES_OK ? SyncConstants.SYNC_PUSH_CHANGES_OK
				: SyncConstants.SYNC_PUSH_CHANGES_ERROR;
	}

	/**
	 * 
	 * @return size of objectValues table
	 */
	public static int getObjectCountFromDatabase(String dbName) {
		IDBResult rows = null;
		try {
			rows = SyncUtil.adapter.executeSQL( "SELECT count(*) from " + dbName, null);
			return rows.getIntByIdx(0,0);
		}catch (DBException e) {
			LOG.ERROR("There was an error update token in sources record", e);
		}finally{
			if ( rows != null)
				rows.close();
		}
		
		return 0;
		/*RubyArray arr = createArray();
		arr.add(createString(dbName));// "object_values")); //table name
		arr.add(createString("*")); // attributes
		// arr.add(createString("source_id")); //not nil attributes
		arr.add(RubyConstant.QNIL); // where

		RubyHash params = createHash();
		params.add(createString("count"), RubyConstant.QTRUE);
		arr.add(params);

		RubyInteger results = (RubyInteger) adapter.selectFromTable(arr);
		return results == null ? 0 : results.toInt();*/
	}

	public static String get_client_id(SyncSource source) {
		String client_id = get_db_client_id();
		if (client_id.length() == 0) {
			String data = null;
			try {
				data = SyncManager.fetchRemoteData(source.get_sourceUrl()
						+ "/clientcreate" + SyncConstants.SYNC_FORMAT, "",
						false);

				if (data != null)
					client_id = SyncJSONParser.parseClientID(data);

				//RubyHash hash = SyncUtil.createHash();
				//hash.add(SyncUtil.createString("client_id"),createString(client_id));
				Object[] values = { client_id };
				
				IDBResult result = null;
				try {
					if (getObjectCountFromDatabase(SyncConstants.CLIENT_INFO) > 0)
						result = SyncUtil.adapter.executeSQL("UPDATE client_info SET client_id=?", values);
						//adapter.updateIntoTable(createString(SyncConstants.CLIENT_INFO), hash,RubyConstant.QNIL);
					else
						result = SyncUtil.adapter.executeSQL("INSERT INTO client_info (client_id) VALUES (?)", values);
						//adapter.insertIntoTable(createString(SyncConstants.CLIENT_INFO), hash);
				}finally {
					if ( result != null )
						result.close();
				}
				
					
			} catch (Exception e) {
				LOG.ERROR("There was an error fetching data from the sync source: ", e);
			}
		}
		return client_id;
	}

	public static String get_session(SyncSource source) {
		String session = "";
		IDBResult rows = null;
		try {
			Object[] values = { new Integer(source.get_sourceId()) };
			rows = SyncUtil.adapter.executeSQL( "SELECT session FROM sources WHERE source_id=?", values);
			if ( rows.getCount() > 0 )
				session = rows.getStringByIdx(0,0);
		}catch (DBException e) {
			LOG.ERROR("There was an error update token in sources record", e);
		}finally{
			if ( rows != null )
				rows.close();
		}
		
		return session;
		
		/*RubyArray arr = createArray();
		arr.add(createString("sources"));
		arr.add(PerstLiteAdapter.SESSION);

		RubyHash where = SyncUtil.createHash();
		where.add(PerstLiteAdapter.SOURCE_ID, createInteger(source
				.get_sourceId()));
		arr.add(where);
		RubyArray res = (RubyArray) adapter.selectFromTable(arr);
		if (res.size() == 0)
			return "";

		RubyHash element = (RubyHash) res.at(SyncUtil.createInteger(0));

		return element.get(PerstLiteAdapter.SESSION).toString();*/
	}

	private static String getSessionByDomain(String url) {
		IDBResult sources = getSourceList();

		try {
			URI uri = new URI(url);
			for (int i = 0; i < sources.getCount(); i++) {
				try {
					String sourceUrl = sources.getString(i, SyncConstants.COL_SOURCEURL);
					String session = sources.getString(i, SyncConstants.COL_SESSION);
					if (sourceUrl == null || sourceUrl.length() == 0)
						continue;

					URI uriSrc = new URI(sourceUrl);
					if (session != null && session.length() > 0
							&& uri.getHost().equalsIgnoreCase(uriSrc.getHost()))
						return session;
				} catch (URI.MalformedURIException exc) {
				}
			}
		}catch (URI.MalformedURIException exc) {
		
		}finally{
			sources.close();
		}

		return "";
	}

	static class ParsedCookie {
		String strAuth;
		String strSession;
	};

	/*
	 * private static void cutCookieField(ParsedCookie cookie, String strField){
	 * int nExp = cookie.strCookie.indexOf(strField); cookie.strFieldValue = "";
	 * if ( nExp > 0 ){ int nExpEnd = cookie.strCookie.indexOf(';', nExp); if (
	 * nExpEnd > 0 ){ cookie.strFieldValue =
	 * cookie.strCookie.substring(nExp+strField.length(), nExpEnd);
	 * cookie.strCookie = cookie.strCookie.substring(0, nExp) +
	 * cookie.strCookie.substring(nExpEnd+1); }else{ cookie.strFieldValue =
	 * cookie.strCookie.substring(nExp+strField.length()); cookie.strCookie =
	 * cookie.strCookie.substring(0, nExp); } } }
	 */

	private static void parseCookie(String value, ParsedCookie cookie) {
		boolean bAuth = false;
		boolean bSession = false;
		Tokenizer stringtokenizer = new Tokenizer(value, ";");
		while (stringtokenizer.hasMoreTokens()) {
			String tok = stringtokenizer.nextToken();
			tok = tok.trim();
			if (tok.length() == 0) {
				continue;
			}
			int i = tok.indexOf('=');
			String s1;
			String s2;
			if (i > 0) {
				s1 = tok.substring(0, i);
				s2 = tok.substring(i + 1);
			} else {
				s1 = tok;
				s2 = "";
			}
			s1 = s1.trim();
			s2 = s2.trim();

			if (s1.equalsIgnoreCase("auth_token") && s2.length() > 0) {
				cookie.strAuth = s1 + "=" + s2;
				bAuth = true;
			} else if (s1.equalsIgnoreCase("path") && s2.length() > 0) {
				if (bAuth)
					cookie.strAuth += ";" + s1 + "=" + s2;
				else if (bSession)
					cookie.strSession += ";" + s1 + "=" + s2;
			} else if (s1.equalsIgnoreCase("rhosync_session")
					&& s2.length() > 0) {
				cookie.strSession = s1 + "=" + s2;
				bSession = true;
			}

		}
	}

	private static String extractToc(String toc_name, String data) {
		int start = data.indexOf(toc_name);
		if (start != -1) {
			int end = data.indexOf(';', start);
			if (end != -1) {
				return data.substring(start, end);
			}
		}
		return null;
	}

	private static ParsedCookie makeCookie(IHttpConnection connection)
			throws IOException {
		ParsedCookie cookie = new ParsedCookie();

		for (int i = 0;; i++) {
			String strField = connection.getHeaderFieldKey(i);
			if (strField == null && i > 0)
				break;

			if (strField != null && strField.equalsIgnoreCase("Set-Cookie")) {
				String header_field = connection.getHeaderField(i);
				LOG.INFO("Set-Cookie: " + header_field);
				parseCookie(header_field, cookie);
				// Hack to make it work on 4.6 device which doesn't parse
				// cookies correctly
				// if (cookie.strAuth==null) {
				// String auth = extractToc("auth_token", header_field);
				// cookie.strAuth = auth;
				// System.out.println("Extracted auth_token: " + auth);
				// }
				if (cookie.strSession == null) {
					String rhosync_session = extractToc("rhosync_session",
							header_field);
					cookie.strSession = rhosync_session;
					LOG.INFO("Extracted rhosync_session: " + rhosync_session);
				}
			}
		}

		return cookie;
	}

	public static boolean fetch_client_login(String strUser, String strPwd) {
		boolean success = true;
		IDBResult sources = getSourceList();
		
		try {
		
			for (int i = 0; i < sources.getCount(); i++) {
				String strSession = "";
				// String strExpire="";
				IHttpConnection connection = null;
	
				String sourceUrl = sources.getString(i, SyncConstants.COL_SOURCEURL);
				int id = sources.getInt(i, SyncConstants.COL_SOURCEID);
	
				if (sourceUrl.length() == 0)
					continue;
	
				strSession = getSessionByDomain(sourceUrl);
				if (strSession.length() == 0) {
					ByteArrayInputStream dataStream = null;
					try {
						String body = "login=" + strUser + "&password=" + strPwd+ "&remember_me=1";
						dataStream = new ByteArrayInputStream(body.getBytes()); 
						
						SyncManager.makePostRequest(sourceUrl + "/client_login", dataStream, "",
								"application/x-www-form-urlencoded");
	
						connection = SyncManager.getConnection();
						int code = connection.getResponseCode();
						if (code == IHttpConnection.HTTP_OK) {
							ParsedCookie cookie = makeCookie(connection);
							strSession = cookie.strAuth + ";" + cookie.strSession
									+ ";";
						} else {
							LOG.ERROR("Error posting data: " + code);
							success = false;
						}
	
					} catch (IOException e) {
						LOG.ERROR("There was an error fetch_client_login: ", e );
						success = false;
					} finally {
						
						if ( dataStream != null ){
							try{dataStream.close();}catch(IOException exc){}
							dataStream = null;
						}
						
						SyncManager.closeConnection();
						connection = null;
					}
				}
	/*
				RubyHash values = SyncUtil.createHash();
				values.add(SyncConstants.SESSION, createString(strSession));
				RubyHash where = SyncUtil.createHash();
				where.add(SyncConstants.SOURCE_ID, createInteger(id));
	
				adapter.updateIntoTable(createString(SyncConstants.SOURCES_TABLE),
						values, where);*/
				IDBResult result = null;
				try {
					Object[] values = { strSession, new Integer(id) };
					result = adapter.executeSQL( "UPDATE sources SET session=? WHERE source_id=?", values);
				}catch (DBException e) {
					LOG.ERROR("There was an error update session in sources record", e);
				}finally{
					if ( result != null ) 
						result.close();
				}
					
			}
		}
		finally {
			sources.close();
		}
		
		return success;
	}

	public static String get_db_client_id() {
		
		String client_id = "";
		IDBResult rows = null;
		try {
			rows = SyncUtil.adapter.executeSQL( "SELECT client_id FROM client_info", null);
			if ( rows.getCount() > 0 )
				client_id = rows.getStringByIdx(0,0);
			
		}catch (DBException e) {
			LOG.ERROR("There was an error update token in sources record", e);
		}finally{
			if ( rows != null )
				rows.close();
		}
		
		return client_id;
		
		/*RubyArray arr = createArray();
		arr.add(createString("client_info")); // table name
		arr.add(createString(attr)); // attributes
		arr.add(RubyConstant.QNIL); // where

		RubyArray results = (RubyArray) adapter.selectFromTable(arr);
		if (results.size() > 0) {
			RubyHash item = (RubyHash) results.get(0);
			RubyValue value = item.getValue(createString(attr));
			return value.toString();
		}
		return "";*/
	}

	public static boolean logged_in() 
	{
		boolean success = false;
		IDBResult sources = SyncUtil.getSourceList();
		try {
			for (int i = 0; i < sources.getCount(); i++) 
			{
				SyncSource current = new SyncSource(sources,i);
				if (get_session(current).length() > 0) {
					success = true;
				}
			}
		}
		finally {
			sources.close();
		}
		return success;
	}

	public static void logout() 
	{
		IDBResult sources = getSourceList();
		try {
			
			for (int i = 0; i < sources.getCount(); i++) 
			{
				IDBResult result = null;
				try {
					Object[] values = { "", new Integer( sources.getInt(i, SyncConstants.COL_SOURCEID)) };
					result = adapter.executeSQL( "UPDATE sources SET session=? WHERE source_id=?", values);
				}catch (DBException e) {
					LOG.ERROR("Logout: There was an error update session in sources record", e);
				}finally{
					if ( result != null )
						result.close();
				}
				
				/*RubyHash values = SyncUtil.createHash();
				values.add(SyncConstants.SESSION, SyncUtil.createString(""));
				RubyHash where = SyncUtil.createHash();
				where.add(SyncConstants.SOURCE_ID, createInteger(id));
				adapter.updateIntoTable(createString(SyncConstants.SOURCES_TABLE),
						values, where);*/
			}
		}
		finally {
			sources.close();
		}
	}

	public static void resetSyncDb() {
		
		try {
		
			LOG.INFO("Deleting all objects from db...");
			
			IDBResult result = null;
			//adapter.deleteAllFromTable(createString(SyncConstants.OBJECTS_TABLE));
			try {
				result = adapter.executeSQL("delete from object_values", null);
			}
			finally {
				if ( result != null )
					result.close();
			}
			
			LOG.INFO("Deleting client info from db...");
			
//			adapter.deleteAllFromTable(createString(SyncConstants.CLIENT_INFO));
			try {
				result = adapter.executeSQL("delete from client_info", null);
			}
			finally {
				if ( result != null )
					result.close();
			}
			LOG.INFO("Clear tokens in source table...");
			
			try {
				Object[] values = { "" };
				result = adapter.executeSQL("UPDATE sources SET token=?", values);
			}
			finally {
				if ( result != null )
					result.close();
			}
/*			RubyHash values = SyncUtil.createHash();
			values.add(SyncConstants.TOKEN, createString(""));
			adapter.updateIntoTable(createString(SyncConstants.SOURCES_TABLE), values, null);*/
			
		}catch (DBException e) {
			LOG.ERROR("Logout: There was an error update session in sources record", e);
		}
		
	}
}