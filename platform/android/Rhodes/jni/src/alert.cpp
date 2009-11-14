#include "JNIRhodes.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Alert"

RHO_GLOBAL void alert_show_popup(char* message)
{
    RHO_LOG_CALLBACK;
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_ALERT);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(cls, "showPopup", "(Ljava/lang/String;)V");
    if (!mid) return;

    JNIEnv *env = jnienv();
    env->CallStaticVoidMethod(cls, mid, env->NewStringUTF(message));
}

RHO_GLOBAL void alert_vibrate(int duration)
{
    RHO_LOG_CALLBACK;
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_ALERT);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(cls, "vibrate", "(I)V");
    if (!mid) return;

    JNIEnv *env = jnienv();
    env->CallStaticVoidMethod(cls, mid, duration);
}

RHO_GLOBAL void alert_play_file(char* file_name, char *media_type)
{
    RHO_LOG_CALLBACK;
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_ALERT);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(cls, "playFile", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!mid) return;

    JNIEnv *env = jnienv();
    env->CallStaticVoidMethod(cls, mid, env->NewStringUTF(file_name),
        media_type ? env->NewStringUTF(media_type) : NULL);
}

