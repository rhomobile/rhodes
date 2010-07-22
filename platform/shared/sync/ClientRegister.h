#pragma once
#ifdef __cplusplus

#include "logging/RhoLog.h"
#include "common/RhoThread.h"
#include "common/ISystemInfo.h"
#include "net/INetRequest.h"
#include "common/IRhoClassFactory.h"

namespace rho{
namespace sync{

class CSyncEngine;

#define WAIT_BEFOREKILL_SECONDS  3
#define POLL_INTERVAL_SECONDS  60
#define POLL_INTERVAL_INFINITE (unsigned int)(-1)
#define DEFAULT_PUSH_PORT 100

class CClientRegister : public common::CRhoThread
{
    DEFINE_LOGCLASS;

	static CClientRegister* m_pInstance;
    common::CAutoPtr<common::ISystemInfo>  m_sysInfo;
	common::CAutoPtr<net::INetRequest>     m_NetRequest;
	String                                 m_strDevicePin;
    unsigned int                           m_nPollInterval;
public:
    static CClientRegister* Create(common::IRhoClassFactory* factory,const char* device_pin);
    static void Destroy();
	static CClientRegister* getInstance() { return m_pInstance; }
	
	virtual void run();	

    const String& getDevicePin(){return m_strDevicePin;}

    String getRegisterBody(const String& strClientID);

    void startUp();
private:
	CClientRegister(common::IRhoClassFactory* factory,const char* device_pin);
    ~CClientRegister();

    boolean doRegister(CSyncEngine& oSync);
    net::INetRequest& getNet(){ return *m_NetRequest; }

};

}
}
#endif //__cplusplus

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
	
void rho_clientregister_create(const char* szDevicePin);
void rho_clientregister_destroy();

#ifdef __cplusplus
};
#endif //__cplusplus
