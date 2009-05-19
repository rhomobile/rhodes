#pragma once

namespace rho{
namespace common{

struct IRhoRunnable
{
    virtual void run() = 0;
};

struct IRhoThreadImpl
{
    virtual ~IRhoThreadImpl(void){}

    virtual void start(IRhoRunnable* pRunnable) = 0;
    virtual void stop(unsigned int nTimeoutToKill) = 0;
    virtual void wait(unsigned int nTimeout) = 0;
    virtual void stopWait() = 0;
};

}
}
