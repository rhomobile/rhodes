/* phonebook.i */
%module Phonebook
%{
#include "ext/rho/rhoruby.h"
extern void* openPhonebook();
extern void  closePhonebook(void* pb);

extern VALUE getallPhonebookRecords(void* pb);
#define allRecords getallPhonebookRecords

extern int getPhonebookRecordCount(void* pb, rho_param* params);
#define countRecords getPhonebookRecordCount

extern VALUE getPhonebookRecords(void* pb, rho_param* params);
#define getRecords getPhonebookRecords

extern void* openPhonebookRecord(void* pb, char* id);
#define openRecord openPhonebookRecord

extern VALUE getPhonebookRecord(void* pb, char* id);
#define getRecord getPhonebookRecord

extern VALUE getfirstPhonebookRecord(void* pb);
#define firstRecord getfirstPhonebookRecord

extern VALUE getnextPhonebookRecord(void* pb);
#define nextRecord getnextPhonebookRecord

extern void* createRecord(void* pb);
extern int setRecordValue(void* record, char* property, char* value);
extern int addRecord(void* pb, void* record);
extern int saveRecord(void* pb, void* record);
extern int deleteRecord(void* pb, void* record);

%}

%typemap(default) (rho_param* params) {
 $1 = NULL;
}

%typemap(in) (rho_param* params) {
    $1 = rho_param_fromvalue($input);
}

%typemap(freearg) (rho_param* params) {
    rho_param_free($1);
}

extern void* openPhonebook();
extern void  closePhonebook(void* pb);
extern VALUE allRecords(void* pb);
extern int countRecords(void* pb, rho_param* params);
extern VALUE getRecords(void* pb, rho_param* params);
extern void* openRecord(void* pb, char* id);
extern VALUE getRecord(void* pb, char* id);
extern VALUE firstRecord(void* pb);
extern VALUE nextRecord(void* pb);
extern void* createRecord(void* pb);
extern int setRecordValue(void* record, char* property, char* value);
extern int addRecord(void* pb, void* record);
extern int saveRecord(void* pb, void* record);
extern int deleteRecord(void* pb, void* record);

