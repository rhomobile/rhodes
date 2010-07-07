package com.rhomobile.rhodes.phonebook;

import java.util.Map;

public interface ContactAccessor {
	
	public Map<String, Contact> getAll() throws Exception;
	public void save(Contact contact) throws Exception;
	public void remove(Contact contact);
	
}
