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
import java.util.List;
import java.util.Map;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesService;

import android.accounts.Account;
import android.accounts.AccountManager;
import android.content.ContentResolver;
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.net.Uri;
import android.provider.ContactsContract.CommonDataKinds.Email;
import android.provider.ContactsContract.CommonDataKinds.Organization;
import android.provider.ContactsContract.CommonDataKinds.Phone;
import android.provider.ContactsContract.CommonDataKinds.StructuredName;
import android.provider.ContactsContract.Data;
import android.provider.ContactsContract.RawContacts;
import android.provider.ContactsContract.Contacts;

public class ContactAccessorNew implements ContactAccessor {
	
	private static final String TAG = "ContactsAccessorNew";
	private static final boolean DEBUG = false;
	
	private ContentResolver cr;
	private String accName;
	private String accType;
	
	public ContactAccessorNew() throws Exception {
		Context ctx = RhodesService.getContext();
		
		Account[] accounts = AccountManager.get(ctx).getAccounts();
		if (accounts.length == 0) {
			accName = null;//"rhodes@rhomobile.com";
			accType = null;//"com.rhomobile";
		}
		else {
			Account acnt = accounts[0];
			accName = acnt.name;
			accType = acnt.type;
		}
		
		cr = ctx.getContentResolver();
	}
	
	public void fillName(String id, Contact contact) {
		if (DEBUG)
			Logger.I(TAG, "fillName("+id+")");
		
		contact.setField(Phonebook.PB_FIRST_NAME, "");
		contact.setField(Phonebook.PB_LAST_NAME, "");
		
		Cursor cursor = cr.query(Data.CONTENT_URI,
				new String[] { StructuredName.GIVEN_NAME, StructuredName.FAMILY_NAME, StructuredName.DISPLAY_NAME },
				Data.CONTACT_ID + "=? AND " + Data.MIMETYPE + "=?",
				new String[] { id, StructuredName.CONTENT_ITEM_TYPE },
				null);
		try {
			if (!cursor.moveToFirst()) {
				if (DEBUG)
					Logger.D(TAG, "No name data records are found.");
				return;
			}
			
			String firstName = cursor.getString(cursor.getColumnIndex(StructuredName.GIVEN_NAME));
			String lastName = cursor.getString(cursor.getColumnIndex(StructuredName.FAMILY_NAME));
			if (firstName != null || lastName != null) {
				if (DEBUG)
					Logger.D(TAG, "fillName() firstName=" + firstName + ", lastName=" + lastName);
				
				if (firstName != null)
					contact.setField(Phonebook.PB_FIRST_NAME, firstName);
				if (lastName != null)
					contact.setField(Phonebook.PB_LAST_NAME, lastName);
			}
			else {
				String displayName = cursor.getString(cursor.getColumnIndex(StructuredName.DISPLAY_NAME));
				if (DEBUG)
					Logger.D(TAG, "fillName() displayName=" + displayName);
				
				if (displayName != null) {
					String[] names = displayName.split(" ");
					if (names.length == 1) {
						contact.setField(Phonebook.PB_FIRST_NAME, names[0]);
					}
					else if (names.length > 1) {
						contact.setField(Phonebook.PB_FIRST_NAME, names[0]);
						contact.setField(Phonebook.PB_LAST_NAME, names[1]);
					}
				}
				else {
					if (DEBUG)
						Logger.D(TAG, "fillName() name is null");
				}
			}
		}
		finally {
			cursor.close();
		}
	}
	
	public void fillPhones(String id, Contact contact) {
		Cursor cursor = cr.query(Data.CONTENT_URI,
				new String[] {Phone.NUMBER, Phone.TYPE},
				Data.RAW_CONTACT_ID + "=? AND " + Data.MIMETYPE + "=?",
				new String[] {id, Phone.CONTENT_ITEM_TYPE},
				null);
		try {
			if (!cursor.moveToFirst())
				return;
			
			int numColumn = cursor.getColumnIndex(Phone.NUMBER);
			int typeColumn = cursor.getColumnIndex(Phone.TYPE);
			do {
				switch (cursor.getInt(typeColumn)) {
				case Phone.TYPE_HOME:
					contact.setField(Phonebook.PB_HOME_NUMBER, cursor.getString(numColumn));
					break;
				case Phone.TYPE_WORK:
					contact.setField(Phonebook.PB_BUSINESS_NUMBER, cursor.getString(numColumn));
					break;
				case Phone.TYPE_MOBILE:
					contact.setField(Phonebook.PB_MOBILE_NUMBER, cursor.getString(numColumn));
					break;
				}
			} while (cursor.moveToNext());
		}
		finally {
			cursor.close();
		}
	}
	
