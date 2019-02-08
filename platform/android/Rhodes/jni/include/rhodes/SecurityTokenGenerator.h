#ifndef SECURITY_TOKEN_GENERATOR_H
#define SECURITY_TOKEN_GENERATOR_H

#include <jni.h>
#include "logging/RhoLog.h"
#include "common/ISecurityTokenGenerator.h"

namespace rho {
namespace common{

class SecurityTokenGenerator : public ISecurityTokenGenerator
{
    DEFINE_LOGCLASS;
public:
    SecurityTokenGenerator();
    ~SecurityTokenGenerator();
    virtual const rho::String& getSecurityToken() const override;

private:
    jclass cls;
    jmethodID mibGetTokenString;
    jmethodID mibGetSecureAgent;
    jobject generator_object;
    mutable rho::String currentTokens;
};

}
}

#endif // SECURITY_TOKEN_GENERATOR_H