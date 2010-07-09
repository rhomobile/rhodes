#pragma once

#include "RhoStd.h"
#include "IRhoThreadImpl.h"
#include "AutoPointer.h"

namespace rho {
namespace common {

struct IRhoClassFactory;
class CRhoThread : public IRhoRunnable
{
    CAutoPtr<IRhoThreadImpl> m_pImpl;
    boolean m_isInWaitState;
    boolean m_bStop;
public:

    CRhoThread(IRhoClassFactory* factory);
    virtual ~CRhoThread(void){}

    virtual void start(EPriority ePriority);
    virtual void stop(unsigned int nTimeoutToKill){ m_bStop = true; m_pImpl->stop(nTimeoutToKill); }
    virtual void wait(unsigned int nTimeout){ m_isInWaitState = true; m_pImpl->wait(nTimeout); m_isInWaitState = false; }
    virtual void stopWait(){ if (m_isInWaitState) m_pImpl->stopWait(); }
    virtual void sleep(unsigned int nTimeout){ m_pImpl->sleep(nTimeout); }

    boolean isStopped(){return m_bStop;}
};

}
}
