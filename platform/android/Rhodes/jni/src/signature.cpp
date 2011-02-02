#include "rhodes/JNIRhodes.h"

#include "rhodes/jni/com_rhomobile_rhodes_signature_Signature.h"

#include <common/RhodesApp.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Signature"

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_signature_Signature_callback
  (JNIEnv *env, jclass, jstring callback, jstring filePath, jstring error, jboolean cancelled)
{
    rho_rhodesapp_callSignatureCallback(rho_cast<std::string>(callback).c_str(),
        rho_cast<std::string>(filePath).c_str(), rho_cast<std::string>(error).c_str(), cancelled);
}


RHO_GLOBAL void rho_signature_take_signature(char* callback_url, char* image_format)
{
///*
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_SIGNATURE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "takeSignature", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!mid) return;
    jhstring objCallback = rho_cast<jhstring>(callback_url);
    jhstring objFormat = rho_cast<jhstring>(image_format);
    env->CallStaticVoidMethod(cls, mid, objCallback.get(), objFormat.get());
//*/
}

