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

#include "common/RhoConf.h"
#include "Push.h"
#include "sync/RhoconnectClientManager.h"
#include "sync/ILoginListener.h"
#import "Rhodes.h"
#import "PushNotificationsReceiver.h"



#include "logging/RhoLog.h"

#include "applepushclient.h"


extern "C" void Init_ApplePushClient();

//----------------------------------------------------------------------------------------------------------------------
void Init_ApplePushClient()
{
    RAWTRACEC("Init_ApplePushClient", "creating Apple client >>>>>>>>>>>>>>");

    rho::apple::ApplePushClient* pClient = new rho::apple::ApplePushClient();

    RAWTRACEC("Init_ApplePushClient", "adding Apple client >>>>>>>>>>>>>>>>");

    rho::push::CPushManager::getInstance()->addClient(pClient);
    
    LOG(TRACE) + "request APNS registration >>>>>>>>>>>>>>>>";
    pClient->doRegister();
}
//----------------------------------------------------------------------------------------------------------------------

namespace rho {
    
namespace apple {

IMPLEMENT_LOGCLASS(ApplePushClient, "ApplePushClient");

void ApplePushClient::doRegister()
{
    LOG(TRACE) + "ApplePushRegister()";
    [[Rhodes sharedInstance] registerForPushNotifications: [PushNotificationsReceiver sharedInstance] ];
}


void ApplePushClient::doUnregister()
{
    LOG(TRACE) + "ApplePushUnregister()";

}
//----------------------------------------------------------------------------------------------------------------------
const String ApplePushClient::s_Type = "apple";

ApplePushClient::ApplePushClient()
{
    m_CallbackIsValid = false;
    m_HasSavedMessage = false;
    m_SavedMessage = "";
    CMethodResult result;
    setProperty("id", s_Type, result);
    setProperty("type", IPush::PUSH_TYPE_NATIVE, result);
}

//----------------------------------------------------------------------------------------------------------------------

void ApplePushClient::getDeviceId(CMethodResult& result)
{
    String deviceId = m_hashProps["deviceId"];

    if(deviceId.length() != 0)
    {
        LOG(TRACE) + "APNS deviceId: " + deviceId;
        result.set(deviceId);
    }
    else
    {
        LOG(TRACE) + "Still waiting for APNS deviceId";
        m_deviceIdResult = result;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void ApplePushClient::startNotifications(CMethodResult& result)
{
    LOG(TRACE) + "Start APNS push notifications";
    m_oResult = result;
    m_CallbackIsValid = true;
    if (m_HasSavedMessage) {
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            m_oResult.setJSON(m_SavedMessage);
            m_SavedMessage = "";
        });
    }
    m_HasSavedMessage = false;
}

//----------------------------------------------------------------------------------------------------------------------
void ApplePushClient::stopNotifications(CMethodResult& result)
{
    LOG(TRACE) + "Stop APNS push notifications";
    m_oResult = CMethodResult();
    m_CallbackIsValid = false;
    m_HasSavedMessage = false;
    m_SavedMessage = "";
}

//----------------------------------------------------------------------------------------------------------------------
void ApplePushClient::setDeviceId(const String& deviceId)
{
    CMethodResult result;
    setProperty("deviceId", deviceId, result);

    LOG(TRACE) + "creating client register";
    rho::sync::RhoconnectClientManager::clientRegisterCreate(deviceId.c_str());

    getDeviceId(m_deviceIdResult);
    m_deviceIdResult = CMethodResult();

}

//----------------------------------------------------------------------------------------------------------------------
bool ApplePushClient::callBack(const String& json)
{
    LOG(TRACE) + "APNS push notification: " + json;

    if (m_CallbackIsValid) {
        m_oResult.setJSON(json);
    }
    else {
        m_SavedMessage = json;
        m_HasSavedMessage = true;
    }
    return true;
}

}}

