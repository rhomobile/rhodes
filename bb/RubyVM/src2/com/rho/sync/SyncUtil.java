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
package com.rho.sync;

import java.io.IOException;
import j2me.util.ArrayList;

import com.rho.db.PerstLiteAdapter;
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
	public static int fetchRemoteChanges(SyncSource source) {
		int count = 0;
		String data = null;
		try {
			data = SyncManager.fetchRemoteData(source.get_sourceUrl()
					+ SyncConstants.SYNC_FORMAT);
		} catch (IOException e) {
			System.out
					.println("There was an error fetching data from the sync source: "
							+ e.getMessage());
		}
		if (data != null) {
			ArrayList list = SyncJSONParser.parseJSONList(data);
			count = list.size();
			if (count > 0) {
				SyncObject.deleteFromDatabaseBySource(source.get_sourceId());
				for (int i = 0; i < count; i++) {
					// Perform the insert on each record
					int success = ((SyncObject) list.get(i)).dehydrate();
					if (success != SyncConstants.SYNC_OBJECT_SUCCESS) {
						System.out
								.println("There was an error saving records.");
					}
				}
			}
		}
		return count;
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
	public static int processLocalChanges() {
		RubyArray sources = SyncUtil.getSourceList();

		for (int i = 0; i < sources.size(); i++) {
			RubyHash element = (RubyHash) sources.at(SyncUtil.createInteger(i));
			String url = element.get(PerstLiteAdapter.URL).toString();
			int id = element.get(PerstLiteAdapter.SOURCE_ID).toInt();
			SyncSource current = new SyncSource(url, id);
			System.out.println("URL: " + current.get_sourceUrl());
			int success = 0;
			success += processOpList(current, "create");
			success += processOpList(current, "update");
			success += processOpList(current, "delete");
			if (success > 0) {
				System.out
						.println("Remote update failed, not continuing with sync...");
			} else {
				int available = SyncUtil.fetchRemoteChanges(current);
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
	private static int processOpList(SyncSource source, String type) {
		int success = SyncConstants.SYNC_PUSH_CHANGES_OK;
		ArrayList list = getOpListFromDatabase(type, source);
		if (list.size() == 0) {
			return success;
		}
		System.out.println("Found " + list.size()
				+ " available records for processing...");
		if (pushRemoteChanges(source, list) != SyncConstants.SYNC_PUSH_CHANGES_OK) {
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
	public static int pushRemoteChanges(SyncSource source, ArrayList list) {
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
					+ ((SyncOperation) list.get(0)).get_operation();
			success = SyncManager.pushRemoteData(url, data.toString());
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
	public static int getObjectCountFromDatabase() {
		RubyArray arr = createArray();
		RubyHash where = createHash();
		arr.add(createString("object_values"));
		arr.add(createString("*"));
		where.add(createString("source_id"), RubyConstant.QNIL);
		arr.add(where);
		RubyArray results = (RubyArray)adapter.selectFromTable(arr);
		return results == null ? 0 : results.size();
	}
}