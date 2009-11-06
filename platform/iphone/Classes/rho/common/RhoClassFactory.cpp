#include "RhoClassfactory.h"
#include "sync/SyncThread.h"
#include "sync/ClientRegister.h"

namespace rho{
namespace common{
	IRhoClassFactory* createClassFactory()
	{
		return new rho::common::CRhoClassFactory;
	}
}
}

extern "C" {
void rho_sync_create()
{
    rho::sync::CSyncThread::Create(new rho::common::CRhoClassFactory);
}
	
void rho_clientregister_create(const char* szDevicePin)
{
	rho::sync::CClientRegister::Create(new rho::common::CRhoClassFactory, szDevicePin);
}
	
};

