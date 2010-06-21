#include "rhodes/JNIRhodes.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Phonebook"

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
    JNIEnv *env = jnienv();
    jclass contactCls = getJNIClass(RHODES_JAVA_CLASS_CONTACT);
    if (!contactCls) return Qnil;
    jclass fieldCls = getJNIClass(RHODES_JAVA_CLASS_CONTACT_FIELD);
    if (!fieldCls) return Qnil;

    jmethodID contactMoveToBeginMID = getJNIClassMethod(env, contactCls, "moveToBegin", "()V");
    if (!contactMoveToBeginMID) return Qnil;
    jmethodID hasNextMID = getJNIClassMethod(env, contactCls, "hasNext", "()Z");
    if (!hasNextMID) return Qnil;
    jmethodID nextMID = getJNIClassMethod(env, contactCls, "next", "()Ljava/lang/Object;");
    if (!nextMID) return Qnil;
    jmethodID getKeyMID = getJNIClassMethod(env, fieldCls, "getKey", "()Ljava/lang/String;");
    if (!getKeyMID) return Qnil;
    jmethodID getValueMID = getJNIClassMethod(env, fieldCls, "getValue", "()Ljava/lang/String;");
    if (!getValueMID) return Qnil;

    CHoldRubyValue contactHash(createHash());
    // contact.moveToBegin();
    env->CallVoidMethod(contactObj, contactMoveToBeginMID);

    // while(contact.hasNext())
    while(env->CallBooleanMethod(contactObj, hasNextMID))
    {
        // Contact.Field entry = (Contact.Field)contact.next();
        jobject entryObj = env->CallObjectMethod(contactObj, nextMID);
        if (!entryObj) return Qnil;
        // String key = entry.getKey();
        jstring keyObj = (jstring)env->CallObjectMethod(entryObj, getKeyMID);
        if (!keyObj) return Qnil;
        // String value = entry.getValue();
        jstring valueObj = (jstring)env->CallObjectMethod(entryObj, getValueMID);
        if (!valueObj) return Qnil;

        addStrToHash(contactHash, rho_cast<std::string>(keyObj).c_str(), rho_cast<std::string>(valueObj).c_str());

        env->DeleteLocalRef(valueObj);
        env->DeleteLocalRef(keyObj);
        env->DeleteLocalRef(entryObj);
    }

    return contactHash;
}

RHO_GLOBAL VALUE getallPhonebookRecords(void* pb)
{
    jobject phonebookObj = (jobject)pb;

    JNIEnv *env = jnienv();

    jclass phonebookCls = getJNIClass(RHODES_JAVA_CLASS_PHONEBOOK);
    if (!phonebookCls) return Qnil;
    jclass contactCls = getJNIClass(RHODES_JAVA_CLASS_CONTACT);
    if (!contactCls) return Qnil;

    jmethodID phonebookMoveToBeginMID = getJNIClassMethod(env, phonebookCls, "moveToBegin", "()V");
    if (!phonebookMoveToBeginMID) return Qnil;
    jmethodID hasNextMID = getJNIClassMethod(env, phonebookCls, "hasNext", "()Z");
    if (!hasNextMID) return Qnil;
    jmethodID nextMID = getJNIClassMethod(env, phonebookCls, "next", "()Ljava/lang/Object;");
    if (!nextMID) return Qnil;
    jmethodID contactIdMID = getJNIClassMethod(env, contactCls, "id", "()Ljava/lang/String;");
    if (!contactIdMID) return Qnil;

    // pb.moveToBegin();
    env->CallVoidMethod(phonebookObj, phonebookMoveToBeginMID);

    CHoldRubyValue hash(createHash());
    // while(pb.hasNext())
    while(env->CallBooleanMethod(phonebookObj, hasNextMID))
    {
        // Contact contact = (Contact)pb.next();
        jobject contactObj = env->CallObjectMethod(phonebookObj, nextMID);
        if (!contactObj) return Qnil;
        // String id = contact.id();
        jstring idObj = (jstring)env->CallObjectMethod(contactObj, contactIdMID);
        if (!idObj) return Qnil;

        addHashToHash(hash, rho_cast<std::string>(idObj).c_str(), createHashFromContact(contactObj));

        env->DeleteLocalRef(idObj);
        env->DeleteLocalRef(contactObj);
    }

    return hash;
}

RHO_GLOBAL void* openPhonebookRecord(void* pb, char* id)
{
    JNIEnv *env = jnienv();
    jobject obj = (jobject)pb;
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_PHONEBOOK);
    if (!cls) return NULL;
    jmethodID mid = getJNIClassMethod(env, cls, "getRecord", "(Ljava/lang/String;)Lcom/rhomobile/rhodes/phonebook/Contact;");
    if (!mid) return NULL;

    jstring objId = rho_cast<jstring>(id);
    jobject recordObj = env->CallObjectMethod(obj, mid, objId);
    env->DeleteLocalRef(objId);
    if (!recordObj) return NULL;
    jobject retval = env->NewGlobalRef(recordObj);
    env->DeleteLocalRef(recordObj);
    if (!retval) return NULL;
    return retval;
}

RHO_GLOBAL VALUE getPhonebookRecord(void* pb, char* id)
{
    jobject recordObj = (jobject)openPhonebookRecord(pb, id);
    if (!recordObj)
        return Qnil;
    VALUE retval = createHashFromContact(recordObj);
    jnienv()->DeleteGlobalRef(recordObj);
    return retval;
}

static VALUE getRecord(void *pb, const char *name)
{
    jobject obj = (jobject)pb;
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_PHONEBOOK);
    if (!cls) return Qnil;
    jmethodID mid = getJNIClassMethod(env, cls, name, "()Lcom/rhomobile/rhodes/phonebook/Contact;");
    if (!mid) return Qnil;

    jobject recordObj = env->CallObjectMethod(obj, mid);
    if (!recordObj)
        return Qnil;
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

RHO_GLOBAL int setRecordValue(void* record, char* property, char* value)
{
    jobject contactObj = (jobject)record;
    JNIEnv *env = jnienv();
    jclass contactCls = getJNIClass(RHODES_JAVA_CLASS_CONTACT);
    if (!contactCls) return 0;
    jmethodID mid = getJNIClassMethod(env, contactCls, "setField", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!mid) return 0;

    jstring objProperty = rho_cast<jstring>(property);
    jstring objValue = rho_cast<jstring>(value);
    env->CallVoidMethod(contactObj, mid, objProperty, objValue);
    env->DeleteLocalRef(objProperty);
    env->DeleteLocalRef(objValue);
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

