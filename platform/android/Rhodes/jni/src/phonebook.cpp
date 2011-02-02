#include "rhodes/JNIRhodes.h"

#include <android/log.h>
#include <logging/RhoLogConf.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Phonebook"

#define logging_enable false


#define PB_FIELDS_COUNT 8

static const char* field_names[PB_FIELDS_COUNT] = {
"id", // 0
"first_name",  // 1
"last_name",  // 2
"mobile_number", // 3
"home_number",  // 4
"business_number",  // 5
"email_address",  // 6
"company_name"  // 7
};

RHO_GLOBAL void* openPhonebook()
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_PHONEBOOK);
    if (!cls) return NULL;
    jmethodID cid = getJNIClassMethod(env, cls, "<init>", "()V");
    if (!cid) return NULL;

    jobject local = env->NewObject(cls, cid);
    jobject obj = env->NewGlobalRef(local);
    env->DeleteLocalRef(local);
    return obj;
}

RHO_GLOBAL void closePhonebook(void* pb)
{
    JNIEnv *env = jnienv();
    jobject obj = (jobject)pb;
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_PHONEBOOK);
    if (!cls) return;
    jmethodID mid = getJNIClassMethod(env, cls, "close", "()V");
    if (!mid) return;

    env->CallVoidMethod(obj, mid);
    env->DeleteGlobalRef(obj);
}

static VALUE createHashFromContact(jobject contactObj)
{
    if (logging_enable) RAWLOG_INFO("createHashFromContact() START");
    JNIEnv *env = jnienv();
    jclass contactCls = getJNIClass(RHODES_JAVA_CLASS_CONTACT);
    if (!contactCls) return Qnil;
    jclass fieldCls = getJNIClass(RHODES_JAVA_CLASS_CONTACT_FIELD);
    if (!fieldCls) return Qnil;

    jmethodID contactGetFieldMID = getJNIClassMethod(env, contactCls, "getField", "(I)Ljava/lang/String;");
    if (!contactGetFieldMID) return Qnil;

    CHoldRubyValue contactHash(rho_ruby_createHash());
    // contact.moveToBegin();

	int i;
	for (i = 0; i < PB_FIELDS_COUNT; i++) {
		jstring value = (jstring)env->CallObjectMethod(contactObj, contactGetFieldMID, i);
		if (value != NULL) {
			addStrToHash(contactHash, field_names[i], rho_cast<std::string>(value).c_str());
		}
		env->DeleteLocalRef(value);
	}
    if (logging_enable) RAWLOG_INFO("createHashFromContact() FINISH");
    return contactHash;
}

RHO_GLOBAL VALUE getallPhonebookRecords(void* pb)
{
    if (logging_enable) RAWLOG_INFO("getallPhonebookRecords() START");
    jobject phonebookObj = (jobject)pb;

    JNIEnv *env = jnienv();

    jclass phonebookCls = getJNIClass(RHODES_JAVA_CLASS_PHONEBOOK);
    if (!phonebookCls) return Qnil;
    jclass contactCls = getJNIClass(RHODES_JAVA_CLASS_CONTACT);
    if (!contactCls) return Qnil;

    jmethodID phonebookPrepareFullListMID = getJNIClassMethod(env, phonebookCls, "prepareFullList", "()V");
    if (!phonebookPrepareFullListMID) return Qnil;

    jmethodID phonebookMoveToBeginMID = getJNIClassMethod(env, phonebookCls, "moveToBegin", "()V");
    if (!phonebookMoveToBeginMID) return Qnil;
    jmethodID hasNextMID = getJNIClassMethod(env, phonebookCls, "hasNext", "()Z");
    if (!hasNextMID) return Qnil;
    jmethodID nextMID = getJNIClassMethod(env, phonebookCls, "next", "()Ljava/lang/Object;");
    if (!nextMID) return Qnil;
    jmethodID contactIdMID = getJNIClassMethod(env, contactCls, "id", "()Ljava/lang/String;");
    if (!contactIdMID) return Qnil;

	jmethodID contactGetFieldMID = getJNIClassMethod(env, contactCls, "getField", "(I)Ljava/lang/String;");
	if (!contactGetFieldMID) return Qnil;


    env->CallVoidMethod(phonebookObj, phonebookPrepareFullListMID);

    // pb.moveToBegin();
    env->CallVoidMethod(phonebookObj, phonebookMoveToBeginMID);


    VALUE valGc = rho_ruby_disable_gc();
    CHoldRubyValue hash(rho_ruby_createHash());
    // while(pb.hasNext())
    while(env->CallBooleanMethod(phonebookObj, hasNextMID))
    {
        // Contact contact = (Contact)pb.next();
        jobject contactObj = env->CallObjectMethod(phonebookObj, nextMID);
        if (!contactObj) return Qnil;
        // String id = contact.id();
        jstring idObj = (jstring)env->CallObjectMethod(contactObj, contactIdMID);
        if (!idObj) return Qnil;

        //addHashToHash(hash, rho_cast<std::string>(idObj).c_str(), createHashFromContact(contactObj));
		CHoldRubyValue contactHash(rho_ruby_createHash());
		// contact.moveToBegin();

		int i;
		for (i = 0; i < PB_FIELDS_COUNT; i++) {
			jstring value = (jstring)env->CallObjectMethod(contactObj, contactGetFieldMID, i);
			if (value != NULL) {
				addStrToHash(contactHash, field_names[i], rho_cast<std::string>(value).c_str());
				env->DeleteLocalRef(value);
			}
		}

		addHashToHash(hash, rho_cast<std::string>(idObj).c_str(), contactHash);

        env->DeleteLocalRef(idObj);
        env->DeleteLocalRef(contactObj);
    }

    rho_ruby_enable_gc(valGc);
    if (logging_enable) RAWLOG_INFO("getallPhonebookRecords() FINISH");
    return hash;
}

