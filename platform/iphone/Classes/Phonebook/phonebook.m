/*
 *  phonebook.c
 *  rhorunner
 *
 *  Created by Vlad on 12/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "ruby/ext/rho/rhoruby.h"
#include "ruby/ext/phonebook/phonebook.h"
#include "phonebook.h"

static int _getProperty(char* property);
static CFStringRef getAddressPartValue(ABRecordRef record, const char* property) ;

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
		addStrToHash(hash, key, buf);
		CFRelease(property);
	}
}

static void _addDatePropertyToHash(VALUE hash, const char* key, NSDate* date) {
	if (date) {
		NSDateFormatter *df = [[NSDateFormatter alloc] init];
		[df setDateFormat:@"yyyy-MM-dd"];
		NSString* dt = [df stringFromDate:date];
		[dt retain];
		_addPropertyToHash(hash, key, (CFStringRef)dt);
		[date release];
		[df release];
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
			} else if(CFStringCompare(label,kABPersonPhoneMainLabel,0)==kCFCompareEqualTo) {
				_addPropertyToHash(hash,RUBY_PB_MAIN_MUMBER,
								   ABMultiValueCopyValueAtIndex(phones,n));				
			} else if(CFStringCompare(label,kABPersonPhonePagerLabel,0)==kCFCompareEqualTo) {
				_addPropertyToHash(hash,RUBY_PB_PAGER_NUMBER,
								   ABMultiValueCopyValueAtIndex(phones,n));				
			} else if(CFStringCompare(label,kABPersonPhoneHomeFAXLabel,0)==kCFCompareEqualTo) {
				_addPropertyToHash(hash,RUBY_PB_HOME_FAX,
								   ABMultiValueCopyValueAtIndex(phones,n));
			} else if(CFStringCompare(label,kABPersonPhoneWorkFAXLabel,0)==kCFCompareEqualTo) {
				_addPropertyToHash(hash,RUBY_PB_WORK_FAX,
								   ABMultiValueCopyValueAtIndex(phones,n));
			} else if(CFStringCompare(label,(CFStringRef)@"assistant_number",0)==kCFCompareEqualTo) {
				_addPropertyToHash(hash,RUBY_PB_ASSISTANT_NUMBER,
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
			if(label == nil || CFStringCompare(label,kABWorkLabel,0)==kCFCompareEqualTo) {
				_addPropertyToHash(hash,RUBY_PB_EMAIL_ADDRESS,
								   ABMultiValueCopyValueAtIndex(emails,n));
			} else if(CFStringCompare(label,kABHomeLabel,0)==kCFCompareEqualTo) {
				_addPropertyToHash(hash,RUBY_PB_HOME_EMAIL_ADDRESS,
								   ABMultiValueCopyValueAtIndex(emails,n));
			} else if(CFStringCompare(label,kABOtherLabel,0)==kCFCompareEqualTo) {
				_addPropertyToHash(hash,RUBY_PB_OTHER_EMAIL_ADDRESS,
								   ABMultiValueCopyValueAtIndex(emails,n));
			}
			if (label) CFRelease(label);
		}
		CFRelease(emails);
	}	
}

static void _addUrlToHash(VALUE hash,ABRecordRef ref) {
	ABMultiValueRef urls = ABRecordCopyValue(ref,kABPersonURLProperty);
	if (urls) {
		int num_urls = ABMultiValueGetCount(urls);
		for (int n = 0; n < num_urls; n++) {
			CFStringRef label = ABMultiValueCopyLabelAtIndex(urls,n);
			if(CFStringCompare(label,kABPersonHomePageLabel,0)==kCFCompareEqualTo) {
				_addPropertyToHash(hash,RUBY_PB_HOME_PAGE,
								   ABMultiValueCopyValueAtIndex(urls,n));
			} 
			CFRelease(label);
		}
		CFRelease(urls);
	}	
}

static void _addRelatedNamesToHash(VALUE hash,ABRecordRef ref) {
	ABMultiValueRef names = ABRecordCopyValue(ref,kABPersonRelatedNamesProperty);
	if (names) {
		int num_names = ABMultiValueGetCount(names);
		for (int n = 0; n < num_names; n++) {
			CFStringRef label = ABMultiValueCopyLabelAtIndex(names,n);
			if(CFStringCompare(label,kABPersonSpouseLabel,0)==kCFCompareEqualTo) {
				_addPropertyToHash(hash,RUBY_PB_SPOUSE_NAME,
								   ABMultiValueCopyValueAtIndex(names,n));
			} else if(CFStringCompare(label,kABPersonAssistantLabel,0)==kCFCompareEqualTo) {
				_addPropertyToHash(hash,RUBY_PB_ASSISTANT_NAME,
								   ABMultiValueCopyValueAtIndex(names,n));
			} 
			CFRelease(label);
		}
		CFRelease(names);
	}	
}

static void _addDatesToHash(VALUE hash,ABRecordRef ref) {
	ABMultiValueRef dates = ABRecordCopyValue(ref,kABPersonDateProperty);
	if (dates) {
		int num_dates = ABMultiValueGetCount(dates);
		for (int n = 0; n < num_dates; n++) {
			CFStringRef label = ABMultiValueCopyLabelAtIndex(dates,n);
			if(CFStringCompare(label,kABPersonAnniversaryLabel,0)==kCFCompareEqualTo) {
				_addDatePropertyToHash(hash,RUBY_PB_ANNIVERSARY,
								   (NSDate*)ABMultiValueCopyValueAtIndex(dates,n));
			} else if(CFStringCompare(label,(CFStringRef)@"created",0)==kCFCompareEqualTo) {
				_addDatePropertyToHash(hash,RUBY_PB_CREATED,
								   (NSDate*)ABMultiValueCopyValueAtIndex(dates,n));
			} else if(CFStringCompare(label,(CFStringRef)@"updated",0)==kCFCompareEqualTo) {
				_addDatePropertyToHash(hash,RUBY_PB_UPDATED,
								   (NSDate*)ABMultiValueCopyValueAtIndex(dates,n));
			} 
			CFRelease(label);
		}
		CFRelease(dates);
	}	
}

static VALUE _getRecord(ABRecordRef ref, ABRecordID* precordId) {
	char buf[256];

	VALUE hash = rho_ruby_createHash();
	rho_ruby_holdValue(hash);
	
	ABRecordID recordId = ABRecordGetRecordID(ref);
	if (precordId) {
		*precordId = recordId;
	}
			
	snprintf(buf, sizeof(buf), "{%d}", recordId);
	addStrToHash(hash, RUBY_PB_ID, buf);
	
	_addPropertyToHash(hash, RUBY_PB_PREFIX, 
					   ABRecordCopyValue(ref, kABPersonPrefixProperty));

	_addPropertyToHash(hash, RUBY_PB_FIRST_NAME, 
					   ABRecordCopyValue(ref, kABPersonFirstNameProperty));

	_addPropertyToHash(hash, RUBY_PB_MIDDLE_NAME, 
					   ABRecordCopyValue(ref, kABPersonMiddleNameProperty));

	_addPropertyToHash(hash, RUBY_PB_LAST_NAME, 
					   ABRecordCopyValue(ref, kABPersonLastNameProperty));
	
	_addPropertyToHash(hash, RUBY_PB_SUFFIX, 
					   ABRecordCopyValue(ref, kABPersonSuffixProperty));
	
	_addPropertyToHash(hash, RUBY_PB_NICKNAME, 
					   ABRecordCopyValue(ref, kABPersonNicknameProperty));

	_addPropertyToHash(hash, RUBY_PB_NOTE, 
					   ABRecordCopyValue(ref, kABPersonNoteProperty));
	
	_addPropertyToHash(hash, RUBY_PB_COMPANY_NAME, 
					   ABRecordCopyValue(ref, kABPersonOrganizationProperty));
	
	_addPropertyToHash(hash, RUBY_PB_JOB_TITLE, 
					   ABRecordCopyValue(ref, kABPersonJobTitleProperty));

	NSDate* birthdate = (NSDate *)ABRecordCopyValue(ref, kABPersonBirthdayProperty);
	NSDateFormatter *df = [[NSDateFormatter alloc] init];
	[df setDateFormat:@"yyyy-MM-dd"];
	NSString* bd = [df stringFromDate:birthdate];
	[bd retain];
	_addPropertyToHash(hash, RUBY_PB_BIRTHDAY, (CFStringRef)bd);
	[birthdate release];
	[df release];
	
	_addPropertyToHash(hash, RUBY_PB_STREET_ADDRESS_1,
					   getAddressPartValue(ref,RUBY_PB_STREET_ADDRESS_1));
	
	_addPropertyToHash(hash, RUBY_PB_CITY_1,
					   getAddressPartValue(ref,RUBY_PB_CITY_1));

	_addPropertyToHash(hash, RUBY_PB_STATE_1,
					   getAddressPartValue(ref,RUBY_PB_STATE_1));

	_addPropertyToHash(hash, RUBY_PB_ZIP_1,
					   getAddressPartValue(ref,RUBY_PB_ZIP_1));

	_addPropertyToHash(hash, RUBY_PB_COUNTRY_1,
					   getAddressPartValue(ref,RUBY_PB_COUNTRY_1));

	_addPropertyToHash(hash, RUBY_PB_STREET_ADDRESS_2,
					   getAddressPartValue(ref,RUBY_PB_STREET_ADDRESS_2));
	
	_addPropertyToHash(hash, RUBY_PB_CITY_2,
					   getAddressPartValue(ref,RUBY_PB_CITY_2));
	
	_addPropertyToHash(hash, RUBY_PB_STATE_2,
					   getAddressPartValue(ref,RUBY_PB_STATE_2));
	
	_addPropertyToHash(hash, RUBY_PB_ZIP_2,
					   getAddressPartValue(ref,RUBY_PB_ZIP_2));
	
	_addPropertyToHash(hash, RUBY_PB_COUNTRY_2,
					   getAddressPartValue(ref,RUBY_PB_COUNTRY_2));
	
	_addPropertyToHash(hash, RUBY_PB_STREET_ADDRESS_3,
					   getAddressPartValue(ref,RUBY_PB_STREET_ADDRESS_3));
	
	_addPropertyToHash(hash, RUBY_PB_CITY_3,
					   getAddressPartValue(ref,RUBY_PB_CITY_3));
	
	_addPropertyToHash(hash, RUBY_PB_STATE_3,
					   getAddressPartValue(ref,RUBY_PB_STATE_3));
	
	_addPropertyToHash(hash, RUBY_PB_ZIP_3,
					   getAddressPartValue(ref,RUBY_PB_ZIP_3));
	
	_addPropertyToHash(hash, RUBY_PB_COUNTRY_3,
					   getAddressPartValue(ref,RUBY_PB_COUNTRY_3));
	
	_addPhonesToHash(hash,ref);
	
	_addEmailToHash(hash,ref);
	
	_addUrlToHash(hash,ref);
	
	_addRelatedNamesToHash(hash, ref);
	
	_addDatesToHash(hash, ref);
	
	rho_ruby_releaseValue(hash);
	return hash;
}

static VALUE _getRecordByIndex(CFArrayRef people, CFIndex index, ABRecordID* precordId) {
	ABRecordRef ref = CFArrayGetValueAtIndex(people, index);
	if (ref!=NULL) {
		return _getRecord(ref,precordId);
	}
	return rho_ruby_get_NIL();
}

VALUE getallPhonebookRecords(void* pb) {
	if (pb) {
		LocalPhonebook* phonebook = pb;
		
		VALUE hash = rho_ruby_createHash();
		VALUE valGc = rho_ruby_disable_gc();
		
		rho_ruby_holdValue(hash);
		ABRecordID recordId;
		VALUE record; char buf[128];
		
		_getAllPeople(phonebook);		
		for (int index = 0; index < phonebook->_len; index++) {
			record = _getRecordByIndex(phonebook->_people, index, &recordId);
			snprintf(buf, sizeof(buf), "{%d}", recordId);
			addHashToHash(hash, buf, record);
		}

        rho_ruby_releaseValue(hash);		
        
        rho_ruby_enable_gc(valGc);
		return hash; 
	}
	return rho_ruby_get_NIL();	
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
	return rho_ruby_get_NIL();		
}

//==================================================================================

VALUE getfirstPhonebookRecord(void* pb) {
	if (pb) {
		LocalPhonebook* phonebook = pb;
		_getAllPeople(phonebook);
		return _getRecordByIndex(phonebook->_people, phonebook->_index, NULL);
	}
	return rho_ruby_get_NIL();
}

VALUE getnextPhonebookRecord(void* pb) {
	if (pb) {
		LocalPhonebook* phonebook = pb;
		phonebook->_index++;
		if( phonebook->_index < phonebook->_len ) {
			return _getRecordByIndex(phonebook->_people, phonebook->_index, NULL);
		}
	}
	return rho_ruby_get_NIL();
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
	} else if (strcmp(RUBY_PB_MOBILE_NUMBER, property)==0) {
		return kABPersonPhoneMobileLabel;
	} else if (strcmp(RUBY_PB_MAIN_MUMBER, property)==0) {
		return kABPersonPhoneMainLabel;
	} else if (strcmp(RUBY_PB_PAGER_NUMBER, property)==0) {
		return kABPersonPhonePagerLabel;
	} else if (strcmp(RUBY_PB_HOME_FAX, property)==0) {
		return kABPersonPhoneHomeFAXLabel;
	} else if (strcmp(RUBY_PB_WORK_FAX, property)==0) {
		return kABPersonPhoneWorkFAXLabel;
	} else if (strcmp(RUBY_PB_ASSISTANT_NUMBER, property)==0) {
		return (CFStringRef)@"assistant_number";
	}	
	return NULL;
}

static CFStringRef _getEmailLabel(const char* property) {
	if (strcmp(RUBY_PB_EMAIL_ADDRESS,property)==0) {
		return kABWorkLabel;
	} else if (strcmp(RUBY_PB_HOME_EMAIL_ADDRESS,property)==0) {
		return kABHomeLabel;
	} else if (strcmp(RUBY_PB_OTHER_EMAIL_ADDRESS,property)==0) {
		return kABOtherLabel;
	} 
	return NULL;
}

static CFStringRef _getUrlLabel(const char* property) {
	if (strcmp(RUBY_PB_HOME_PAGE,property)==0) {
		return kABPersonHomePageLabel;
	} 
	return NULL;
}

static CFStringRef _getRelatedNamesLabel(const char* property) {
	if (strcmp(RUBY_PB_SPOUSE_NAME,property)==0) {
		return kABPersonSpouseLabel;
	} else if (strcmp(RUBY_PB_ASSISTANT_NAME,property)==0) {
		return kABPersonAssistantLabel;
	} 
	return NULL;
}

static CFStringRef _getDatesLabel(const char* property) {
	if (strcmp(RUBY_PB_ANNIVERSARY,property)==0) {
		return kABPersonAnniversaryLabel;
	} else if (strcmp(RUBY_PB_CREATED,property)==0) {
		return (CFStringRef)@"created";
	} else if (strcmp(RUBY_PB_UPDATED,property)==0) {
		return (CFStringRef)@"updated";
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

static void __updateMultiValueProperty(ABRecordRef record, ABPropertyID propId, CFStringRef label, CFTypeRef v) {
	if (!v) return;
	ABMutableMultiValueRef mv = ABRecordCopyValue(record,propId);
	if (!mv) {
		mv = ABMultiValueCreateMutable(kABMultiStringPropertyType); 
		ABMultiValueAddValueAndLabel(mv,v,label,nil);
	} else {
		ABMutableMultiValueRef lmv  = ABMultiValueCreateMutableCopy(mv);
		boolean_t replaced = FALSE;
		int num_phones = ABMultiValueGetCount(lmv);
		for (int n = 0; n < num_phones; n++) {
			replaced = _replaceProperty(lmv,label,v,n);
			if (replaced) break;
		}
		if (!replaced) {
			ABMultiValueAddValueAndLabel(lmv,v,label,nil);
		}
		CFRelease(mv);
		mv = lmv;
	}
	ABRecordSetValue(record, propId, mv, nil);
	CFRelease(mv);
	CFRelease(v);	
}

static void _updateMultiValueProperty(ABRecordRef record, ABPropertyID propId, CFStringRef label, const char* value) {
	CFStringRef v = CFStringCreateWithCString(NULL, value, CFStringGetSystemEncoding());
	__updateMultiValueProperty(record, propId, label, v);
}

bool getAddressLabelAndKey(const char* property, CFStringRef* label, CFStringRef* key) {
	if (strcmp(property,RUBY_PB_STREET_ADDRESS_1)==0) {
		*label = kABWorkLabel;
		*key = kABPersonAddressStreetKey;
	} else if (strcmp(property,RUBY_PB_CITY_1)==0) {
		*label = kABWorkLabel;
		*key = kABPersonAddressCityKey;
	} else if (strcmp(property,RUBY_PB_STATE_1)==0) {
		*label = kABWorkLabel;
		*key = kABPersonAddressStateKey;
	} else if (strcmp(property,RUBY_PB_ZIP_1)==0) {
		*label = kABWorkLabel;
		*key = kABPersonAddressZIPKey;
	} else if (strcmp(property,RUBY_PB_COUNTRY_1)==0) {
		*label = kABWorkLabel;
		*key = kABPersonAddressCountryKey;
	} else if (strcmp(property,RUBY_PB_STREET_ADDRESS_2)==0) {
		*label = kABHomeLabel;
		*key = kABPersonAddressStreetKey;
	} else if (strcmp(property,RUBY_PB_CITY_2)==0) {
		*label = kABHomeLabel;
		*key = kABPersonAddressCityKey;
	} else if (strcmp(property,RUBY_PB_STATE_2)==0) {
		*label = kABHomeLabel;
		*key = kABPersonAddressStateKey;
	} else if (strcmp(property,RUBY_PB_ZIP_2)==0) {
		*label = kABHomeLabel;
		*key = kABPersonAddressZIPKey;
	} else if (strcmp(property,RUBY_PB_COUNTRY_2)==0) {
		*label = kABHomeLabel;
		*key = kABPersonAddressCountryKey;
	} else if (strcmp(property,RUBY_PB_STREET_ADDRESS_3)==0) {
		*label = kABOtherLabel;
		*key = kABPersonAddressStreetKey;
	} else if (strcmp(property,RUBY_PB_CITY_3)==0) {
		*label = kABOtherLabel;
		*key = kABPersonAddressCityKey;
	} else if (strcmp(property,RUBY_PB_STATE_3)==0) {
		*label = kABOtherLabel;
		*key = kABPersonAddressStateKey;
	} else if (strcmp(property,RUBY_PB_ZIP_3)==0) {
		*label = kABOtherLabel;
		*key = kABPersonAddressZIPKey;
	} else if (strcmp(property,RUBY_PB_COUNTRY_3)==0) {
		*label = kABOtherLabel;
		*key = kABPersonAddressCountryKey;
	} else {
		return false;
	}
	return true;
}

CFStringRef getAddressPartValue(ABRecordRef record, const char* property) {
	CFStringRef  label;
	CFStringRef  key;
	CFStringRef  v = nil;
	
	if ( getAddressLabelAndKey(property, &label, &key) ) {
		ABMutableMultiValueRef mv = ABRecordCopyValue(record,kABPersonAddressProperty);
		if (mv) {
			int addresses = ABMultiValueGetCount(mv);
			for (int n = 0; n < addresses; n++) {
				CFStringRef lbl = ABMultiValueCopyLabelAtIndex(mv,n);
                if (lbl) {
                    if (CFStringCompare(lbl,label,0)==kCFCompareEqualTo) {
                        NSMutableDictionary *addressDictionary = 
                            (NSMutableDictionary *)ABMultiValueCopyValueAtIndex(mv,n);
                        if (addressDictionary) {
                            v = (CFStringRef)[addressDictionary objectForKey:(NSString*)key];
                            //[addressDictionary release];
                            CFRelease(lbl);
                            break;
                        }
                    }
                    CFRelease(lbl);
                }
			}
			CFRelease(mv);
		}
	}
	return v;
}

bool setAddressValue(void* record, const char* property, const char* value) {
	CFStringRef  label;
	CFStringRef  key;
	
	if ( !getAddressLabelAndKey(property, &label, &key) ) {
		return false;
	}
		
	CFStringRef v = CFStringCreateWithCString(NULL, value, CFStringGetSystemEncoding());
	ABMutableMultiValueRef mv = ABRecordCopyValue(record,kABPersonAddressProperty);
	if (!mv) {
		mv = ABMultiValueCreateMutable(kABMultiDictionaryPropertyType);
	} else {
		ABMutableMultiValueRef lmv  = ABMultiValueCreateMutableCopy(mv);
		CFRelease(mv);
		mv = lmv;		
	}

	int addresses = ABMultiValueGetCount(mv);
	int index = -1;
	for (int n = 0; n < addresses; n++) {
		CFStringRef lbl = ABMultiValueCopyLabelAtIndex(mv,n);
		if (CFStringCompare(lbl,label,0)==kCFCompareEqualTo) {
			index = n;
			break;
		}
	}
	
	NSMutableDictionary *addressDictionary;
	
	if (index>=0) {
		addressDictionary = (NSMutableDictionary *)ABMultiValueCopyValueAtIndex(mv,index);
	} else {
		addressDictionary = [[NSMutableDictionary alloc] init];
	}

	[addressDictionary setObject:(NSString *)v forKey:(NSString *)key];

	if (index>=0) {
		ABMultiValueReplaceValueAtIndex(mv, addressDictionary, index);
	} else {
		ABMultiValueAddValueAndLabel(mv, addressDictionary, label, NULL);
	}

	ABRecordSetValue(record, kABPersonAddressProperty, mv, nil);
	
	//[addressDictionary release];
	CFRelease(mv);
	CFRelease(v);
	return true;
}

NSDate* string2date(CFStringRef v) {
	NSDateFormatter *df = [[NSDateFormatter alloc] init];
	[df setDateFormat:@"yyyy-MM-dd"];
	NSDate *date = [df dateFromString: (NSString*)v];
	[date retain];
	[df release];
	return date;
}

int setRecordValue(void* record, char* property, char* value) {
	if (record && (value!=NULL) && strlen(value)) {
		int prop = _getProperty(property);
		if (prop >= 0) {
			CFStringRef v = CFStringCreateWithCString(NULL, value, 
													  CFStringGetSystemEncoding());
			if (prop == kABPersonBirthdayProperty) {
				NSDate *birthdate = string2date(v);
				ABRecordSetValue(record, prop, birthdate, nil);
				[birthdate release];
			} else {
				ABRecordSetValue(record, prop, v, nil);
			}
			CFRelease(v);
		} else if ( !setAddressValue(record,property,value) ) {
			CFStringRef label = _getPhoneLabel(property);
			if (label) {
				_updateMultiValueProperty(record,kABPersonPhoneProperty,label,value);
			} else {
				label = _getEmailLabel(property);
				if (label) {
					_updateMultiValueProperty(record,kABPersonEmailProperty,label,value);
				} else {
					label = _getUrlLabel(property);
					if (label) {
						_updateMultiValueProperty(record,kABPersonURLProperty,label,value);
					} else {
						label = _getRelatedNamesLabel(property);
						if (label) {
							_updateMultiValueProperty(record,kABPersonRelatedNamesProperty,label,value);
						} else {
							label = _getDatesLabel(property);
							if (label) {
								CFStringRef v = CFStringCreateWithCString(NULL, value, 
																		  CFStringGetSystemEncoding());
								NSDate *date = string2date(v);
								__updateMultiValueProperty(record,kABPersonDateProperty,label,date);
							}
						}
					}
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

enum {pbUnknown=0,pbPrefix,pbFirstName,pbMiddleName,pbLastName,pbSuffix,pbNickname,pbNote,pbCompanyName,pbJobTitle,pbBirthday};
static const struct {
	const char* _name;
	const int  _value;
} pbp[] = {
{ RUBY_PB_PREFIX, pbPrefix },
{ RUBY_PB_FIRST_NAME, pbFirstName },
{ RUBY_PB_MIDDLE_NAME, pbMiddleName }, 
{ RUBY_PB_LAST_NAME, pbLastName },
{ RUBY_PB_SUFFIX, pbSuffix },
{ RUBY_PB_NICKNAME, pbNickname },
{ RUBY_PB_NOTE, pbNote },
{ RUBY_PB_COMPANY_NAME, pbCompanyName },
{ RUBY_PB_JOB_TITLE, pbJobTitle },
{ RUBY_PB_BIRTHDAY, pbBirthday },
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
		case pbPrefix:
			return kABPersonPrefixProperty;
		case pbFirstName:
			return kABPersonFirstNameProperty;
		case pbMiddleName:
			return kABPersonMiddleNameProperty;
		case pbLastName:
			return kABPersonLastNameProperty;
		case pbSuffix:	
			return kABPersonSuffixProperty;
		case pbNickname:
			return kABPersonNicknameProperty;
		case pbNote:
			return kABPersonNoteProperty;
		case pbCompanyName:
			return kABPersonOrganizationProperty;
		case pbJobTitle:	
			return kABPersonJobTitleProperty;
		case pbBirthday:
			return kABPersonBirthdayProperty;
	}
	return -1;
}

