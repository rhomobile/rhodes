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

#include "common/RhoStd.h"
#include "common/RhodesAppBase.h"
#include "logging/RhoLog.h"

#include "common/push/RhoPushManager.h"
#include "common/push/IRhoPushClient.h"

namespace rho { namespace common {

IMPLEMENT_LOGCLASS(PushManager, "RhoPush");


//----------------------------------------------------------------------------------------------------------------------
IRhoPushClient* PushManager::getDefaultClient()
{
    return m_Clients.empty() ? 0 : m_Clients.front();
}

//----------------------------------------------------------------------------------------------------------------------
void PushManager::addClient(IRhoPushClient* pClient)
{
    if(getClient(pClient->getType()))
    {
        LOG(ERROR) + "Cannot add push client ("+pClient->getType()+"): already exist";
        return;
    }
    m_Clients.addElement(pClient);
    LOG(INFO) + "Push client has added ("+pClient->getType()+")";
    executeCallBacks();
}

//----------------------------------------------------------------------------------------------------------------------
void PushManager::initClients()
{
    for(Vector<IRhoPushClient*>::iterator I = m_Clients.begin(); I != m_Clients.end(); ++I)
    {
        (*I)->init();
    }
    executeCallBacks();
}

//----------------------------------------------------------------------------------------------------------------------
void PushManager::setNotificationUrl(const String& pushType, const String& url, const String& urlParams)
{
    IRhoPushClient* pClient = getClient(pushType);
    if(pClient)
    {
        pClient->setNotificationUrl(url, urlParams);
        LOG(INFO) + "Push notification URL is set for: \'"+pushType+"\'";
    }
    else
    {
        LOG(WARNING) + "Cannot set push notification:  \'"+pushType+"\' does not exist";
    }
}

//----------------------------------------------------------------------------------------------------------------------
void PushManager::setNotificationUrl(const String& url, const String& urlParams)
{
    for(Vector<IRhoPushClient*>::iterator I = m_Clients.begin(); I != m_Clients.end(); ++I)
    {
        (*I)->setNotificationUrl(url, urlParams);
        LOG(INFO) + "Push notification URL is set: \'"+(*I)->getType()+"\'";
    }
}

//----------------------------------------------------------------------------------------------------------------------
bool PushManager::callNotification(const String& type, const String& json, const String& data)
{
    if(type.length() > 0)
    {
        LOG(INFO) + "Push message: " + type;
        IRhoPushClient* pClient = getClient(type);
        if(pClient)
        {
            return pClient->callNotification(json, data);
        } else
        {
            pushQueue.push(QueueType(type,json,data));
            if (pushQueue.size() > 1024) pushQueue.pop();

            LOG(WARNING) + "Wrong push message type: " + type;
            return false;
        }
    } else
    {
        LOG(INFO) + "Push message: legacy";
        return m_rhoAppBase.callPushCallback(data);
    }
}

//----------------------------------------------------------------------------------------------------------------------
IRhoPushClient* PushManager::getClient(const String& pushType)
{
    for(Vector<IRhoPushClient*>::iterator I = m_Clients.begin(); I != m_Clients.end(); ++I)
    {
        if((*I)->getType() == pushType) return *I;
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------


void PushManager::executeCallBacks()
{
    std::queue<QueueType> localQueue = pushQueue;
    while (!pushQueue.empty()){pushQueue.pop();}
    while (!localQueue.empty()){
        QueueType item = localQueue.front();
        callNotification(item.pushType, item.json, item.data);
        localQueue.pop();
    }

}

}}

