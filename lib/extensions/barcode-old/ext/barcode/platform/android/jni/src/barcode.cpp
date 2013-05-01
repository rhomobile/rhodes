//#include "rhodes/JNIRhodes.h"
#include <rhodes.h>
#include <android/log.h>
#include <common/RhodesApp.h>
#include <logging/RhoLog.h>
#include <stdlib.h>

#include "ruby/ext/rho/rhoruby.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "BarcodeJNI"

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
        RAWLOG_ERROR("Failed to recognize.");
        return NULL;
    }

    if(env->GetStringUTFLength(res) < (sizeof(barcode_result)/sizeof(barcode_result[0])))
    {
        const char* code = env->GetStringUTFChars(res,0);
        strcpy(barcode_result, code);
        env->ReleaseStringUTFChars(res, code);
    } else
    {
        RAWLOG_ERROR("Recognized data is too large.");
        return NULL;
    }

    return barcode_result;
}

extern "C" void rho_barcode_take_barcode(const char* callback, VALUE options) {
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/rhomobile/barcode/Barcode");
    if (!cls) return;
    jmethodID mid = env->GetStaticMethodID( cls, "take", "(Ljava/lang/String;I)V");
    if (!mid) return;
    
    int camera_index = 0;
    if (!rho_ruby_is_NIL(options)) {
        // parse options for camera index ( 0 - main, 1 - front)
        rho_param *param = rho_param_fromvalue(options);
        if (param->type == RHO_PARAM_HASH) {
            int i;
            for (i = 0; i < param->v.hash->size; i++) {
                if (strcasecmp(param->v.hash->name[i], "camera") == 0) {
                    if (param->v.hash->value[i]->type == RHO_PARAM_STRING) {
                        if (strcasecmp(param->v.hash->value[i]->v.string, "front") == 0) {
                            camera_index = 1;
                        }
                    }
                }
            }
        }
        rho_param_free(param);
    }

    jstring objCallback = env->NewStringUTF(callback);
    if(objCallback != NULL)
    {
        env->CallStaticObjectMethod(cls, mid, objCallback, camera_index);
        env->DeleteLocalRef(objCallback);
    } else
    {
        RAWLOG_ERROR1("Cannot take barcode, no memory: %d", strlen(callback));
    }
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_barcode_Barcode_callback
  (JNIEnv *env, jclass, jstring callback_url, jstring body) {
    char* url = rho_http_normalizeurl(rho_cast<std::string>(env, callback_url).c_str());
    rho_net_request_with_data(url, rho_cast<std::string>(env, body).c_str());
    rho_http_free(url);
}



class RhoScannersListContainer : public rho::ICallbackObject
{
public:
	RhoScannersListContainer() {
	}
    
	virtual ~RhoScannersListContainer() {
	}
    
	virtual unsigned long getObjectValue() {
        VALUE b_array = rho_ruby_create_array();
        VALUE b_hash = rho_ruby_createHash();
        rho_ruby_add_to_hash(b_hash, rho_ruby_create_string("friendlyName"), rho_ruby_create_string("RhoBarcode based on ZXing"));
        rho_ruby_add_to_hash(b_hash, rho_ruby_create_string("deviceName"), rho_ruby_create_string("RhoBarcode"));
        rho_ruby_add_to_array(b_array, b_hash);
        return b_array;
	}
};





RHO_GLOBAL void JNICALL Java_com_rhomobile_barcode_Barcode_enumeratecallback
(JNIEnv *env, jclass, jstring callback_url) {
    char* url = rho_http_normalizeurl(rho_cast<std::string>(env, callback_url).c_str());
    
    char body[2048];
    
	strcpy(body, "&status=ok&rho_callback=1&");
	strcat(body, (RHODESAPP().addCallbackObject( new RhoScannersListContainer(), "scannerArray")).c_str());
    char* norm_url = rho_http_normalizeurl(url);
    
    rho_net_request_with_data(norm_url, body);
    rho_http_free(norm_url);    
    
    
    
}

extern "C" void rho_motobarcode_enumerate(const char* callback) {
    
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/rhomobile/barcode/Barcode");
    if (!cls) return;
    jmethodID mid = env->GetStaticMethodID( cls, "callMotoEnumerateCallback", "(Ljava/lang/String;)V");
    if (!mid) return;
    
    jstring objCallback = env->NewStringUTF(callback);
    if(objCallback != NULL)
    {
        env->CallStaticObjectMethod(cls, mid, objCallback);
        env->DeleteLocalRef(objCallback);
    }

    //VALUE b_array = rho_ruby_create_array();
    //rho_ruby_add_to_array(b_array, rho_ruby_create_string("RhoBarcode based on ZXing"));
	//return b_array;
   
}

extern "C" void  rho_motobarcode_enable(const char* callback, rho_param* p) {
    __android_log_write(ANDROID_LOG_INFO, "RhoBarcode", "Barcode::enable() DO NOT SUPPORT BY RHODES SW SCANNER !");    
}

extern "C" void  rho_motobarcode_disable() {
    __android_log_write(ANDROID_LOG_INFO, "RhoBarcode", "Barcode::disable() DO NOT SUPPORT BY RHODES SW SCANNER !");    
}

extern "C" void  rho_motobarcode_start() {
    __android_log_write(ANDROID_LOG_INFO, "RhoBarcode", "Barcode::start() DO NOT SUPPORT BY RHODES SW SCANNER !");    
}

extern "C" void  rho_motobarcode_stop() {
    __android_log_write(ANDROID_LOG_INFO, "RhoBarcode", "Barcode::stop() DO NOT SUPPORT BY RHODES SW SCANNER !");    
}

