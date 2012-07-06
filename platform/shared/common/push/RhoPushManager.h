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

namespace rho { namespace common {

class IRhoPushClient;
class RhoPushManager {
public:
    RhoPushManager() {}
    ~RhoPushManager();

    DEFINE_LOGCLASS;

public:
    void addClient(IRhoPushClient* pClient);

    void registerClient(const String& url, const String& urlParams, const String& pushType);
    void unregisterClient(const String& pushType);
    //void checkClientRegistered(const String& pushType);

    void registerAllClients(const String& url, const String& urlParams);
    void unregisterAllClients();

private:
    IRhoPushClient* getClient(const String& pushType);
//    {
//        for(Vector<IRhoPushClient*>::iterator I = m_Clients.begin(); I != m_Clients.end(); ++I)
//        {
//            if((*I)->getType() == pushType) return *I;
//        }
//        return 0;
//    }
//    const IRhoPushClient* getClient(const String& pushType) const
//    {
//        for(Vector<IRhoPushClient*>::iterator I = m_Clients.begin(); I != m_Clients.end(); ++I)
//        {
//            if((*I)->getType() == pushType) return *I;
//        }
//        return 0;
//    }
private:
    Vector<IRhoPushClient*> m_Clients;
};

}}


#endif /* RHOPUSHMANAGER_H_ */
