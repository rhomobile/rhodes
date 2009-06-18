#pragma once

namespace rho {
namespace net{
    struct INetRequest;
}

namespace common {
struct IRhoThreadImpl;

struct IRhoClassFactory
{
    virtual ~IRhoClassFactory(void){;}

    virtual net::INetRequest* createNetRequest() = 0;
    virtual IRhoThreadImpl* createThreadImpl() = 0;
};

}
}