#include "RhoClassfactory.h"
#include "sync/SyncThread.h"
#include "sync/ClientRegister.h"

static rho::common::CRhoClassFactory g_oRhoClassFactory;
rho::common::IRhoClassFactory* rho_get_RhoClassFactory()
{
    return &g_oRhoClassFactory;
}

extern "C" {
	
void rho_clientregister_create(const char* szDevicePin)
{
	rho::sync::CClientRegister::Create(szDevicePin);
}
	
};

