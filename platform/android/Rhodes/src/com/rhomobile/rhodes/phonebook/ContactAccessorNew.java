package com.rhomobile.rhodes.phonebook;

import java.util.HashMap;
import java.util.Map;

import com.rhomobile.rhodes.Rhodes;
import com.rhomobile.rhodes.RhodesInstance;

import android.accounts.Account;
import android.accounts.AccountManager;
import android.content.ContentResolver;
import android.content.ContentUris;
import android.content.ContentValues;
import android.database.Cursor;
import android.net.Uri;
import android.provider.ContactsContract.CommonDataKinds.Email;
import android.provider.ContactsContract.CommonDataKinds.Organization;
import android.provider.ContactsContract.CommonDataKinds.Phone;
import android.provider.ContactsContract.CommonDataKinds.StructuredName;
import android.provider.ContactsContract.Data;
import android.provider.ContactsContract.RawContacts;

public class ContactAccessorNew implements ContactAccessor {
	
	//private static final String TAG = "ContactsAccessorNew";
	
	private static final String PB_ID = Phonebook.PB_ID;
	private static final String PB_FIRST_NAME = Phonebook.PB_FIRST_NAME;
	private static final String PB_LAST_NAME = Phonebook.PB_LAST_NAME;
	private static final String PB_MOBILE_NUMBER = Phonebook.PB_MOBILE_NUMBER;
	private static final String PB_HOME_NUMBER = Phonebook.PB_HOME_NUMBER;
	private static final String PB_BUSINESS_NUMBER = Phonebook.PB_BUSINESS_NUMBER;
	private static final String PB_EMAIL_ADDRESS = Phonebook.PB_EMAIL_ADDRESS;
	private static final String PB_COMPANY_NAME = Phonebook.PB_COMPANY_NAME;
	
	private ContentResolver cr;
	private String accName;
	private String accType;
	
	public ContactAccessorNew() throws Exception {
		Rhodes r = RhodesInstance.getInstance();
		
		Account[] accounts = AccountManager.get(r).getAccounts();
		if (accounts.length == 0) {
			accName = "rhodes@rhomobile.com";
			accType = "com.rhomobile";
		}
		else {
			Account acnt = accounts[0];
			accName = acnt.name;
			accType = acnt.type;
		}
		
		cr = r.getContentResolver();
	}
	
	private void fillName(String id, Contact contact) {
		contact.setField(PB_FIRST_NAME, "");
		contact.setField(PB_LAST_NAME, "");
		
		Cursor cursor = cr.query(Data.CONTENT_URI,
				new String[] {StructuredName.DISPLAY_NAME},
				Data.RAW_CONTACT_ID + "=? AND " + Data.MIMETYPE + "=?",
				new String[] {id, StructuredName.CONTENT_ITEM_TYPE},
				null);
		try {
			if (!cursor.moveToFirst())
				return;
			
			String name = cursor.getString(cursor.getColumnIndex(StructuredName.DISPLAY_NAME));
			if (name != null) {
				String[] names = name.split(" ");
				if (names.length == 1) {
					contact.setField(PB_FIRST_NAME, name);
				}
				else {
					contact.setField(PB_FIRST_NAME, names[0]);
					contact.setField(PB_LAST_NAME, names[1]);
				}
			}
		}
		finally {
			cursor.close();
		}
	}
	
