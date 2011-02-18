#pragma once

namespace rho {
namespace net{
    struct INetRequest;
    struct ISSL;
}

struct ICallbackObject
{
    virtual ~ICallbackObject(){};
    virtual unsigned long getObjectValue() = 0;
};

namespace common {
struct IRhoThreadImpl;
struct IRhoCrypt;

struct IRhoClassFactory
{
    virtual ~IRhoClassFactory(void){;}

    virtual net::INetRequest* createNetRequest() = 0;
    virtual IRhoThreadImpl* createThreadImpl() = 0;
    virtual net::ISSL* createSSLEngine() = 0;

    virtual IRhoCrypt* createRhoCrypt() = 0;
};

}
}

rho::common::IRhoClassFactory* rho_get_RhoClassFactory();

