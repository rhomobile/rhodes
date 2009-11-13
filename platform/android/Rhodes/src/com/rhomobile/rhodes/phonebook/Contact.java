package com.rhomobile.rhodes.phonebook;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;

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
	
	public String id() {
		return getField(Phonebook.PB_ID);
	}
	
	public Iterator<Entry<String, String>> iterator() {
		Iterator<Entry<String, String>> it = fields.entrySet().iterator();
		return it;
	}
	
}
