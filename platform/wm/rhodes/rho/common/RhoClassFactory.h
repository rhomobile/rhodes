#pragma once

#include "common/IRhoClassFactory.h"
#include "../net/NetRequest.h"
#include "RhoThreadImpl.h"

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

    net::ISSL* createSSLEngine()
    {
        return null;
    }

};

}
}

