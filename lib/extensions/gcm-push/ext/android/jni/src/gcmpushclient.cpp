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


#include "logging/RhoLog.h"

#include "gcmpushclient.h"

//----------------------------------------------------------------------------------------------------------------------
extern "C" void Init_GCMPushClient()
{
    // create GCM push client
    RAWTRACEC("Init_GCMPushClient", "creating GCM client >>>>>>>>>>>>>>");

    rho::gcm::GcmPushClient* pClient = new rho::gcm::GcmPushClient();

    RAWTRACEC("Init_GCMPushClient", "adding GCM client >>>>>>>>>>>>>>>>");

    rho::push::CPushManager::getInstance()->addClient(pClient);

}
//----------------------------------------------------------------------------------------------------------------------

namespace rho { namespace gcm {

IMPLEMENT_LOGCLASS(GcmPushClient, "GcmPushClient");

//----------------------------------------------------------------------------------------------------------------------

const char* const GcmPushClient::s_GCM_FACADE_CLASS = "com.rhomobile.rhodes.gcm.GCMFacade";

//----------------------------------------------------------------------------------------------------------------------
void GcmPushClient::doRegister()
{
    LOG(TRACE) + "GcmPushRegister()";

    JNIEnv *env = jnienv();

    static jclass cls = rho_find_class(env, s_GCM_FACADE_CLASS);
    if (!cls) {
        LOG(ERROR) + "Cannot find class: " + s_GCM_FACADE_CLASS;
        return;
    }

    static jmethodID mid = env->GetStaticMethodID(cls, "Register", "(Ljava/lang/String;)V");
    if (!mid) {
        LOG(ERROR) + "Cannot get " + s_GCM_FACADE_CLASS + ".Register() method";
        return;
    }

    jhstring jhSenderId = rho_cast<jstring>(m_hashProps["senderId"]);

    env->CallStaticVoidMethod(cls, mid, jhSenderId.get());
}

//----------------------------------------------------------------------------------------------------------------------
void GcmPushClient::doUnregister()
{
    LOG(TRACE) + "GcmPushUnregister()";

    JNIEnv *env = jnienv();

    static jclass cls = rho_find_class(env, s_GCM_FACADE_CLASS);
    if (!cls) {
        LOG(ERROR) + "Cannot find class: " + s_GCM_FACADE_CLASS;
        return;
    }

    static jmethodID mid = env->GetStaticMethodID(cls, "Unregister", "()V");
    if (!mid) {
        LOG(ERROR) + "Cannot get " + s_GCM_FACADE_CLASS + ".Unregister() method";
        return;
    }

    env->CallStaticVoidMethod(cls, mid);
}
//----------------------------------------------------------------------------------------------------------------------
const String GcmPushClient::s_Type = "gcm";

GcmPushClient::GcmPushClient()
{
    CMethodResult result;
    setProperty("id", s_Type, result);
    setProperty("type", IPush::PUSH_TYPE_NATIVE, result);
    setProperty("senderId", RHOCONF().getString("Push.gcm.senderId"), result);

    LOG(TRACEC) + "request GCM registration >>>>>>>>>>>>>>>>";
    doRegister();
}

//----------------------------------------------------------------------------------------------------------------------

void GcmPushClient::getDeviceId(CMethodResult& result)
{
    String deviceId = m_hashProps["deviceId"];

    if(deviceId.length() != 0)
    {
        LOG(TRACE) + "GCM deviceId: " + deviceId;
        result.set(deviceId);
    }
    else
    {
        LOG(TRACE) + "Still waiting for GCM deviceId";
        m_deviceIdResult = result;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void GcmPushClient::startNotifications(CMethodResult& result)
{
    LOG(TRACE) + "Start GCM push notifications";
    m_oResult = result;
}

//----------------------------------------------------------------------------------------------------------------------
void GcmPushClient::stopNotifications(CMethodResult& result)
{
    LOG(TRACE) + "Stop GCM push notifications";
    m_oResult = CMethodResult();
}

//----------------------------------------------------------------------------------------------------------------------
void GcmPushClient::setDeviceId(const String& deviceId)
{
    CMethodResult result;
    setProperty("deviceId", deviceId, result);

    LOG(TRACE) + "creating client register";
    rho::sync::RhoconnectClientManager::clientRegisterCreate(deviceId.c_str());

    getDeviceId(m_deviceIdResult);
    m_deviceIdResult = CMethodResult();

}

//----------------------------------------------------------------------------------------------------------------------
bool GcmPushClient::callBack(const String& json)
{
    LOG(TRACE) + "GCM push notification: " + json;

    m_oResult.setJSON(json);

    return true;
}

}}

