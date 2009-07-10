package com.rhomobile.rhodes.phonebook;

import android.content.ContentUris;
import android.content.ContentValues;
import android.database.Cursor;
import android.net.Uri;
import android.provider.Contacts;
import android.provider.Contacts.Organizations;
import android.provider.Contacts.People;
import android.provider.Contacts.Phones;

import com.rhomobile.rhodes.Platform;
import com.rhomobile.rhodes.RhodesInstance;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyString;
import com.xruby.runtime.builtin.RubyHash;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.lang.*;

//@RubyLevelClass(name="Phonebook")
public class RhoPhonebook extends RubyBasic {

	private ContactList contactList = new ContactList();
	private Platform activity;
	private Cursor cursor;

	static final RubyString RUBY_PB_ID = ObjectFactory.createString("id");
	static final RubyString RUBY_PB_FIRST_NAME = ObjectFactory
			.createString("first_name");
	static final RubyString RUBY_PB_LAST_NAME = ObjectFactory
			.createString("last_name");
	static final RubyString RUBY_PB_MOBILE_NUMBER = ObjectFactory
			.createString("mobile_number");
	static final RubyString RUBY_PB_HOME_NUMBER = ObjectFactory
			.createString("home_number");
	static final RubyString RUBY_PB_BUSINESS_NUMBER = ObjectFactory
			.createString("business_number");
	static final RubyString RUBY_PB_EMAIL_ADDRESS = ObjectFactory
			.createString("email_address");
	static final RubyString RUBY_PB_COMPANY_NAME = ObjectFactory
			.createString("company_name");

	public static class PBRecord extends RubyBasic {

		private Contact contact;

		public PBRecord(RubyClass arg0) {
			super(arg0);
		}

		public PBRecord(Contact contact) {
			super(RubyRuntime.PBRecordClass);

			this.contact = contact;
		}

		public Contact getContact() {
			return contact;
		}

		public void setContact(Contact contact) {
			this.contact = contact;
		}
	}

