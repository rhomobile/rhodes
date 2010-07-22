#pragma once

#include "common/IRhoThreadImpl.h"

namespace rho{
namespace common{

class CRhoThreadImpl : public IRhoThreadImpl
{
    DEFINE_LOGCLASS;

    HANDLE m_hAwakeEvent;
    HANDLE m_hThread;
public:
    CRhoThreadImpl();
    ~CRhoThreadImpl();

    virtual void start(IRhoRunnable* pRunnable, IRhoRunnable::EPriority ePriority);
    virtual void stop(unsigned int nTimeoutToKill);
    virtual void wait(unsigned int nTimeout);
    virtual void stopWait();
    virtual void sleep(unsigned int nTimeout);

private:
    void setThreadPriority(IRhoRunnable::EPriority ePriority);
};

}
}
