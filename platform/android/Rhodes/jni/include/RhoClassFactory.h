#ifndef RHO_CLASS_FACTORY_b15393f788404746b0b59f04829e14ea
#define RHO_CLASS_FACTORY_b15393f788404746b0b59f04829e14ea

#include <common/IRhoClassFactory.h>
#include <net/INetRequest.h>
#include <common/ISystemInfo.h>
#include <common/IRhoThreadImpl.h>

namespace rho
{
namespace common
{

class CRhoClassFactory : public IRhoClassFactory
{
public:
    virtual ~CRhoClassFactory() {}
    net::INetRequest *createNetRequest();
    IRhoThreadImpl *createThreadImpl();
    ISystemInfo *createSystemInfo();
};

} // namespace common
} // namespace rho

#endif // RHO_CLASS_FACTORY_b15393f788404746b0b59f04829e14ea

