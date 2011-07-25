#include "common/RhoPort.h"
#include <e32std.h>
#include <e32base.h>
extern "C" {
void* rho_nativethread_start()
{
    CActiveScheduler* Scheduler = new ( ELeave ) CActiveScheduler;
    CleanupStack::PushL( Scheduler );
    CActiveScheduler::Install( Scheduler );
    return Scheduler;
}

void rho_nativethread_end(void* pData)
{
    if(pData)
        CleanupStack::PopAndDestroy(pData);
}

} //extern "C"
