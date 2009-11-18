#include "JNIRhodes.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Ringtone"

RHO_GLOBAL VALUE ringtone_manager_get_all()
{
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RINGTONE_MANAGER);
    if (!cls) return Qnil;

    jmethodID mid = getJNIClassStaticMethod(cls, "getAllRingtones", "()Ljava/util/Map;");
    if (!mid) return Qnil;

    JNIEnv *env = jnienv();
    jobject obj = env->CallStaticObjectMethod(cls, mid);
    if (!obj) return Qnil;
    return convertJavaMapToRubyHash(obj);
}

RHO_GLOBAL void ringtone_manager_stop()
{
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RINGTONE_MANAGER);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(cls, "stop", "()V");
    if (!mid) return;
    jnienv()->CallStaticVoidMethod(cls, mid);
}

RHO_GLOBAL void ringtone_manager_play(char* file_name)
{
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RINGTONE_MANAGER);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(cls, "play", "(Ljava/lang/String;)V");
    if (!mid) return;
    JNIEnv *env = jnienv();
    jstring objFileName = env->NewStringUTF(file_name);
    env->CallStaticVoidMethod(cls, mid, objFileName);
    env->DeleteLocalRef(objFileName);
}

