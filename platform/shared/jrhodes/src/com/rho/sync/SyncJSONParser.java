package com.rho.sync;

import j2me.util.ArrayList;

import org.json.me.JSONArray;
import org.json.me.JSONObject;

/**
 * The Class SyncJSONParser.
 */
public class SyncJSONParser {

	static class SyncHeader
	{ 
		int			_count = -1;
		String		_token = "";
	};
	
	/**
	 * Parses the json list.
	 * 
	 * @param input the input
	 * 
	 * @return the array list
	 */
	public static ArrayList parseObjectValues(String input, SyncHeader header) {
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
				
				if ( element.has("object_value"))
				{
				        JSONObject current = (JSONObject) element.get("object_value");
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
					newObject.set_token(header._token);
					
					list.add(newObject);
				}else if ( element.has("count") ){
					header._count = element.getInt("count");
				}else if ( element.has("token") ){
					header._token = element.getString("token");
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
