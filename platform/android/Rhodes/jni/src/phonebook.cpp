#include "JNIRhodes.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Phonebook"

RHO_GLOBAL void* openPhonebook()
{
    // TODO:
    RHO_NOT_IMPLEMENTED;
    return NULL;
}

RHO_GLOBAL void closePhonebook(void* pb)
{
    // TODO:
    RHO_NOT_IMPLEMENTED;
}

RHO_GLOBAL VALUE getallPhonebookRecords(void* pb)
{
    // TODO:
    RHO_NOT_IMPLEMENTED;
    return 0;
}

RHO_GLOBAL void* openPhonebookRecord(void* pb, char* id)
{
    // TODO:
    RHO_NOT_IMPLEMENTED;
    return NULL;
}

RHO_GLOBAL VALUE getPhonebookRecord(void* pb, char* id)
{
    // TODO:
    RHO_NOT_IMPLEMENTED;
    return 0;
}

RHO_GLOBAL VALUE getfirstPhonebookRecord(void* pb)
{
    // TODO:
    RHO_NOT_IMPLEMENTED;
    return 0;
}

RHO_GLOBAL VALUE getnextPhonebookRecord(void* pb)
{
    // TODO:
    RHO_NOT_IMPLEMENTED;
    return 0;
}

RHO_GLOBAL void* createRecord(void* pb)
{
    // TODO:
    RHO_NOT_IMPLEMENTED;
    return NULL;
}

RHO_GLOBAL int setRecordValue(void* record, char* property, char* value)
{
    // TODO:
    RHO_NOT_IMPLEMENTED;
    return 0;
}

RHO_GLOBAL int addRecord(void* pb, void* record)
{
    // TODO:
    RHO_NOT_IMPLEMENTED;
    return 0;
}

RHO_GLOBAL int saveRecord(void* pb, void* record)
{
    // TODO:
    RHO_NOT_IMPLEMENTED;
    return 0;
}

RHO_GLOBAL int deleteRecord(void* pb, void* record)
{
    // TODO:
    RHO_NOT_IMPLEMENTED;
    return 0;
}

