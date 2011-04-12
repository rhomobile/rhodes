/*
 *  phonebook.h
 *  rhorunner
 *
 *  Created by Vlad on 12/12/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef __LOCAL_PHONEBOOK_H__
#define __LOCAL_PHONEBOOK_H__

#if defined(_WIN32_WCE) && !defined( OS_PLATFORM_CE )
#include "NativeAddressBook.h"
#endif

#if defined(__cplusplus)
extern "C" {
#endif

void* openPhonebook();
void  closePhonebook(void* pb);
VALUE getallPhonebookRecords(void* pb);
VALUE getPhonebookRecord(void* pb, char* id);

VALUE getfirstPhonebookRecord(void* pb);
VALUE getnextPhonebookRecord(void* pb);

void* createRecord(void* pb);
void* openPhonebookRecord(void* pb, char* id);
int setRecordValue(void* record, char* prop, char* value);
int addRecord(void* pb, void* record);
int saveRecord(void* pb, void* record);
int deleteRecord(void* pb, void* record);

#if defined(__cplusplus)
}
#endif

#endif //__LOCAL_PHONEBOOK_H__