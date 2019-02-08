#ifndef SECURITY_TOKEN_GENERATORIMPL_H
#define SECURITY_TOKEN_GENERATORIMPL_H

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
    mutable rho::String currentTokens;
};

}
}

#endif // SECURITY_TOKEN_GENERATORIMPL_H