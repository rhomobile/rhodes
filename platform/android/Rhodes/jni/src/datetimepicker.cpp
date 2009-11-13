#include "JNIRhodes.h"

#include <com_rhomobile_rhodes_datetime_DateTimePicker.h>

#include <common/RhodesApp.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "DateTimePicker"

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_datetime_DateTimePicker_callback
  (JNIEnv *env, jclass, jstring callbackObj, jlong result, jstring opaqueObj, jboolean cancelled)
{
    RHO_LOG_JNI_CALL;
    const char *callbackStr = env->GetStringUTFChars(callbackObj, JNI_FALSE);
    const char *opaqueStr = env->GetStringUTFChars(opaqueObj, JNI_FALSE);
    rho_rhodesapp_callDateTimeCallback(callbackStr, result, opaqueStr, cancelled);
    env->ReleaseStringUTFChars(callbackObj, callbackStr);
    env->ReleaseStringUTFChars(opaqueObj, opaqueStr);
}

RHO_GLOBAL void choose_datetime(char* callback, char* title, long initial_time, int format, char* data)
{
    RHO_LOG_CALLBACK;
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_DATE_TIME_PICKER);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(cls, "choose", "(Ljava/lang/String;Ljava/lang/String;JILjava/lang/String;)V");
    if (!mid) return;

    JNIEnv *env = jnienv();
    env->CallStaticVoidMethod(cls, mid, env->NewStringUTF(callback), env->NewStringUTF(title),
        (jlong)initial_time, format, env->NewStringUTF(data));
}

