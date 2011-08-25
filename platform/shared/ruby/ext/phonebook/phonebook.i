/* system.i */
%module Phonebook
%{
extern void* openPhonebook();
extern void  closePhonebook(void* pb);
extern VALUE getallPhonebookRecords(void* pb);
extern int getPhonebookRecordCount(void* pb);
extern VALUE getPhonebookRecords(void* pb, int offset, int max_results);
extern void* openPhonebookRecord(void* pb, char* id);
extern VALUE getPhonebookRecord(void* pb, char* id);
extern VALUE getfirstPhonebookRecord(void* pb);
extern VALUE getnextPhonebookRecord(void* pb);
extern void* createRecord(void* pb);
extern int setRecordValue(void* record, char* property, char* value);
extern int addRecord(void* pb, void* record);
extern int saveRecord(void* pb, void* record);
extern int deleteRecord(void* pb, void* record);
%}

extern void* openPhonebook();
extern void  closePhonebook(void* pb);
extern VALUE getallPhonebookRecords(void* pb);
extern int getPhonebookRecordCount(void* pb);
extern VALUE getPhonebookRecords(void* pb, int offset, int max_results);
extern void* openPhonebookRecord(void* pb, char* id);
extern VALUE getPhonebookRecord(void* pb, char* id);
extern VALUE getfirstPhonebookRecord(void* pb);
extern VALUE getnextPhonebookRecord(void* pb);
extern void* createRecord(void* pb);
extern int setRecordValue(void* record, char* property, char* value);
extern int addRecord(void* pb, void* record);
extern int saveRecord(void* pb, void* record);
extern int deleteRecord(void* pb, void* record);
