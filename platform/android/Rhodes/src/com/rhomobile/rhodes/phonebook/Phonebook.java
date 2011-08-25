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

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import android.os.Build;

import com.rhomobile.rhodes.Capabilities;
import com.rhomobile.rhodes.Logger;

//@RubyLevelClass(name="Phonebook")
public class Phonebook {

	private static final String TAG = "Phonebook";
	private static final boolean logging_enable = false;
	
	public static final String PB_ID = "id";
	public static final String PB_FIRST_NAME = "first_name";
	public static final String PB_LAST_NAME = "last_name";
	public static final String PB_MOBILE_NUMBER = "mobile_number";
	public static final String PB_HOME_NUMBER = "home_number";
	public static final String PB_BUSINESS_NUMBER = "business_number";
	public static final String PB_EMAIL_ADDRESS = "email_address";
	public static final String PB_COMPANY_NAME = "company_name";
	
	public static final int PB_FIELDS_COUNT = 8;
	public static final int PB_I_ID = 0;
	public static final int PB_I_FIRST_NAME = 1;
	public static final int PB_I_LAST_NAME = 2;
	public static final int PB_I_MOBILE_NUMBER = 3;
	public static final int PB_I_HOME_NUMBER = 4;
	public static final int PB_I_BUSINESS_NUMBER = 5;
	public static final int PB_I_EMAIL_ADDRESS = 6;
	public static final int PB_I_COMPANY_NAME = 7;
	
	
	
	
	
	private Map<String, Contact> contactList;
	private ContactAccessor accessor;
	private Iterator<Contact> iter = null;
	private boolean mIsContactsFilled = false;

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
	
	public int queryContactCount() {
		int res = 0;
		if (checkState()) {
			try {
				res = accessor.getCount();
			} catch (Exception e) {
				Logger.E(TAG, e);
			}
		}
		return res;
	}
	
	public void queryAllContacts() { queryContacts(0, -1); }

	public void queryContacts(int offset, int max_results) {
		if (!checkState())
			return;
		try {
			if (!mIsContactsFilled) {
				Logger.I(TAG, "Phonebook.prepareFullList()");
				contactList = accessor.getContacts(offset, max_results);
				mIsContactsFilled = true;
			}
			//moveToBegin();
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
//			if (!mIsContactsFilled) {
//				prepareFullList();
//			}	
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
			//if (!mIsContactsFilled) {
			//	prepareFullList();
			//}	
			
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
			//if (!mIsContactsFilled) {
			//	prepareFullList();
			//}	
			
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
			//if (!mIsContactsFilled) {
			//	prepareFullList();
			//}	
			
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
			//if (!mIsContactsFilled) {
			//	prepareFullList();
			//}	
			
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
			//if (!checkState())
			//	return null;
			if (contactList == null) {
				if (logging_enable) Logger.I(TAG, "Phonebook.getRecord() contackList is null !");
			}
			if ((accessor != null) && (accessor instanceof ContactAccessorNew)) {
			//if (false) {
				if (logging_enable) Logger.I(TAG, "Phonebook.getRecord() new accessor !");
				Contact c = contactList.get(idd);
				if (c != null) {
					if (logging_enable) Logger.I(TAG, "Phonebook.getRecord() found in list");
					return c;
				}
				c = ((ContactAccessorNew)accessor).getContactByID(Contact.convertRhodeIDtoPlatformID(idd));
				if (c != null) {
					if (logging_enable) Logger.I(TAG, "Phonebook.getRecord() found in system");
					contactList.put(c.getField(Phonebook.PB_I_ID), c);
				}
				else {
					if (logging_enable) Logger.I(TAG, "Phonebook.getRecord() not found in system");
				}
				return c;
			}
			else {
				if (logging_enable) Logger.I(TAG, "Phonebook.getRecord() old accessor class");
				if (!mIsContactsFilled) {
					queryAllContacts();
				}	
			}
			Contact cc = contactList.get(idd);
			if (cc != null) {
				if (logging_enable) Logger.I(TAG, "Phonebook.getRecord() return record");
			}
			else {
				if (logging_enable) Logger.I(TAG, "Phonebook.getRecord() return NULL");
			}
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
			contactList.remove(contact.getField(Phonebook.PB_I_ID));
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
			contactList.put(contact.getField(Phonebook.PB_I_ID), contact);
		}
		catch (Exception e) {
			Logger.E(TAG, e);
		}
	}

}
