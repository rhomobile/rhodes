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