	public RhoPhonebook(RubyClass arg0) {
		super(arg0);

		try {
			activity = RhodesInstance.getInstance();
			cursor = activity.getContentResolver().query(People.CONTENT_URI,
					null, null, null, null);
			activity.startManagingCursor(cursor);

			// load contacts

			if (cursor.moveToFirst()) {
				do {
					Contact contact = new Contact();

					contact.setField(RUBY_PB_ID.asString(), cursor
							.getString(cursor.getColumnIndex(People._ID)));

					// contact.setField(RUBY_PB_COMPANY_NAME.asString(),
					// cursor.getString(cursor.getColumnIndex(People.COMPANY)));

					String name = cursor.getString(cursor
							.getColumnIndex(People.NAME));

					contact.setField(RUBY_PB_FIRST_NAME.asString(), "");
					contact.setField(RUBY_PB_LAST_NAME.asString(), "");
					
					if (name != null) {
						String[] names = name.split(" ");

						if (names.length == 1) {
							contact.setField(RUBY_PB_FIRST_NAME.asString(), names[0]);
						}
						else if (names.length > 1) {
							contact.setField(RUBY_PB_FIRST_NAME.asString(),
									names[0]);
							contact.setField(RUBY_PB_LAST_NAME.asString(), name
									.replaceFirst(names[0] + " ", ""));
						}
					}

					long personId = cursor.getLong(cursor
							.getColumnIndex(People._ID));

					String whereClause = "person=" + personId;

					String[] phonesProjection = new String[] { Phones.NUMBER,
							Phones.TYPE };
					Cursor phonesCursor = activity.managedQuery(
							Contacts.Phones.CONTENT_URI, phonesProjection,
							whereClause, null, null);

					int phonesCursorCount = phonesCursor.getCount();
					if (phonesCursorCount > 0) {
						phonesCursor.moveToFirst();
						int numberColumn = phonesCursor
								.getColumnIndex(Phones.NUMBER);
						int typeColumn = phonesCursor
								.getColumnIndex(Phones.TYPE);
						for (int j = 0; j < phonesCursorCount; j++) {
							switch (phonesCursor.getInt(typeColumn)) {
							case Phones.TYPE_WORK:
								contact.setField(RUBY_PB_BUSINESS_NUMBER
										.asString(), phonesCursor
										.getString(numberColumn));
								break;
							case Phones.TYPE_HOME:
								contact.setField(
										RUBY_PB_HOME_NUMBER.asString(),
										phonesCursor.getString(numberColumn));
								break;
							case Phones.TYPE_MOBILE:
								contact.setField(RUBY_PB_MOBILE_NUMBER
										.asString(), phonesCursor
										.getString(numberColumn));
								break;
							}
							phonesCursor.moveToNext();
						}
					}

					Uri uri = ContentUris.withAppendedId(People.CONTENT_URI,
							Long.parseLong(contact.getField(RUBY_PB_ID
									.asString())));

					Uri orgUri = Uri.withAppendedPath(uri,
							Contacts.Organizations.CONTENT_DIRECTORY);

					String[] organizationProjection = new String[] { Organizations.COMPANY };
					Cursor organizationCursor = activity.managedQuery(orgUri,
							organizationProjection, whereClause, null, null);

					int organizationCursorCount = organizationCursor.getCount();
					if (organizationCursorCount > 0) {
						organizationCursor.moveToFirst();
						int numberColumn = organizationCursor
								.getColumnIndex(Organizations.COMPANY);

						if (numberColumn != -1)
							contact.setField(RUBY_PB_COMPANY_NAME.asString(),
									organizationCursor.getString(numberColumn));
					}

					String[] contactProjection = new String[] {
							Contacts.ContactMethods.KIND,
							Contacts.ContactMethods.DATA };

					Cursor contactCursor = activity.managedQuery(
							Contacts.ContactMethods.CONTENT_URI,
							contactProjection, whereClause, null, null);

					int contactCursorCount = contactCursor.getCount();
					if (contactCursorCount > 0) {
						contactCursor.moveToFirst();
						int numberColumn = contactCursor
								.getColumnIndex(Contacts.ContactMethods.DATA);
						int typeColumn = contactCursor
								.getColumnIndex(Contacts.ContactMethods.KIND);
						for (int j = 0; j < contactCursorCount; j++) {
							switch (contactCursor.getInt(typeColumn)) {
							case Contacts.ContactMethods.TYPE_HOME:
								contact.setField(RUBY_PB_EMAIL_ADDRESS
										.asString(), contactCursor
										.getString(numberColumn));
								break;
							}
							contactCursor.moveToNext();
						}
					}

					this.contactList.addContact(contact);
				} while (cursor.moveToNext());
			}

		} catch (Exception e) {
			new RubyException(e.getMessage());
		}
	}

	public void close() {
		this.contactList.getContacts().clear();
		activity.stopManagingCursor(cursor);
	}

	public void removeContact(Contact contact) throws Exception {
		Uri uri = People.CONTENT_URI;

		activity.getContentResolver().delete(uri,
				People._ID + "=" + contact.getField(RUBY_PB_ID.asString()),
				null);
	}