RHO_GLOBAL void* openPhonebookRecord(void* pb, char* id)
{
    if (logging_enable) RAWLOG_INFO("openPhonebookRecord() START");
    JNIEnv *env = jnienv();
    jobject obj = (jobject)pb;
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_PHONEBOOK);
    if (!cls) return NULL;
    jmethodID mid = getJNIClassMethod(env, cls, "getRecord", "(Ljava/lang/String;)Lcom/rhomobile/rhodes/phonebook/Contact;");
    if (!mid) return NULL;

    jhstring objId = rho_cast<jhstring>(id);
    jhobject recordObj = jhobject(env->CallObjectMethod(obj, mid, objId.get()));
    if (!recordObj) return NULL;
    jhobject retval = jhobject(env->NewGlobalRef(recordObj.get()));
    if (logging_enable) RAWLOG_INFO("openPhonebookRecord() FINISH");
    if (!retval) return NULL;
    return retval.release();
}

RHO_GLOBAL VALUE getPhonebookRecord(void* pb, char* id)
{
    if (logging_enable) RAWLOG_INFO("getPhonebookRecord() START");
    jobject recordObj = (jobject)openPhonebookRecord(pb, id);
    if (!recordObj) {
	if (logging_enable) RAWLOG_INFO("getPhonebookRecord() FINISH return NIL");
        return Qnil;
    }
    VALUE retval = createHashFromContact(recordObj);
    jnienv()->DeleteGlobalRef(recordObj);
    if (logging_enable) RAWLOG_INFO("getPhonebookRecord() FINISH");
    return retval;
}

static VALUE getRecord(void *pb, const char *name)
{
    if (logging_enable) RAWLOG_INFO("getRecord() START");
    jobject obj = (jobject)pb;
    JNIEnv *env = jnienv();
    if (!env) {
        if (logging_enable) RAWLOG_INFO("getRecord() FINISH return NIL0");
        return Qnil;
    }
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_PHONEBOOK);
    if (!cls) {
        if (logging_enable) RAWLOG_INFO("getRecord() FINISH return NIL1");
	return Qnil;
    }
    jmethodID mid = getJNIClassMethod(env, cls, name, "()Lcom/rhomobile/rhodes/phonebook/Contact;");
    if (!mid) {
        if (logging_enable) RAWLOG_INFO("getRecord() FINISH return NIL2");
	return Qnil; 
    }	
    jobject recordObj = env->CallObjectMethod(obj, mid);
    if (!recordObj) {
        if (logging_enable) RAWLOG_INFO("getRecord() FINISH return NIL");
        return Qnil;
    }
    if (logging_enable) RAWLOG_INFO("getRecord() FINISH");
    return createHashFromContact(recordObj);
}

RHO_GLOBAL VALUE getfirstPhonebookRecord(void* pb)
{
    return getRecord(pb, "getFirstRecord");
}

RHO_GLOBAL VALUE getnextPhonebookRecord(void* pb)
{
    return getRecord(pb, "getNextRecord");
}

RHO_GLOBAL void* createRecord(void* pb)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_CONTACT);
    if (!cls) return NULL;
    jmethodID cid = getJNIClassMethod(env, cls, "<init>", "()V");
    if (!cid) return NULL;

    jobject local = env->NewObject(cls, cid);
    jobject obj = env->NewGlobalRef(local);
    env->DeleteLocalRef(local);
    return obj;
}

int get_value_index_by_name(char* name) {
	int i;
	for (i = 0; i < PB_FIELDS_COUNT; i++) {
		if (strcmp(name, field_names[i]) == 0) {
			return i;
		}
	}
	return -1;
}

RHO_GLOBAL int setRecordValue(void* record, char* property, char* value)
{
    jobject contactObj = (jobject)record;
    JNIEnv *env = jnienv();
    jclass contactCls = getJNIClass(RHODES_JAVA_CLASS_CONTACT);
    if (!contactCls) return 0;
    jmethodID mid = getJNIClassMethod(env, contactCls, "setField", "(ILjava/lang/String;)V");
    if (!mid) return 0;

    jhstring objValue = rho_cast<jhstring>(value);

	int index = get_value_index_by_name(property);
	if (index >= 0) {
		env->CallVoidMethod(contactObj, mid, index, objValue.get());
	}
	else {
		RAWLOG_ERROR("Phonebook.cpp invalid property name for Contact Record");
	}
    return 1;
}

static void doContactOp(jobject pbObj, jobject contactObj, const char *name)
{
    JNIEnv *env = jnienv();
    jclass pbCls = getJNIClass(RHODES_JAVA_CLASS_PHONEBOOK);
    if (!pbCls) return;
    jmethodID mid = getJNIClassMethod(env, pbCls, name, "(Lcom/rhomobile/rhodes/phonebook/Contact;)V");
    if (!mid) return;

    env->CallVoidMethod(pbObj, mid, contactObj);
}

RHO_GLOBAL int addRecord(void* pb, void* record)
{
    doContactOp((jobject)pb, (jobject)record, "saveContact");
    jnienv()->DeleteGlobalRef((jobject)record);
    return 1;
}

RHO_GLOBAL int saveRecord(void* pb, void* record)
{
    doContactOp((jobject)pb, (jobject)record, "saveContact");
    return 1;
}

RHO_GLOBAL int deleteRecord(void* pb, void* record)
{
    doContactOp((jobject)pb, (jobject)record, "removeContact");
    jnienv()->DeleteGlobalRef((jobject)record);
    return 1;
}

