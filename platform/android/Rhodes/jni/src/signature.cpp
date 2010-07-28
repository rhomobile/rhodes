#include "rhodes/JNIRhodes.h"

//#include "rhodes/jni/com_rhomobile_rhodes_camera_Camera.h"

#include <common/RhodesApp.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Signature"


RHO_GLOBAL void rho_signature_take_signature(char* callback_url)
{
/*
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_CAMERA);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "takePicture", "(Ljava/lang/String;)V");
    if (!mid) return;
    jstring objCallback = rho_cast<jstring>(callback_url);
    env->CallStaticVoidMethod(cls, mid, objCallback);
    env->DeleteLocalRef(objCallback);
*/
}

