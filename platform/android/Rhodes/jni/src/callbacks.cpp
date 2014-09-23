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
#include "rhodes/JNIRhoRuby.h"

#include "rhodes/RhoClassFactory.h"
#include "rhodes/fileapi.h"

#include "ruby/ext/rho/rhoruby.h"
#include "MethodResult.h"

#include "json/JSONIterator.h"

#include "common/IRhoThreadImpl.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Callbacks"


extern "C" void rho_webview_navigate(const char* url, int index);

RHO_GLOBAL void rho_map_location(char* query)
{
    rho::String url = "http://maps.google.com/?";
    url += query;
    rho_webview_navigate(url.c_str(), 0);
}

RHO_GLOBAL void rho_appmanager_load( void* /*httpContext*/, const char* /*szQuery*/)
{
    // Nothing
}

RHO_GLOBAL VALUE rho_sys_has_network()
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return rho_ruby_create_boolean(0);
    jmethodID mid = getJNIClassStaticMethod(env, cls, "hasNetwork", "()Z");
    if (!mid) return rho_ruby_create_boolean(0);
    return rho_ruby_create_boolean(env->CallStaticBooleanMethod(cls, mid));
}

RHO_GLOBAL void rho_file_impl_delete_files_in_folder(const char *szFolderPath)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "deleteFilesInFolder", "(Ljava/lang/String;)V");
    if (!mid) return;
    jhstring objFolderPath = rho_cast<jstring>(env, szFolderPath);
    env->CallStaticVoidMethod(cls, mid, objFolderPath.get());
}

RHO_GLOBAL void rho_platform_restart_application() {

}

RHO_GLOBAL VALUE rho_sys_makephonecall(const char* callname, int nparams, char** param_names, char** param_values)
{
    // TODO:
    RHO_NOT_IMPLEMENTED;
    return 0;
}

RHO_GLOBAL void rho_net_impl_network_indicator(int enable)
{
    // No GUI indicator on Android
}

RHO_GLOBAL int rho_net_ping_network(const char* szHost)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "pingHost", "(Ljava/lang/String;)Z");
    if (!mid) return 0;

    jhstring jhHost = rho_cast<jstring>(env, szHost);
    return (int)env->CallStaticBooleanMethod(cls, mid, jhHost.get());
}

RHO_GLOBAL int rho_sysimpl_get_property(const char* szPropName, rho::apiGenerator::CMethodResult& result)
{
    RAWTRACE1("getProperty: %s", szPropName);

    JNIEnv *env = jnienv();

    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "getProperty", "(Ljava/lang/String;)Ljava/lang/Object;");
    if (!mid) return 0;

    jhstring propNameObj = rho_cast<jstring>(env, szPropName);
    jhobject jhresult = env->CallStaticObjectMethod(cls, mid, propNameObj.get());
    JNI_EXCEPTION_CHECK_AND_RETURN(env, result, 0)
    if (!jhresult) return 0;

    jclass clsBoolean = getJNIClass(RHODES_JAVA_CLASS_BOOLEAN);
    jclass clsInteger = getJNIClass(RHODES_JAVA_CLASS_INTEGER);
    jclass clsFloat = getJNIClass(RHODES_JAVA_CLASS_FLOAT);
    jclass clsDouble = getJNIClass(RHODES_JAVA_CLASS_DOUBLE);
    jclass clsString = getJNIClass(RHODES_JAVA_CLASS_STRING);

    if (env->IsInstanceOf(jhresult.get(), clsBoolean)) {
        jmethodID midValue = getJNIClassMethod(env, clsBoolean, "booleanValue", "()Z");
        bool res = static_cast<bool>(env->CallBooleanMethod(jhresult.get(), midValue));
        result.set(res);
        return 1;
    }
    else if (env->IsInstanceOf(jhresult.get(), clsInteger)) {
        jmethodID midValue = getJNIClassMethod(env, clsInteger, "intValue", "()I");
        int res = static_cast<int>(env->CallIntMethod(jhresult.get(), midValue));
        result.set(res);
        return 1;
    }
    else if (env->IsInstanceOf(jhresult.get(), clsFloat)) {
        jmethodID midValue = getJNIClassMethod(env, clsFloat, "floatValue", "()F");
        double res = static_cast<double>(env->CallFloatMethod(jhresult.get(), midValue));
        result.set(res);
        return 1;
    }
    else if (env->IsInstanceOf(jhresult.get(), clsDouble)) {
        jmethodID midValue = getJNIClassMethod(env, clsDouble, "doubleValue", "()D");
        double res = static_cast<double>(env->CallFloatMethod(jhresult.get(), midValue));
        result.set(res);
        return 1;
    }
    else if (env->IsInstanceOf(jhresult.get(), clsString)) {
        jstring resStrObj = (jstring)jhresult.get();
        rho::String res = rho_cast<rho::String>(env, resStrObj);
        result.set(res);
        return 1;
    } 

    // clear result
    result.set("");    
    return 0;
}

rho::String rho_sysimpl_get_phone_id()
{
    rho::apiGenerator::CMethodResult result;
    rho_sysimpl_get_property("phone_id", result);
    return result.getString();
}

RHO_GLOBAL int rho_sys_set_sleeping(int sleeping) {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "rho_sys_set_sleeping", "(I)I");
    if (!mid) return 0;
    return env->CallStaticIntMethod(cls, mid, sleeping);
}

