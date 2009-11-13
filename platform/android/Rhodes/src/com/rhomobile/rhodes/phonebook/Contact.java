package com.rhomobile.rhodes.phonebook;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

public class Contact {
	
	private Map<String, ContactField> fields = new HashMap<String, ContactField>();
	private Iterator<ContactField> iter = null;
	
	public Contact() {
		moveToBegin();
	}
	
	public void setField(String key, String value)
	{
		fields.put(key, new ContactField(key, value));
	}
	
	public String getField(String key)
	{
		if ( fields.containsKey(key) )
			return fields.get(key).getValue();
		
		return "";
	}
	
	public String id() {
		return getField(Phonebook.PB_ID);
	}
	
	public void moveToBegin() {
		iter = fields.values().iterator();
	}
	
	public boolean hasNext() {
		return iter.hasNext();
	}
	
	public Object next() {
		return iter.next();
	}
	
}
