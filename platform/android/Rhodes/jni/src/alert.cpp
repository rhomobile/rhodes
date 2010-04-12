#include "JNIRhodes.h"

#include <common/rhoparams.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Alert"

RHO_GLOBAL void alert_show_popup(rho_param *p)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_ALERT);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "showPopup", "(Ljava/lang/Object;)V");
    if (!mid) return;

    if (p->type != RHO_PARAM_STRING && p->type != RHO_PARAM_HASH) {
        RAWLOG_ERROR("show_popup: wrong input parameter (expect String or Hash)");
        return;
    }

    jobject paramsObj = RhoValueConverter(env).createObject(p);
    env->CallStaticVoidMethod(cls, mid, paramsObj);
    env->DeleteLocalRef(paramsObj);
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

    jstring objFileName = rho_cast<jstring>(file_name);
    jstring objMediaType = media_type ? rho_cast<jstring>(media_type) : NULL;
    env->CallStaticVoidMethod(cls, mid, objFileName, objMediaType);
    env->DeleteLocalRef(objFileName);
    if (objMediaType) env->DeleteLocalRef(objMediaType);
}

