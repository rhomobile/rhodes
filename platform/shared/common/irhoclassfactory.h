#pragma once

namespace rho {
namespace net{
    struct INetRequest;
}

namespace common {

struct IRhoClassFactory
{
    virtual ~IRhoClassFactory(void){;}

    virtual net::INetRequest* createNetRequest() = 0;
};

}
}