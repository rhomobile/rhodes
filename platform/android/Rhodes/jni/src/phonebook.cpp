#include "JNIRhodes.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Phonebook"

RHO_GLOBAL void* openPhonebook()
{
    RHO_LOG_CALLBACK;
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_PHONEBOOK);
    if (!cls) return NULL;
    jmethodID cid = getJNIClassMethod(cls, "<init>", "()V");
    if (!cid) return NULL;

    JNIEnv *env = jnienv();
    jobject local = env->NewObject(cls, cid);
    jobject obj = env->NewGlobalRef(local);
    env->DeleteLocalRef(local);
    return obj;
}

RHO_GLOBAL void closePhonebook(void* pb)
{
    RHO_LOG_CALLBACK;
    jobject obj = (jobject)pb;
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_PHONEBOOK);
    if (!cls) return;
    jmethodID mid = getJNIClassMethod(cls, "close", "()V");
    if (!mid) return;

    JNIEnv *env = jnienv();
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

    jmethodID contactMoveToBeginMID = getJNIClassMethod(contactCls, "moveToBegin", "()V");
    if (!contactMoveToBeginMID) return Qnil;
    jmethodID hasNextMID = getJNIClassMethod(contactCls, "hasNext", "()Z");
    if (!hasNextMID) return Qnil;
    jmethodID nextMID = getJNIClassMethod(contactCls, "next", "()Ljava/lang/Object;");
    if (!nextMID) return Qnil;
    jmethodID getKeyMID = getJNIClassMethod(fieldCls, "getKey", "()Ljava/lang/String;");
    if (!getKeyMID) return Qnil;
    jmethodID getValueMID = getJNIClassMethod(fieldCls, "getValue", "()Ljava/lang/String;");
    if (!getValueMID) return Qnil;

    VALUE contactHash = createHash();

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

        const char *keyStr = env->GetStringUTFChars(keyObj, JNI_FALSE);
        const char *valueStr = env->GetStringUTFChars(valueObj, JNI_FALSE);
        addStrToHash(contactHash, keyStr, valueStr, strlen(valueStr));
        env->ReleaseStringUTFChars(keyObj, keyStr);
        env->ReleaseStringUTFChars(valueObj, valueStr);
    }

    return contactHash;
}

RHO_GLOBAL VALUE getallPhonebookRecords(void* pb)
{
    RHO_LOG_CALLBACK;
    jobject phonebookObj = (jobject)pb;

    JNIEnv *env = jnienv();

    jclass phonebookCls = getJNIClass(RHODES_JAVA_CLASS_PHONEBOOK);
    if (!phonebookCls) return Qnil;
    jclass contactCls = getJNIClass(RHODES_JAVA_CLASS_CONTACT);
    if (!contactCls) return Qnil;

    jmethodID phonebookMoveToBeginMID = getJNIClassMethod(phonebookCls, "moveToBegin", "()V");
    if (!phonebookMoveToBeginMID) return Qnil;
    jmethodID hasNextMID = getJNIClassMethod(phonebookCls, "hasNext", "()Z");
    if (!hasNextMID) return Qnil;
    jmethodID nextMID = getJNIClassMethod(phonebookCls, "next", "()Ljava/lang/Object;");
    if (!nextMID) return Qnil;
    jmethodID contactIdMID = getJNIClassMethod(contactCls, "id", "()Ljava/lang/String;");
    if (!contactIdMID) return Qnil;

    // pb.moveToBegin();
    env->CallVoidMethod(phonebookObj, phonebookMoveToBeginMID);

    VALUE hash = createHash();
    // while(pb.hasNext())
    while(env->CallBooleanMethod(phonebookObj, hasNextMID))
    {
        // Contact contact = (Contact)pb.next();
        jobject contactObj = env->CallObjectMethod(phonebookObj, nextMID);
        if (!contactObj) return Qnil;
        // String id = contact.id();
        jstring idObj = (jstring)env->CallObjectMethod(contactObj, contactIdMID);
        if (!idObj) return Qnil;
        const char *idStr = env->GetStringUTFChars(idObj, JNI_FALSE);

        addHashToHash(hash, idStr, createHashFromContact(contactObj));
        env->ReleaseStringUTFChars(idObj, idStr);
    }
    return hash;
}

RHO_GLOBAL void* openPhonebookRecord(void* pb, char* id)
{
    RHO_LOG_CALLBACK;
    jobject obj = (jobject)pb;
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_PHONEBOOK);
    if (!cls) return NULL;
    jmethodID mid = getJNIClassMethod(cls, "getRecord", "(Ljava/lang/String;)Lcom/rhomobile/rhodes/phonebook/Contact;");
    if (!mid) return NULL;

    JNIEnv *env = jnienv();
    jobject recordObj = env->CallObjectMethod(obj, mid, env->NewStringUTF(id));
    jobject retval = env->NewGlobalRef(recordObj);
    env->DeleteLocalRef(recordObj);
    return retval;
}

RHO_GLOBAL VALUE getPhonebookRecord(void* pb, char* id)
{
    RHO_LOG_CALLBACK;
    jobject obj = (jobject)pb;
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_PHONEBOOK);
    if (!cls) return Qnil;
    jmethodID mid = getJNIClassMethod(cls, "getRecord", "(Ljava/lang/String;)Lcom/rhomobile/rhodes/phonebook/Contact;");
    if (!mid) return Qnil;

    JNIEnv *env = jnienv();
    jobject recordObj = env->CallObjectMethod(obj, mid, env->NewStringUTF(id));
    if (!recordObj)
        return Qnil;
    return createHashFromContact(recordObj);
}

RHO_GLOBAL VALUE getfirstPhonebookRecord(void* pb)
{
    RHO_LOG_CALLBACK;
    jobject obj = (jobject)pb;
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_PHONEBOOK);
    if (!cls) return Qnil;
    jmethodID mid = getJNIClassMethod(cls, "getFirstRecord", "()Lcom/rhomobile/rhodes/phonebook/Contact;");
    if (!mid) return Qnil;

    JNIEnv *env = jnienv();
    jobject recordObj = env->CallObjectMethod(obj, mid);
    if (!recordObj)
        return Qnil;
    return createHashFromContact(recordObj);
}

RHO_GLOBAL VALUE getnextPhonebookRecord(void* pb)
{
    RHO_LOG_CALLBACK;
    jobject obj = (jobject)pb;
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_PHONEBOOK);
    if (!cls) return Qnil;
    jmethodID mid = getJNIClassMethod(cls, "getNextRecord", "()Lcom/rhomobile/rhodes/phonebook/Contact;");
    if (!mid) return Qnil;

    JNIEnv *env = jnienv();
    jobject recordObj = env->CallObjectMethod(obj, mid);
    if (!recordObj)
        return Qnil;
    return createHashFromContact(recordObj);
}

RHO_GLOBAL void* createRecord(void* pb)
{
    RHO_LOG_CALLBACK;
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_CONTACT);
    if (!cls) return NULL;
    jmethodID cid = getJNIClassMethod(cls, "<init>", "()V");
    if (!cid) return NULL;

    JNIEnv *env = jnienv();
    jobject local = env->NewObject(cls, cid);
    jobject obj = env->NewGlobalRef(local);
    env->DeleteLocalRef(local);
    return obj;
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

