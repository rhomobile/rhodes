#pragma once

#include "common/IRhoClassFactory.h"
#include "net/NetRequest.h"

namespace rho {

class CRhoClassFactory : public common::IRhoClassFactory
{
public:
    virtual ~CRhoClassFactory(void){}
    net::INetRequest* createNetRequest()
    {
        return new net::CNetRequest();
    }
};

}