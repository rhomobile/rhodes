#pragma once

#include "common/ISystemInfo.h"

namespace rho{
namespace common{

class CSystemInfoImpl : public ISystemInfo
{
public:
    virtual String getPlatform(){ return "Iphone"; }
};

}
}
