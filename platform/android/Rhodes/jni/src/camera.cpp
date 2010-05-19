#include "rhodes/JNIRhodes.h"

#include "rhodes/jni/com_rhomobile_rhodes_camera_Camera.h"

#include <common/RhodesApp.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Camera"

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_camera_Camera_callback
  (JNIEnv *env, jclass, jstring callback, jstring filePath, jstring error, jboolean cancelled)
{
    rho_rhodesapp_callCameraCallback(rho_cast<std::string>(callback).c_str(),
        rho_cast<std::string>(filePath).c_str(), rho_cast<std::string>(error).c_str(), cancelled);
}

RHO_GLOBAL void take_picture(char* callback_url)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_CAMERA);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "takePicture", "(Ljava/lang/String;)V");
    if (!mid) return;
    jstring objCallback = rho_cast<jstring>(callback_url);
    env->CallStaticVoidMethod(cls, mid, objCallback);
    env->DeleteLocalRef(objCallback);
}

RHO_GLOBAL void choose_picture(char* callback_url)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_CAMERA);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "choosePicture", "(Ljava/lang/String;)V");
    if (!mid) return;
    jstring objCallback = rho_cast<jstring>(callback_url);
    env->CallStaticVoidMethod(cls, mid, objCallback);
    env->DeleteLocalRef(objCallback);
}

