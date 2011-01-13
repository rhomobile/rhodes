#include <net/CURLNetRequest.h>
#include <common/PosixThreadImpl.h>

#include "rhodes/RhoClassFactory.h"
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

net::ISSL *CRhoClassFactory::createSSLEngine()
{
    return new net::SSLImpl();
}

IRhoCrypt *CRhoClassFactory::createRhoCrypt()
{
    return 0;
}

} // namespace common
} // namespace rho

