#pragma once

#ifdef __cplusplus

#include "logging/RhoLog.h"
#include "common/RhoThread.h"
#include "RhoClassfactory.h"
#include "net/INetRequest.h"

namespace rho{
namespace common{

class CClientRegister : public common::CRhoThread
{
    DEFINE_LOGCLASS;
public:
	static CClientRegister* create(IRhoClassFactory* factory,const char* device_pin);
	static CClientRegister* getInstance() { return m_pInstance; }
	
	virtual void run();	

private:	
	CClientRegister(common::IRhoClassFactory* factory,const char* device_pin);
	
	static CClientRegister* m_pInstance;
	net::INetRequest* m_NetRequest;
	String m_token;
	bool   m_set;

	bool post_token();
};

}
}

#endif //__cplusplus

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
	
void rho_client_register(const char* device_pin);

#ifdef __cplusplus
};
#endif //__cplusplus
