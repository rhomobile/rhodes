#include "RhoClassfactory.h"
#include "sync/SyncThread.h"

extern "C" {
void rho_sync_create()
{
    rho::sync::CSyncThread::Create(new rho::common::CRhoClassFactory);
}
};

