package com.rho.rubyext;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.xruby.runtime.builtin.RubyArray;
import com.xruby.runtime.builtin.RubyString;
import com.xruby.runtime.builtin.RubyHash;
import com.xruby.runtime.builtin.ObjectFactory;
import com.xruby.runtime.lang.*;

import javax.microedition.pim.*;

//@RubyLevelClass(name="Phonebook")
public class RhoPhonebook extends RubyBasic {
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("Phonebook");

	private ContactList m_contactList;

	static final RubyString RUBY_PB_ID  = ObjectFactory.createString("id");
	static final RubyString RUBY_PB_FIRST_NAME = ObjectFactory.createString("first_name");
	static final RubyString RUBY_PB_LAST_NAME = ObjectFactory.createString("last_name");
	static final RubyString RUBY_PB_MOBILE_NUMBER = ObjectFactory.createString("mobile_number");
	static final RubyString RUBY_PB_HOME_NUMBER = ObjectFactory.createString("home_number");
	static final RubyString RUBY_PB_BUSINESS_NUMBER = ObjectFactory.createString("business_number");
	static final RubyString RUBY_PB_EMAIL_ADDRESS = ObjectFactory.createString("email_address");
	static final RubyString RUBY_PB_COMPANY_NAME = ObjectFactory.createString("company_name");
	
	public static class PBRecord extends RubyBasic {

		Contact m_contact;
		public PBRecord(RubyClass arg0) {
			super(arg0);
		}

		public PBRecord(Contact contact) {
			super(RubyRuntime.PBRecordClass);
			
			m_contact = contact;
		}
		
	}
	
	public RhoPhonebook(RubyClass arg0) {
		super(arg0);
		
		try {
			m_contactList = (ContactList) PIM.getInstance().openPIMList(
					PIM.CONTACT_LIST, PIM.READ_WRITE);
		} catch (PIMException e) {
			new RubyException(e.getMessage());
		}
	}
    //@RubyAllocMethod
    public static RhoPhonebook alloc(RubyValue receiver) {
    	return new RhoPhonebook(RubyRuntime.PhonebookClass);
    }

	public static RubyValue openPhonebook() {
		RhoPhonebook pb = alloc(null);
		return pb;
	}
	public static RubyValue closePhonebook(RubyValue arg0) {
		try {
			RhoPhonebook pb = (RhoPhonebook)arg0;
			pb.m_contactList.close();
			pb.m_contactList = null;
		} catch (PIMException e) {
			new RubyException(e.getMessage());
		}
		
		return RubyConstant.QNIL;
	}

	RubyHash getPBRecord( Contact contact ){
		RubyHash record = ObjectFactory.createHash();
		
		if ( m_contactList.isSupportedField(Contact.UID) &&
			 contact.countValues(Contact.UID) > 0 ){
			String strValue = "{" + contact.getString(Contact.UID, 0) + "}";
			record.add(RUBY_PB_ID, ObjectFactory.createString(strValue));			
		}
		if ( m_contactList.isSupportedField(Contact.TEL) ){
			for (int i = 0, size = contact.countValues(Contact.TEL); i < size; i++) {
				String strValue = contact.getString(Contact.TEL, i);
				if ( strValue.length() == 0 )
					continue;
				
				int attr = contact.getAttributes(Contact.TEL, i);

				if ((attr & Contact.ATTR_MOBILE) != 0)
					record.add(RUBY_PB_MOBILE_NUMBER, ObjectFactory.createString(strValue));
				else if ((attr & Contact.ATTR_HOME) != 0)
					record.add(RUBY_PB_HOME_NUMBER, ObjectFactory.createString(strValue));
				else if ((attr & Contact.ATTR_WORK) != 0)
					record.add(RUBY_PB_BUSINESS_NUMBER, ObjectFactory.createString(strValue));
			}
		}
		if ( m_contactList.isSupportedField(Contact.EMAIL) ){
			for (int i = 0, size = contact.countValues(Contact.EMAIL); i < size; i++) {
				String strValue = contact.getString(Contact.EMAIL, i);
				if ( strValue.length() == 0 )
					continue;
				
				record.add(RUBY_PB_EMAIL_ADDRESS, ObjectFactory.createString(strValue));
				break;
			}
		}
		if ( m_contactList.isSupportedField(Contact.ORG) && 
			 contact.countValues(Contact.ORG) > 0 ){
			String strValue = contact.getString(Contact.ORG, 0);
			if ( strValue.length() > 0 )
				record.add(RUBY_PB_COMPANY_NAME, ObjectFactory.createString(strValue));			
		}
		
		boolean bNameFound = false;
		if ( m_contactList.isSupportedField(Contact.NAME) && 
			contact.countValues(Contact.NAME) > 0 ){
            String[] names = contact.getStringArray(Contact.NAME, 0);
            if ( names != null ){
                if (names[Contact.NAME_GIVEN] != null){
					record.add(RUBY_PB_FIRST_NAME, ObjectFactory.createString(names[Contact.NAME_GIVEN]));
					bNameFound = true;
                }
                
                if (names[Contact.NAME_FAMILY] != null){
					record.add(RUBY_PB_LAST_NAME, ObjectFactory.createString(names[Contact.NAME_FAMILY]));
					bNameFound = true;
                }
            }
		}
		
		if ( !bNameFound && m_contactList.isSupportedField(Contact.FORMATTED_NAME) && 
				   contact.countValues(Contact.FORMATTED_NAME) > 0 ){
			
			String strValue = contact.getString(Contact.FORMATTED_NAME, 0);
			if ( strValue.length() > 0 )
				record.add(RUBY_PB_FIRST_NAME, ObjectFactory.createString(strValue));			
		}
		
		return record;
	}
	
