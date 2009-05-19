#pragma once

#include "common/IRhoThreadImpl.h"
#include "logging/RhoLog.h"

namespace rho{
namespace common{

class CRhoThreadImpl : public IRhoThreadImpl
{
    DEFINE_LOGCLASS;

	pthread_cond_t m_condSync;
	common::CMutex m_mxSync;
	pthread_t      m_thread;

public:
	CRhoThreadImpl();
	
    virtual void start(IRhoRunnable* pRunnable, IRhoRunnable::EPriority ePriority );
    virtual void stop(unsigned int nTimeoutToKill);
    virtual void wait(unsigned int nTimeout);
    virtual void stopWait();
};

}
}
