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

#ifndef RHOPUSHMANAGER_H_
#define RHOPUSHMANAGER_H_

#include "common/RhoStd.h"
#include "logging/RhoLog.h"
#include "common/push/IRhoPushClient.h"

namespace rho { namespace common {

class CRhodesApp;

class PushManager {
public:
    DEFINE_LOGCLASS;

public:
    PushManager(CRhodesApp& rhoApp) : m_rhoApp(rhoApp) {}

    IRhoPushClient* getDefaultClient();

    void addClient(IRhoPushClient* pClient);

    void initClients();
    void setNotificationUrl(const String& url, const String& urlParams, const String& pushType);
    void setNotificationUrl(const String& url, const String& urlParams);
    bool callNotification(const String& pushType, const String& json, const String& data);

private:
    IRhoPushClient* getClient(const String& pushType);

private:
    CRhodesApp& m_rhoApp;
    VectorPtr<IRhoPushClient*> m_Clients;
};

}}


#endif /* RHOPUSHMANAGER_H_ */
