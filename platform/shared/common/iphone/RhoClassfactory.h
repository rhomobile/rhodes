#pragma once

#include "common/IRhoClassFactory.h"
#include "net/CURLNetRequest.h"
#include "common/PosixThreadImpl.h"
#include "net/iphone/sslimpl.h"

namespace rho {
namespace common {
		
class CRhoClassFactory : public common::IRhoClassFactory
{
public:
    net::INetRequest* createNetRequest()
    {
        return new net::CURLNetRequest();
    }
    common::IRhoThreadImpl* createThreadImpl()
    {
        return new CPosixThreadImpl;
    }
    net::ISSL* createSSLEngine()
    {
        return new net::SSLImpl();
    }
};

}
}
