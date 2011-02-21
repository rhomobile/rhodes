#include <net/CURLNetRequest.h>
#include <common/PosixThreadImpl.h>

#include "rhodes/RhoClassFactory.h"
#include "rhodes/sslimpl.h"
#include "rhodes/rhocryptimpl.h"

namespace rho
{
namespace common
{

net::INetRequestImpl* CRhoClassFactory::createNetRequestImpl()
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
    return new CRhoCryptImpl;
}

} // namespace common
} // namespace rho

