#include "JNIRhodes.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Alert"

RHO_GLOBAL void alert_show_popup(char* message)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_ALERT);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "showPopup", "(Ljava/lang/String;)V");
    if (!mid) return;

    jstring msgObj = env->NewStringUTF(message);
    env->CallStaticVoidMethod(cls, mid, msgObj);
    env->DeleteLocalRef(msgObj);
}

RHO_GLOBAL void alert_vibrate(void *arg)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_ALERT);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "vibrate", "(I)V");
    if (!mid) return;

    jint duration = 2500;
    if (arg)
        duration = (jint)arg;
    env->CallStaticVoidMethod(cls, mid, duration);
}

RHO_GLOBAL void alert_play_file(char* file_name, char *media_type)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_ALERT);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "playFile", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!mid) return;

    jstring objFileName = env->NewStringUTF(file_name);
    jstring objMediaType = media_type ? env->NewStringUTF(media_type) : NULL;
    env->CallStaticVoidMethod(cls, mid, objFileName, objMediaType);
    env->DeleteLocalRef(objFileName);
    if (objMediaType) env->DeleteLocalRef(objMediaType);
}

