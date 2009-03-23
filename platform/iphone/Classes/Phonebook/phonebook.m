/*
 *  phonebook.c
 *  rhorunner
 *
 *  Created by Vlad on 12/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "rhoruby.h"
#include "ruby/ext/phonebook/phonebook.h"
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

static void _addPropertyToHash(VALUE hash, const char* key, CFStringRef property) {
	if (property) {
		char buf[256];
		snprintf(buf, sizeof(buf), "%s", CFStringGetCStringPtr(property, CFStringGetSystemEncoding()));
		addStrToHash(hash, key, buf, strlen(buf));
		CFRelease(property);
	}
}

static void _addPhonesToHash(VALUE hash,ABRecordRef ref) {
	ABMultiValueRef phones = ABRecordCopyValue(ref,kABPersonPhoneProperty);
	if (phones) {
		int num_phones = ABMultiValueGetCount(phones);
		for (int n = 0; n < num_phones; n++) {
			CFStringRef label = ABMultiValueCopyLabelAtIndex(phones,n);
			if(CFStringCompare(label,kABWorkLabel,0)==kCFCompareEqualTo) {
				_addPropertyToHash(hash,RUBY_PB_BUSINESS_NUMBER,
					ABMultiValueCopyValueAtIndex(phones,n));
			} else if(CFStringCompare(label,kABHomeLabel,0)==kCFCompareEqualTo) {
				_addPropertyToHash(hash,RUBY_PB_HOME_NUMBER,
								ABMultiValueCopyValueAtIndex(phones,n));
			} else if(CFStringCompare(label,kABPersonPhoneMobileLabel,0)==kCFCompareEqualTo) {
				_addPropertyToHash(hash,RUBY_PB_MOBILE_NUMBER,
								ABMultiValueCopyValueAtIndex(phones,n));
			} 
			CFRelease(label);
		}
		CFRelease(phones);
	}	
}

static void _addEmailToHash(VALUE hash,ABRecordRef ref) {
	ABMultiValueRef emails = ABRecordCopyValue(ref,kABPersonEmailProperty);
	if (emails) {
		int num_phones = ABMultiValueGetCount(emails);
		for (int n = 0; n < num_phones; n++) {
			CFStringRef label = ABMultiValueCopyLabelAtIndex(emails,n);
			if(CFStringCompare(label,kABWorkLabel,0)==kCFCompareEqualTo) {
				_addPropertyToHash(hash,RUBY_PB_EMAIL_ADDRESS,
								   ABMultiValueCopyValueAtIndex(emails,n));
			}
			CFRelease(label);
		}
		CFRelease(emails);
	}	
}

static VALUE _getRecord(ABRecordRef ref, ABRecordID* precordId) {
	char buf[256];

	VALUE hash = createHash();
	
	ABRecordID recordId = ABRecordGetRecordID(ref);
	if (precordId) {
		*precordId = recordId;
	}
			
	snprintf(buf, sizeof(buf), "{%d}", recordId);
	addStrToHash(hash, RUBY_PB_ID, buf, strlen(buf));
	
	_addPropertyToHash(hash, RUBY_PB_FIRST_NAME, 
		ABRecordCopyValue(ref, kABPersonFirstNameProperty));

	_addPropertyToHash(hash, RUBY_PB_LAST_NAME, 
					   ABRecordCopyValue(ref, kABPersonLastNameProperty));
	
	_addPropertyToHash(hash, RUBY_PB_COMPANY_NAME, 
					   ABRecordCopyValue(ref, kABPersonOrganizationProperty));
	
	_addPhonesToHash(hash,ref);
	
	_addEmailToHash(hash,ref);
	
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

void* createRecord(void* pb) {
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

static CFStringRef _getPhoneLabel(const char* property) {
	if (strcmp(RUBY_PB_BUSINESS_NUMBER,property)==0) {
		return kABWorkLabel;
	} else if (strcmp(RUBY_PB_HOME_NUMBER, property)==0) {
		return kABHomeLabel;
	} if (strcmp(RUBY_PB_MOBILE_NUMBER, property)==0) {
		return kABPersonPhoneMobileLabel;
	}	
	return NULL;
}

static CFStringRef _getEmailLabel(const char* property) {
	if (strcmp(RUBY_PB_EMAIL_ADDRESS,property)==0) {
		return kABWorkLabel;
	} 
	return NULL;
}

static boolean_t _replaceProperty(ABMutableMultiValueRef mv, CFStringRef label, CFStringRef value, int index) {
	CFStringRef lbl = ABMultiValueCopyLabelAtIndex(mv,index);
	boolean_t found = CFStringCompare(lbl,label,0)==kCFCompareEqualTo;
	if (found) {
		ABMultiValueReplaceValueAtIndex(mv, value, index);
	} 
	CFRelease(lbl);
	return found;
}

static void _updateMultiValueProperty(ABRecordRef record, ABPropertyID propId, CFStringRef label, const char* value) {
	CFStringRef v = CFStringCreateWithCString(NULL, value, CFStringGetSystemEncoding());
	ABMutableMultiValueRef mv = ABRecordCopyValue(record,propId);
	if (!mv) {
		mv = ABMultiValueCreateMutable(kABMultiStringPropertyType); 
		ABMultiValueAddValueAndLabel(mv,v,label,nil);
	} else {
		boolean_t replaced = FALSE;
		int num_phones = ABMultiValueGetCount(mv);
		for (int n = 0; n < num_phones; n++) {
			replaced = _replaceProperty(mv,label,v,n);
			if (replaced) break;
		}
		if (!replaced) {
			ABMultiValueAddValueAndLabel(mv,v,label,nil);
		}
	}
	ABRecordSetValue(record, propId, mv, nil);
	CFRelease(mv);
	CFRelease(v);	
}

int setRecordValue(void* record, char* property, char* value) {
	if (record) {
		int prop = _getProperty(property);
		if (prop >= 0) {
			CFStringRef v = CFStringCreateWithCString(NULL, value, 
												  CFStringGetSystemEncoding());
			ABRecordSetValue(record, prop, v, nil);
			CFRelease(v);
		} else {
			CFStringRef label = _getPhoneLabel(property);
			if (label) {
				_updateMultiValueProperty(record,kABPersonPhoneProperty,label,value);
			} else {
				label = _getEmailLabel(property);
				if (label) {
					_updateMultiValueProperty(record,kABPersonEmailProperty,label,value);
				}
			}
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

enum {pbUnknown=0,pbFirstName,pbLastName,pbCompanyName};
static const struct {
	const char* _name;
	const int  _value;
} pbp[] = {
{ RUBY_PB_FIRST_NAME, pbFirstName },
{ RUBY_PB_LAST_NAME, pbLastName },
{ RUBY_PB_COMPANY_NAME, pbCompanyName },
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
		case pbCompanyName:
			return kABPersonOrganizationProperty;
	}
	return -1;
}

