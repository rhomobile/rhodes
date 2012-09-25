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

#pragma once
#ifdef __cplusplus

#include "logging/RhoLog.h"
#include "common/RhoThread.h"
#include "common/RhoStd.h"
#include "net/INetRequest.h"
#include "common/IRhoClassFactory.h"

namespace rho{
namespace sync{

class ILoginListener;
class CSyncEngine;

#define WAIT_BEFOREKILL_SECONDS  3
#define POLL_INTERVAL_SECONDS  60
#define POLL_INTERVAL_INFINITE (unsigned int)(-1)
#define DEFAULT_PUSH_PORT 100

class CClientRegister : protected common::CRhoThread
{
    DEFINE_LOGCLASS;

    static CClientRegister* m_pInstance;
    static bool s_sslVerifyPeer;
    static VectorPtr<ILoginListener*> s_loginListeners;

    NetRequest              m_NetRequest;
    String                  m_strDevicePin;
    unsigned int            m_nPollInterval;
public:
    static void SetSslVerifyPeer(boolean b);
    static void AddLoginListener(ILoginListener* listener);
    static CClientRegister* Get();
    static CClientRegister* Create();
    static CClientRegister* Create(const String& devicePin);
    static void Stop();
    static void Destroy();
    static CClientRegister* getInstance() { return m_pInstance; }

    void setRhoconnectCredentials(const String& user, const String& pass, const String& session);
    void dropRhoconnectCredentials(const String& session);
    void setDevicehPin(const String& pin);
    const String& getDevicePin() const { return m_strDevicePin; }

    void startUp();

protected:
    virtual void run();

private:
    String getRegisterBody(const String& strClientID);

    CClientRegister();
    ~CClientRegister();

    void doStop();
    boolean doRegister(CSyncEngine& oSync);
    net::CNetRequestWrapper getNet(){ return getNetRequest(&m_NetRequest); }

};

}
}
#endif //__cplusplus

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
	
void rho_clientregister_create(const char* szDevicePin);

#ifdef __cplusplus
};
#endif //__cplusplus