	public void saveContact(Contact contact) throws Exception {
		String rbID = contact.getField(RUBY_PB_ID.asString());
		Uri uri = null;

		String name = contact.getField(RUBY_PB_FIRST_NAME.asString()) + " "
				+ contact.getField(RUBY_PB_LAST_NAME.asString());

		boolean isNew = false;
		if (rbID != null && !"".equals(rbID))// update
		{
			uri = ContentUris.withAppendedId(People.CONTENT_URI, Long
					.parseLong(contact.getField(RUBY_PB_ID.asString())));
		} else // create
		{
			isNew = true;
			ContentValues person = new ContentValues();
			person.put(Contacts.People.NAME, name);

			uri = activity.getContentResolver().insert(
					Contacts.People.CONTENT_URI, person);
		}

		if (uri != null) {

			if (!isNew) {
				ContentValues values = new ContentValues();
				values.put(People.NAME, name);

				activity.getContentResolver().update(uri, values, null, null);
			}

			String pathLeaf = (String) uri.getPathSegments().get(
					uri.getPathSegments().size() - 1);

			contact.setField(RUBY_PB_ID.asString(), pathLeaf);

			// add mobile phone number

			if (!contact.getField(RUBY_PB_MOBILE_NUMBER.asString()).equals("")) {
				ContentValues number = new ContentValues();
				number.put(Contacts.Phones.PERSON_ID, pathLeaf);
				number.put(Contacts.Phones.NUMBER, contact
						.getField(RUBY_PB_MOBILE_NUMBER.asString()));
				number.put(Contacts.Phones.TYPE, Contacts.Phones.TYPE_MOBILE);

				Uri phoneUpdate = activity.getContentResolver().insert(
						Contacts.Phones.CONTENT_URI, number);

				if (phoneUpdate == null) {
					int retval = activity.getContentResolver().update(
							People.CONTENT_URI, number, null, null);

					if (retval == 0)
						throw new Exception("Failed to insert mobile number");
				}
			}

			// add home phone number
			if (!contact.getField(RUBY_PB_HOME_NUMBER.asString()).equals("")) {
				ContentValues home = new ContentValues();
				home.put(Contacts.Phones.PERSON_ID, pathLeaf);
				home.put(Contacts.Phones.NUMBER, contact
						.getField(RUBY_PB_HOME_NUMBER.asString()));
				home.put(Contacts.Phones.TYPE, Contacts.Phones.TYPE_HOME);

				Uri homeUpdate = activity.getContentResolver().insert(
						Contacts.Phones.CONTENT_URI, home);

				if (homeUpdate == null) {
					int retval = activity.getContentResolver().update(
							People.CONTENT_URI, home, null, null);

					if (retval == 0)
						throw new Exception("Failed to insert home number");
				}
			}
			// add work phone number
			if (!contact.getField(RUBY_PB_BUSINESS_NUMBER.asString())
					.equals("")) {
				ContentValues work = new ContentValues();
				work.put(Contacts.Phones.PERSON_ID, pathLeaf);
				work.put(Contacts.Phones.NUMBER, contact
						.getField(RUBY_PB_BUSINESS_NUMBER.asString()));
				work.put(Contacts.Phones.TYPE, Contacts.Phones.TYPE_WORK);

				Uri workUpdate = activity.getContentResolver().insert(
						Contacts.Phones.CONTENT_URI, work);

				if (workUpdate == null) {
					int retval = activity.getContentResolver().update(
							People.CONTENT_URI, work, null, null);

					if (retval == 0)
						throw new Exception("Failed to insert work number");
				}
			}
			// add email
			if (!contact.getField(RUBY_PB_EMAIL_ADDRESS.asString()).equals("")) {
				ContentValues email = new ContentValues();
				email.put(Contacts.ContactMethods.PERSON_ID, pathLeaf);
				email.put(Contacts.ContactMethods.KIND,
						Contacts.ContactMethods.TYPE_HOME);

				email.put(Contacts.ContactMethods.DATA, contact
						.getField(RUBY_PB_EMAIL_ADDRESS.asString()));

				email.put(Contacts.ContactMethods.TYPE,
						Contacts.ContactMethods.CONTENT_EMAIL_ITEM_TYPE);

				Uri emailUpdate = activity.getContentResolver().insert(
						Uri.withAppendedPath(uri,
								Contacts.ContactMethods.CONTENT_URI.getPath()
										.substring(1)), email);
				if (emailUpdate == null) {
					throw new Exception("Failed to insert primary email");
				}
			}
			// add organization
			if (!contact.getField(RUBY_PB_COMPANY_NAME.asString()).equals("")) {
				Uri orgUri = Uri.withAppendedPath(uri,
						Contacts.Organizations.CONTENT_DIRECTORY);

				ContentValues company = new ContentValues();
				company.put(Contacts.Organizations.PERSON_ID, pathLeaf);
				company.put(Contacts.Organizations.TYPE,
						Contacts.Organizations.TYPE_WORK);

				company.put(Contacts.Organizations.COMPANY, contact
						.getField(RUBY_PB_COMPANY_NAME.asString()));

				Uri companyUpdate = activity.getContentResolver().insert(
						orgUri, company);

				if (companyUpdate == null) {
					throw new Exception("Failed to insert company");
				}
			}
		}
	}

	// @RubyAllocMethod
	public static RhoPhonebook alloc(RubyValue receiver) {
		return new RhoPhonebook(RubyRuntime.PhonebookClass);
	}

	public static RubyValue openPhonebook() {
		RhoPhonebook pb = alloc(null);
		return pb;
	}

