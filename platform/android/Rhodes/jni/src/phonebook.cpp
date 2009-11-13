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

RHO_GLOBAL VALUE getallPhonebookRecords(void* pb)
{
    RHO_LOG_CALLBACK;
    jobject phonebookObj = (jobject)pb;

    JNIEnv *env = jnienv();

    jclass phonebookCls = getJNIClass(RHODES_JAVA_CLASS_PHONEBOOK);
    if (!phonebookCls) return Qnil;
    jclass contactCls = getJNIClass(RHODES_JAVA_CLASS_CONTACT);
    if (!contactCls) return Qnil;
    jclass iteratorCls = env->FindClass("java/util/Iterator");
    if (!iteratorCls) return Qnil;
    jclass entryCls = env->FindClass("java/util/Map/Entry");
    if (!entryCls) return Qnil;

    jmethodID phonebookIteratorMID = getJNIClassMethod(phonebookCls, "iterator", "()Ljava/util/Iterator;");
    if (!phonebookIteratorMID) return Qnil;
    jmethodID contactIteratorMID = getJNIClassMethod(contactCls, "iterator", "()Ljava/util/Iterator;");
    if (!contactIteratorMID) return Qnil;
    jmethodID contactIdMID = getJNIClassMethod(contactCls, "id", "()Ljava/lang/String;");
    if (!contactIdMID) return Qnil;
    jmethodID hasNextMID = getJNIClassMethod(iteratorCls, "hasNext", "()Z");
    if (!hasNextMID) return Qnil;
    jmethodID nextMID = getJNIClassMethod(iteratorCls, "next", "()Ljava/lang/Object;");
    if (!nextMID) return Qnil;
    jmethodID getKeyMID = getJNIClassMethod(entryCls, "getKey", "()Ljava/lang/Object;");
    if (!getKeyMID) return Qnil;
    jmethodID getValueMID = getJNIClassMethod(entryCls, "getValue", "()Ljava/lang/Object;");
    if (!getValueMID) return Qnil;

    // Iterator iter = pb->iterator();
    jobject iterObj = env->CallObjectMethod(phonebookObj, phonebookIteratorMID);
    if (!iterObj) return Qnil;

    VALUE hash = createHash();
    // while(iter.hasNext())
    while(env->CallBooleanMethod(iterObj, hasNextMID))
    {
        VALUE contactHash = createHash();

        // Contact contact = (Contact)iter.next();
        jobject contactObj = env->CallObjectMethod(iterObj, nextMID);
        if (!contactObj) return Qnil;
        // String id = contact.id();
        jstring idObj = (jstring)env->CallObjectMethod(contactObj, contactIdMID);
        if (!idObj) return Qnil;
        const char *idStr = env->GetStringUTFChars(idObj, JNI_FALSE);

        // Iterator iter2 = contact.iterator();
        jobject iter2Obj = env->CallObjectMethod(contactObj, contactIteratorMID);
        if (!iter2Obj) return Qnil;

        // while(iter2.hasNext())
        while(env->CallBooleanMethod(iter2Obj, hasNextMID))
        {
            // Entry<String, String> entry = (Entry<String, String>)iter2.next();
            jobject entryObj = env->CallObjectMethod(iter2Obj, nextMID);
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

        addHashToHash(hash, idStr, contactHash);
        env->ReleaseStringUTFChars(idObj, idStr);
    }
    return hash;
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

