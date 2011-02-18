#include "RhoThread.h"
#include "IRhoClassFactory.h"

namespace rho {
namespace common {

CRhoThread::CRhoThread()
{
    m_nState = TS_NONE;
    m_pImpl = rho_get_RhoClassFactory()->createThreadImpl();
}

void CRhoThread::start(EPriority ePriority)
{
    if ( !isAlive() )
    {
        m_pImpl->start(this, ePriority);

        m_nState = TS_RUNNING;
    }
}

}
}