	public static RubyValue closePhonebook(RubyValue arg0) {
		RhoPhonebook pb = (RhoPhonebook) arg0;
		pb.close();

		return RubyConstant.QNIL;
	}

	RubyHash getPBRecord(Contact contact) {
		RubyHash record = ObjectFactory.createHash();

		String strValue = "{" + contact.getField(RUBY_PB_ID.asString()) + "}";
		record.add(RUBY_PB_ID, ObjectFactory.createString(strValue));

		strValue = contact.getField(RUBY_PB_MOBILE_NUMBER.asString());
		record.add(RUBY_PB_MOBILE_NUMBER, ObjectFactory.createString(strValue));

		strValue = contact.getField(RUBY_PB_HOME_NUMBER.asString());
		record.add(RUBY_PB_HOME_NUMBER, ObjectFactory.createString(strValue));

		strValue = contact.getField(RUBY_PB_BUSINESS_NUMBER.asString());
		record.add(RUBY_PB_BUSINESS_NUMBER, ObjectFactory
				.createString(strValue));

		strValue = contact.getField(RUBY_PB_EMAIL_ADDRESS.asString());
		record.add(RUBY_PB_EMAIL_ADDRESS, ObjectFactory.createString(strValue));

		strValue = contact.getField(RUBY_PB_COMPANY_NAME.asString());
		record.add(RUBY_PB_COMPANY_NAME, ObjectFactory.createString(strValue));

		strValue = contact.getField(RUBY_PB_FIRST_NAME.asString());
		record.add(RUBY_PB_FIRST_NAME, ObjectFactory.createString(strValue));

		strValue = contact.getField(RUBY_PB_LAST_NAME.asString());
		record.add(RUBY_PB_LAST_NAME, ObjectFactory.createString(strValue));

		return record;
	}

	public static RubyValue getallPhonebookRecords(RubyValue arg0) {
		RhoPhonebook pb = (RhoPhonebook) arg0;
		RubyHash res = ObjectFactory.createHash();

		int count = pb.contactList.getContacts().size();
		for (int i = 0; i < count; i++) {
			Contact contact = pb.contactList.getContacts().get(i);
			RubyHash record = pb.getPBRecord(contact);
			res.add(record.get(RUBY_PB_ID), record);
		}

		return res;
	}

	private Contact findContactByID(RubyValue arg0, RubyValue arg1) {
		Contact contact = null;

		String id = arg1.toString();
		id = id.substring(1, id.length() - 1);

		int count = contactList.getContacts().size();
		for (int i = 0; i < count; i++) {
			Contact cnt = contactList.getContacts().get(i);

			if (cnt.getField(RUBY_PB_ID.asString()).equals(id)) {
				contact = cnt;
				break;
			}
		}

		return contact;
	}

	public static RubyValue getPhonebookRecord(RubyValue arg0, RubyValue arg1) {
		RhoPhonebook pb = (RhoPhonebook) arg0;
		RubyHash record = ObjectFactory.createHash();

		Contact contact = pb.findContactByID(arg0, arg1);
		if (contact != null)
			record = pb.getPBRecord(contact);

		return record;
	}

	public static RubyValue openPhonebookRecord(RubyValue arg0, RubyValue arg1) {
		RhoPhonebook pb = (RhoPhonebook) arg0;
		Contact contact = pb.findContactByID(arg0, arg1);
		if (contact != null)
			return new PBRecord(contact);

		return RubyConstant.QNIL;
	}

	public static RubyValue setRecordValue(RubyArray ar) {
		if (ar.get(0) == RubyConstant.QNIL || ar.get(0) == null)
			return RubyConstant.QFALSE;

		PBRecord record = (PBRecord) ar.get(0);
		RubyString strKey = ar.get(1).toRubyString();
		String strValue = ar.get(2).toStr();
		Contact contact = record.contact;

		if (strKey.opEql(RUBY_PB_ID) == RubyConstant.QTRUE)
			return RubyConstant.QFALSE;

		contact.setField(strKey.asString(), strValue);
		return RubyConstant.QTRUE;
	}

