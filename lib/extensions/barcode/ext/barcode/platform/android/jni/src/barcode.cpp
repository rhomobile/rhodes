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

