/*
 *  rhodes
 *
 *  Copyright (C) 2008 Lars Burgess. All rights reserved.
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
package rhomobile.sync;

import java.io.IOException;

import javax.microedition.io.HttpConnection;

import rhomobile.URI;

import j2me.util.ArrayList;

import rhomobile.db.PerstLiteAdapter;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyHash;
import com.xruby.runtime.builtin.RubyInteger;
import com.xruby.runtime.builtin.RubyString;
import com.xruby.runtime.lang.RubyConstant;
import com.xruby.runtime.lang.RubyValue;

/**
 * The Class SyncUtil.
 */
public class SyncUtil {

	/** The adapter. */
	public static PerstLiteAdapter adapter = null;

	/**
	 * Creates the array.
	 * 
	 * @return the ruby array
	 */
	public static RubyArray createArray() {
		return new RubyArray();
	}

	/**
	 * Creates the hash.
	 * 
	 * @return the ruby hash
	 */
	public static RubyHash createHash() {
		return ObjectFactory.createHash();
	}

	/**
	 * Creates the integer.
	 * 
	 * @param val
	 *            the val
	 * 
	 * @return the ruby integer
	 */
	public static RubyInteger createInteger(long val) {
		return ObjectFactory.createInteger(val);
	}

	/**
	 * Creates the string.
	 * 
	 * @param val
	 *            the val
	 * 
	 * @return the ruby string
	 */
	public static RubyString createString(String val) {
		return ObjectFactory.createString(val);
	}

	/**
	 * Fetch remote changes.
	 * 
	 * @param source
	 *            the source
	 * 
	 * @return the int
	 */
	public static int fetchRemoteChanges(SyncSource source, String client_id) {
		int count = 0;
		int success=0, deleted=0, inserted=0;
		long start=0, duration=0;
		String data = null;
		try {
			start = System.currentTimeMillis();
			String session = get_session(source);
			data = SyncManager.fetchRemoteData(source.get_sourceUrl()
					+ SyncConstants.SYNC_FORMAT + "&client_id=" + client_id, session, true);
		} catch (IOException e) {
			System.out
					.println("There was an error fetching data from the sync source: "
							+ e.getMessage());
		}
		if (data != null) {
			ArrayList list = SyncJSONParser.parseObjectValues(data);
			count = list.size();
			String type;
			if (count > 0) {
				for (int i = 0; i < count; i++) {
				  type = ((SyncObject)list.get(i)).getDbOperation();
				  if (type != null) {
				    if (type.equalsIgnoreCase("insert")) {
				    	((SyncObject)list.get(i)).insertIntoDatabase();
				    	inserted++;
				    } else if (type.equalsIgnoreCase("delete")) {
				    	((SyncObject)list.get(i)).deleteFromDatabase();
				    	deleted++;
				    }
				  }
				}
			}
			success = 1;
		}
		duration = (System.currentTimeMillis() - start) / 1000L;
		updateSourceSyncStatus(source, inserted, deleted, duration, success);
		return count;
	}

	/**
	 * Update the sync source status after each sync run
	 * @param source
	 * @param inserted
	 * @param deleted
	 * @param duration
	 * @param success
	 */
	private static void updateSourceSyncStatus(SyncSource source, int inserted,
			int deleted, long duration, int success) {
		RubyHash values = SyncUtil.createHash();
		long now = System.currentTimeMillis() / 1000;
		values.add(PerstLiteAdapter.Table_sources.LAST_UPDATED, createInteger(now));
		values.add(PerstLiteAdapter.Table_sources.LAST_INSERTED_SIZE, createInteger(inserted));
		values.add(PerstLiteAdapter.Table_sources.LAST_DELETED_SIZE, createInteger(deleted));
		values.add(PerstLiteAdapter.Table_sources.LAST_SYNC_DURATION, createInteger(duration));
		values.add(PerstLiteAdapter.Table_sources.LAST_SYNC_SUCCESS, createInteger(success));
		RubyHash where = SyncUtil.createHash();
		where.add(PerstLiteAdapter.SOURCE_ID, createInteger(source.get_sourceId()));
		adapter.updateIntoTable(createString(SyncConstants.SOURCES_TABLE), values, where);
	}

