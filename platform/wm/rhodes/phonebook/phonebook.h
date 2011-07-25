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