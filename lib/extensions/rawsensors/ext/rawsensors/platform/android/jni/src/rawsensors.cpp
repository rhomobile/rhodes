
#include <rhodes.h>
#include <android/log.h>

#include "rubyext/WebView.h"

#include "RhodesApp.h"

#include "ruby/ext/rho/rhoruby.h"
#include <string.h>
#include <stdlib.h>

#define logging_enable false

static void logi(const char* text) {
	if (logging_enable) __android_log_write(ANDROID_LOG_INFO, "RawSensors jni", text);
}

static void loge(const char* text) {
	__android_log_write(ANDROID_LOG_INFO, "RawSensors jni ERROR : ", text);
}


extern "C" VALUE rawsensors_get_property(const char* property_name) {
    logi("rawsensors_get_property() START");
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/rawsensors/Rawsensors");
    if (!cls) {
        loge("rawsensors_get_property() class not found");
        return rho_ruby_get_NIL();
    }
    jmethodID midGetType = env->GetStaticMethodID( cls, "getPropertyType", "(Ljava/lang/String;)I");
    if (!midGetType) {
        loge("rawsensors_get_property() method get type not found");
        return rho_ruby_get_NIL();
    }
    jmethodID midGetStringValue = env->GetStaticMethodID( cls, "getStringProperty", "(Ljava/lang/String;)Ljava/lang/String;");
    if (!midGetStringValue) {
        loge("rawsensors_get_property() method get string not found");
        return rho_ruby_get_NIL();
    }
    jmethodID midGetIntValue = env->GetStaticMethodID( cls, "getIntProperty", "(Ljava/lang/String;)I");
    if (!midGetIntValue) {
        loge("rawsensors_get_property() method get int not found");
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
        logi("rawsensors_get_property() FINISH1");
        return result;
    }
    if (type == 2) {
        jstring objStr = env->NewStringUTF(property_name);
        int value = env->CallStaticIntMethod(cls, midGetIntValue, objStr);
        env->DeleteLocalRef(objStr);
        VALUE result = rho_ruby_create_boolean(value);
        logi("rawsensors_get_property() FINISH1");
        return result;
    }
    if (type == 3) {
        jstring objStr = env->NewStringUTF(property_name);
        int value = env->CallStaticIntMethod(cls, midGetIntValue, objStr);
        env->DeleteLocalRef(objStr);
        VALUE result = rho_ruby_create_integer(value);
        logi("rawsensors_get_property() FINISH3");
        return result;
    }
    logi("rawsensors_get_property() FINISH");
    return rho_ruby_get_NIL();
}

extern "C" void rawsensors_set_property(const char* property_name, const char* value) {
    ///*
    logi("rawsensors_set_property() START");
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/rawsensors/Rawsensors");
    if (!cls) {
        loge("rawsensors_set_property() class not found");
        return;
    }
    jmethodID mid = env->GetStaticMethodID( cls, "setProperty", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!mid) {
        loge("rawsensors_set_property() method not found");
        return;
    }
    jstring objStr1 = env->NewStringUTF(property_name);
    if (!objStr1) {
        loge("rawsensors_set_property() can not make string1");
        return;
    }
    jstring objStr2 = env->NewStringUTF(value);
    if (!objStr2) {
        loge("rawsensors_set_property() can not make string2");
        return;
    }
    
    env->CallStaticVoidMethod(cls, mid, objStr1, objStr2);
    
    env->DeleteLocalRef(objStr1);
    env->DeleteLocalRef(objStr2);
    logi("rawsensors_set_property() FINISH");
    // */
}

extern "C" VALUE rawsensors_is_available(const char* sensor_name) {
    ///*
    logi("rawsensors_is_available() START");
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/rawsensors/Rawsensors");
    if (!cls) {
        loge("rawsensors_is_available() class not found");
        return rho_ruby_get_NIL();
    }
    jmethodID midGetIntValue = env->GetStaticMethodID( cls, "isAvailable", "(Ljava/lang/String;)I");
    if (!midGetIntValue) {
        loge("rawsensors_is_available() method not found");
        return rho_ruby_get_NIL();
    }
    jstring objStr = env->NewStringUTF(sensor_name);
    if (!objStr) {
        loge("rawsensors_is_available() can not make string");
        return rho_ruby_get_NIL();
    }
    int value = env->CallStaticIntMethod(cls, midGetIntValue, objStr);
    env->DeleteLocalRef(objStr);
    VALUE result = rho_ruby_create_boolean(value);
    logi("rawsensors_is_available() FINISH");
    return result;
    // */
}

extern "C" void rawsensors_getSensorData() {
    logi("rawsensors_getSensorData() START");
    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, "com/rawsensors/Rawsensors");
    if (!cls) {
        loge("rawsensors_getSensorData() class not found");
        return;
    }
    jmethodID mid = env->GetStaticMethodID( cls, "getSensorData", "()V");
    if (!mid) {
        loge("rawsensors_getSensorData() method not found");
        return;
    }
    
    env->CallStaticVoidMethod(cls, mid);
    logi("rawsensors_getSensorData() FINISH");
}

extern "C" void JNICALL Java_com_rawsensors_Rawsensors_fireRhoCallback
(JNIEnv *env, jclass, jstring js_callback_url, jstring js_body)
{
    logi("Java_com_rawsensors_Rawsensors_fireRhoCallback() START");
    std::string url = rho_cast<std::string>(env, js_callback_url);
    std::string body = rho_cast<std::string>(env, js_body);
   
    logi(url.c_str());
    logi(body.c_str());
    rho_net_request_with_data(url.c_str(), body.c_str());
    logi("Java_com_rawsensors_Rawsensors_fireRhoCallback() FINISH");
}