	public void fillEmails(String id, Contact contact) {
		Cursor cursor = cr.query(Data.CONTENT_URI,
				new String[] {Email.DATA},
				Data.RAW_CONTACT_ID + "=? AND " + Data.MIMETYPE + "=?",
				new String[] {id, Email.CONTENT_ITEM_TYPE},
				null);
		try {
			if (!cursor.moveToFirst())
				return;
			
			String data = cursor.getString(cursor.getColumnIndex(Email.DATA));
			contact.setField(Phonebook.PB_EMAIL_ADDRESS, data);
		}
		finally {
			cursor.close();
		}
	}
	
	public void fillCompany(String id, Contact contact) {
		Cursor cursor = cr.query(Data.CONTENT_URI,
				new String[] {Organization.COMPANY},
				Data.RAW_CONTACT_ID + "=? AND " + Data.MIMETYPE + "=?",
				new String[] {id, Organization.CONTENT_ITEM_TYPE},
				null);
		try {
			if (!cursor.moveToFirst())
				return;
			String company = cursor.getString(cursor.getColumnIndex(Organization.COMPANY));
			contact.setField(Phonebook.PB_COMPANY_NAME, company);
		}
		finally {
			cursor.close();
		}
	}
	
	@Override
	public int getCount(int offset, int max_results) {
		StringBuilder sortMode = new StringBuilder();
		if (max_results > 0 || offset > 0) {
			sortMode.append(Contacts._ID).append(" ASC");
			if (max_results > 0)
				sortMode.append(" LIMIT ").append(max_results);
			if (offset > 0)
				sortMode.append(" OFFSET ").append(offset);
		}

		Cursor cursor = cr.query(Contacts.CONTENT_URI,
				new String[] {Contacts._ID},
				null, null, sortMode.toString());
		int count = -1;
		try {
			count = cursor.getCount();
		} finally {
			cursor.close();
		}
		return count;
	}

	@Override
	public Map<String, Contact> getContacts(int offset, int max_results, List<String> select) throws Exception {
		Map<String, Contact> contacts = new HashMap<String, Contact>();
		
		StringBuilder sortMode = new StringBuilder();
		sortMode.append(Contacts.DISPLAY_NAME);//.append(" ASC");
		if (max_results > 0)
			sortMode.append(" LIMIT ").append(max_results);
		if (offset > 0)
			sortMode.append(" OFFSET ").append(offset);
		
		Cursor cursor = cr.query(Contacts.CONTENT_URI,
				new String[] {Contacts._ID, Contacts.LOOKUP_KEY, Contacts.DISPLAY_NAME},
				null, null, sortMode.toString());
		try {
			if (!cursor.moveToFirst())
				return contacts;
			
			do {
				
				String key = cursor.getString(cursor.getColumnIndex(Contacts.LOOKUP_KEY));
				String id = cursor.getString(cursor.getColumnIndex(Contacts._ID));
				String displayName = cursor.getString(cursor.getColumnIndex(Contacts.DISPLAY_NAME));
				Contact contact = new Contact(this, key, displayName);
				
				if (select != null) {
					if (select.contains(Phonebook.PB_FIRST_NAME) ||
					    select.contains(Phonebook.PB_LAST_NAME)) {
						fillName(id, contact);
					}
					if (select.contains(Phonebook.PB_MOBILE_NUMBER) ||
					    select.contains(Phonebook.PB_HOME_NUMBER) ||
					    select.contains(Phonebook.PB_BUSINESS_NUMBER)) {
						fillPhones(id, contact);
					}
					if (select.contains(Phonebook.PB_EMAIL_ADDRESS)) {
						fillEmails(id, contact);
					}
					if (select.contains(Phonebook.PB_COMPANY_NAME)) {
						fillCompany(id, contact);
					}
				}
				
				if (DEBUG) Logger.D(TAG, "Filling contact with ID: " + key);
				contacts.put(contact.id(), contact);
				
			} while (cursor.moveToNext());
		}
		finally {
			cursor.close();
		}
		
		return contacts;
	}
	
	public Contact getContact(String lookupKey) throws Exception {
		if (DEBUG)
			Logger.D(TAG, "getContact(lookupKey="+lookupKey+")");
		
		Contact contact = null; 
		
		Cursor cursor = cr.query(Contacts.CONTENT_URI,
				new String[] {Contacts.DISPLAY_NAME, Contacts._ID},
				Contacts.LOOKUP_KEY + "=?",
				new String[] {lookupKey}, null);		
		
		try {
			if (!cursor.moveToFirst()) {
				Logger.D(TAG, "getContact() not found");
				return null;
			}
			contact = new Contact(this, lookupKey, cursor.getString(cursor.getColumnIndex(Contacts.DISPLAY_NAME)));

			String id = cursor.getString(cursor.getColumnIndex(Contacts._ID));
			Logger.D(TAG, "Contact id: " + id);

			fillName(id, contact);
			fillPhones(id, contact);
			fillEmails(id, contact);
			fillCompany(id, contact);
		}
		finally {
			cursor.close();
		}
		if (DEBUG)
			Logger.D(TAG, "getContactByID() found");
		return contact;
	}

