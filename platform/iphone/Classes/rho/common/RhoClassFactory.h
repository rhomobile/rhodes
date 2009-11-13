#pragma once

#include "common/IRhoClassFactory.h"
#include "net/CURLNetRequest.h"
#include "common/PosixThreadImpl.h"
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
        return new CPosixThreadImpl;
    }
    virtual ISystemInfo* createSystemInfo()
    {
        return new CSystemInfoImpl;
    }
};

}
}
