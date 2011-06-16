#include <time.h>
#include "ext/rho/rhoruby.h"

extern "C" {

void* openPhonebook()
{
    return NULL;
}

void  closePhonebook(void* pb)
{
}

void _addRecordValue(const char* key, const char* value, void* hash)
{
    addStrToHash(*((unsigned long*)hash), key, value);
}

VALUE getallPhonebookRecords(void* pb)
{
    return rho_ruby_get_NIL();    
}

VALUE getPhonebookRecord(void* pb, char* id)
{
    return rho_ruby_get_NIL();    
}

VALUE getfirstPhonebookRecord(void* pb)
{
    return rho_ruby_get_NIL();
}

VALUE getnextPhonebookRecord(void* pb)
{
    return rho_ruby_get_NIL();
}

void* createRecord(void* pb)
{
    return NULL;
}

void* openPhonebookRecord(void* pb, char* id)
{
    return NULL;
}

int setRecordValue(void* record, char* prop, char* value)
{
    return 1;
}

int addRecord(void* pb, void* record)
{
    return 1;
}

int saveRecord(void* pb, void* record)
{
    return 1;
}

int deleteRecord(void* pb, void* record)
{
    return 1;
}

} //extern "C"
