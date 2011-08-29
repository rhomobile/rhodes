/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

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
#define DEFAULT_LOGCATEGORY "BluetoothJNI"

#include "rhodes/jni/com_rhomobile_rhodes_bluetooth_RhoBluetoothManager.h"

#include <common/RhodesApp.h>
#include "ruby/ext/rho/rhoruby.h"

static const char* const BTC_OK = "OK";
static const char* const BTC_CANCEL = "CANCEL";
static const char* const BTC_ERROR = "ERROR";

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_bluetooth_RhoBluetoothManager_onCallback
  (JNIEnv *env, jclass, jstring callback_url, jstring body)
{
    rho_net_request_with_data(
            RHODESAPP().canonicalizeRhoUrl(rho_cast<std::string>(env, callback_url)).c_str(),
            rho_cast<std::string>(env, body).c_str());
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
    jhstring objDeviceName = rho_cast<jhstring>(env, device_name);
    env->CallStaticVoidMethod(cls, mid, objDeviceName.get());
}

RHO_GLOBAL VALUE rho_bluetooth_get_device_name() {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return rho_ruby_get_NIL();
    jmethodID mid = getJNIClassStaticMethod(env, cls, "get_device_name", "()Ljava/lang/String;");
    if (!mid) return rho_ruby_get_NIL();
    jstring jname = static_cast<jstring>(env->CallStaticObjectMethod(cls, mid));
    if (!jname) return rho_ruby_get_NIL();
    std::string name = rho_cast<std::string>(env, jname); 
    RAWLOG_INFO1("rho_bluetooth_get_device_name() : %s", name.c_str());
    return rho_ruby_create_string(name.c_str());
}

RHO_GLOBAL const char* rho_bluetooth_get_last_error() {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return "";
    jmethodID mid = getJNIClassStaticMethod(env, cls, "get_last_error", "()I");
    if (!mid) return "";
    jint res = env->CallStaticIntMethod(cls, mid);
    switch(res)
    {
    case com_rhomobile_rhodes_bluetooth_RhoBluetoothManager_BTC_OK:
        return BTC_OK;
    case com_rhomobile_rhodes_bluetooth_RhoBluetoothManager_BTC_CANCEL:
        return BTC_CANCEL;
    case com_rhomobile_rhodes_bluetooth_RhoBluetoothManager_BTC_ERROR:
        return BTC_ERROR;
    }
    //unreachable point
    return "";
}

RHO_GLOBAL const char* rho_bluetooth_create_session(const char* role, const char* callback_url) {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return BTC_ERROR;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "create_session", "(Ljava/lang/String;Ljava/lang/String;)I");
    if (!mid) return BTC_ERROR;
    jhstring objStr1 = rho_cast<jhstring>(env, role);
    jhstring objStr2 = rho_cast<jhstring>(env, callback_url);
    jint res = env->CallStaticIntMethod(cls, mid, objStr1.get(), objStr2.get());
    switch(res)
    {
    case com_rhomobile_rhodes_bluetooth_RhoBluetoothManager_BTC_OK:
        return BTC_OK;
    case com_rhomobile_rhodes_bluetooth_RhoBluetoothManager_BTC_CANCEL:
        return BTC_CANCEL;
    case com_rhomobile_rhodes_bluetooth_RhoBluetoothManager_BTC_ERROR:
        return BTC_ERROR;
    }
    //unreachable point
    return "";
}


RHO_GLOBAL const char* rho_bluetooth_create_custom_server_session(const char* client_name, const char* callback_url, int accept_any_device) {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "create_custom_server_session", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!mid) return 0;
    jhstring objStr1 = rho_cast<jhstring>(env, client_name);
    jhstring objStr2 = rho_cast<jhstring>(env, callback_url);
    env->CallStaticObjectMethod(cls, mid, objStr1.get(), objStr2.get());
    return "OK";
}

RHO_GLOBAL const char* rho_bluetooth_create_custom_client_session(const char* server_name, const char* callback_url) {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "create_custom_client_session", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!mid) return 0;
    jhstring objStr1 = rho_cast<jhstring>(env, server_name);
    jhstring objStr2 = rho_cast<jhstring>(env, callback_url);
    env->CallStaticObjectMethod(cls, mid, objStr1.get(), objStr2.get());
    return "OK";
}

