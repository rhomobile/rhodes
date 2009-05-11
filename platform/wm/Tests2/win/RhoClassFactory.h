#pragma once

#include "common/IRhoClassFactory.h"
#include "NetRequest.h"

namespace rho {

class CRhoClassFactory : public common::IRhoClassFactory
{
public:
    virtual ~CRhoClassFactory(void){}
    INetRequest* createNetRequest()
    {
        return new CNetReqest();
    }
};

}