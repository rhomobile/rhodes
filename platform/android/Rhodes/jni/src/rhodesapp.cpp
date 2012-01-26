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

#include "rhodes/jni/com_rhomobile_rhodes_RhodesService.h"
#include "rhodes/jni/com_rhomobile_rhodes_RhodesAppOptions.h"

#include <common/RhoConf.h>
#include <common/app_build_configs.h>
#include <logging/RhoLogConf.h>
#include <common/RhodesApp.h>
#include <common/AutoPointer.h>
#include <sync/SyncThread.h>
#include <sync/ClientRegister.h>

#include <genconfig.h>

#include "rhodes/JNIRhodes.h"
#include "rhodes/JNIRhoRuby.h"
#include "rhodes/RhoClassFactory.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhodesAppJNI"


RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_makeLink
  (JNIEnv *env, jclass, jstring src, jstring dst)
{
    // We should not use rho_cast functions here because this function
    // called very early when jnienv() is not yet initialized
    const char *strSrc = env->GetStringUTFChars(src, JNI_FALSE);
    const char *strDst = env->GetStringUTFChars(dst, JNI_FALSE);
    ::unlink(strDst);
    int err = ::symlink(strSrc, strDst);
    env->ReleaseStringUTFChars(src, strSrc);
    env->ReleaseStringUTFChars(dst, strDst);
    if (err != 0)
        env->ThrowNew(getJNIClass(RHODES_JAVA_CLASS_RUNTIME_EXCEPTION), "Can not create symlink");
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesService_getInvalidSecurityTokenMessage(JNIEnv* env, jclass) {
    
    const char* message = "Invalid security token !";
    
    //rho_conf_Init(rho_native_rhopath());
    
    if (rho_conf_is_property_exists("invalid_security_token_message")) {
        const char* conf_message = rho_conf_getString("invalid_security_token_message");
        message = conf_message;
    }
    
    jstring objStr = env->NewStringUTF(message);
    return objStr;
    
    
}

static jobject g_classLoader = NULL;
static jmethodID g_loadClass = NULL;

jclass rho_find_class(JNIEnv *env, const char *c)
{
    jstring className = env->NewStringUTF(c);
    jclass cls = (jclass)env->CallObjectMethod(g_classLoader, g_loadClass, className);
    env->DeleteLocalRef(className);
    return cls;
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesApplication_initClassLoader
  (JNIEnv *env, jclass, jobject cl)
{
    g_classLoader = env->NewGlobalRef(cl);
    jclass javaLangClassLoader = env->FindClass("java/lang/ClassLoader");
    g_loadClass = env->GetMethodID(javaLangClassLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesApplication_setupRhodesApp
  (JNIEnv *env, jclass)
{
    android_setup(env);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesApplication_createRhodesApp
  (JNIEnv *env, jclass)
{
    // Start Rhodes application
    rho_rhodesapp_create(rho_native_rhopath());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesApplication_startRhodesApp
  (JNIEnv *, jclass)
{
    rho_rhodesapp_start();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesApplication_stopRhodesApp
  (JNIEnv *, jclass)
{
    rho_rhodesapp_destroy();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_doSyncAllSources
  (JNIEnv *, jobject, jboolean show_status_popup)
{
    rho_sync_doSyncAllSources(show_status_popup, "");
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_doSyncSource
  (JNIEnv *env, jobject, jstring sourceObj)
{
    std::string source = rho_cast<std::string>(env, sourceObj);
    rho_sync_doSyncSourceByName(source.c_str());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesApplication_setStartParameters
  (JNIEnv *env, jclass, jstring strUrl)
{
    std::string url = rho_cast<std::string>(env, strUrl);
    RHODESAPP().setStartParameters(url.c_str());
}


RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesAppOptions_getOptionsUrl
  (JNIEnv *env, jclass)
{
    const char *s = RHODESAPP().getOptionsUrl().c_str();
    return rho_cast<jhstring>(env, s).release();
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesAppOptions_getStartUrl
  (JNIEnv *env, jclass)
{
    const char *s = RHODESAPP().getStartUrl().c_str();
    return rho_cast<jhstring>(env, s).release();
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesAppOptions_getCurrentUrl
  (JNIEnv *env, jclass)
{
    const char *s = RHODESAPP().getCurrentUrl(0).c_str();
    return rho_cast<jhstring>(env, s).release();
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesAppOptions_getAppBackUrl
  (JNIEnv *env, jclass)
{
    const char *s = RHODESAPP().getAppBackUrl().c_str();
    return rho_cast<jhstring>(env, s).release();
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesAppOptions_getBlobPath
  (JNIEnv *env, jclass)
{
    const char *s = RHODESAPP().getBlobsDirPath().c_str();
    return rho_cast<jhstring>(env, s).release();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_doRequest
  (JNIEnv *env, jobject, jstring strUrl)
{
    std::string const &url = rho_cast<std::string>(strUrl);
    rho_net_request(url.c_str());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_doRequestAsync
  (JNIEnv *env, jobject, jstring strUrl)
{
    std::string const &url = rho_cast<std::string>(strUrl);
    RHODESAPP().runCallbackInThread(url.c_str(), "");
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesService_normalizeUrl
  (JNIEnv *env, jobject, jstring strUrl)
{
    std::string const &s = rho_cast<std::string>(strUrl);
    std::string const &cs = RHODESAPP().canonicalizeRhoUrl(s);
    return rho_cast<jhstring>(env, cs).release();
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesService_getBuildConfig
  (JNIEnv *env, jclass, jstring key)
{
    std::string const &s = rho_cast<std::string>(key);
    const char* cs = get_app_build_config_item(s.c_str());
    return rho_cast<jhstring>(env, cs).release();
}

RHO_GLOBAL jboolean JNICALL Java_com_rhomobile_rhodes_RhodesService_isOnStartPage
  (JNIEnv *env, jclass)
{
    bool res = RHODESAPP().isOnStartPage();
    return (jboolean)res;
}


RHO_GLOBAL jboolean JNICALL Java_com_rhomobile_rhodes_RhodesService_isTitleEnabled
  (JNIEnv *, jclass)
{
    bool value = true;
    const char* svalue = get_app_build_config_item("android_title");
    if (svalue != NULL) {
        value = svalue[0] != '0';
    } 
    return (jboolean)value;
}

RHO_GLOBAL jboolean JNICALL Java_com_rhomobile_rhodes_RhodesApplication_canStartApp
  (JNIEnv *, jclass, jstring cmdLine, jstring sep)
{
    std::string const &strCmdLine = rho_cast<std::string>(cmdLine);
    std::string const &strSep = rho_cast<std::string>(sep);

    int nRes = rho_rhodesapp_canstartapp(strCmdLine.c_str(), strSep.c_str());
    return (jboolean)(nRes ? true : false);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_navigateBack
  (JNIEnv *, jclass)
{
    rho_rhodesapp_navigate_back();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_loadUrl
  (JNIEnv *env, jclass, jstring str)
{
    rho_rhodesapp_load_url(rho_cast<std::string>(env, str).c_str());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_onScreenOrientationChanged
  (JNIEnv *env, jclass, jint width, jint height, jint angle)
{
	rho_rhodesapp_callScreenRotationCallback(width, height, angle);
	//RAWLOG_ERROR3("$$$$$$$$$$$$$$$$ SCREEN : [%d]x[%d] angle[%d]", width, height, angle);	
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_callUiCreatedCallback
  (JNIEnv *, jclass)
{
    rho_rhodesapp_callUiCreatedCallback();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_callUiDestroyedCallback
  (JNIEnv *, jclass)
{
    rho_rhodesapp_callUiDestroyedCallback();
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_callActivationCallback
  (JNIEnv *, jclass, jboolean active)
{
    rho_rhodesapp_callAppActiveCallback(active);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_setPushRegistrationId
  (JNIEnv *env, jobject, jstring jId)
{
    std::string id = rho_cast<std::string>(env, jId);
    rho::sync::CClientRegister* pClientRegister = rho::sync::CClientRegister::getInstance();
    if(pClientRegister && (pClientRegister->getDevicePin().compare(id) != 0))
    {
        rho::sync::CClientRegister::Destroy();
    }
    rho::sync::CClientRegister::Create(id.c_str());
    RHOCONF().setString("push_pin", id.c_str(), true);
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesService_getPushRegistrationId
  (JNIEnv * env, jobject)
{
    return rho_cast<jhstring>(RHOCONF().getString("push_pin").c_str()).release();
}

RHO_GLOBAL jboolean JNICALL Java_com_rhomobile_rhodes_RhodesService_callPushCallback
  (JNIEnv *env, jobject, jstring jData)
{
    std::string data = rho_cast<std::string>(env, jData);
    return (jboolean)rho_rhodesapp_callPushCallback(data.c_str());
}

RHO_GLOBAL char *rho_timezone()
{
    static char *tz = NULL;
    if (!tz)
    {
        JNIEnv *env = jnienv();
        jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
        if (!cls) return NULL;
        jmethodID mid = getJNIClassStaticMethod(env, cls, "getTimezoneStr", "()Ljava/lang/String;");
        if (!mid) return NULL;
        jstring s = (jstring)env->CallStaticObjectMethod(cls, mid);
        std::string tzs = rho_cast<std::string>(env, s);
        tz = strdup(tzs.c_str());
    }
    return tz;
}

RHO_GLOBAL void rho_conf_show_log()
{
    JNIEnv *env = jnienv();
    jclass cls = getJNIClass(RHODES_JAVA_CLASS_RHODES_SERVICE);
    if (!cls) return;
    jmethodID mid = getJNIClassStaticMethod(env, cls, "showLogView", "()V");
    if (!mid) return;

    env->CallStaticVoidMethod(cls, mid);
}

