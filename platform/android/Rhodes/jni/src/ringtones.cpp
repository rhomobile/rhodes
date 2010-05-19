#include "rhodes/JNIRhodes.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Ringtone"

RHO_GLOBAL VALUE rho_ringtone_manager_get_all()
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RINGTONE_MANAGER);
    if (!cls) return Qnil;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "getAllRingtones", "()Ljava/util/Map;");
    if (!mid) return Qnil;

    jobject obj = env->CallStaticObjectMethod(cls, mid);
    if (!obj) return Qnil;
    return convertJavaMapToRubyHash(obj);
}

RHO_GLOBAL void rho_ringtone_manager_stop()
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RINGTONE_MANAGER);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "stop", "()V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid);
}

RHO_GLOBAL void rho_ringtone_manager_play(char* file_name)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RINGTONE_MANAGER);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "play", "(Ljava/lang/String;)V");
    if (!mid) return;
    jstring objFileName = rho_cast<jstring>(file_name);
    env->CallStaticVoidMethod(cls, mid, objFileName);
    env->DeleteLocalRef(objFileName);
}

