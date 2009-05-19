#pragma once

#include "common/IRhoThreadImpl.h"

namespace rho{
namespace common{

class CRhoThreadImpl : public IRhoThreadImpl
{
public:
    virtual void start(IRhoRunnable* pRunnable);
    virtual void stop(unsigned int nTimeoutToKill);
    virtual void wait(unsigned int nTimeout);
    virtual void stopWait();
};

}
}
