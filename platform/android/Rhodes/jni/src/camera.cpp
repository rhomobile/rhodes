#include "rhodes/JNIRhodes.h"

#include "rhodes/jni/com_rhomobile_rhodes_camera_Camera.h"

#include <common/RhodesApp.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Camera"

//RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_camera_Camera_callback
//  (JNIEnv *env, jclass, jstring callback, jstring filePath, jstring error, jboolean cancelled)
//{
//    rho_rhodesapp_callCameraCallback(rho_cast<std::string>(callback).c_str(),
//        rho_cast<std::string>(filePath).c_str(), rho_cast<std::string>(error).c_str(), cancelled);
//}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_camera_Camera_callback
  (JNIEnv *env, jclass, jstring callback_url, jstring body) {
    rho_net_request_with_data(rho_http_normalizeurl(rho_cast<std::string>(callback_url).c_str()), rho_cast<std::string>(body).c_str());
}




RHO_GLOBAL void take_picture(char* callback_url, rho_param *options_hash )
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_CAMERA);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "takePicture", "(Ljava/lang/String;Ljava/lang/Object;)V");
    if (!mid) return;

    if ((options_hash != NULL) && (options_hash->type != RHO_PARAM_HASH)) {
        RAWLOG_ERROR("Unexpected parameter type, should be Hash");
        return;
    }

    jobject paramsObj = NULL;
    if (options_hash != NULL) { 
         paramsObj = RhoValueConverter(env).createObject(options_hash);
    }
    
    env->CallStaticVoidMethod(cls, mid, rho_cast<jhstring>(callback_url).get(), paramsObj);

    if (options_hash != NULL) { 
         env->DeleteLocalRef(paramsObj);
    }

}

RHO_GLOBAL void choose_picture(char* callback_url)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_CAMERA);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "choosePicture", "(Ljava/lang/String;)V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid, rho_cast<jhstring>(callback_url).get());
}

RHO_GLOBAL VALUE get_camera_info(const char* camera_type) {
    
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_CAMERA);
    if (!cls) return rho_ruby_get_NIL();
    jmethodID mid_w = getJNIClassStaticMethod(env, cls, "getMaxCameraWidth", "(Ljava/lang/String;)I");
    jmethodID mid_h = getJNIClassStaticMethod(env, cls, "getMaxCameraHeight", "(Ljava/lang/String;)I");
    if ((!mid_w) || (!mid_h)) return rho_ruby_get_NIL();
    int w = env->CallStaticIntMethod(cls, mid_w, rho_cast<jhstring>(camera_type).get());
    int h = env->CallStaticIntMethod(cls, mid_h, rho_cast<jhstring>(camera_type).get());
    
    if ((w <= 0) || (h <= 0)) {
        return rho_ruby_get_NIL();
    }

    VALUE hash = rho_ruby_createHash();
    
    VALUE hash_max_resolution = rho_ruby_createHash();
    
    rho_ruby_add_to_hash(hash_max_resolution, rho_ruby_create_string("width"), rho_ruby_create_integer(w));
    rho_ruby_add_to_hash(hash_max_resolution, rho_ruby_create_string("height"), rho_ruby_create_integer(h));

    rho_ruby_add_to_hash(hash, rho_ruby_create_string("max_resolution"), hash_max_resolution);
    
    return hash;
}