	/**
	 * Gets the object value list.
	 * 
	 * @param id
	 *            the id
	 * 
	 * @return the object value list
	 */
	public static RubyArray getObjectValueList(int id) {
		RubyArray arr = createArray();
		RubyHash where = createHash();
		arr.add(createString("object_values"));
		arr.add(createString("*"));
		where.add(createString("source_id"), createInteger(id));
		arr.add(where);
		return (RubyArray) adapter.selectFromTable(arr);
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
		System.out.println("Checking database for " + type + " operations...");
		RubyArray arr = createArray();
		RubyHash where = createHash();
		String operation = null;
		arr.add(createString("object_values"));
		arr.add(createString("*"));
		where.add(createString("update_type"), createString(type));
		where.add(createString("source_id"), createInteger(source
				.get_sourceId()));
		arr.add(where);
		RubyArray rows = (RubyArray) adapter.selectFromTable(arr);
		ArrayList objects = getSyncObjectList(rows);
		System.out.println("Found " + objects.size() + " records for " + type
				+ " processing...");
		ArrayList results = new ArrayList();

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
			System.out
					.println("Adding sync operation (attrib, source_id, object, value, update_type, uri): "
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
		RubyHash where = createHash();
		where.add(createString("update_type"), createString(type));
		where.add(createString("source_id"), createInteger(source
				.get_sourceId()));
		adapter.deleteFromTable(createString("object_values"), where);
	}

	/**
	 * Gets the source list.
	 * 
	 * @return the source list
	 */
	public static RubyArray getSourceList() {
		RubyArray arr = createArray();
		if ( adapter == null )
			return arr;
		
		arr.add(createString("sources"));
		arr.add(createString("*"));
		return (RubyArray) adapter.selectFromTable(arr);
	}

	/**
	 * Gets the sync object list.
	 * 
	 * @param list
	 *            the list
	 * 
	 * @return the sync object list
	 */
	public static ArrayList getSyncObjectList(RubyArray list) {
		ArrayList results = new ArrayList();
		for (int i = 0; i < list.size(); i++) {
			RubyHash element = (RubyHash) list.at(createInteger(i));
			String attrib = element.get(createString("attrib")).asString();
			RubyValue val = element.get(createString("value"));
			String value = val == null ? null : val.asString();
			String object = element.get(createString("object")).asString();
			String updateType = element.get(createString("update_type"))
					.asString();
			int sourceId = element.get(createString("source_id")).toInt();
			results.add(new SyncObject(attrib, sourceId, object, value,
					updateType));
		}
		return results;
	}

	/**
	 * Prints the results.
	 * 
	 * @param objects
	 *            the objects
	 */
	public static void printResults(RubyArray objects) {
		// Debug code to print results
		for (int j = 0; j < objects.size(); j++) {
			RubyHash objectElement = (RubyHash) objects.at(SyncUtil
					.createInteger(j));
			String value = objectElement.get(SyncUtil.createString("value"))
					.toString();
			String attrib = objectElement.get(SyncUtil.createString("attrib"))
					.toString();
			System.out.println("value[" + j + "][" + attrib + "]: " + value);
		}
	}

	/**
	 * Process local changes.
	 * 
	 * @return the int
	 */
	public static int processLocalChanges(SyncThread thread) {
		RubyArray sources = SyncUtil.getSourceList();

		String client_id = null;
		for (int i = 0; i < sources.size() && !thread.isStop(); i++) {
			RubyHash element = (RubyHash) sources.at(SyncUtil.createInteger(i));
			String url = element.get(PerstLiteAdapter.SOURCE_URL).toString();
			int id = element.get(PerstLiteAdapter.SOURCE_ID).toInt();
			SyncSource current = new SyncSource(url, id);
			if ( client_id == null )
				client_id = get_client_id(current);
			
			if ( thread.isStop() )	break;
			
			System.out.println("URL: " + current.get_sourceUrl());
			int success = 0;
			success += processOpList(current, "create", client_id);
			if ( thread.isStop() )	break;			
			success += processOpList(current, "update", client_id);
			if ( thread.isStop() )	break;			
			success += processOpList(current, "delete", client_id);
			if ( thread.isStop() )	break;
			
			if (success > 0) {
				System.out
						.println("Remote update failed, not continuing with sync...");
			} else {
				int available = SyncUtil.fetchRemoteChanges(current,client_id);
				System.out.println("Successfully processed " + available
						+ " records...");
				if (SyncConstants.DEBUG) {
					RubyArray objects = SyncUtil.getObjectValueList(current
							.get_sourceId());
					SyncUtil.printResults(objects);
				}
			}
		}
		return SyncConstants.SYNC_PROCESS_CHANGES_OK;
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
	private static int processOpList(SyncSource source, String type, String clientId) {
		int success = SyncConstants.SYNC_PUSH_CHANGES_OK;
		ArrayList list = getOpListFromDatabase(type, source);
		if (list.size() == 0) {
			return success;
		}
		System.out.println("Found " + list.size()
				+ " available records for processing...");
		if (pushRemoteChanges(source, list, clientId) != SyncConstants.SYNC_PUSH_CHANGES_OK) {
			success = SyncConstants.SYNC_PUSH_CHANGES_ERROR;
		} else {
			// We're done processsing, remove from database so we
			// don't process again
			removeOpListFromDatabase(type, source);
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
	public static int pushRemoteChanges(SyncSource source, ArrayList list, String clientId) {
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
		try {
			// Construct the post url
			url = source.get_sourceUrl() + "/"
					+ ((SyncOperation) list.get(0)).get_operation() + "?client_id=" + clientId;
			String session = get_session(source);
			success = SyncManager.pushRemoteData(url, data.toString(), session, true);
		} catch (IOException e) {
			System.out.println("There was an error pushing changes: "
					+ e.getMessage());
			success = SyncConstants.SYNC_PUSH_CHANGES_ERROR;
		}

		return success == SyncConstants.SYNC_PUSH_CHANGES_OK ? SyncConstants.SYNC_PUSH_CHANGES_OK
				: SyncConstants.SYNC_PUSH_CHANGES_ERROR;
	}

	/**
	 * 
	 * @return size of objectValues table
	 */
	public static int getObjectCountFromDatabase( String dbName ) {
		RubyArray arr = createArray();
		arr.add(createString(dbName));//"object_values")); //table name
		arr.add(createString("*")); //attributes
		//arr.add(createString("source_id")); //not nil attributes
		arr.add(RubyConstant.QNIL); //where

		RubyHash params = createHash();
		params.add(createString("count"), RubyConstant.QTRUE);
		arr.add(params);
		
		RubyInteger results = (RubyInteger)adapter.selectFromTable(arr);
		return results == null ? 0 : results.toInt();
	}
	
	public static String get_client_id(SyncSource source) {
		String client_id = get_client_db_info("client_id");
		if ( client_id.length() == 0 ){
			String data = null;
			try {
				data = SyncManager.fetchRemoteData(source.get_sourceUrl()+"/clientcreate"
						+ SyncConstants.SYNC_FORMAT, "", false);
				
				if (data != null)
					client_id = SyncJSONParser.parseClientID(data);

				RubyHash hash = SyncUtil.createHash();
				hash.add(SyncUtil.createString("client_id"), createString(client_id));
				
				if ( getObjectCountFromDatabase(SyncConstants.CLIENT_INFO) > 0 )
					adapter.updateIntoTable(createString(SyncConstants.CLIENT_INFO), hash, RubyConstant.QNIL);
				else
					adapter.insertIntoTable(createString(SyncConstants.CLIENT_INFO), hash);
			} catch (IOException e) {
				System.out
						.println("There was an error fetching data from the sync source: "
								+ e.getMessage());
			}
		}
		return client_id;
	}

	public static String get_session(SyncSource source) {
		RubyArray arr = createArray();
		arr.add(createString("sources"));
		arr.add(PerstLiteAdapter.SESSION);
		
		RubyHash where = SyncUtil.createHash();
		where.add(PerstLiteAdapter.SOURCE_ID, createInteger(source.get_sourceId()));
		arr.add(where);
		RubyArray res = (RubyArray) adapter.selectFromTable(arr);
		if ( res.size() == 0 )
			return "";
		
		RubyHash element = (RubyHash) res.at(SyncUtil.createInteger(0));
		
		return element.get(PerstLiteAdapter.SESSION).toString();
	}
	
	private static String getSessionByDomain(String url){
		RubyArray sources = getSourceList();

		URI uri = new URI(url);
		for( int i = 0; i < sources.size(); i++ )
		{
			RubyHash element = (RubyHash) sources.at(SyncUtil.createInteger(i));
			String sourceUrl = element.get(PerstLiteAdapter.SOURCE_URL).toString();
			String session = element.get(PerstLiteAdapter.SESSION).toString();
			URI uriSrc = new URI(sourceUrl);
			if ( session != null && session.length() > 0 &&
				 uri.getHost().equalsIgnoreCase(uriSrc.getHost()))
				return session;
		}
		
		return "";
	}
	
	static class ParsedCookie {
		String strAuth;
		String strSession;
	};
	/*
	private static void cutCookieField(ParsedCookie cookie, String strField){
		int nExp = cookie.strCookie.indexOf(strField);
		cookie.strFieldValue = "";
		if ( nExp > 0 ){
			int nExpEnd = cookie.strCookie.indexOf(';', nExp);
			if ( nExpEnd > 0 ){
				cookie.strFieldValue = cookie.strCookie.substring(nExp+strField.length(), nExpEnd);
				cookie.strCookie = cookie.strCookie.substring(0, nExp) + cookie.strCookie.substring(nExpEnd+1);  
			}else{
				cookie.strFieldValue = cookie.strCookie.substring(nExp+strField.length());
				cookie.strCookie = cookie.strCookie.substring(0, nExp);
			}
		}
	}*/
	
	private static void parseCookie( String value, ParsedCookie cookie ){
		boolean bAuth = false;
		boolean bSession = false;
		Tokenizer stringtokenizer = new Tokenizer(value, ";");
		while (stringtokenizer.hasMoreTokens()) {
			String tok = stringtokenizer.nextToken();
			tok = tok.trim();
			if (tok.length()==0) {
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
			
			if ( s1.equalsIgnoreCase("auth_token") && s2.length() > 0 ){
				cookie.strAuth = s1 + "=" + s2;
				bAuth = true;
			}else if ( s1.equalsIgnoreCase("path") && s2.length() > 0 ){
				if ( bAuth )
					cookie.strAuth += ";" + s1 + "=" + s2;
				else if (bSession)
					cookie.strSession += ";" + s1 + "=" + s2;
			}else if ( s1.equalsIgnoreCase("rhosync_session") && s2.length() > 0 ){
				cookie.strSession = s1 + "=" + s2;
				bSession = true;
			}

		}
	}
	
	private static String extractToc(String toc_name, String data) {
		int start = data.indexOf(toc_name);
		if (start!=-1) {
			int end = data.indexOf(';', start);
			if (end!=-1) {
				return data.substring(start, end);
			}
		}		
		return null;
	}
	
	private static ParsedCookie makeCookie( HttpConnection connection )throws IOException{
		ParsedCookie cookie = new ParsedCookie();
		
		for ( int i = 0; ; i++ ){
			String strField = connection.getHeaderFieldKey(i);
			if ( strField == null )
				break;
			
			if ( strField.equalsIgnoreCase("Set-Cookie")) {
				String header_field = connection.getHeaderField(i);
				System.out.println("Set-Cookie: " + header_field);
				parseCookie( header_field, cookie );
				// Hack to make it work on 4.6 device which doesn't parse cookies correctly
//				if (cookie.strAuth==null) {
//					String auth = extractToc("auth_token", header_field);
//					cookie.strAuth = auth;
//					System.out.println("Extracted auth_token: " + auth);
//				}
				if (cookie.strSession==null) {
					String rhosync_session = extractToc("rhosync_session", header_field);
					cookie.strSession = rhosync_session;
					System.out.println("Extracted rhosync_session: " + rhosync_session);
				}
			}
		}
		
		return cookie;
	}

	public static boolean fetch_client_login( String strUser, String strPwd )
	{
		boolean success = true;
		RubyArray sources = getSourceList();
		for( int i = 0; i < sources.size(); i++ )
		{
			String strSession="";
			//String strExpire="";
			HttpConnection connection = null;
			
			RubyHash element = (RubyHash) sources.at(SyncUtil.createInteger(i));
			String sourceUrl = element.get(PerstLiteAdapter.SOURCE_URL).toString();
			int id = element.get(PerstLiteAdapter.SOURCE_ID).toInt();

			strSession = getSessionByDomain(sourceUrl);
			if ( strSession.length() == 0 ){
				try {
					SyncManager.makePostRequest(sourceUrl+"/client_login", 
							"login=" + strUser+ "&password="+strPwd+"&remember_me=1", "");
		
					connection = SyncManager.getConnection(); 
					int code = connection.getResponseCode();
					if (code == HttpConnection.HTTP_OK ){
						ParsedCookie cookie = makeCookie(connection);
						strSession = cookie.strAuth+";" +cookie.strSession+";";
					}
					else{
						System.out.println("Error posting data: " + code);
			            success = false;
					}

				} catch (IOException e) {
					System.out.println("There was an error fetch_client_login: "
							+ e.getMessage());
				} finally {
					SyncManager.closeConnection();
					connection = null;
				}
			}

			RubyHash values = SyncUtil.createHash();
			values.add(PerstLiteAdapter.SESSION, createString(strSession));
			RubyHash where = SyncUtil.createHash();
			where.add(PerstLiteAdapter.SOURCE_ID, createInteger(id));
			
			adapter.updateIntoTable(createString(SyncConstants.SOURCES_TABLE), values, where);
		}
		
		return success;
	}
	
	public static String get_client_db_info(String attr) {
		RubyArray arr = createArray();
		arr.add(createString("client_info")); //table name
		arr.add(createString(attr)); //attributes
		arr.add(RubyConstant.QNIL); //where

		RubyArray results = (RubyArray)adapter.selectFromTable(arr);
		if ( results.size() > 0 ){
			RubyHash item = (RubyHash)results.get(0);
			RubyValue value = item.getValue(createString(attr)); 
			return value.toString();
		}
		return "";
	}

	public static boolean logged_in() {
		boolean success = false;
		RubyArray sources = SyncUtil.getSourceList();
		for (int i = 0; i < sources.size(); i++) {
			RubyHash element = (RubyHash) sources.at(SyncUtil.createInteger(i));
			String url = element.get(PerstLiteAdapter.SOURCE_URL).toString();
			int id = element.get(PerstLiteAdapter.SOURCE_ID).toInt();
			SyncSource current = new SyncSource(url, id);
			if (get_session(current).length() > 0) {
				success = true;
			}
		}
		return success;
	}

	public static void logout() {
		RubyArray sources = SyncUtil.getSourceList();
		for (int i = 0; i < sources.size(); i++) {
			RubyHash element = (RubyHash) sources.at(SyncUtil.createInteger(i));
			int id = element.get(PerstLiteAdapter.SOURCE_ID).toInt();
			RubyHash values = SyncUtil.createHash();
			values.add(PerstLiteAdapter.SESSION, SyncUtil.createString(""));
			RubyHash where = SyncUtil.createHash();
			where.add(PerstLiteAdapter.SOURCE_ID, createInteger(id));
			adapter.updateIntoTable(createString(SyncConstants.SOURCES_TABLE), values, where);
		}
	}

	public static void resetSyncDb() {
		adapter.deleteAllFromTable(createString(SyncConstants.CLIENT_INFO));
		adapter.deleteAllFromTable(createString(SyncConstants.OBJECTS_TABLE));
	}
}