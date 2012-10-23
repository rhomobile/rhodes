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
#include "common/RhodesApp.h"
#include "sync/ClientRegister.h"
#include "sync/ILoginListener.h"

#include "logging/RhoLog.h"

#include "gcmpushclient.h"
#include "com_rhomobile_rhodes_gcm_GCMRhoListener.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "GcmPushJNI"

//----------------------------------------------------------------------------------------------------------------------
extern "C" void Init_GCMPushClient()
{
    // create GCM push client
    RHODESAPP().addPushClient(new rho::gcm::GcmPushClient());
}

//----------------------------------------------------------------------------------------------------------------------
RHO_GLOBAL void JNICALL Java_com_rhomobile_rhodes_gcm_GCMRhoListener_nativeAddGCMPushClient (JNIEnv *, jclass)
{

    // create GCM push client
    RHODESAPP().addPushClient(new rho::gcm::GcmPushClient());
}


namespace rho { namespace gcm {

//----------------------------------------------------------------------------------------------------------------------
class GcmPushClient::SyncLoginListener : public sync::ILoginListener
{
    static const char* const s_GCM_FACADE_CLASS;

    static void GcmPushRegister();
    static void GcmPushUnregister();
public:
    virtual ~SyncLoginListener() {}
    virtual void onLogin(const String& user, const String& pass, const String& session) const;
    virtual void onLogout(const String& session) const;
};
//----------------------------------------------------------------------------------------------------------------------

const char* const GcmPushClient::SyncLoginListener::s_GCM_FACADE_CLASS = "com.rhomobile.rhodes.gcm.GCMFacade";

//----------------------------------------------------------------------------------------------------------------------
void GcmPushClient::SyncLoginListener::GcmPushRegister()
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
void GcmPushClient::SyncLoginListener::GcmPushUnregister()
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
void GcmPushClient::SyncLoginListener::onLogin(const String& user, const String& pass, const String& session) const
{
    sync::CClientRegister::Get()->setDevicehPin("");
    GcmPushRegister();
}
//----------------------------------------------------------------------------------------------------------------------
void GcmPushClient::SyncLoginListener::onLogout(const String& session) const
{
    sync::CClientRegister::Get()->setDevicehPin("");
    GcmPushUnregister();
}
//----------------------------------------------------------------------------------------------------------------------
const String GcmPushClient::s_Type = "gcm";

GcmPushClient::GcmPushClient()
{
    sync::CClientRegister::AddLoginListener(new SyncLoginListener());
}

//----------------------------------------------------------------------------------------------------------------------
void GcmPushClient::init()
{
    if(!sync::CClientRegister::getInstance())
    {
        sync::CClientRegister::Create();
    }
}

//----------------------------------------------------------------------------------------------------------------------
void GcmPushClient::setNotificationUrl(const String& callbackUrl, const String& callbackParam)
{
    m_strCallbackUrl = callbackUrl;
    m_strCallbackParam = callbackParam;
}

//----------------------------------------------------------------------------------------------------------------------
bool GcmPushClient::callNotification(const String& json, const String& data)
{
    String finalData = m_strCallbackParam;
    if(data.length() > 0)
    {
        if(finalData.length() > 0)
            finalData += '&';
        finalData += data;
    }
    if(m_strCallbackUrl.length() > 0)
    {
        return RHODESAPP().callPushCallbackWithJsonBody(m_strCallbackUrl, json, finalData);
    }

    return false;
}

}}

