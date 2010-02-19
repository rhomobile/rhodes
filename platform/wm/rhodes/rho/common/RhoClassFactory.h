#pragma once

#include "common/IRhoClassFactory.h"
#include "../net/NetRequest.h"
#include "RhoThreadImpl.h"
#include "SystemInfoImpl.h"

namespace rho {
namespace common {

class CRhoClassFactory : public IRhoClassFactory
{
public:
    virtual ~CRhoClassFactory(void){}
    net::INetRequest* createNetRequest()
    {
        return new net::CNetRequest();
    }

    virtual IRhoThreadImpl* createThreadImpl()
    {
        return new CRhoThreadImpl;
    }

    virtual ISystemInfo* createSystemInfo()
    {
        return new CSystemInfoImpl;
    }

    net::ISSL* createSSLEngine()
    {
        return null;
    }

};

}
}

