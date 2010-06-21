/*
 *  phonebook.c
 *  rhorunner
 *
 *  Created by Vlad on 12/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include "stdafx.h"

#include "ext/rho/rhoruby.h"
#include "phonebook.h"

void* openPhonebook() {
#if defined (_WIN32_WCE)
	CNativeAddressBook* phonebook = new CNativeAddressBook();

	if (!phonebook->openAB()) {
		delete phonebook;
		return NULL;
	}

	return phonebook;
#else
	return NULL;
#endif
}

void  closePhonebook(void* pb) {
#if defined (_WIN32_WCE)
	if (pb) {
		CNativeAddressBook* phonebook = (CNativeAddressBook*)pb;
		delete phonebook;
	}
#endif
}

void _addRecordValue(const char* key, const char* value, void* hash) {
	addStrToHash(*((unsigned long*)hash), key, value);
}

#if defined (_WIN32_WCE)
static VALUE _getRecord(CABRecord* record) {
	if (record) {
        CHoldRubyValue hash(rho_ruby_createHash());
		record->enumValues(_addRecordValue,&(hash.m_value));
		return hash;
	}
	return rho_ruby_get_NIL();
}
#endif

VALUE getallPhonebookRecords(void* pb) {
#if defined (_WIN32_WCE)
	if (pb) {
		CNativeAddressBook* phonebook = (CNativeAddressBook*)pb;
        CHoldRubyValue hash(rho_ruby_createHash());
		
		std::vector<CABRecord*> records;
		phonebook->getAllRecords(records);
		while(records.size() > 0) {
			CABRecord* record = records.back();
			const char* rid = record->getValue("id").c_str();
			if (rid) {
				LOGC(TRACE,"Phonebook") + "Adding record " + rid;
				addHashToHash(hash,rid,_getRecord(record));
			}
			delete record;
			records.pop_back();
		}

		return hash;
	}
#endif
	return rho_ruby_get_NIL();	
}

VALUE getPhonebookRecord(void* pb, char* id) {
#if defined (_WIN32_WCE)
	if (pb && id) {
		CNativeAddressBook* phonebook = (CNativeAddressBook*)pb;
		CABRecord* record = phonebook->getRecord(id);
		if (record) {
			VALUE hash = _getRecord(record);
			delete record;
			return hash;
		}
	}
#endif
	return rho_ruby_get_NIL();	
}

//==================================================================================

VALUE getfirstPhonebookRecord(void* pb) {
	return rho_ruby_get_NIL();
}

VALUE getnextPhonebookRecord(void* pb) {
	return rho_ruby_get_NIL();}

//==================================================================================

void* createRecord(void* pb) {
#if defined (_WIN32_WCE)
	return new CABOutlookRecord(NULL);
#else
	return NULL;
#endif
}

void* openPhonebookRecord(void* pb, char* id) {
#if defined (_WIN32_WCE)
	if (pb && id) {
		CNativeAddressBook* phonebook = (CNativeAddressBook*)pb;
		return phonebook->getRecord(id);
	}
#endif
	return NULL;
}

int setRecordValue(void* record, char* prop, char* value) {
#if defined (_WIN32_WCE)
	if (record) {
		CABRecord* r = (CABRecord*)record;
		r->setValue(prop,value);
	}
#endif
	return 1;
}

int addRecord(void* pb, void* record) {
#if defined (_WIN32_WCE)
	if (pb && record) {
		CNativeAddressBook* phonebook = (CNativeAddressBook*)pb;
		phonebook->addRecord((CABRecord*)record);
		delete record;
	}
#endif
	return 1;
}

int saveRecord(void* pb, void* record) {
#if defined (_WIN32_WCE)
	if (record) {
		CABRecord* r = (CABRecord*)record;
		LOGC(INFO,"Phonebook") + "About to save:";
		r->dump();
		r->save();
		delete  r;
	}
#endif
	return 1;
}

int deleteRecord(void* pb, void* record) {
#if defined (_WIN32_WCE)
	if (record) {
		CABRecord* r = (CABRecord*)record;
		r->remove();
		delete r;
	}
#endif
	return 1;
}

//==================================================================================


