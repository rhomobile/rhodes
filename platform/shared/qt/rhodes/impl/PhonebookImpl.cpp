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
