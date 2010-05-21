#include "RhoThread.h"
#include "IRhoClassFactory.h"

namespace rho {
namespace common {

CRhoThread::CRhoThread(IRhoClassFactory* factory)
{
    m_isInWaitState = false;
    m_bStop = false;
    m_pImpl = factory->createThreadImpl();
}

void CRhoThread::start(EPriority ePriority)
{
    m_pImpl->start(this, ePriority);
}

}
}
