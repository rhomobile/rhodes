#include "rhodes/JNIRhodes.h"
#include <android/log.h>

#include <common/RhoConf.h>
#include <logging/RhoLogConf.h>
#include <common/RhodesApp.h>
#include <sync/SyncThread.h>

#include <sys/stat.h>
#include <sys/resource.h>

#include "rhodes/JNIRhodes.h"

#include <common/rhoparams.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Bluetooth"


#include "rhodes/jni/com_rhomobile_rhodes_bluetooth_RhoBluetoothManager.h"

#include <common/RhodesApp.h>






RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_bluetooth_RhoBluetoothManager_onCallback
  (JNIEnv *env, jclass, jstring callback_url, jstring body) {

    rho_net_request_with_data(rho_http_normalizeurl(rho_cast<std::string>(callback_url).c_str()), rho_cast<std::string>(body).c_str());
}


RHO_GLOBAL int rho_bluetooth_is_bluetooth_available() {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "is_bluetooth_available", "()I");
    if (!mid) return 0;
    return env->CallStaticIntMethod(cls, mid);
}

RHO_GLOBAL void rho_bluetooth_off_bluetooth() {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "off_bluetooth", "()V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid);
}

RHO_GLOBAL void rho_bluetooth_set_device_name(const char* device_name) {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "set_device_name", "(Ljava/lang/String;)V");
    if (!mid) return;
    jstring objDeviceName = rho_cast<jstring>(device_name);
    env->CallStaticVoidMethod(cls, mid, objDeviceName);
    env->DeleteLocalRef(objDeviceName);
}

RHO_GLOBAL const char* rho_bluetooth_get_device_name() {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "get_device_name", "()Ljava/lang/String;");
    if (!mid) return 0;
    jstring name = (jstring)env->CallStaticObjectMethod(cls, mid);
    if (!name) return 0;
    const char* cname = rho_cast<std::string>(name).c_str(); 
    RAWLOG_INFO1("rho_bluetooth_get_device_name() : %s", cname);
    return cname;
}

RHO_GLOBAL const char* rho_bluetooth_get_last_error() {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "get_last_error", "()Ljava/lang/String;");
    if (!mid) return 0;
    jstring name = (jstring)env->CallStaticObjectMethod(cls, mid);
    return rho_cast<std::string>(name).c_str();
}

RHO_GLOBAL const char* rho_bluetooth_create_session(const char* role, const char* callback_url) {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "create_session", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
    if (!mid) return 0;
    jstring objStr1 = rho_cast<jstring>(role);
    jstring objStr2 = rho_cast<jstring>(callback_url);
    jstring res = (jstring)env->CallStaticObjectMethod(cls, mid, objStr1, objStr2);
    env->DeleteLocalRef(objStr1);
    env->DeleteLocalRef(objStr2);
    return rho_cast<std::string>(res).c_str();
}

RHO_GLOBAL void rho_bluetooth_session_set_callback(const char* connected_device_name, const char* callback_url) {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "session_set_callback", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!mid) return;
    jstring objStr1 = rho_cast<jstring>(connected_device_name);
    jstring objStr2 = rho_cast<jstring>(callback_url);
    env->CallStaticVoidMethod(cls, mid, objStr1, objStr2);
    env->DeleteLocalRef(objStr1);
    env->DeleteLocalRef(objStr2);
}

RHO_GLOBAL void rho_bluetooth_session_disconnect(const char* connected_device_name) {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "session_disconnect", "(Ljava/lang/String;)V");
    if (!mid) return;
    jstring objStr1 = rho_cast<jstring>(connected_device_name);
    env->CallStaticVoidMethod(cls, mid, objStr1);
    env->DeleteLocalRef(objStr1);
}

RHO_GLOBAL int rho_bluetooth_session_get_status(const char* connected_device_name) {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "session_get_status", "(Ljava/lang/String;)I");
    jstring objStr1 = rho_cast<jstring>(connected_device_name);
    if (!mid) return 0;
    int res = env->CallStaticIntMethod(cls, mid, objStr1);
    env->DeleteLocalRef(objStr1);
    return res;
}

RHO_GLOBAL const char* rho_bluetooth_session_read_string(const char* connected_device_name) {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "session_read_string", "(Ljava/lang/String;)Ljava/lang/String;");
    jstring objStr1 = rho_cast<jstring>(connected_device_name);
    if (!mid) return 0;
    jstring res = (jstring)env->CallStaticObjectMethod(cls, mid, objStr1);
    env->DeleteLocalRef(objStr1);
    return rho_cast<std::string>(res).c_str();
}

RHO_GLOBAL void rho_bluetooth_session_write_string(const char* connected_device_name, const char* str) {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "session_write_string", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!mid) return;
    jstring objStr1 = rho_cast<jstring>(connected_device_name);
    jstring objStr2 = rho_cast<jstring>(str);
    env->CallStaticVoidMethod(cls, mid, objStr1, objStr2);
    env->DeleteLocalRef(objStr1);
    env->DeleteLocalRef(objStr2);
}

RHO_GLOBAL VALUE rho_bluetooth_session_read_data(const char* connected_device_name) {
	return 0;
}

RHO_GLOBAL void rho_bluetooth_session_write_data(const char* connected_device_name, VALUE data) {
}

