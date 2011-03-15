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

RHO_GLOBAL void choose_datetime_with_range(char* callback, char* title, long initial_time, int format, char* data, long min_time, long max_time)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_DATE_TIME_PICKER);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "choose", "(Ljava/lang/String;Ljava/lang/String;JI[BJJ)V");
    if (!mid) return;

    jsize data_size = strlen(data);
    jholder<jbyteArray> opaqueObj = jholder<jbyteArray>(env->NewByteArray(data_size));
    if (!opaqueObj) return;
    env->SetByteArrayRegion(opaqueObj.get(), 0, data_size, (const jbyte *)data);
    jhstring objCallback = rho_cast<jhstring>(callback);
    jhstring objTitle = rho_cast<jhstring>(title);
    env->CallStaticVoidMethod(cls, mid, objCallback.get(), objTitle.get(),
        (jlong)initial_time, format, opaqueObj.get(), (jlong)min_time, (jlong)max_time);
}

RHO_GLOBAL void choose_datetime(char* callback, char* title, long initial_time, int format, char* data)
{
    choose_datetime_with_range(callback, title, initial_time, format, data, 0, 0);
}

RHO_GLOBAL void set_change_value_callback_datetime(char* callback) {
}
