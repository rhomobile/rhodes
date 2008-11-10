package com.rho.sync;

import java.util.ArrayList;

import org.json.me.JSONArray;
import org.json.me.JSONObject;

public class SyncJSONParser {

	public static ArrayList parseJSONList(String input) {
		ArrayList list = new ArrayList();

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
					int sourceId = ((Integer) current.get("source_id"))
							.intValue();
					String object = (String) current.get("object");
					Object obValue = current.get("value");
					String value = obValue == JSONObject.NULL ? null
							: (String) obValue;
					String updateType = (String) current.get("update_type");
					SyncObject newObject = new SyncObject(attrib, sourceId,
							object, value, updateType);
					list.add(newObject);
				}
			}

		} catch (Exception e) {
			System.out.println("There was an error processing the json list: "
					+ e.getMessage());
		}
		return list;
	}
}