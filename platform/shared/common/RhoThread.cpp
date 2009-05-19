#include "RhoThread.h"
#include "IRhoClassFactory.h"

namespace rho {
namespace common {

CRhoThread::CRhoThread(IRhoClassFactory* factory)
{
    m_pImpl = factory->createThreadImpl();
}

void CRhoThread::start()
{
    m_pImpl->start(this);
}

}
}
