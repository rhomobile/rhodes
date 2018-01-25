/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2008-2012 Rhomobile, Inc.
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
#include "common/RhoConf.h"
#include "Push.h"
#include "sync/RhoconnectClientManager.h"
#include "sync/ILoginListener.h"
#include "ruby/ext/rho/rhoruby.h"

#include "logging/RhoLog.h"

#include "fcmpushclient.h"

//----------------------------------------------------------------------------------------------------------------------
extern "C" void Init_FCMPushClient()
{
    // create FCM push client
    RAWTRACEC("Init_FCMPushClient", "creating FCM client >>>>>>>>>>>>>>");

    rho::fcm::FcmPushClient* pClient = new rho::fcm::FcmPushClient();

    RAWTRACEC("Init_FCMPushClient", "adding FCM client >>>>>>>>>>>>>>>>");

    rho::push::CPushManager::getInstance()->addClient(pClient);
}
//----------------------------------------------------------------------------------------------------------------------

namespace rho { namespace fcm {

IMPLEMENT_LOGCLASS(FcmPushClient, "FcmPushClient");

//----------------------------------------------------------------------------------------------------------------------

const char* const FcmPushClient::s_FCM_FACADE_CLASS = "com.rhomobile.rhodes.fcm.FCMFacade";

//----------------------------------------------------------------------------------------------------------------------

const String FcmPushClient::s_Type = "fcm";
String FcmPushClient::token = "";

FcmPushClient::FcmPushClient()
{
    LOG(TRACE) + "FcmPushInit()";
    canExecuteNotifications = false;
    JNIEnv *env = jnienv();

    static jclass cls = rho_find_class(env, s_FCM_FACADE_CLASS);
    if (!cls) {
        LOG(ERROR) + "Cannot find class: " + s_FCM_FACADE_CLASS;
        return;
    }

    static jmethodID mid = env->GetStaticMethodID(cls, "initFireBase", "()V");
    if (!mid) {
        LOG(ERROR) + "Cannot get " + s_FCM_FACADE_CLASS + ".initFireBase() method";
        return;
    }

    env->CallStaticVoidMethod(cls, mid);

    CMethodResult result;
    setProperty("id", s_Type, result);
    setProperty("type", IPush::PUSH_TYPE_NATIVE, result);
    setProperty("senderId", RHOCONF().getString("Push.fcm.senderId"), result);
    setPropertyFromMethod("google_app_id", result);
    //setPropertyFromMethod("google_api_key", result);
    setPropertyFromMethod("gcm_defaultSenderId", result);

}

//----------------------------------------------------------------------------------------------------------------------

void FcmPushClient::setPropertyFromMethod(const char* methodName, CMethodResult &result) {

    JNIEnv *env = jnienv();
    jclass cls = rho_find_class(env, s_FCM_FACADE_CLASS);
    if (!cls) {
        LOG(ERROR) + "Cannot get " + s_FCM_FACADE_CLASS;
        return;
    }

    jmethodID mid = env->GetStaticMethodID( cls, methodName, "()Ljava/lang/String;");
    if (!mid){
        LOG(ERROR) + "Cannot get " + s_FCM_FACADE_CLASS + "." + methodName + " method";
        return;
    } 

    jstring jstr = (jstring)env->CallStaticObjectMethod(cls, mid);
    const char* buf = env->GetStringUTFChars(jstr,0);

    setProperty(methodName, buf, result);
    env->ReleaseStringUTFChars(jstr, buf);
}

void FcmPushClient::refreshToken(){
    LOG(TRACE) + "refreshToken()";
    JNIEnv *env = jnienv();

    static jclass cls = rho_find_class(env, s_FCM_FACADE_CLASS);
    if (!cls) {
        LOG(ERROR) + "Cannot find class: " + s_FCM_FACADE_CLASS;
        return;
    }

    static jmethodID mid = env->GetStaticMethodID(cls, "refreshToken", "()V");
    if (!mid) {
        LOG(ERROR) + "Cannot get " + s_FCM_FACADE_CLASS + ".refreshToken() method";
        return;
    }

    env->CallStaticVoidMethod(cls, mid);

}

void FcmPushClient::getDeviceId(CMethodResult& result)
{
    String deviceId = m_hashProps["deviceId"];

    if(deviceId.length() != 0)
    {
        LOG(TRACE) + "FCM deviceId: " + deviceId;
        result.set(deviceId);
    }
    else
    {
        LOG(TRACE) + "Still waiting for FCM deviceId";
        m_deviceIdResult = result;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void FcmPushClient::startNotifications(CMethodResult& result)
{
    LOG(TRACE) + "Start FCM push notifications";
    m_oResult = result;
    canExecuteNotifications = true;
    executeCallBacks();
}

//----------------------------------------------------------------------------------------------------------------------
void FcmPushClient::stopNotifications(CMethodResult& result)
{
    LOG(TRACE) + "Stop FCM push notifications";
    canExecuteNotifications = false;
    m_oResult = CMethodResult();

}

//----------------------------------------------------------------------------------------------------------------------
void FcmPushClient::setDeviceId(const String& deviceId)
{
    CMethodResult result;
    setProperty("deviceId", deviceId, result);

    LOG(TRACE) + "creating client register";
    rho::sync::RhoconnectClientManager::clientRegisterCreate(deviceId.c_str());

    getDeviceId(m_deviceIdResult);
    m_deviceIdResult = CMethodResult();

}

//----------------------------------------------------------------------------------------------------------------------
bool FcmPushClient::callBack(const String& json)
{
    LOG(TRACE) + "FCM push notification: " + json;

    if (canExecuteNotifications){
        m_oResult.setJSON(json);
    }else{
        if (callBacksQueue.size() < 1024){
            callBacksQueue.push(json);
        }
    }

    return true;
}

void FcmPushClient::executeCallBacks()
{

    while (!callBacksQueue.empty()){
        callBack(callBacksQueue.front());
        callBacksQueue.pop();
    }

}


}}

