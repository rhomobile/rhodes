#pragma once

#include "common/IRhoClassFactory.h"
#include "../net/NetRequestImpl.h"
#include "RhoThreadImpl.h"

namespace rho {
namespace common {

class CRhoClassFactory : public IRhoClassFactory
{
public:
    virtual ~CRhoClassFactory(void){}
    virtual net::INetRequestImpl* createNetRequestImpl()
    {
        return new net::CNetRequestImpl();
    }

    virtual IRhoThreadImpl* createThreadImpl()
    {
        return new CRhoThreadImpl;
    }

    net::ISSL* createSSLEngine()
    {
        return null;
    }

    virtual IRhoCrypt* createRhoCrypt();
};

}
}

