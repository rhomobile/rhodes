#include "stdafx.h"

#include "RhoClassfactory.h"
#include "sync/SyncThread.h"
#include "sync/ClientRegister.h"
#include "RhoCryptImpl.h"

namespace rho{
namespace common{
IRhoClassFactory* rho_impl_createClassFactory()
{
    return new rho::common::CRhoClassFactory;
}

IRhoCrypt* CRhoClassFactory::createRhoCrypt()
{
    return new CRhoCryptImpl();
}

}
}

extern "C" {
void rho_clientregister_create(const char* szDevicePin)
{
    rho::sync::CClientRegister::Create(rho::common::rho_impl_createClassFactory(), szDevicePin);
}

void rho_clientregister_destroy()
{
    rho::sync::CClientRegister::Destroy();
}

};

