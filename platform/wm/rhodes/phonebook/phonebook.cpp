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

#include "stdafx.h"

#include "ext/rho/rhoruby.h"
#include "phonebook.h"

void* openPhonebook() {
#if defined (_WIN32_WCE) && !defined( OS_PLATFORM_MOTCE )
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
#if defined (_WIN32_WCE)&& !defined( OS_PLATFORM_MOTCE )
	if (pb) {
		CNativeAddressBook* phonebook = (CNativeAddressBook*)pb;
		delete phonebook;
	}
#endif
}

void _addRecordValue(const char* key, const char* value, void* hash) {
	addStrToHash(*((unsigned long*)hash), key, value);
}

#if defined (_WIN32_WCE)&& !defined( OS_PLATFORM_MOTCE )
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
#if defined (_WIN32_WCE)&& !defined( OS_PLATFORM_MOTCE )
	if (pb) {
		CNativeAddressBook* phonebook = (CNativeAddressBook*)pb;
        VALUE valGc = rho_ruby_disable_gc();
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

        rho_ruby_enable_gc(valGc);
		return hash;
	}
#endif
	return rho_ruby_get_NIL();
}

int getPhonebookRecordCount(void* pb) {
    return 0;
}

VALUE getPhonebookRecords(void* pb, int offset, int max_results, rho_param* select_param) {
	return rho_ruby_get_NIL();
}


VALUE getPhonebookRecord(void* pb, char* id) {
#if defined (_WIN32_WCE)&& !defined( OS_PLATFORM_MOTCE )
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
#if defined (_WIN32_WCE)&& !defined( OS_PLATFORM_MOTCE )
	return new CABOutlookRecord(NULL);
#else
	return NULL;
#endif
}

void* openPhonebookRecord(void* pb, char* id) {
#if defined (_WIN32_WCE)&& !defined( OS_PLATFORM_MOTCE )
	if (pb && id) {
		CNativeAddressBook* phonebook = (CNativeAddressBook*)pb;
		return phonebook->getRecord(id);
	}
#endif
	return NULL;
}

int setRecordValue(void* record, char* prop, char* value) {
#if defined (_WIN32_WCE)&& !defined( OS_PLATFORM_MOTCE )
	if (record) {
		CABRecord* r = (CABRecord*)record;
		r->setValue(prop,value);
	}
#endif
	return 1;
}

int addRecord(void* pb, void* record) {
#if defined (_WIN32_WCE)&& !defined( OS_PLATFORM_MOTCE )
	if (pb && record) {
		CNativeAddressBook* phonebook = (CNativeAddressBook*)pb;
		phonebook->addRecord((CABRecord*)record);
		delete record;
	}
#endif
	return 1;
}

int saveRecord(void* pb, void* record) {
#if defined (_WIN32_WCE)&& !defined( OS_PLATFORM_MOTCE )
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
#if defined (_WIN32_WCE)&& !defined( OS_PLATFORM_MOTCE )
	if (record) {
		CABRecord* r = (CABRecord*)record;
		r->remove();
		delete r;
	}
#endif
	return 1;
}

//==================================================================================


