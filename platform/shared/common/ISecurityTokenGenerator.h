#pragma once

#include "common/RhoStd.h"

namespace rho{
namespace common{

class ISecurityTokenGenerator
{
public:
    virtual ~ISecurityTokenGenerator(){}
    virtual const rho::String& getSecurityToken() const = 0;
};

}
}