	public static RubyValue getallPhonebookRecords(RubyValue arg0) {
		RhoPhonebook pb = (RhoPhonebook)arg0;
		RubyHash res = ObjectFactory.createHash();
		
		try {
			java.util.Enumeration contacts = pb.m_contactList.items();
			while (contacts.hasMoreElements()) {
				Contact contact = (Contact) contacts.nextElement();
				RubyHash record = pb.getPBRecord( contact );
				res.add(record.get(RUBY_PB_ID), record);
			}
			
		} catch (PIMException e) {
			new RubyException(e.getMessage());
		}
		
		return res;
	}

	private Contact findContactByID(RubyValue arg0, RubyValue arg1 ){
		Contact contact = null;
		
		try {
			Contact matching = m_contactList.createContact();
			String id = arg1.toString();
			id = id.substring(1, id.length()-1);
			matching.addString(Contact.UID, Contact.ATTR_NONE, id);
			java.util.Enumeration contacts = m_contactList.items(matching);
			if (contacts.hasMoreElements())
				contact = (Contact) contacts.nextElement();
		} catch (PIMException e) {
			new RubyException(e.getMessage());
		}
			
		return contact;
	}
	
	public static RubyValue getPhonebookRecord(RubyValue arg0, RubyValue arg1) {
		RhoPhonebook pb = (RhoPhonebook)arg0;
		RubyHash record = ObjectFactory.createHash();
		
		Contact contact = pb.findContactByID(arg0,arg1);
		if ( contact != null )
			record = pb.getPBRecord( contact );
		
		return record;
	}

	public static RubyValue openPhonebookRecord(RubyValue arg0, RubyValue arg1) {
		RhoPhonebook pb = (RhoPhonebook)arg0;
		Contact contact = pb.findContactByID(arg0,arg1);
		if ( contact != null )
			return new PBRecord(contact);
		
		return RubyConstant.QNIL;
	}
	
