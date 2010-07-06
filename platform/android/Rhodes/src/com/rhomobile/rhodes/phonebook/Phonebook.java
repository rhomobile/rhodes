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

import java.util.Iterator;
import java.util.Map;

import android.os.Build;

import com.rhomobile.rhodes.Capabilities;
import com.rhomobile.rhodes.Logger;

//@RubyLevelClass(name="Phonebook")
public class Phonebook {

	private static final String TAG = "Phonebook";
	
	public static final String PB_ID = "id";
	public static final String PB_FIRST_NAME = "first_name";
	public static final String PB_LAST_NAME = "last_name";
	public static final String PB_MOBILE_NUMBER = "mobile_number";
	public static final String PB_HOME_NUMBER = "home_number";
	public static final String PB_BUSINESS_NUMBER = "business_number";
	public static final String PB_EMAIL_ADDRESS = "email_address";
	public static final String PB_COMPANY_NAME = "company_name";
	
	private Map<String, Contact> contactList;
	private ContactAccessor accessor;
	private Iterator<Contact> iter = null;

	private boolean checkState() {
		if (!Capabilities.PIM_ENABLED)
			Logger.E(TAG, "Can not execute: PIM disabled");
		return Capabilities.PIM_ENABLED;
	}
	
	private ContactAccessor createAccessor() {
		String className;
		int sdkVersion = Integer.parseInt(Build.VERSION.SDK);
		if (sdkVersion < Build.VERSION_CODES.ECLAIR)
			className = "ContactAccessorOld";
		else
			className = "ContactAccessorNew";
		
		try {
			Class<? extends ContactAccessor> klass =
				Class.forName(ContactAccessor.class.getPackage() + "." + className)
					.asSubclass(ContactAccessor.class);
			return klass.newInstance();
		}
		catch (Exception e) {
			throw new IllegalStateException(e);
		}
	}
	
	public Phonebook() {
		try {
			if (!checkState())
				return;
			
			accessor = createAccessor();
			contactList = accessor.getAll();
			moveToBegin();
		}
		catch (Exception e) {
			Logger.E(TAG, e);
		}
	}
	
	public void close() {
		try {
			if (!checkState())
				return;
			
			this.contactList.clear();
		}
		catch (Exception e) {
			Logger.E(TAG, e);
		}
	}
	
	public void moveToBegin() {
		try {
			if (!checkState())
				return;
			
			iter = contactList.values().iterator();
		}
		catch (Exception e) {
			Logger.E(TAG, e);
		}
	}
	
	public boolean hasNext() {
		try {
			if (!checkState())
				return false;
			
			return iter.hasNext();
		}
		catch (Exception e) {
			Logger.E(TAG, e);
			return false;
		}
	}
	
	public Object next() {
		try {
			if (!checkState())
				return null;
			
			return iter.next();
		}
		catch (Exception e) {
			Logger.E(TAG, e);
			return null;
		}
	}
	
	public Contact getFirstRecord() {
		try {
			if (!checkState())
				return null;
			
			moveToBegin();
			if (!iter.hasNext())
				return null;
			return iter.next();
		}
		catch (Exception e) {
			Logger.E(TAG, e);
			return null;
		}
	}
	
	public Contact getNextRecord() {
		try {
			if (!checkState())
				return null;
			
			return iter.next();
		}
		catch (Exception e) {
			Logger.E(TAG, e);
			return null;
		}
	}
	
	public Contact getRecord(String id) {
		try {
			if (!checkState())
				return null;
			
			return contactList.get(id);
		}
		catch (Exception e) {
			Logger.E(TAG, e);
			return null;
		}
	}

	public void removeContact(Contact contact) {
		try {
			if (!checkState())
				return;
			
			accessor.remove(contact);
		}
		catch (Exception e) {
			Logger.E(TAG, e);
		}
	}

	public void saveContact(Contact contact) {
		try {
			if (!checkState())
				return;
			
			accessor.save(contact);
		}
		catch (Exception e) {
			Logger.E(TAG, e);
		}
	}

}
