#include "RhoClassFactory.h"
#include "NetRequest.h"
#include "RhoThreadImpl.h"
#include "SystemInfoImpl.h"

namespace rho
{
namespace common
{

net::INetRequest *CRhoClassFactory::createNetRequest()
{
    //return new net::CNetRequest();
    return 0;
}

IRhoThreadImpl *CRhoClassFactory::createThreadImpl()
{
    //return new CRhoThreadImpl();
    return 0;
}

ISystemInfo *CRhoClassFactory::createSystemInfo()
{
    return new CSystemInfoImpl();
}

} // namespace common
} // namespace rho