	private void fillPhones(String id, Contact contact) {
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
					contact.setField(PB_HOME_NUMBER, cursor.getString(numColumn));
					break;
				case Phone.TYPE_WORK:
					contact.setField(PB_BUSINESS_NUMBER, cursor.getString(numColumn));
					break;
				case Phone.TYPE_MOBILE:
					contact.setField(PB_MOBILE_NUMBER, cursor.getString(numColumn));
					break;
				}
			} while (cursor.moveToNext());
		}
		finally {
			cursor.close();
		}
	}
	
	private void fillEmails(String id, Contact contact) {
		Cursor cursor = cr.query(Data.CONTENT_URI,
				new String[] {Email.DATA},
				Data.RAW_CONTACT_ID + "=? AND " + Data.MIMETYPE + "=?",
				new String[] {id, Email.CONTENT_ITEM_TYPE},
				null);
		try {
			if (!cursor.moveToFirst())
				return;
			
			String data = cursor.getString(cursor.getColumnIndex(Email.DATA));
			contact.setField(PB_EMAIL_ADDRESS, data);
		}
		finally {
			cursor.close();
		}
	}
	
	private void fillCompany(String id, Contact contact) {
		Cursor cursor = cr.query(Data.CONTENT_URI,
				new String[] {Organization.COMPANY},
				Data.RAW_CONTACT_ID + "=? AND " + Data.MIMETYPE + "=?",
				new String[] {id, Organization.CONTENT_ITEM_TYPE},
				null);
		try {
			if (!cursor.moveToFirst())
				return;
			String company = cursor.getString(cursor.getColumnIndex(Organization.COMPANY));
			contact.setField(PB_COMPANY_NAME, company);
		}
		finally {
			cursor.close();
		}
	}

	public Map<String, Contact> getAll() throws Exception {
		Map<String, Contact> contacts = new HashMap<String, Contact>();
		
		Cursor cursor = cr.query(RawContacts.CONTENT_URI,
				new String[] {RawContacts._ID},
				RawContacts.DELETED + "=0", null, null);
		try {
			if (!cursor.moveToFirst())
				return contacts;
			
			do {
				Contact contact = new Contact();
				
				String id = cursor.getString(cursor.getColumnIndex(RawContacts._ID));
				contact.setId(id);
				
				fillName(id, contact);
				fillPhones(id, contact);
				fillEmails(id, contact);
				fillCompany(id, contact);
				
				contacts.put(contact.getField(PB_ID), contact);
			} while (cursor.moveToNext());
		}
		finally {
			cursor.close();
		}
		
		return contacts;
	}
	
	private void create(Contact contact) throws Exception {
		String id = contact.id();
		
		if (id == null || id.length() == 0) {
			ContentValues values = new ContentValues();
			values.put(RawContacts.ACCOUNT_NAME, accName);
			values.put(RawContacts.ACCOUNT_TYPE, accType);
			values.put(RawContacts.AGGREGATION_MODE, RawContacts.AGGREGATION_MODE_DISABLED);
			Uri uri = cr.insert(RawContacts.CONTENT_URI, values);
			id = String.valueOf(ContentUris.parseId(uri));
			contact.setId(id);
		}
	}
	
	private void saveData(Contact contact, ContentValues values, String where, String[] whereValues) throws Exception {
		String id = contact.id();
		values.put(Data.RAW_CONTACT_ID, Long.parseLong(id));
		
		// Search is there is already record with the same raw_contact_id and mimetype
		long dataId = 0;
		
		StringBuilder w = new StringBuilder();
		w.append(Data.RAW_CONTACT_ID + "=? AND " + Data.MIMETYPE + "=?");
		if (where != null)
			w.append(" AND " + where);
		where = w.toString();
		
		String mimetype = (String)values.get(Data.MIMETYPE);
		int wvSize = 2;
		if (whereValues != null)
			wvSize += whereValues.length;
		String[] wv = new String[wvSize];
		wv[0] = id;
		wv[1] = mimetype;
		if (whereValues != null)
			System.arraycopy(whereValues, 0, wv, 2, whereValues.length);
		
		Cursor cursor = cr.query(Data.CONTENT_URI, new String[] {Data._ID}, where, wv, null);
		try {
			if (cursor.moveToFirst())
				dataId = cursor.getLong(cursor.getColumnIndex(Data._ID));
		}
		finally {
			cursor.close();
		}
		
		// Do update if found, insert otherwise
		if (dataId != 0)
			cr.update(Data.CONTENT_URI, values,
					Data._ID + "=?", new String[] {String.valueOf(dataId)});
		else
			cr.insert(Data.CONTENT_URI, values);
	}
	
	private void saveData(Contact contact, ContentValues values) throws Exception {
		saveData(contact, values, null, null);
	}
	
	private void updateName(Contact contact) throws Exception {
		String firstName = contact.getField(PB_FIRST_NAME);
		String lastName = contact.getField(PB_LAST_NAME);

		ContentValues values = new ContentValues();
		values.put(Data.MIMETYPE, StructuredName.CONTENT_ITEM_TYPE);
		values.put(StructuredName.DISPLAY_NAME, firstName + " " + lastName);
		values.put(StructuredName.GIVEN_NAME, firstName);
		values.put(StructuredName.FAMILY_NAME, lastName);
		saveData(contact, values);
	}
	
	private void updatePhones(Contact contact) throws Exception {
		String[] phones = {PB_MOBILE_NUMBER, PB_HOME_NUMBER, PB_BUSINESS_NUMBER};
		int[] types = {Phone.TYPE_MOBILE, Phone.TYPE_HOME, Phone.TYPE_WORK};
		
		for (int i = 0; i < phones.length; ++i) {
			String phName = phones[i];
			String value = contact.getField(phName);
			if (value == null || value.length() == 0)
				continue;
			
			ContentValues values = new ContentValues();
			values.put(Data.MIMETYPE, Phone.CONTENT_ITEM_TYPE);
			values.put(Phone.NUMBER, value);
			values.put(Phone.TYPE, types[i]);
			saveData(contact, values, Phone.TYPE + "=?", new String[] {String.valueOf(types[i])});
		}
	}
	
	private void updateEmails(Contact contact) throws Exception {
		String value = contact.getField(PB_EMAIL_ADDRESS);
		if (value == null || value.length() == 0)
			return;
		
		ContentValues values = new ContentValues();
		values.put(Data.MIMETYPE, Email.CONTENT_ITEM_TYPE);
		values.put(Email.DATA, value);
		saveData(contact, values);
	}
	
	private void updateCompany(Contact contact) throws Exception {
		String value = contact.getField(PB_COMPANY_NAME);
		if (value == null || value.length() == 0)
			return;
		
		ContentValues values = new ContentValues();
		values.put(Data.MIMETYPE, Organization.CONTENT_ITEM_TYPE);
		values.put(Organization.COMPANY, value);
		saveData(contact, values);
	}

	public void save(Contact contact) throws Exception {
		create(contact);
		
		updateName(contact);
		updatePhones(contact);
		updateEmails(contact);
		updateCompany(contact);
	}
	
	public void remove(Contact contact) {
		String id = contact.id();
		if (id == null || id.length() == 0)
			return;
		cr.delete(RawContacts.CONTENT_URI, RawContacts._ID + "=?", new String[] {id});
	}

}
