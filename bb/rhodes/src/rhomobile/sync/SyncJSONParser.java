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

import j2me.util.ArrayList;

import org.json.me.JSONArray;
import org.json.me.JSONObject;

/**
 * The Class SyncJSONParser.
 */
public class SyncJSONParser {

	/**
	 * Parses the json list.
	 * 
	 * @param input the input
	 * 
	 * @return the array list
	 */
	public static ArrayList parseObjectValues(String input) {
		ArrayList list = new ArrayList();

		// Iterates over a JSONArray provided by input and parses
		// a SyncObject ArrayList
		try {
			JSONArray object_array = new JSONArray(input);
			for (int i = 0; i < object_array.length(); i++) {
				JSONObject element = (JSONObject) object_array.get(i);
				if (element == null) {
					continue;
				}
				JSONObject current = (JSONObject) element.get("object_value");
				if (current != null) {
					String attrib = (String) current.get("attrib");
					int sourceId = ((Integer) current.get("source_id")).intValue();
					String object = (String) current.get("object");
					Object obValue = current.get("value");
					String value = obValue == JSONObject.NULL ? null : (String) obValue;
					
					String updateType = (String) current.get("update_type");
					SyncObject newObject = new SyncObject(attrib, sourceId,	object, 
							value, updateType);
					
					newObject.setDbOperation((String)current.get("db_operation"));
					newObject.setPrimaryKey(((Integer) current.get("id")).intValue());
					
					list.add(newObject);
				}
			}

		} catch (Exception e) {
			System.out.println("There was an error processing the json list: "
					+ e.getMessage());
		}
		return list;
	}
	
	public static String parseClientID(String input) {
		String client_id = "";

		// Iterates over a JSONArray provided by input and parses
		// a SyncObject ArrayList
		try {
			JSONObject object = new JSONObject(input);
			JSONObject client = (JSONObject)object.get("client");
			if (client != null)
				client_id = (String) client.get("client_id");

		} catch (Exception e) {
			System.out.println("There was an error processing the json list: "
					+ e.getMessage());
		}
		return client_id;
	}
	
}