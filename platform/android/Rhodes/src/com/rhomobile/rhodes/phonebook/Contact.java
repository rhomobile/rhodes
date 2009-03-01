package com.rhomobile.rhodes.phonebook;

import java.util.HashMap;
import java.util.Map;

public class Contact {

	private Map<String, String> fields = new HashMap<String, String>();
	
	public void setField(String key, String value)
	{
		fields.put(key, value);
	}
	
	public String getField(String key)
	{
		if ( fields.containsKey(key) )
			return fields.get(key);
		
		return "";
	}
}
