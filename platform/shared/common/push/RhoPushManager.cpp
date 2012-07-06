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
#include "logging/RhoLog.h"

#include "common/push/RhoPushManager.h"
#include "common/push/IRhoPushClient.h"


namespace rho { namespace common {


IMPLEMENT_LOGCLASS(RhoPushManager, "RhoPush");

//----------------------------------------------------------------------------------------------------------------------
RhoPushManager::~RhoPushManager()
{
    for(Vector<IRhoPushClient*>::iterator I = m_Clients.begin(); I != m_Clients.end(); ++I)
    {
        delete *I;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void RhoPushManager::addClient(IRhoPushClient* pClient)
{
    if(getClient(pClient->getType()))
    {
        LOG(ERROR) + "Cannot add push client ("+pClient->getType()+"): already exist";
        return;
    }
    m_Clients.addElement(pClient);
    LOG(INFO) + "Push client has added ("+pClient->getType()+")";
}

//----------------------------------------------------------------------------------------------------------------------
void RhoPushManager::registerClient(const String& url, const String& urlParams, const String& pushType)
{
    IRhoPushClient* pClient = getClient(pushType);
    if(pClient)
    {
        pClient->doRegister(url, urlParams);
        LOG(INFO) + "Push client registration has requested ("+pushType+")";
    }
    else
    {
        LOG(WARNING) + "Cannot register Push client ("+pushType+"): not exist";
    }
}

//----------------------------------------------------------------------------------------------------------------------
void RhoPushManager::unregisterClient(const String& pushType)
{
    IRhoPushClient* pClient = getClient(pushType);
    if(pClient)
    {
        pClient->doUnregister();
        LOG(INFO) + "Push client unregistration has requested ("+pushType+")";
    }
    else
    {
        LOG(WARNING) + "Cannot unregister Push client ("+pushType+"): not exist";
    }
}

//----------------------------------------------------------------------------------------------------------------------
//void RhoPushManager::checkClientRegistered(const String& pushType)
//{
//    IRhoPushClient* pClient = getClient(pushType);
//    if(pClient)
//    {
//        pClient->checkRegistered(url);
//        LOG(INFO) + "Push client registration check has requested ("+pushType+")";
//    }
//    else
//    {
//        LOG(WARNING) + "Cannot check Push client registration ("+pushType+"): not exist";
//    }
//}

//----------------------------------------------------------------------------------------------------------------------
void RhoPushManager::registerAllClients(const String& url, const String& urlParams)
{
    for(Vector<IRhoPushClient*>::iterator I = m_Clients.begin(); I != m_Clients.end(); ++I)
    {
        (*I)->doRegister(url, urlParams);
        LOG(INFO) + "Push client registration has requested ("+(*I)->getType()+")";
    }
}

//----------------------------------------------------------------------------------------------------------------------
void RhoPushManager::unregisterAllClients()
{
    for(Vector<IRhoPushClient*>::iterator I = m_Clients.begin(); I != m_Clients.end(); ++I)
    {
        (*I)->doUnregister();
        LOG(INFO) + "Push client unregistration has requested ("+(*I)->getType()+")";
    }
}

//----------------------------------------------------------------------------------------------------------------------
IRhoPushClient* RhoPushManager::getClient(const String& pushType)
{
    for(Vector<IRhoPushClient*>::iterator I = m_Clients.begin(); I != m_Clients.end(); ++I)
    {
        if((*I)->getType() == pushType) return *I;
    }
    return 0;
}


}}

