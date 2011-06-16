#pragma once

#include <time.h>
#include "common/IRhoClassFactory.h"
#include "net/CURLNetRequest.h"
#include "common/PosixThreadImpl.h"
// #include "net/iphone/sslimpl.h"
// #include "RhoCryptImpl.h"

namespace rho {
namespace common {
		
class CRhoClassFactory : public common::IRhoClassFactory
{
public:
    net::INetRequestImpl* createNetRequestImpl()
    {
        return new net::CURLNetRequest();
    }
    common::IRhoThreadImpl* createThreadImpl()
    {
        return new CPosixThreadImpl;
    }
    net::ISSL* createSSLEngine()
    {
        //TODO: createSSLEngine
        return NULL; // new net::SSLImpl();
    }

    IRhoCrypt* createRhoCrypt()
    {
        //TODO: createRhoCrypt
        return NULL; // new CRhoCryptImpl();
    }
};

}
}
