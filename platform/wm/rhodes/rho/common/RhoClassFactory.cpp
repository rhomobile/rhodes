#include "stdafx.h"

#include "RhoClassfactory.h"
#include "sync/SyncThread.h"
#include "sync/ClientRegister.h"

extern "C" {
void rho_sync_create()
{
    rho::sync::CSyncThread::Create(new rho::common::CRhoClassFactory);
}

void rho_clientregister_create()
{
    rho::sync::CClientRegister::Create(new rho::common::CRhoClassFactory, "PIN_1");
}

};

