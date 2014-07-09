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
#include <sync/RhoconnectClientManager.h>
#include "Push.h"

#include "rhodes/JNIRhodes.h"
#include "rhodes/JNIRhoRuby.h"
#include "rhodes/RhoClassFactory.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhodesAppJNI"

static rho::common::CAutoPtr<rho::common::AndroidNetworkStatusMonitor> s_network_status_monitor(new rho::common::AndroidNetworkStatusMonitor());


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
    
    jstring objStr = rho_cast<jstring>(env,message);
    return objStr;
    
    
}

static jobject g_classLoader = NULL;
static jmethodID g_loadClass = NULL;

jclass rho_find_class(JNIEnv *env, const char *c)
{
    //RAWTRACE2("%s - %s", __FUNCTION__, c);
    jstring className = env->NewStringUTF(c);
    jclass cls = (jclass)env->CallObjectMethod(g_classLoader, g_loadClass, className);
    if(env->ExceptionCheck() == JNI_TRUE) {
        env->ExceptionClear();
        cls = 0;
    } else
    {
        env->DeleteLocalRef(className);
    }
    return cls;
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_ScreenReceiver_notifyDeviceScreenEvent
  (JNIEnv *env, jclass, int event)
{
    if (RHODESAPP().getApplicationEventReceiver())
    {
        RHODESAPP().getApplicationEventReceiver()->onDeviceScreenEvent(event);
    }
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
	RHODESAPP().setNetworkStatusMonitor(s_network_status_monitor);
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
    //rho_sync_doSyncAllSources(show_status_popup, "", 0);
	if ( rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl() ) {
		rho::sync::RhoconnectClientManager::doSyncAllSources(show_status_popup,"",0);
	}
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_doSyncSource
  (JNIEnv *env, jobject, jstring sourceObj)
{
	if ( rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl() ) {
		std::string source = rho_cast<std::string>(env, sourceObj);
//		rho_sync_doSyncSourceByName(source.c_str());
		rho::sync::RhoconnectClientManager::doSyncSourceByName(source.c_str());
	}
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
    return rho_cast<jstring>(env, s);
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesAppOptions_getStartUrl
  (JNIEnv *env, jclass)
{
    const char *s = RHODESAPP().getStartUrl().c_str();
    return rho_cast<jstring>(env, s);
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesAppOptions_getCurrentUrl
  (JNIEnv *env, jclass)
{
    const char *s = RHODESAPP().getCurrentUrl(0).c_str();
    return rho_cast<jstring>(env, s);
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesAppOptions_getAppBackUrl
  (JNIEnv *env, jclass)
{
    const char *s = RHODESAPP().getAppBackUrl().c_str();
    return rho_cast<jstring>(env, s);
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesAppOptions_getBlobPath
  (JNIEnv *env, jclass)
{
    const char *s = RHODESAPP().getBlobsDirPath().c_str();
    return rho_cast<jstring>(env, s);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_doRequest
  (JNIEnv *env, jclass, jstring strUrl)
{
    std::string url = rho_cast<std::string>(env, strUrl);
    rho_net_request(url.c_str());
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_doRequestAsync
  (JNIEnv *env, jclass, jstring strUrl)
{
    std::string url = rho_cast<std::string>(env, strUrl);
    RHODESAPP().runCallbackInThread(url, "");
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_doRequestEx
  (JNIEnv *env, jclass, jstring jUrl, jstring jBody, jstring jData, jboolean waitForResponse)
{
    std::string url = rho_cast<std::string>(env, jUrl);
    std::string body = rho_cast<std::string>(env, jBody);
    std::string data = rho_cast<std::string>(env, jData);
    RHODESAPP().callCallbackWithData(url, body, data, waitForResponse);
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_doRequestJson
  (JNIEnv *env, jclass, jstring jUrl, jstring jBody, jstring jData, jboolean waitForResponse)
{
    std::string url = rho_cast<std::string>(env, jUrl);
    std::string body = rho_cast<std::string>(env, jBody);
    std::string data = rho_cast<std::string>(env, jData);

    RHODESAPP().callCallbackWithJsonBody(url.c_str(), body.c_str(), data.c_str(), waitForResponse);
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesService_normalizeUrl
  (JNIEnv *env, jobject, jstring strUrl)
{
    std::string const &s = rho_cast<std::string>(env, strUrl);
    std::string const &cs = RHODESAPP().canonicalizeRhoUrl(s);
    return rho_cast<jstring>(env, cs);
}

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesService_getBuildConfig
  (JNIEnv *env, jclass, jstring key)
{
    std::string const &s = rho_cast<std::string>(env, key);
    const char* cs = get_app_build_config_item(s.c_str());
    return rho_cast<jstring>(env, cs);
}

//RHO_GLOBAL jboolean JNICALL Java_com_rhomobile_rhodes_RhodesService_isMotorolaLicencePassed
//(JNIEnv *env, jclass, jstring jLicense, jstring jCompany, jstring jAppName)
//{
//    int res = rho_can_app_started_with_current_licence(
//                    jLicense ? rho_cast<std::string>(env, jLicense).c_str() : 0,
//                    jCompany ? rho_cast<std::string>(env, jCompany).c_str() : 0,
//                    jAppName ? rho_cast<std::string>(env, jAppName).c_str() : 0);

//    return (jboolean)(res == 1);
//}


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
  (JNIEnv *env, jclass, jstring cmdLine, jstring sep)
{
    std::string const &strCmdLine = rho_cast<std::string>(env, cmdLine);
    std::string const &strSep = rho_cast<std::string>(env, sep);

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

RHO_GLOBAL jstring JNICALL Java_com_rhomobile_rhodes_RhodesService_currentLocation(JNIEnv * env, jclass, jint jTab)
{
    std::string strLocation = RHODESAPP().getCurrentUrl(static_cast<int>(jTab));
    RAWTRACE2("Controller currentLocation (tab: %d): %s", static_cast<int>(jTab), strLocation.c_str());
    return rho_cast<jstring>(env, strLocation);
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
  (JNIEnv *env, jobject, jstring jType, jstring jId)
{
    std::string strType = jType ? rho_cast<std::string>(env, jType) : "";
    std::string deviceId = rho_cast<std::string>(env, jId);

    rho::push::CPushManager::getInstance()->setDeviceId(strType, deviceId);
}

RHO_GLOBAL jboolean JNICALL Java_com_rhomobile_rhodes_RhodesService_callPushCallback
  (JNIEnv *env, jobject, jstring jType, jstring jJson)
{
    std::string strType = jType ? rho_cast<std::string>(env, jType) : "";
    std::string strJson = jJson ? rho_cast<std::string>(env, jJson) : "";

    rho::push::CPushManager::getInstance()->callBack(strType, strJson);

    return (jboolean)true;
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_resetHttpLogging
  (JNIEnv *env, jobject, jstring jId)
{
    //RAWLOG_ERROR("$$$$$$$$$$$ RESET HTTP LOGGING 1 $$$$$$$$$$$$$");
    std::string url = rho_cast<std::string>(env, jId);
    rho_log_resetup_http_url(url.c_str());	
    //RAWLOG_ERROR("$$$$$$$$$$$ RESET HTTP LOGGING 2 $$$$$$$$$$$$$");
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_resetFileLogging
(JNIEnv *env, jobject, jstring jId)
{
    std::string path = rho_cast<std::string>(env, jId);
    LOGCONF().setLogFilePath(path);
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

RHO_GLOBAL void rho_title_change(const int tabIndex, const char* strTitle)
{
    // not implemented
}

RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_RhodesService_notifyNetworkStatusChanged(JNIEnv *env, jobject, int status)
{
    RAWLOG_ERROR("nativeNotify");
    rho::common::enNetworkStatus s = rho::common::networkStatusUnknown;
    switch(status)
    {
    case 1:
        s = rho::common::networkStatusConnected;
        break;
    case 2:
        s = rho::common::networkStatusDisconnected;
        break;
    }
    s_network_status_monitor->notifyReceiver(s);
}
