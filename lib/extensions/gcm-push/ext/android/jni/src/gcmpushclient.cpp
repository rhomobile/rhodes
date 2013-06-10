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
#include "com_rhomobile_rhodes_gcm_GCMRhoListener.h"

//----------------------------------------------------------------------------------------------------------------------
extern "C" void Init_GCMPushClient()
{
    // create GCM push client
    rho::push::CPushManager::getInstance()->addClient(new rho::gcm::GcmPushClient());

    if (rho::sync::RhoconnectClientManager::haveRhoconnectClientImpl()) {
        rho::sync::RhoconnectClientManager::clientRegisterSetDevicePin("");
    }
    
    rho::gcm::GcmPushClient::GcmPushRegister();
}

//----------------------------------------------------------------------------------------------------------------------

namespace rho { namespace gcm {

IMPLEMENT_LOGCLASS(GcmPushClient, "GcmPushClient");

//----------------------------------------------------------------------------------------------------------------------

const char* const GcmPushClient::s_GCM_FACADE_CLASS = "com.rhomobile.rhodes.gcm.GCMFacade";

//----------------------------------------------------------------------------------------------------------------------
void GcmPushClient::GcmPushRegister()
{
    LOG(TRACE) + "GcmPushRegister()";

    JNIEnv *env = jnienv();

    static jclass cls = rho_find_class(env, s_GCM_FACADE_CLASS);
    if (!cls) {
        LOG(ERROR) + "Cannot find class: " + s_GCM_FACADE_CLASS;
        return;
    }

    static jmethodID mid = env->GetStaticMethodID(cls, "Register", "()V");
    if (!mid) {
        LOG(ERROR) + "Cannot get " + s_GCM_FACADE_CLASS + ".Register() method";
        return;
    }

    env->CallStaticVoidMethod(cls, mid);
}

//----------------------------------------------------------------------------------------------------------------------
void GcmPushClient::GcmPushUnregister()
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
}

//----------------------------------------------------------------------------------------------------------------------
void GcmPushClient::init()
{
    if ( sync::RhoconnectClientManager::haveRhoconnectClientImpl() && (!sync::RhoconnectClientManager::clientRegisterHaveInstance())) {
        sync::RhoconnectClientManager::clientRegisterCreate();
    }
}

//----------------------------------------------------------------------------------------------------------------------

void GcmPushClient::getDeviceId(CMethodResult& result)
{
    if(m_hashProps.containsKey("deviceId"))
    {
        getProperty("deviceId", result);
    }
    else
    {
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

    getDeviceId(m_deviceIdResult);
}

//----------------------------------------------------------------------------------------------------------------------
bool GcmPushClient::callBack(const String& json)
{
    LOG(TRACE) + "GCM push notification: " + json;

    m_oResult.setJSON(json);

    return true;
}

}}

