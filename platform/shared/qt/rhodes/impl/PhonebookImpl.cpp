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

#include "common/RhoPort.h"
#include "ext/rho/rhoruby.h"

extern "C" {

void* openPhonebook()
{
    //TODO: openPhonebook
    return NULL;
}

void  closePhonebook(void* pb)
{
    //TODO: closePhonebook
}

VALUE getallPhonebookRecords(void* pb)
{
    //TODO: getallPhonebookRecords
    return rho_ruby_get_NIL();    
}

int getPhonebookRecordCount(void* pb)
{
    //TODO: getPhonebookRecordCount
    return 0;
}

VALUE getPhonebookRecords(void* pb, int offset, int max_results, rho_param* select_param)
{
    //TODO: getPhonebookRecords
	return rho_ruby_get_NIL();	
}

VALUE getPhonebookRecord(void* pb, char* id)
{
    //TODO: getPhonebookRecord
    return rho_ruby_get_NIL();    
}

VALUE getfirstPhonebookRecord(void* pb)
{
    //TODO: getfirstPhonebookRecord
    return rho_ruby_get_NIL();
}

VALUE getnextPhonebookRecord(void* pb)
{
    //TODO: getnextPhonebookRecord
    return rho_ruby_get_NIL();
}

void* createRecord(void* pb)
{
    //TODO: createRecord
    return NULL;
}

void* openPhonebookRecord(void* pb, char* id)
{
    //TODO: openPhonebookRecord
    return NULL;
}

int setRecordValue(void* record, char* prop, char* value)
{
    //TODO: setRecordValue
    return 1;
}

int addRecord(void* pb, void* record)
{
    //TODO: addRecord
    return 1;
}

int saveRecord(void* pb, void* record)
{
    //TODO: saveRecord
    return 1;
}

int deleteRecord(void* pb, void* record)
{
    //TODO: deleteRecord
    return 1;
}

} //extern "C"
