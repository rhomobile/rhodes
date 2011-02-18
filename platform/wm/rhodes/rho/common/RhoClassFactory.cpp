#include "stdafx.h"

#include "RhoClassfactory.h"
#include "sync/SyncThread.h"
#include "sync/ClientRegister.h"
#include "RhoCryptImpl.h"

static rho::common::CRhoClassFactory g_oRhoClassFactory;
rho::common::IRhoClassFactory* rho_get_RhoClassFactory()
{
    return &g_oRhoClassFactory;
}

namespace rho{
namespace common{

IRhoCrypt* CRhoClassFactory::createRhoCrypt()
{
    return new CRhoCryptImpl();
}

}
}

extern "C" {
void rho_clientregister_create(const char* szDevicePin)
{
    rho::sync::CClientRegister::Create(szDevicePin);
}

void rho_clientregister_destroy()
{
    rho::sync::CClientRegister::Destroy();
}

};