RHO_GLOBAL void rho_sys_app_exit()
{
    JNIEnv *env = jnienv();

    int isKill = 0;
    void *q = NULL;
    if (env == 0) {
    	isKill = 1;
    	q = rho_nativethread_start();
    	env = jnienv();
    }
    if (env == 0) {
    	RAWLOG_ERROR("JNIEnv is not set for this thread!!! WTF ?!");
    }

    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return;
    jmethodID mid = NULL;
    if (isKill) {
    	mid = getJNIClassStaticMethod(env, cls, "kill", "()V");
    }
    else {
    	mid = getJNIClassStaticMethod(env, cls, "exit", "()V");
    }
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid);
    if (q != NULL) {
    	rho_nativethread_end(q);
    }
}

RHO_GLOBAL void rho_sys_run_app(const rho::String& appname, const rho::String& params, rho::apiGenerator::CMethodResult& result)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "runApplication", "(Ljava/lang/String;Ljava/lang/Object;)V");
    if (!mid) return;
    jhstring jhName = rho_cast<jstring>(env, appname);
    jhstring jhParams = rho_cast<jstring>(env, params);
    env->CallStaticVoidMethod(cls, mid, jhName.get(), jhParams.get());
    JNI_EXCEPTION_CHECK(env, result);
}

RHO_GLOBAL void rho_sys_bring_to_front()
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "bringToFront", "()V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid);
}

RHO_GLOBAL void rho_sys_minimize()
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "minimize", "()V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid);
}

RHO_GLOBAL void rho_sys_report_app_started()
{
    RHODESAPP().initPushClients();

    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "handleAppStarted", "()V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid);
}

RHO_GLOBAL void rho_sys_is_app_installed(const rho::String& appname, rho::apiGenerator::CMethodResult& result)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "isAppInstalled", "(Ljava/lang/String;)Z");
    if (!mid) return;

    jhstring jhAppName = rho_cast<jstring>(env, appname);
    jboolean res = static_cast<bool>(env->CallStaticBooleanMethod(cls, mid, jhAppName.get()));
    result.set(res);
    JNI_EXCEPTION_CHECK(env, result);
}

RHO_GLOBAL void rho_sys_app_install(const rho::String& url, rho::apiGenerator::CMethodResult& result)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "installApplication", "(Ljava/lang/String;)V");
    if (!mid) return;

    jhstring jhUrl = rho_cast<jstring>(env, url);
    env->CallStaticVoidMethod(cls, mid, jhUrl.get());
    JNI_EXCEPTION_CHECK(env, result);
}

RHO_GLOBAL void rho_sys_app_uninstall(const rho::String& appname, rho::apiGenerator::CMethodResult& result)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "uninstallApplication", "(Ljava/lang/String;)V");
    if (!mid) return;

    jhstring jhAppName = rho_cast<jstring>(env, appname);
    env->CallStaticVoidMethod(cls, mid, jhAppName.get());
    JNI_EXCEPTION_CHECK(env, result);
}

RHO_GLOBAL void rho_sys_set_application_icon_badge(int badge_number) {
    //unsupported on Android
}

RHO_GLOBAL void rho_sys_set_full_screen_mode(bool enable)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "setFullscreen", "(Z)V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid, static_cast<jboolean>(enable));
}

RHO_GLOBAL void rho_sys_get_full_screen_mode(rho::apiGenerator::CMethodResult& result)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "getFullscreen", "()Z");
    if (!mid) return;
    result.set(static_cast<bool>(env->CallStaticBooleanMethod(cls, mid)));
}

RHO_GLOBAL void rho_sys_set_screen_auto_rotate_mode(bool enable)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "setScreenAutoRotate", "(Z)V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid, static_cast<jboolean>(enable));
}

RHO_GLOBAL void rho_sys_get_screen_auto_rotate_mode(rho::apiGenerator::CMethodResult& result)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "getScreenAutoRotate", "()Z");
    if (!mid) return;
    result.set(static_cast<bool>(env->CallStaticBooleanMethod(cls, mid)));
}

static rho::Hashtable<int, rho::common::IRhoRunnable*> cbk_array;
static int counter = 0;

extern "C" void rho_os_impl_performOnUiThread(rho::common::IRhoRunnable* pTask)
{
    if (pTask == NULL) {
        return;
    }
    int curr = ++counter;
    cbk_array[curr] = pTask;
    
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "runOnUiThread", "(I)V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid, curr);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_onUiThreadCallback(JNIEnv *env, jint idx)
{
    int cbk_idx = static_cast<int>(idx);
    if (cbk_array.count(cbk_idx) > 0) {
        cbk_array[cbk_idx]->runObject();
        cbk_array.erase(cbk_idx);
    }
}

namespace rho {

void rho_impl_setNativeMenu(const rho::Vector<rho::String>& menu)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "setNativeMenu", "(Ljava/util/List;)V");
    if (!mid) return;

    jhobject jhMenuItems = rho_cast<jobject>(env, menu);

    env->CallStaticVoidMethod(cls, mid, jhMenuItems.get());
}

String rho_impl_getNativeMenu()
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return "";
    jmethodID mid = getJNIClassStaticMethod(env, cls, "getNativeMenu", "()Ljava/lang/String;");
    if (!mid) return "";

    jhstring jhMenuItems = static_cast<jstring>(env->CallStaticObjectMethod(cls, mid));

    return rho_cast<String>(env, jhMenuItems.get());
}

}
