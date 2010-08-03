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
    static const int TS_NONE = 0;
    static const int TS_WAIT = 1;
    static const int TS_STOPPING = 2;
    static const int TS_RUNNING = 4;

    int m_nState;
public:

    CRhoThread(IRhoClassFactory* factory);
    virtual ~CRhoThread(void){};

    virtual void start(EPriority ePriority);
    virtual void stop(unsigned int nTimeoutToKill){ m_nState |= TS_STOPPING; if (m_nState&TS_RUNNING) m_pImpl->stop(nTimeoutToKill); m_nState &= ~TS_STOPPING; }
    virtual void wait(unsigned int nTimeout){ m_nState |= TS_WAIT; if ((m_nState&TS_RUNNING) && !(m_nState&TS_STOPPING) ) m_pImpl->wait(nTimeout); m_nState &= ~TS_WAIT; }
    virtual void stopWait(){ if (isWaiting()) m_pImpl->stopWait(); }
    virtual void sleep(unsigned int nTimeout){ m_pImpl->sleep(nTimeout); }
    virtual void run() = 0;
    virtual void runObject()
    {
        run();
        m_nState = TS_NONE;
    }

    boolean isStopping(){return (m_nState&TS_STOPPING) != 0;}
    boolean isWaiting(){return (m_nState&TS_WAIT) != 0;}
    boolean isRunning(){return (m_nState&TS_RUNNING) != 0;}

};

}
}