RHO_GLOBAL const char* rho_bluetooth_stop_current_connection_process() {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "stop_current_connection_process", "()V");
    if (!mid) return 0;
    env->CallStaticObjectMethod(cls, mid);
    return "OK";
}



RHO_GLOBAL void rho_bluetooth_session_set_callback(const char* connected_device_name, const char* callback_url) {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "session_set_callback", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!mid) return;
    jhstring objStr1 = rho_cast<jhstring>(env, connected_device_name);
    jhstring objStr2 = rho_cast<jhstring>(env, callback_url);
    env->CallStaticVoidMethod(cls, mid, objStr1.get(), objStr2.get());
}

RHO_GLOBAL void rho_bluetooth_session_disconnect(const char* connected_device_name) {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "session_disconnect", "(Ljava/lang/String;)V");
    if (!mid) return;
    jhstring objStr1 = rho_cast<jhstring>(env, connected_device_name);
    env->CallStaticVoidMethod(cls, mid, objStr1.get());
}

RHO_GLOBAL int rho_bluetooth_session_get_status(const char* connected_device_name) {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "session_get_status", "(Ljava/lang/String;)I");
    if (!mid) return 0;
    jhstring objStr1 = rho_cast<jhstring>(env, connected_device_name);
    return env->CallStaticIntMethod(cls, mid, objStr1.get());
}

RHO_GLOBAL VALUE rho_bluetooth_session_read_string(const char* connected_device_name) {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "session_read_string", "(Ljava/lang/String;)Ljava/lang/String;");
    if (!mid) return 0;
    jhstring objStr1 = rho_cast<jhstring>(env, connected_device_name);
    jstring res = (jstring)env->CallStaticObjectMethod(cls, mid, objStr1.get());
    std::string msg = rho_cast<std::string>(env, res);
    return rho_ruby_create_string(msg.c_str());
}

RHO_GLOBAL void rho_bluetooth_session_write_string(const char* connected_device_name, const char* str) {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "session_write_string", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (!mid) return;
    jhstring objStr1 = rho_cast<jhstring>(env, connected_device_name);
    jhstring objStr2 = rho_cast<jhstring>(env, str);
    env->CallStaticVoidMethod(cls, mid, objStr1.get(), objStr2.get());
}

RHO_GLOBAL VALUE rho_bluetooth_session_read_data(const char* connected_device_name) {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "session_read_data", "(Ljava/lang/String;[BI)I");
    if (!mid) return 0;
    jhstring objStr1 = rho_cast<jhstring>(env, connected_device_name);

    int buf_size = env->CallStaticIntMethod(cls, mid, objStr1.get(), 0, 0);

    if (buf_size == 0) {
        // nothing for receive
        return rho_ruby_get_NIL();
    }

    jholder<jbyteArray> buf_j = jholder<jbyteArray>(env->NewByteArray(buf_size));
    int real_readed = env->CallStaticIntMethod(cls, mid, objStr1.get(), buf_j.get(), buf_size);

    jbyte* buf_p = env->GetByteArrayElements(buf_j.get(), 0);
 
    VALUE val = rho_ruby_create_byte_array((unsigned char*)buf_p, real_readed);

    env->ReleaseByteArrayElements(buf_j.get(), buf_p, 0); 

    return val;
}

// public static int session_read_data(String connected_device_name, byte[] buf, int max_length)
// public static void session_write_data(String connected_device_name, byte[] buf, int length)


RHO_GLOBAL void rho_bluetooth_session_write_data(const char* connected_device_name, VALUE data) {
    int size = rho_ruby_unpack_byte_array(data, 0, 0);
    if (size <= 0) {
	return;
    }

    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHOBLUETOOTHMANAGER);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "session_write_data", "(Ljava/lang/String;[BI)V");
    if (!mid) return;
    jhstring objStr1 = rho_cast<jhstring>(env, connected_device_name);

    jholder<jbyteArray> buf_j = jholder<jbyteArray>(env->NewByteArray(size));
    jbyte* buf_p = env->GetByteArrayElements(buf_j.get(), 0);

    size = rho_ruby_unpack_byte_array(data, (unsigned char*)buf_p, size);

    env->CallStaticVoidMethod(cls, mid, objStr1.get(), buf_j.get(), size);

    env->ReleaseByteArrayElements(buf_j.get(), buf_p, 0); 
}

