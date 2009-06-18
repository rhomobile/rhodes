#pragma once

#include "IRhoThreadImpl.h"
#include "AutoPointer.h"

namespace rho {
namespace common {

struct IRhoClassFactory;
class CRhoThread : public IRhoRunnable
{
    CAutoPtr<IRhoThreadImpl> m_pImpl;
public:

    CRhoThread(IRhoClassFactory* factory);
    virtual ~CRhoThread(void){}

    virtual void start(EPriority ePriority);
    virtual void stop(unsigned int nTimeoutToKill){ m_pImpl->stop(nTimeoutToKill); }
    virtual void wait(unsigned int nTimeout){ m_pImpl->wait(nTimeout); }
    virtual void stopWait(){ m_pImpl->stopWait(); }
};

}
}