	public static RubyValue setRecordValue(RubyArray ar) {
		if ( ar.get(0) == RubyConstant.QNIL || ar.get(0) == null )
			return RubyConstant.QFALSE;
		
		PBRecord record = (PBRecord)ar.get(0);
		RubyString strKey = ar.get(1).toRubyString();
		String strValue = ar.get(2).toStr();
		Contact contact = record.m_contact;
		int field = 0, index = 0, attributes = Contact.ATTR_NONE;
		if ( strKey.opEql(RUBY_PB_ID) == RubyConstant.QTRUE )
			return RubyConstant.QFALSE;

		if ( contact.getPIMList().isSupportedField(Contact.NAME) ){ 
			if ( strKey.opEql(RUBY_PB_FIRST_NAME) == RubyConstant.QTRUE ){
				
				if ( contact.countValues(Contact.NAME) > 0 ){
					String[] names = contact.getStringArray(Contact.NAME, 0);
					names[Contact.NAME_GIVEN] = strValue;
					contact.setStringArray(Contact.NAME, 0, Contact.ATTR_NONE, names);
				}else{
					String[] names = new String[contact.getPIMList().stringArraySize(Contact.NAME)];
					names[Contact.NAME_GIVEN] = strValue;
					contact.addStringArray(Contact.NAME, Contact.ATTR_NONE, names);
				}
				
				return RubyConstant.QTRUE;
			}else if ( strKey.opEql(RUBY_PB_LAST_NAME) == RubyConstant.QTRUE ){
				if ( contact.countValues(Contact.NAME) > 0 ){
					String[] names = contact.getStringArray(Contact.NAME, 0);
					names[Contact.NAME_FAMILY] = strValue;
					contact.setStringArray(Contact.NAME, 0, Contact.ATTR_NONE, names);
				}else{
					String[] names = new String[contact.getPIMList().stringArraySize(Contact.NAME)];
					names[Contact.NAME_FAMILY] = strValue;
					contact.addStringArray(Contact.NAME, Contact.ATTR_NONE, names);
				}
				return RubyConstant.QTRUE;
			}
		}
		
		if ( strKey.opEql(RUBY_PB_EMAIL_ADDRESS) == RubyConstant.QTRUE ){
			field = Contact.EMAIL;
		}else if ( strKey.opEql(RUBY_PB_COMPANY_NAME) == RubyConstant.QTRUE ){
			field = Contact.ORG;
		}else if ( strKey.opEql(RUBY_PB_MOBILE_NUMBER) == RubyConstant.QTRUE ){
			field = Contact.TEL;
			attributes = Contact.ATTR_MOBILE;
			boolean bFound = false;
			if ( contact.getPIMList().isSupportedField(Contact.TEL) ){
				for (int i = 0, size = contact.countValues(Contact.TEL); i < size; i++) {
					int attr = contact.getAttributes(Contact.TEL, i);
					if ((attr & Contact.ATTR_MOBILE) != 0){
						index = i;
						bFound = true;
						break;
					}
				}
			}
			if ( !bFound )
				index = -1;				
		}else if ( strKey.opEql(RUBY_PB_HOME_NUMBER) == RubyConstant.QTRUE ){
			field = Contact.TEL;
			attributes = Contact.ATTR_HOME;
			boolean bFound = false;
			if ( contact.getPIMList().isSupportedField(Contact.TEL) ){
				for (int i = 0, size = contact.countValues(Contact.TEL); i < size; i++) {
					int attr = contact.getAttributes(Contact.TEL, i);
					if ((attr & Contact.ATTR_HOME) != 0){
						index = i;
						bFound = true;
						break;
					}
				}
			}
			if ( !bFound )
				index = -1;				
		}else if ( strKey.opEql(RUBY_PB_BUSINESS_NUMBER) == RubyConstant.QTRUE ){
			field = Contact.TEL;
			attributes = Contact.ATTR_WORK;
			boolean bFound = false;
			if ( contact.getPIMList().isSupportedField(Contact.TEL) ){
				for (int i = 0, size = contact.countValues(Contact.TEL); i < size; i++) {
					int attr = contact.getAttributes(Contact.TEL, i);
					if ((attr & Contact.ATTR_WORK) != 0){
						index = i;
						bFound = true;
						break;
					}
				}
			}
			if ( !bFound )
				index = -1;				
		}else
			return RubyConstant.QFALSE;

		if ( index == -1 )
			contact.addString(field, attributes, strValue);
		else if ( contact.countValues(field) > 0 ){
			contact.removeValue(field, index);
			contact.addString(field, attributes, strValue);
			//contact.setString(field, index, attributes, strValue);
		}
		else
			contact.addString(field, attributes, strValue);
		
		return RubyConstant.QTRUE;
	}

