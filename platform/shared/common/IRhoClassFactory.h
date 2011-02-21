#pragma once

namespace rho {
namespace net{
    struct INetRequestImpl;
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

    virtual IRhoThreadImpl* createThreadImpl() = 0;
    virtual net::ISSL* createSSLEngine() = 0;

    virtual IRhoCrypt* createRhoCrypt() = 0;

    virtual net::INetRequestImpl* createNetRequestImpl() = 0;
};

}
}

rho::common::IRhoClassFactory* rho_get_RhoClassFactory();

