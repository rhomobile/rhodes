#include <net/CURLNetRequest.h>
#include <common/PosixThreadImpl.h>

#include "rhodes/RhoClassFactory.h"
#include "rhodes/SystemInfoImpl.h"
#include "rhodes/sslimpl.h"

namespace rho
{
namespace common
{

net::INetRequest *CRhoClassFactory::createNetRequest()
{
    return new net::CURLNetRequest();
}

IRhoThreadImpl *CRhoClassFactory::createThreadImpl()
{
    return new CPosixThreadImpl();
}

ISystemInfo *CRhoClassFactory::createSystemInfo()
{
    return new CSystemInfoImpl();
}

net::ISSL *CRhoClassFactory::createSSLEngine()
{
    return new net::SSLImpl();
}

} // namespace common
} // namespace rho

