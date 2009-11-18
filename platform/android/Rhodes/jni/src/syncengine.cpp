#include "JNIRhodes.h"

#include <sync/SyncThread.h>
#include "RhoClassFactory.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoSync"

RHO_GLOBAL void rho_sync_create()
{
    rho::sync::CSyncThread::Create(new rho::common::CRhoClassFactory);
}