    private String queryContactId(String key)
    {
        Cursor cursor = cr.query(Contacts.CONTENT_URI,
                new String[] {Contacts._ID},
                Contacts.LOOKUP_KEY + "=?",
                new String[] {key}, null);
        
        long contactId = 0;
        try {
            if (cursor.moveToFirst())
                contactId = cursor.getLong(cursor.getColumnIndex(Contacts._ID));
        }
        finally {
            cursor.close();
        }
        if (contactId == 0) {
            Logger.D(TAG, "No contact found for key: " + key);
        }
        return String.valueOf(contactId);
    }

	private String create(Contact contact) throws Exception {
		String key = contact.id();
		
		if (key == null || key.length() == 0) {
		    
		    // Create RawContact
			ContentValues values = new ContentValues();
			if (accName != null && accName.length() > 0) {
				values.put(RawContacts.ACCOUNT_NAME, accName);
				values.put(RawContacts.ACCOUNT_TYPE, accType);
			}
			//values.put(RawContacts.AGGREGATION_MODE, RawContacts.AGGREGATION_MODE_DISABLED);
			Uri uri = cr.insert(RawContacts.CONTENT_URI, values);
			String id = String.valueOf(ContentUris.parseId(uri));

			//Query Contact ID
			Cursor cursor = cr.query(RawContacts.CONTENT_URI,
			        new String[] {RawContacts.CONTACT_ID},
			        RawContacts._ID + "=?",
			        new String[] {id}, null);

            String contactId = new String();
            try {
                if (cursor.moveToFirst())
                    contactId = String.valueOf(cursor.getLong(cursor.getColumnIndex(RawContacts.CONTACT_ID)));
            }
            finally {
                cursor.close();
            }
            if (contactId.length() == 0) {
                Logger.D(TAG, "No raw contact found for new raw id: " + id);
            }

            // Query lookup key
            cursor = cr.query(Contacts.CONTENT_URI,
                    new String[] {Contacts.LOOKUP_KEY},
                    Contacts._ID + "=?",
                    new String[] {String.valueOf(contactId)}, null);

            try {
                if (cursor.moveToFirst())
                    key = cursor.getString(cursor.getColumnIndex(Contacts.LOOKUP_KEY));
            }
            finally {
                cursor.close();
            }
            if (key == null || key.length() == 0) {
                Logger.D(TAG, "No contact found for new contact id: " + contactId);
            }

            contact.setId(key);

            Logger.I(TAG, "New contact id: " + contactId);

            return contactId;
        }
        return queryContactId(key);
	}
	
	private void saveData(String contactId, ContentValues values, String where, String[] whereValues) throws Exception
	{
		StringBuilder w0 = new StringBuilder();
		w0.append(Data.CONTACT_ID + "=? AND " + Data.MIMETYPE + "=?");
		if (where != null)
			w0.append(" AND " + where);

		String mimetype = (String)values.get(Data.MIMETYPE);
		int wvSize0 = 2;
		if (whereValues != null)
			wvSize0 += whereValues.length;
		String[] wv0 = new String[wvSize0];
		wv0[0] = contactId;
		wv0[1] = mimetype;
		if (whereValues != null)
			System.arraycopy(whereValues, 0, wv0, 2, whereValues.length);

		Cursor cursor = cr.query(Data.CONTENT_URI,
				new String[] {Data._ID, Data.RAW_CONTACT_ID},
				w0.toString(),
				wv0, null);		

		long rawDataId = 0;
		try {
			if (cursor.moveToFirst()) {
				rawDataId = cursor.getLong(cursor.getColumnIndex(Data._ID));
			}
		}
		finally {
			cursor.close();
		}
		if (rawDataId == 0) {
			Logger.D(TAG, "No data records found for contact: " + contactId);
		}

		// Do update if found, insert otherwise
		if (rawDataId != 0) {

			// Search is there is already record with the same raw_contact_id and mimetype

			StringBuilder w1 = new StringBuilder();
			w1.append(Data._ID + "=? AND " + Data.MIMETYPE + "=?");
			if (where != null)
				w1.append(" AND " + where);
			//where = w1.toString();

			int wvSize = 2;
			if (whereValues != null)
				wvSize += whereValues.length;
			String[] wv1 = new String[wvSize];
			wv1[0] = String.valueOf(rawDataId);
			wv1[1] = mimetype;
			if (whereValues != null)
				System.arraycopy(whereValues, 0, wv1, 2, whereValues.length);
			//whereValues = wv1;
			
			Logger.D(TAG, "Updating contact: " + values.toString());
			Logger.D(TAG, "Updating contact: " + w1 + "; " + wv1);
			int res = cr.update(Data.CONTENT_URI, values, w1.toString(), wv1);
			Logger.D(TAG, "Contact updated (id:" + contactId + ",dataId:" + rawDataId + "): " + res + " rows.");
		} else {
			cursor = cr.query(RawContacts.CONTENT_URI,
							  new String[] { RawContacts._ID },
							  RawContacts.CONTACT_ID + "=?",
							  new String[] { String.valueOf(contactId) }, null);
			
			long rawContactId = 0;
			try {
				if (cursor.moveToFirst()) {
					rawContactId = cursor.getLong(cursor.getColumnIndex(RawContacts._ID));
					//rawContactId = cursor.getLong(cursor.getColumnIndex(Data.RAW_CONTACT_ID));
				}
			}
			finally {
				cursor.close();
			}
			if (rawContactId == 0) {
				Logger.D(TAG, "No raw contact records found for contact: " + contactId);
			}
			
			values.put(Data.RAW_CONTACT_ID, rawContactId);
			Uri uri = cr.insert(Data.CONTENT_URI, values);
			Logger.D(TAG, "New data record has inserved for raw contact: " + rawContactId + ", URI: " + uri);
		}
	}
	
