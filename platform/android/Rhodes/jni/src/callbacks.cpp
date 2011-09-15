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

#include <ruby/ext/rho/rhoruby.h>

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

RHO_GLOBAL void delete_files_in_folder(const char *szFolderPath)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "deleteFilesInFolder", "(Ljava/lang/String;)V");
    if (!mid) return;
    jhstring objFolderPath = rho_cast<jhstring>(szFolderPath);
    env->CallStaticVoidMethod(cls, mid, objFolderPath.get());
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
	return (int)env->CallStaticBooleanMethod(cls, mid, rho_cast<jhstring>(szHost).get());
}

rho::String rho_sysimpl_get_phone_id()
{
    JNIEnv *env = jnienv();

    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "getProperty", "(Ljava/lang/String;)Ljava/lang/Object;");
    if (!mid) return 0;

    jhstring propNameObj = rho_cast<jhstring>("phone_id");
    jhobject result = jhobject(env->CallStaticObjectMethod(cls, mid, propNameObj.get()));
    if (!result) return 0;

    jstring resStrObj = (jstring)result.get();
    return rho_cast<std::string>(resStrObj);
}

RHO_GLOBAL int rho_sysimpl_get_property(char* szPropName, VALUE* resValue)
{
    JNIEnv *env = jnienv();

    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "getProperty", "(Ljava/lang/String;)Ljava/lang/Object;");
    if (!mid) return 0;

    jhstring propNameObj = rho_cast<jhstring>(szPropName);
    jhobject result = jhobject(env->CallStaticObjectMethod(cls, mid, propNameObj.get()));
    if (!result) return 0;

    jclass clsBoolean = getJNIClass(RHODES_JAVA_CLASS_BOOLEAN);
    jclass clsInteger = getJNIClass(RHODES_JAVA_CLASS_INTEGER);
    jclass clsFloat = getJNIClass(RHODES_JAVA_CLASS_FLOAT);
    jclass clsDouble = getJNIClass(RHODES_JAVA_CLASS_DOUBLE);
    jclass clsString = getJNIClass(RHODES_JAVA_CLASS_STRING);

    if (env->IsInstanceOf(result.get(), clsBoolean)) {
        jmethodID midValue = getJNIClassMethod(env, clsBoolean, "booleanValue", "()Z");
        *resValue = rho_ruby_create_boolean((int)env->CallBooleanMethod(result.get(), midValue));
        return 1;
    }
    else if (env->IsInstanceOf(result.get(), clsInteger)) {
        jmethodID midValue = getJNIClassMethod(env, clsInteger, "intValue", "()I");
        *resValue = rho_ruby_create_integer((int)env->CallIntMethod(result.get(), midValue));
        return 1;
    }
    else if (env->IsInstanceOf(result.get(), clsFloat)) {
        jmethodID midValue = getJNIClassMethod(env, clsFloat, "floatValue", "()F");
        *resValue = rho_ruby_create_double((double)env->CallFloatMethod(result.get(), midValue));
        return 1;
    }
    else if (env->IsInstanceOf(result.get(), clsDouble)) {
        jmethodID midValue = getJNIClassMethod(env, clsDouble, "doubleValue", "()D");
        *resValue = rho_ruby_create_double((double)env->CallDoubleMethod(result.get(), midValue));
        return 1;
    }
    else if (env->IsInstanceOf(result.get(), clsString)) {
        jstring resStrObj = (jstring)result.get();
        *resValue = rho_ruby_create_string(rho_cast<std::string>(resStrObj).c_str());
        return 1;
    }

    return 0;
}


RHO_GLOBAL int rho_sys_set_sleeping(int sleeping) {
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "rho_sys_set_sleeping", "(I)I");
    if (!mid) return 0;
    return env->CallStaticIntMethod(cls, mid, sleeping);
}

RHO_GLOBAL VALUE rho_sys_get_locale()
{
    VALUE res;
    if ( rho_sysimpl_get_property((char*)"locale", &res) )
        return res;

    return rho_ruby_get_NIL();
}

RHO_GLOBAL int rho_sys_get_screen_width()
{
    VALUE res;
    if ( rho_sysimpl_get_property((char*)"screen_width", &res) )
        return NUM2INT(res);

    return 0;
}

RHO_GLOBAL int rho_sys_get_screen_height()
{
    VALUE res;
    if ( rho_sysimpl_get_property((char*)"screen_height", &res) )
        return NUM2INT(res);

    return 0;
}

RHO_GLOBAL void rho_sys_app_exit()
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "exit", "()V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid);
}

RHO_GLOBAL void rho_sys_run_app(const char *appname, VALUE params)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "runApplication", "(Ljava/lang/String;Ljava/lang/Object;)V");
    if (!mid) return;
    jhobject jhParams = rho_cast<jobject>(env, params);
    env->CallStaticVoidMethod(cls, mid, rho_cast<jhstring>(env, appname).get(), jhParams.get());
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

RHO_GLOBAL void rho_sys_report_app_started()
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "handleAppStarted", "()V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid);
}

RHO_GLOBAL void rho_sys_open_url(const char *url)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "openExternalUrl", "(Ljava/lang/String;)V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid, rho_cast<jhstring>(url).get());
}

RHO_GLOBAL int rho_sys_is_app_installed(const char *appname)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return 0;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "isAppInstalled", "(Ljava/lang/String;)Z");
    if (!mid) return 0;
    return (int)env->CallStaticBooleanMethod(cls, mid, rho_cast<jhstring>(appname).get());
}

RHO_GLOBAL void rho_sys_app_install(const char *url)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "installApplication", "(Ljava/lang/String;)V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid, rho_cast<jhstring>(url).get());
}

RHO_GLOBAL void rho_sys_app_uninstall(const char *appname)
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "uninstallApplication", "(Ljava/lang/String;)V");
    if (!mid) return;
    env->CallStaticVoidMethod(cls, mid, rho_cast<jhstring>(appname).get());
}

RHO_GLOBAL void rho_sys_set_application_icon_badge(int badge_number) {
    //unsupported on Android
}
