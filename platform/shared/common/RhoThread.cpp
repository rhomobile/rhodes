#include "RhoThread.h"
#include "IRhoClassFactory.h"

namespace rho {
namespace common {

CRhoThread::CRhoThread(IRhoClassFactory* factory)
{
    m_nState = TS_NONE;
    m_pImpl = factory->createThreadImpl();
}

void CRhoThread::start(EPriority ePriority)
{
    if ( !isRunning() )
    {
        m_pImpl->start(this, ePriority);

        m_nState = TS_RUNNING;
    }
}

}
}
