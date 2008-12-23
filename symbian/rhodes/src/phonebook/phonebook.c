/*
 ============================================================================
 Name		: HttpClient.cpp
 Author	  : Anton Antonov
 Version	 : 1.0
 Copyright   :   Copyright (C) 2008 Rhomobile. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 Description : CHttpClient implementation
 ============================================================================
 */

#include "ruby/ruby.h"

//Stubs for phonebook

void* openPhonebook() {
	return NULL;
}

void  closePhonebook(void* pb) {
}

VALUE getallPhonebookRecords(void* pb) {
	return getnil();	
}

VALUE getPhonebookRecord(void* pb, char* id) {
	return getnil();	
}

//==================================================================================

VALUE getfirstPhonebookRecord(void* pb) {
	return getnil();
}

VALUE getnextPhonebookRecord(void* pb) {
	return getnil();}

//==================================================================================

void* createRecord() {
	return NULL;
}

void* openPhonebookRecord(void* pb, char* id) {
	return NULL;
}

int setRecordValue(void* record, char* prop, char* value) {
	return 1;
}

int addRecord(void* pb, void* record) {
	return 1;
}

int saveRecord(void* pb, void* record) {
	return 1;
}

int deleteRecord(void* pb, void* record) {
	return 1;
}
