//#include "rhodes/JNIRhodes.h"
#include <rhodes.h>
#include <android/log.h>
#include <common/RhodesApp.h>
#include <logging/RhoLogConf.h>
#include <stdlib.h>


#define logging_enable false

static char barcode_result[16000];

extern "C" const char* rho_barcode_barcode_recognize(const char* filename) {
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/rhomobile/barcode/Barcode");
    if (!cls) return NULL;
    jmethodID mid = env->GetStaticMethodID( cls, "recognize", "(Ljava/lang/String;)Ljava/lang/String;");
    if (!mid) return NULL;

    jstring objFilename = env->NewStringUTF(filename);
    jstring res = (jstring)env->CallStaticObjectMethod(cls, mid, objFilename);
    env->DeleteLocalRef(objFilename);

    if (res == NULL) {
         return NULL;
    }

    const char* code = env->GetStringUTFChars(res,0);
    strcpy(barcode_result, code);
    env->ReleaseStringUTFChars(res, code);

    return barcode_result;
}

extern "C" void rho_barcode_take_barcode(const char* callback) {
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/rhomobile/barcode/Barcode");
    if (!cls) return;
    jmethodID mid = env->GetStaticMethodID( cls, "take", "(Ljava/lang/String;)V");
    if (!mid) return;

    jstring objCallback = env->NewStringUTF(callback);
    env->CallStaticObjectMethod(cls, mid, objCallback);
    env->DeleteLocalRef(objCallback);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_barcode_Barcode_callback
  (JNIEnv *env, jclass, jstring callback_url, jstring body) {
    rho_net_request_with_data(rho_http_normalizeurl(rho_cast<std::string>(callback_url).c_str()), rho_cast<std::string>(body).c_str());
}