	public static RubyValue saveRecord(RubyValue arg0, RubyValue arg1) {
		RhoPhonebook pb = (RhoPhonebook) arg0;
		PBRecord record = (PBRecord) arg1;

		try {
			pb.saveContact(record.contact);
		} catch (Exception e) {
			new RubyException(e.getMessage());
		}
		return RubyConstant.QTRUE;
	}

	public static RubyValue createRecord(RubyValue arg0) {
		RhoPhonebook pb = (RhoPhonebook) arg0;
		Contact contact = new Contact();
		pb.contactList.addContact(contact);

		if (contact != null)
			return new PBRecord(contact);

		return RubyConstant.QNIL;
	}

	public static RubyValue addRecord(RubyValue arg0, RubyValue arg1) {
		return saveRecord(arg0, arg1);
	}

	public static RubyValue deleteRecord(RubyValue arg0, RubyValue arg1) {
		RhoPhonebook pb = (RhoPhonebook) arg0;
		PBRecord record = (PBRecord) arg1;

		try {
			pb.removeContact(record.contact);
		} catch (Exception e) {
			new RubyException(e.getMessage());
		}

		return RubyConstant.QTRUE;
	}

	public static RubyValue getfirstPhonebookRecord(RubyValue arg0) {
		return RubyConstant.QNIL;
	}

	public static RubyValue getnextPhonebookRecord(RubyValue arg0) {
		return RubyConstant.QNIL;
	}

	public static void initMethods(RubyClass klass) {
		klass.defineAllocMethod(new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) {
				return RhoPhonebook.alloc(receiver);
			}
		});

		klass.getSingletonClass().defineMethod("openPhonebook",
				new RubyNoArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyBlock block) {
						return RhoPhonebook.openPhonebook();
					}
				});
		klass.getSingletonClass().defineMethod("closePhonebook",
				new RubyOneArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyValue arg0,
							RubyBlock block) {
						return RhoPhonebook.closePhonebook(arg0);
					}
				});
		klass.getSingletonClass().defineMethod("getallPhonebookRecords",
				new RubyOneArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyValue arg0,
							RubyBlock block) {
						return RhoPhonebook.getallPhonebookRecords(arg0);
					}
				});
		klass.getSingletonClass().defineMethod("openPhonebookRecord",
				new RubyTwoArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyValue arg0,
							RubyValue arg1, RubyBlock block) {
						return RhoPhonebook.openPhonebookRecord(arg0, arg1);
					}
				});
		klass.getSingletonClass().defineMethod("getPhonebookRecord",
				new RubyTwoArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyValue arg0,
							RubyValue arg1, RubyBlock block) {
						return RhoPhonebook.getPhonebookRecord(arg0, arg1);
					}
				});
		klass.getSingletonClass().defineMethod("getfirstPhonebookRecord",
				new RubyOneArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyValue arg0,
							RubyBlock block) {
						return RhoPhonebook.getfirstPhonebookRecord(arg0);
					}
				});
		klass.getSingletonClass().defineMethod("getnextPhonebookRecord",
				new RubyOneArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyValue arg0,
							RubyBlock block) {
						return RhoPhonebook.getnextPhonebookRecord(arg0);
					}
				});
		klass.getSingletonClass().defineMethod("createRecord",
				new RubyOneArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyValue arg0,
							RubyBlock block) {
						return RhoPhonebook.createRecord(arg0);
					}
				});
		klass.getSingletonClass().defineMethod("setRecordValue",
				new RubyVarArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyArray ar,
							RubyBlock block) {
						return RhoPhonebook.setRecordValue(ar);
					}
				});
		klass.getSingletonClass().defineMethod("addRecord",
				new RubyTwoArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyValue arg0,
							RubyValue arg1, RubyBlock block) {
						return RhoPhonebook.addRecord(arg0, arg1);
					}
				});
		klass.getSingletonClass().defineMethod("saveRecord",
				new RubyTwoArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyValue arg0,
							RubyValue arg1, RubyBlock block) {
						return RhoPhonebook.saveRecord(arg0, arg1);
					}
				});
		klass.getSingletonClass().defineMethod("deleteRecord",
				new RubyTwoArgMethod() {
					protected RubyValue run(RubyValue receiver, RubyValue arg0,
							RubyValue arg1, RubyBlock block) {
						return RhoPhonebook.deleteRecord(arg0, arg1);
					}
				});

	}
}
