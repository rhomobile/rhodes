#pragma once

#include "common/IRhoClassFactory.h"
#include "net/CURLNetRequest.h"
#include "RhoThreadImpl.h"
#include "SystemInfoImpl.h"

namespace rho {
namespace common {
		
class CRhoClassFactory : public common::IRhoClassFactory
{
public:
    virtual ~CRhoClassFactory(void){}
    net::INetRequest* createNetRequest()
    {
        return new net::CURLNetRequest();
    }
    virtual common::IRhoThreadImpl* createThreadImpl()
    {
        return new common::CRhoThreadImpl;
    }
    virtual ISystemInfo* createSystemInfo()
    {
        return new CSystemInfoImpl;
    }
};

}
}
