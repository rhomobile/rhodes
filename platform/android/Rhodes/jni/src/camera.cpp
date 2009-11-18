#include "JNIRhodes.h"

#include <com_rhomobile_rhodes_camera_Camera.h>

#include <common/RhodesApp.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Camera"

JNIEXPORT void JNICALL Java_com_rhomobile_rhodes_camera_Camera_callback
  (JNIEnv *env, jclass, jstring callbackObj, jstring filePathObj, jstring errorObj, jboolean cancelled)
{
    const char *callbackStr = env->GetStringUTFChars(callbackObj, JNI_FALSE);
    const char *filePathStr = env->GetStringUTFChars(filePathObj, JNI_FALSE);
    const char *errorStr = env->GetStringUTFChars(errorObj, JNI_FALSE);
    rho_rhodesapp_callCameraCallback(callbackStr, filePathStr, errorStr, cancelled);
    env->ReleaseStringUTFChars(callbackObj, callbackStr);
    env->ReleaseStringUTFChars(filePathObj, filePathStr);
    env->ReleaseStringUTFChars(errorObj, errorStr);
}

RHO_GLOBAL void take_picture(char* callback_url)
{
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_CAMERA);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(cls, "takePicture", "(Ljava/lang/String;)V");
    if (!mid) return;
    JNIEnv *env = jnienv();
    jstring objCallback = env->NewStringUTF(callback_url);
    env->CallStaticVoidMethod(cls, mid, objCallback);
    env->DeleteLocalRef(objCallback);
}

RHO_GLOBAL void choose_picture(char* callback_url)
{
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_CAMERA);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(cls, "choosePicture", "(Ljava/lang/String;)V");
    if (!mid) return;
    JNIEnv *env = jnienv();
    jstring objCallback = env->NewStringUTF(callback_url);
    env->CallStaticVoidMethod(cls, mid, objCallback);
    env->DeleteLocalRef(objCallback);
}