	private void saveData(String contactId, ContentValues values) throws Exception {
		saveData(contactId, values, null, null);
	}
	
	private void updateName(String contactId, Contact contact) throws Exception {
		String firstName = contact.getField(Phonebook.PB_FIRST_NAME);
		String lastName = contact.getField(Phonebook.PB_LAST_NAME);

		ContentValues values = new ContentValues();
		values.put(Data.MIMETYPE, StructuredName.CONTENT_ITEM_TYPE);
		values.put(StructuredName.DISPLAY_NAME, firstName + " " + lastName);
		values.put(StructuredName.GIVEN_NAME, firstName);
		values.put(StructuredName.FAMILY_NAME, lastName);
		saveData(contactId, values);
	}
	
	private void updatePhones(String contactId, Contact contact) throws Exception {
		String[] phones = {Phonebook.PB_MOBILE_NUMBER, Phonebook.PB_HOME_NUMBER, Phonebook.PB_BUSINESS_NUMBER};
		int[] types = {Phone.TYPE_MOBILE, Phone.TYPE_HOME, Phone.TYPE_WORK};
		
		for (int i = 0; i < phones.length; ++i) {
			String value = contact.getField(phones[i]);
			if (value == null || value.length() == 0)
				continue;
			
			ContentValues values = new ContentValues();
			values.put(Data.MIMETYPE, Phone.CONTENT_ITEM_TYPE);
			values.put(Phone.NUMBER, value);
			values.put(Phone.TYPE, types[i]);
			saveData(contactId, values, Phone.TYPE + "=?", new String[] {String.valueOf(types[i])});
		}
	}
	
	private void updateEmails(String contactId, Contact contact) throws Exception {
		String value = contact.getField(Phonebook.PB_EMAIL_ADDRESS);
		if (value == null || value.length() == 0)
			return;
		
		ContentValues values = new ContentValues();
		values.put(Data.MIMETYPE, Email.CONTENT_ITEM_TYPE);
		values.put(Email.DATA, value);
		saveData(contactId, values);
	}
	
	private void updateCompany(String contactId, Contact contact) throws Exception {
		String value = contact.getField(Phonebook.PB_COMPANY_NAME);
		if (value == null || value.length() == 0)
			return;
		
		ContentValues values = new ContentValues();
		values.put(Data.MIMETYPE, Organization.CONTENT_ITEM_TYPE);
		values.put(Organization.COMPANY, value);
		saveData(contactId, values);
	}

	public void save(Contact contact) throws Exception {
		String contactId = create(contact);
		
		updateName(contactId, contact);
		updatePhones(contactId, contact);
		updateEmails(contactId, contact);
		updateCompany(contactId, contact);
//		contact.makeAllFilled();
	}

    public void remove(Contact contact) {
        String key = contact.id();
        if (key == null || key.length() == 0)
            return;

        String id = queryContactId(contact.id());
        if (id == null || id.length() == 0)
            return;

        Logger.D(TAG, "Removing contact: " + id);

        cr.delete(RawContacts.CONTENT_URI, RawContacts.CONTACT_ID + "=?", new String[] {id});
    }
}