	public static RubyValue saveRecord(RubyValue arg0, RubyValue arg1) {
		PBRecord record = (PBRecord)arg1;
		try {
			record.m_contact.commit();
		} catch (PIMException e) {
			new RubyException(e.getMessage());
		}
		
		return RubyConstant.QTRUE;
	}

	public static RubyValue createRecord(RubyValue arg0) {
		RhoPhonebook pb = (RhoPhonebook)arg0;
		Contact contact = pb.m_contactList.createContact();
		if ( contact != null )
			return new PBRecord(contact);
		
		return RubyConstant.QNIL;
	}

	public static RubyValue addRecord(RubyValue arg0, RubyValue arg1) {
		return saveRecord(arg0, arg1);
	}
	
	public static RubyValue deleteRecord(RubyValue arg0, RubyValue arg1) {
		RhoPhonebook pb = (RhoPhonebook)arg0;
		PBRecord record = (PBRecord)arg1;

		try {
			pb.m_contactList.removeContact(record.m_contact);
		} catch (PIMException e) {
			new RubyException(e.getMessage());
		}
			
		return RubyConstant.QTRUE;
	}

	public static void initMethods( RubyClass klass){
		klass.defineAllocMethod(new RubyNoArgMethod(){
			protected RubyValue run(RubyValue receiver, RubyBlock block )	{
				return RhoPhonebook.alloc(receiver);}
		});
		
		klass.getSingletonClass().defineMethod("openPhonebook", new RubyNoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyBlock block) 
			{
				try {
					return RhoPhonebook.openPhonebook();
				} catch(Exception e) {
					LOG.ERROR("openPhonebook failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
					
			}
		});
		klass.getSingletonClass().defineMethod("closePhonebook", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block) 
			{
				try {
					return RhoPhonebook.closePhonebook(arg0);
				} catch(Exception e) {
					LOG.ERROR("closePhonebook failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
					
			}
		});
		klass.getSingletonClass().defineMethod("getallPhonebookRecords", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block) 
			{
				try {
					return RhoPhonebook.getallPhonebookRecords(arg0);
				} catch(Exception e) {
					LOG.ERROR("getallPhonebookRecords failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
					
			}
		});
		klass.getSingletonClass().defineMethod("openPhonebookRecord", new RubyTwoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) 
			{
				try {
					return RhoPhonebook.openPhonebookRecord(arg0, arg1);
				} catch(Exception e) {
					LOG.ERROR("openPhonebookRecord failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
					
			}
		});
		klass.getSingletonClass().defineMethod("getPhonebookRecord", new RubyTwoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) 
			{
				try {
					return RhoPhonebook.getPhonebookRecord(arg0, arg1);
				} catch(Exception e) {
					LOG.ERROR("getPhonebookRecord failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
					
			}
		});
		klass.getSingletonClass().defineMethod("createRecord", new RubyOneArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyBlock block) 
			{
				try {
					return RhoPhonebook.createRecord(arg0);
				} catch(Exception e) {
					LOG.ERROR("createRecord failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
					
			}
		});
		klass.getSingletonClass().defineMethod("setRecordValue", new RubyVarArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyArray ar, RubyBlock block) 
			{
				try {
					return RhoPhonebook.setRecordValue(ar);
				} catch(Exception e) {
					LOG.ERROR("setRecordValue failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
					
			}
		});
		klass.getSingletonClass().defineMethod("addRecord", new RubyTwoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) 
			{
				try {
					return RhoPhonebook.addRecord(arg0, arg1);
				} catch(Exception e) {
					LOG.ERROR("addRecord failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
					
			}
		});
		klass.getSingletonClass().defineMethod("saveRecord", new RubyTwoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) 
			{
				try {
					return RhoPhonebook.saveRecord(arg0, arg1);
				} catch(Exception e) {
					LOG.ERROR("saveRecord failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
					
			}
		});
		klass.getSingletonClass().defineMethod("deleteRecord", new RubyTwoArgMethod() {
			protected RubyValue run(RubyValue receiver, RubyValue arg0, RubyValue arg1, RubyBlock block) 
			{
				try {
					return RhoPhonebook.deleteRecord(arg0, arg1);
				} catch(Exception e) {
					LOG.ERROR("deleteRecord failed", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
					
			}
		});
		
	}
}
