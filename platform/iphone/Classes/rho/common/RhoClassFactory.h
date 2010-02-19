#pragma once

#include "common/IRhoClassFactory.h"
#include "net/CURLNetRequest.h"
#include "common/PosixThreadImpl.h"
#include "SystemInfoImpl.h"
#include "../net/sslimpl.h"

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
    ISystemInfo* createSystemInfo()
    {
        return new CSystemInfoImpl;
    }
    net::ISSL* createSSLEngine()
    {
        return new net::SSLImpl();
    }
};

}
}
