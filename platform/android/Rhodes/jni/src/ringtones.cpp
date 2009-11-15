#include "JNIRhodes.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Ringtone"

RHO_GLOBAL VALUE ringtone_manager_get_all()
{
    RHO_LOG_CALLBACK;
    jclass clsRingtoneManager = getJNIClass(RHODES_JAVA_CLASS_RINGTONE_MANAGER);
    if (!clsRingtoneManager) return Qnil;
    jclass clsMap = getJNIClass(RHODES_JAVA_CLASS_MAP);
    if (!clsMap) return Qnil;
    jclass clsSet = getJNIClass(RHODES_JAVA_CLASS_SET);
    if (!clsSet) return Qnil;
    jclass clsIterator = getJNIClass(RHODES_JAVA_CLASS_ITERATOR);
    if (!clsIterator) return Qnil;

    jmethodID midGetAllRingtones = getJNIClassStaticMethod(clsRingtoneManager, "getAllRingtones", "()Ljava/util/Map;");
    if (!midGetAllRingtones) return Qnil;
    jmethodID midGet = getJNIClassMethod(clsMap, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
    if (!midGet) return Qnil;
    jmethodID midKeySet = getJNIClassMethod(clsMap, "keySet", "()Ljava/util/Set;");
    if (!midKeySet) return Qnil;
    jmethodID midIterator = getJNIClassMethod(clsSet, "iterator", "()Ljava/util/Iterator;");
    if (!midIterator) return Qnil;
    jmethodID midHasNext = getJNIClassMethod(clsIterator, "hasNext", "()Z");
    if (!midHasNext) return Qnil;
    jmethodID midNext = getJNIClassMethod(clsIterator, "next", "()Ljava/lang/Object;");
    if (!midNext) return Qnil;

    JNIEnv *env = jnienv();
    jobject objMap = env->CallStaticObjectMethod(clsRingtoneManager, midGetAllRingtones);
    if (!objMap) return Qnil;
    jobject objSet = env->CallObjectMethod(objMap, midKeySet);
    if (!objSet) return Qnil;
    jobject objIterator = env->CallObjectMethod(objSet, midIterator);
    if (!objIterator) return Qnil;

    VALUE retval = createHash();
    while(env->CallObjectMethod(objIterator, midHasNext))
    {
        jstring objKey = (jstring)env->CallObjectMethod(objIterator, midNext);
        if (!objKey) return Qnil;
        jstring objValue = (jstring)env->CallObjectMethod(objMap, midGet, objKey);
        if (!objValue) return Qnil;

        const char *strKey = env->GetStringUTFChars(objKey, JNI_FALSE);
        const char *strValue = env->GetStringUTFChars(objValue, JNI_FALSE);
        addStrToHash(retval, strKey, strValue, strlen(strValue));
        env->ReleaseStringUTFChars(objKey, strKey);
        env->ReleaseStringUTFChars(objValue, strValue);

        env->DeleteLocalRef(objKey);
        env->DeleteLocalRef(objValue);
    }
    return retval;
}

RHO_GLOBAL void ringtone_manager_stop()
{
    RHO_LOG_CALLBACK;
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RINGTONE_MANAGER);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(cls, "stop", "()V");
    if (!mid) return;
    jnienv()->CallStaticVoidMethod(cls, mid);
}

RHO_GLOBAL void ringtone_manager_play(char* file_name)
{
    RHO_LOG_CALLBACK;
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RINGTONE_MANAGER);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(cls, "play", "(Ljava/lang/String;)V");
    if (!mid) return;
    JNIEnv *env = jnienv();
    env->CallStaticVoidMethod(cls, mid, env->NewStringUTF(file_name));
}

