#pragma once

namespace rho {
namespace net{
    struct INetRequest;
}

namespace common {

struct IRhoClassFactory
{
    virtual ~IRhoClassFactory(void)=0;

    virtual net::INetRequest* createNetRequest() = 0;
};

}
}