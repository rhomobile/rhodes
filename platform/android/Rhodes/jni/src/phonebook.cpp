#include "JNIRhodes.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Phonebook"

RHO_GLOBAL void* openPhonebook()
{
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
        env->DeleteLocalRef(idObj);
        env->DeleteLocalRef(contactObj);
    }

    return hash;
}

RHO_GLOBAL void* openPhonebookRecord(void* pb, char* id)
{
    jobject obj = (jobject)pb;
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_PHONEBOOK);
    if (!cls) return NULL;
    jmethodID mid = getJNIClassMethod(cls, "getRecord", "(Ljava/lang/String;)Lcom/rhomobile/rhodes/phonebook/Contact;");
    if (!mid) return NULL;

    JNIEnv *env = jnienv();
    jstring objId = env->NewStringUTF(id);
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
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_PHONEBOOK);
    if (!cls) return Qnil;
    jmethodID mid = getJNIClassMethod(cls, name, "()Lcom/rhomobile/rhodes/phonebook/Contact;");
    if (!mid) return Qnil;

    JNIEnv *env = jnienv();
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
    jobject contactObj = (jobject)record;
    jclass contactCls = getJNIClass(RHODES_JAVA_CLASS_CONTACT);
    if (!contactCls) return 0;
    jmethodID mid = getJNIClassMethod(contactCls, "setField", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!mid) return 0;

    JNIEnv *env = jnienv();
    jstring objProperty = env->NewStringUTF(property);
    jstring objValue = env->NewStringUTF(value);
    env->CallVoidMethod(contactObj, mid, objProperty, objValue);
    env->DeleteLocalRef(objProperty);
    env->DeleteLocalRef(objValue);
    return 1;
}

static void doContactOp(jobject pbObj, jobject contactObj, const char *name)
{
    jclass pbCls = getJNIClass(RHODES_JAVA_CLASS_PHONEBOOK);
    if (!pbCls) return;
    jmethodID mid = getJNIClassMethod(pbCls, name, "(Lcom/rhomobile/rhodes/phonebook/Contact;)V");
    if (!mid) return;

    JNIEnv *env = jnienv();
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

