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


import com.rhomobile.rhodes.Logger;

public class Contact {
	
	private static final String TAG = "Contact";	
	//private static final boolean logging_enable = false;
	
	private String id = null;
	
	//private Map<String, ContactField> fields = new HashMap<String, ContactField>();
	//private Iterator<ContactField> iter = null;
	private ContactAccessor mAccessor = null;

	private boolean mIsNameFilled = false;
	private boolean mIsPhoneFilled = false;
	private boolean mIsEmailsFilled = false;
	private boolean mIsCompanyFilled = false;
	
	private String mFields[] = new String[Phonebook.PB_FIELDS_COUNT];
	
	
	public Contact() {
		//moveToBegin();
	}
	
	public void setAccessor(ContactAccessor accessor) {
		mAccessor = accessor;		
	}
	
	public void makeAllFilled() {
		mIsNameFilled = true;
		mIsPhoneFilled = true;
		mIsEmailsFilled = true;
		mIsCompanyFilled = true;
	}
	
	/*
	private boolean isFullFilled() {
		return (mIsNameFilled && mIsPhoneFilled && mIsEmailsFilled && mIsCompanyFilled);
	}
	*/

	
	public void setFieldInner(int key, String value) {
		if ((key >= 0) && (key < Phonebook.PB_FIELDS_COUNT)) {
			mFields[key] = value;
		}
		else {
			Logger.E(TAG, "Invalid field index !");
		}
	}
	
	
	private void prepareField(int key) {
		if ((mAccessor != null) && (mAccessor instanceof ContactAccessorNew)) {
			
			if ((!mIsNameFilled) && ((key == Phonebook.PB_I_FIRST_NAME) || (key == Phonebook.PB_I_LAST_NAME))) {
				((ContactAccessorNew)mAccessor).fillName(id, this);
				mIsNameFilled = true;
			}
			else {
				if ((!mIsPhoneFilled) && ((key == Phonebook.PB_I_MOBILE_NUMBER) || (key == Phonebook.PB_I_HOME_NUMBER) || (key == Phonebook.PB_I_BUSINESS_NUMBER))) {
					((ContactAccessorNew)mAccessor).fillPhones(id, this);
					mIsPhoneFilled = true;
				}
				else {
					if ((!mIsEmailsFilled) && (key == Phonebook.PB_I_EMAIL_ADDRESS)) {
						((ContactAccessorNew)mAccessor).fillEmails(id, this);
						mIsEmailsFilled = true;
					}
					else {
						if ((mIsCompanyFilled) && (key == Phonebook.PB_I_COMPANY_NAME)) {
							((ContactAccessorNew)mAccessor).fillCompany(id, this);
							mIsCompanyFilled = true;
						}
					}
				}
			}	
		}
	}
	
	public void setField(int key, String value)
	{
		if ((key >= 0) && (key < Phonebook.PB_FIELDS_COUNT)) {
			prepareField(key);
			setFieldInner(key, value);
		}
		else {
			Logger.E(TAG, "Invalid field index !");
		}
	}
	
	public String getField(int key)
	{
		if ((key >= 0) && (key < Phonebook.PB_FIELDS_COUNT)) {
			prepareField(key);
			return mFields[key];
		}
		return null;
	}
	
	public void setId(String v) {
		id = v;
		setFieldInner(Phonebook.PB_I_ID, convertPlatformIDtoRhodeID(v));
	}
	
	public static String convertPlatformIDtoRhodeID(String id) {
		return "{" + id + "}";
	}

	public static String convertRhodeIDtoPlatformID(String id) {
		return id.substring(1, id.length()-1);
	}
	
	public String id() {
		return id;
	}

	/*
	private void fillAll() {
		if (!isFullFilled()) {
			int i;
			for (i = 0; i < Phonebook.PB_FIELDS_COUNT; i++) {
				prepareField(i);
			}
		}
	}
	*/
	

	/*
	public void moveToBegin() {
		fillAll();
		iter = fields.values().iterator();
	}
	
	public boolean hasNext() {
		return iter.hasNext();
	}
	
	public Object next() {
		return iter.next();
	}
	*/
}
