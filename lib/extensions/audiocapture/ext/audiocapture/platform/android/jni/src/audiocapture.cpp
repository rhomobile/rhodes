#include <rhodes.h>
#include "rubyext/WebView.h"
#include <stdlib.h>
#include "ruby/ext/rho/rhoruby.h"

#include <android/log.h>

#include "RhodesApp.h"

#include <string.h>



#define logging_enable true

static void logi(const char* text) {
	if (logging_enable) __android_log_write(ANDROID_LOG_INFO, "AudioCapture jni", text);
}

static void loge(const char* text) {
	__android_log_write(ANDROID_LOG_INFO, "AudioCapture jni ERROR : ", text);
}



extern "C" VALUE audiocapture_get_property(const char* property_name) {
    logi("audiocapture_get_property() START");
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/audiocapture/Audiocapture");
    if (!cls) {
        loge("audiocapture_get_property() class not found");
        return rho_ruby_get_NIL();
    }
    jmethodID midGetType = env->GetStaticMethodID( cls, "getPropertyType", "(Ljava/lang/String;)I");
    if (!midGetType) {
        loge("audiocapture_get_property() method get type not found");
        return rho_ruby_get_NIL();
    }
    jmethodID midGetStringValue = env->GetStaticMethodID( cls, "getStringProperty", "(Ljava/lang/String;)Ljava/lang/String;");
    if (!midGetStringValue) {
        loge("audiocapture_get_property() method get string not found");
        return rho_ruby_get_NIL();
    }
    jmethodID midGetIntValue = env->GetStaticMethodID( cls, "getIntProperty", "(Ljava/lang/String;)I");
    if (!midGetIntValue) {
        loge("audiocapture_get_property() method get int not found");
        return rho_ruby_get_NIL();
    }
    
    
    
    // 1 - string
	// 2 - bool
	// 3 - int
    
    jstring objStr = env->NewStringUTF(property_name);
    int type = env->CallStaticIntMethod(cls, midGetType, objStr);
    env->DeleteLocalRef(objStr);
    
    if (type == 1) {
        jstring objStr = env->NewStringUTF(property_name);
        jstring jstr = (jstring)env->CallStaticObjectMethod(cls, midGetStringValue, objStr);
        env->DeleteLocalRef(objStr);
        const char* buf = env->GetStringUTFChars(jstr,0);
        VALUE result = rho_ruby_create_string(buf);
        env->ReleaseStringUTFChars(jstr, buf);
        logi("audiocapture_get_property() FINISH1");
        return result;
    }
    if (type == 2) {
        jstring objStr = env->NewStringUTF(property_name);
        int value = env->CallStaticIntMethod(cls, midGetIntValue, objStr);
        env->DeleteLocalRef(objStr);
        VALUE result = rho_ruby_create_boolean(value);
        logi("audiocapture_get_property() FINISH1");
        return result;
    }
    if (type == 3) {
        jstring objStr = env->NewStringUTF(property_name);
        int value = env->CallStaticIntMethod(cls, midGetIntValue, objStr);
        env->DeleteLocalRef(objStr);
        VALUE result = rho_ruby_create_integer(value);
        logi("audiocapture_get_property() FINISH3");
        return result;
    }
    logi("audiocapture_get_property() FINISH");
    return rho_ruby_get_NIL();
}

extern "C" void audiocapture_set_property(const char* property_name, const char* value) {
    logi("audiocapture_set_property() START");
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/audiocapture/Audiocapture");
    if (!cls) {
        loge("audiocapture_set_property() class not found");
        return;
    }
    jmethodID mid = env->GetStaticMethodID( cls, "setProperty", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!mid) {
        loge("audiocapture_set_property() method not found");
        return;
    }
    jstring objStr1 = env->NewStringUTF(property_name);
    if (!objStr1) {
        loge("audiocapture_set_property() can not make string1");
        return;
    }
    jstring objStr2 = env->NewStringUTF(value);
    if (!objStr2) {
        loge("audiocapture_set_property() can not make string2");
        return;
    }
    
    env->CallStaticVoidMethod(cls, mid, objStr1, objStr2);
    
    env->DeleteLocalRef(objStr1);
    env->DeleteLocalRef(objStr2);
    logi("audiocapture_set_property() FINISH");
}

extern "C" void audiocapture_start() {
    logi("audiocapture_start() START");
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/audiocapture/Audiocapture");
    if (!cls) {
        loge("audiocapture_start() class not found");
        return;
    }
    jmethodID mid = env->GetStaticMethodID( cls, "start", "()V");
    if (!mid) {
        loge("rawsensors_start() method not found");
        return;
    }
    
    env->CallStaticVoidMethod(cls, mid);
    logi("audiocapture_start() FINISH");
}

extern "C" void audiocapture_stop() {
    logi("audiocapture_stop() START");
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/audiocapture/Audiocapture");
    if (!cls) {
        loge("audiocapture_stopt() class not found");
        return;
    }
    jmethodID mid = env->GetStaticMethodID( cls, "stop", "()V");
    if (!mid) {
        loge("rawsensors_stop() method not found");
        return;
    }
    
    env->CallStaticVoidMethod(cls, mid);
    logi("audiocapture_stop() FINISH");
}

extern "C" void audiocapture_cancel() {
    logi("audiocapture_cancel() START");
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/audiocapture/Audiocapture");
    if (!cls) {
        loge("audiocapture_cancel() class not found");
        return;
    }
    jmethodID mid = env->GetStaticMethodID( cls, "cancel", "()V");
    if (!mid) {
        loge("rawsensors_cancel() method not found");
        return;
    }
    
    env->CallStaticVoidMethod(cls, mid);
    logi("audiocapture_cancel() FINISH");
}



extern "C" void JNICALL Java_com_audiocapture_Audiocapture_fireRhoCallback
(JNIEnv *env, jclass, jstring js_callback_url, jstring js_body)
{
    logi("Java_com_audiocapture_Audiocapture_fireRhoCallback() START");
    std::string url = rho_cast<std::string>(env, js_callback_url);
    std::string body = rho_cast<std::string>(env, js_body);
    
    logi(url.c_str());
    logi(body.c_str());
    rho_net_request_with_data(url.c_str(), body.c_str());
    logi("Java_com_audiocapture_Audiocapture_fireRhoCallback() FINISH");
}

