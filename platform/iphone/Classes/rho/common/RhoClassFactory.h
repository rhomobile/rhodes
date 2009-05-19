#pragma once

#include "common/IRhoClassFactory.h"
#include "../net/NetRequest.h"
#include "RhoThreadImpl.h"

namespace rho {
namespace common {
		
class CRhoClassFactory : public common::IRhoClassFactory
{
public:
    virtual ~CRhoClassFactory(void){}
    net::INetRequest* createNetRequest()
    {
        return new net::CNetRequest();
    }
    virtual common::IRhoThreadImpl* createThreadImpl()
    {
        return new common::CRhoThreadImpl;
    }

};

}
}
