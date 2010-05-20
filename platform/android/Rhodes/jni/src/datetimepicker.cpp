#include "rhodes/JNIRhodes.h"

#include "rhodes/jni/com_rhomobile_rhodes_datetime_DateTimePicker.h"

#include <common/RhodesApp.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "DateTimePicker"

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_datetime_DateTimePicker_callback
  (JNIEnv *env, jclass, jstring callback, jlong result, jbyteArray opaqueObj, jboolean cancelled)
{
    jbyte *opaqueStr = env->GetByteArrayElements(opaqueObj, JNI_FALSE);
    rho_rhodesapp_callDateTimeCallback(rho_cast<std::string>(callback).c_str(), result, (const char *)opaqueStr, cancelled);
    env->ReleaseByteArrayElements(opaqueObj, opaqueStr, 0);
}

RHO_GLOBAL void choose_datetime(char* callback, char* title, long initial_time, int format, char* data)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_DATE_TIME_PICKER);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "choose", "(Ljava/lang/String;Ljava/lang/String;JI[B)V");
    if (!mid) return;

    jsize data_size = strlen(data);
    jbyteArray opaqueObj = env->NewByteArray(data_size);
    if (!opaqueObj) return;
    env->SetByteArrayRegion(opaqueObj, 0, data_size, (const jbyte *)data);
    jstring objCallback = rho_cast<jstring>(callback);
    jstring objTitle = rho_cast<jstring>(title);
    env->CallStaticVoidMethod(cls, mid, objCallback, objTitle,
        (jlong)initial_time, format, opaqueObj);
    env->DeleteLocalRef(objCallback);
    env->DeleteLocalRef(objTitle);
    env->DeleteLocalRef(opaqueObj);
}

