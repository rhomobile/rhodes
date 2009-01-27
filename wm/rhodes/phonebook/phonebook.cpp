/*
 *  phonebook.c
 *  rhorunner
 *
 *  Created by Vlad on 12/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#include "../stdafx.h"

#include "rhoruby/rhoruby.h"
#include "phonebook.h"

void* openPhonebook() {
	CNativeAddressBook* phonebook = new CNativeAddressBook();

	if (!phonebook->openAB()) {
		delete phonebook;
		return NULL;
	}

	return phonebook;
}

void  closePhonebook(void* pb) {
	if (pb) {
		CNativeAddressBook* phonebook = (CNativeAddressBook*)pb;
		delete phonebook;
	}
}

void _addRecordValue(const char* key, const char* value, void* hash) {
	addStrToHash(*((unsigned long*)hash), key, value, strlen(value));
}

static VALUE _getRecord(CABRecord* record) {
	if (record) {
		VALUE hash = createHash();
		record->enumValues(_addRecordValue,&hash);
		return hash;
	}
	return getnil();
}

VALUE getallPhonebookRecords(void* pb) {
	if (pb) {
		CNativeAddressBook* phonebook = (CNativeAddressBook*)pb;
		VALUE hash = createHash();
		
		std::vector<CABRecord*> records;
		phonebook->getAllRecords(records);
		while(records.size() > 0) {
			CABRecord* record = records.back();
			const char* rid = record->getValue("id").c_str();
			if (rid) {
				printf("Adding record %s\n", rid);
				addHashToHash(hash,rid,_getRecord(record));
			}
			delete record;
			records.pop_back();
		}

		return hash;
	}
	return getnil();	
}

VALUE getPhonebookRecord(void* pb, char* id) {
	if (pb && id) {
		CNativeAddressBook* phonebook = (CNativeAddressBook*)pb;
		CABRecord* record = phonebook->getRecord(id);
		if (record) {
			VALUE hash = _getRecord(record);
			delete record;
			return hash;
		}
	}
	return getnil();	
}

//==================================================================================

VALUE getfirstPhonebookRecord(void* pb) {
	return getnil();
}

VALUE getnextPhonebookRecord(void* pb) {
	return getnil();}

//==================================================================================

void* createRecord(void* pb) {
	return new CABOutlookRecord(NULL);
}

void* openPhonebookRecord(void* pb, char* id) {
	if (pb && id) {
		CNativeAddressBook* phonebook = (CNativeAddressBook*)pb;
		return phonebook->getRecord(id);
	}
	return NULL;
}

int setRecordValue(void* record, char* prop, char* value) {
	if (record) {
		CABRecord* r = (CABRecord*)record;
		r->setValue(prop,value);
	}
	return 1;
}

int addRecord(void* pb, void* record) {
	if (pb && record) {
		CNativeAddressBook* phonebook = (CNativeAddressBook*)pb;
		phonebook->addRecord((CABRecord*)record);
		delete record;
	}
	return 1;
}

int saveRecord(void* pb, void* record) {
	if (record) {
		CABRecord* r = (CABRecord*)record;
		printf("About to save:\n");
		r->dump();
		r->save();
		delete  r;
	}
	return 1;
}

int deleteRecord(void* pb, void* record) {
	if (record) {
		CABRecord* r = (CABRecord*)record;
		r->remove();
		delete r;
	}
	return 1;
}

//==================================================================================


