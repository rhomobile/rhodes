/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

package com.rhomobile.rhodes.phonebook;

import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import android.os.Build;

import com.rhomobile.rhodes.Capabilities;
import com.rhomobile.rhodes.Logger;

//@RubyLevelClass(name="Phonebook")
public class Phonebook {

	private static final String TAG = "Phonebook";
	private static final boolean logging_enable = true;
	
	public static final String PB_ID = "id";
	public static final String PB_DISPLAY_NAME = "display_name";
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
			String pkgname = ContactAccessor.class.getPackage().getName();
			String fullName = pkgname + "." + className;
			Class<? extends ContactAccessor> klass =
				Class.forName(fullName).asSubclass(ContactAccessor.class);
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
			contactList = new HashMap<String, Contact>();
		}
		catch (Exception e) {
			Logger.E(TAG, e);
		}
	}
	
	public int queryContactCount(int offset, int limit) {
		int res = 0;
		if (checkState()) {
			try {
				res = accessor.getCount(offset, limit);
			} catch (Exception e) {
				Logger.E(TAG, e);
			}
		}
		return res;
	}
	
	public void queryAllContacts() {
		String[] allColumns = new String[] {
			PB_ID,
			PB_DISPLAY_NAME,
			PB_FIRST_NAME,
			PB_LAST_NAME,
			PB_MOBILE_NUMBER,
			PB_HOME_NUMBER,
			PB_BUSINESS_NUMBER,
			PB_EMAIL_ADDRESS,
			PB_COMPANY_NAME };
		queryContacts(0, -1, Arrays.asList(allColumns));
	}

	public void queryContacts(int offset, int max_results, List<String> select) {
		if (!checkState())
			return;
		try {
			Logger.I(TAG, "queryContacts()");
			contactList = accessor.getContacts(offset, max_results, select);
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
	
	public Contact getRecord(String idd) {
		if (logging_enable) Logger.I(TAG, "Phonebook.getRecord("+idd+")");
		try {
			if (!checkState())
				return null;

			Contact cc = accessor.getContact(idd);
			if (cc == null)
				Logger.W(TAG, "Phonebook.getRecord() contact record not found.");
				
			return cc;
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
			contactList.remove(contact.getField(Phonebook.PB_ID));
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
			contactList.put(contact.getField(Phonebook.PB_ID), contact);
		}
		catch (Exception e) {
			Logger.E(TAG, e);
		}
	}
}
