/*
 *  phonebook.c
 *  rhorunner
 *
 *  Created by Vlad on 12/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "rhoruby.h"
#include "phonebook.h"

static int _getProperty(char* property);

void* openPhonebook() {
	
	LocalPhonebook* phonebook = CFAllocatorAllocate(NULL, sizeof(LocalPhonebook), 0);
	
	// Fail if unable to create context
	if (phonebook == NULL)
		return NULL;
	
	memset(phonebook, 0, sizeof(LocalPhonebook));
	
	phonebook->_ab = ABAddressBookCreate();
	if (phonebook->_ab) {
		phonebook->_people = NULL;
		phonebook->_len = 0;
	}
	return phonebook;
}

void  closePhonebook(void* pb) {
	if (pb) {
		LocalPhonebook* phonebook = pb;
		// Free array of pb records
		if	(phonebook->_people) {
			CFRelease(phonebook->_people);
		}
		// Free the memory in use by context.
		CFAllocatorDeallocate(NULL, pb);
	}
}

static void _getAllPeople(LocalPhonebook* phonebook) {
	if (phonebook->_ab) {
		if	(phonebook->_people) {
			CFRelease(phonebook->_people);
		}
		phonebook->_people = ABAddressBookCopyArrayOfAllPeople(phonebook->_ab);	
		if (phonebook->_people) {
			phonebook->_len = ABAddressBookGetPersonCount(phonebook->_ab);
			phonebook->_index = 0;
		}
	}
}

static VALUE _getRecord(ABRecordRef ref, ABRecordID* precordId) {
	char buf[256];

	VALUE hash = createHash();
	
	ABRecordID recordId = ABRecordGetRecordID(ref);
	if (precordId) {
		*precordId = recordId;
	}
	CFStringRef firstName = ABRecordCopyValue(ref, kABPersonFirstNameProperty);
	CFStringRef lastName = ABRecordCopyValue(ref, kABPersonLastNameProperty);
	
	snprintf(buf, sizeof(buf), "{%d}", recordId);
	addStrToHash(hash, "id", buf, strlen(buf));
	snprintf(buf, sizeof(buf), "%s", CFStringGetCStringPtr(firstName, CFStringGetSystemEncoding()));
	addStrToHash(hash, "first_name", buf, strlen(buf));
	snprintf(buf, sizeof(buf), "%s", CFStringGetCStringPtr(lastName, CFStringGetSystemEncoding()));
	addStrToHash(hash, "last_name", buf, strlen(buf));
	
	CFRelease(firstName);
	CFRelease(lastName);
	
	return hash;
}

static VALUE _getRecordByIndex(CFArrayRef people, CFIndex index, ABRecordID* precordId) {
	ABRecordRef ref = CFArrayGetValueAtIndex(people, index);
	if (ref!=NULL) {
		return _getRecord(ref,precordId);
	}
	return getnil();
}

VALUE getallPhonebookRecords(void* pb) {
	if (pb) {
		LocalPhonebook* phonebook = pb;
		VALUE hash = createHash();
		ABRecordID recordId;
		VALUE record; char buf[128];
		
		_getAllPeople(phonebook);		
		for (int index = 0; index < phonebook->_len; index++) {
			record = _getRecordByIndex(phonebook->_people, index, &recordId);
			snprintf(buf, sizeof(buf), "{%d}", recordId);
			addHashToHash(hash, buf, record);
		}
		
		return hash; 
	}
	return getnil();	
}

VALUE getPhonebookRecord(void* pb, char* id) {
	if (pb && id) {
		LocalPhonebook* phonebook = pb;
		int recordId;
		if ( sscanf(id, "{%d}", &recordId) == 1 ) {
			ABRecordRef ref = 
			ABAddressBookGetPersonWithRecordID(phonebook->_ab,
											   (ABRecordID)recordId); 
			return _getRecord(ref,NULL);
		}
	}
	return getnil();		
}

//==================================================================================

VALUE getfirstPhonebookRecord(void* pb) {
	if (pb) {
		LocalPhonebook* phonebook = pb;
		_getAllPeople(phonebook);
		return _getRecordByIndex(phonebook->_people, phonebook->_index, NULL);
	}
	return getnil();
}

VALUE getnextPhonebookRecord(void* pb) {
	if (pb) {
		LocalPhonebook* phonebook = pb;
		phonebook->_index++;
		if( phonebook->_index < phonebook->_len ) {
			return _getRecordByIndex(phonebook->_people, phonebook->_index, NULL);
		}
	}
	return getnil();
}

//==================================================================================

void* createRecord() {
	return (void*) ABPersonCreate();
}

void* openPhonebookRecord(void* pb, char* id) {
	if (pb && id) {
		LocalPhonebook* phonebook = pb;
		int recordId;
		if ( sscanf(id, "{%d}", &recordId) == 1 ) {
			ABRecordRef ref = 
			ABAddressBookGetPersonWithRecordID(phonebook->_ab,
											   (ABRecordID)recordId); 
			return (void*)ref;
		}
	}
	return NULL;		
}

int setRecordValue(void* record, char* property, char* value) {
	if (record) {
		int prop = _getProperty(property);
		if (prop >= 0) {
			CFStringRef v = CFStringCreateWithCString(NULL, value, 
												  CFStringGetSystemEncoding());
			ABRecordSetValue(record, prop, v, nil);
			CFRelease(v);
		}
	}
	return 1;
}

int addRecord(void* pb, void* record) {
	if (pb && record) {
		LocalPhonebook* phonebook = pb;
		ABAddressBookAddRecord(phonebook->_ab, record, nil);
		ABAddressBookSave(phonebook->_ab, nil);
		CFRelease(record);
	}
	return 1;
}

int saveRecord(void* pb, void* record) {
	if (pb && record) {
		LocalPhonebook* phonebook = pb;
		ABAddressBookSave(phonebook->_ab, nil);
	}
	return 1;
}

int deleteRecord(void* pb, void* record) {
	if (pb && record) {
		LocalPhonebook* phonebook = pb;
		ABAddressBookRemoveRecord(phonebook->_ab, record, nil);
		ABAddressBookSave(phonebook->_ab, nil);
	}
	return 1;
}

//==================================================================================

enum {pbUnknown=0,pbFirstName,pbLastName};
static const struct {
	const char* _name;
	const int  _value;
} pbp[] = {
{ "first_name", pbFirstName },
{ "last_name", pbLastName },
{ NULL, 0 }
};

static int _getProperty(char* property) {
	int prop = pbUnknown;
	for(int i = 0; pbp[i]._name; i++) {
		if (strcmp(property,pbp[i]._name)==0) {
			prop = pbp[i]._value;
			break;
		}
	}
	switch(prop) {
		case pbFirstName:
			return kABPersonFirstNameProperty;
		case pbLastName:
			return kABPersonLastNameProperty;
	}
	return -1;
}

