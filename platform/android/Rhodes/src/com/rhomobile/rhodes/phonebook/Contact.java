/*
 ============================================================================
 Author	    : Dmitry Moskalchuk
 Version	: 1.5
 Copyright  : Copyright (C) 2008 Rhomobile. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ============================================================================
 */
package com.rhomobile.rhodes.phonebook;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

public class Contact {
	
	private String id = null;
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
		
		return null;
	}
	
	public void setId(String v) {
		id = v;
		setField(Phonebook.PB_ID, "{" + v + "}");
	}
	
	public String id() {
		return id;
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
