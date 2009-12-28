#pragma once

namespace rho {
namespace common {

struct ISystemInfo
{
    virtual ~ISystemInfo(void){;}

    virtual String getPlatform() = 0;
    virtual int getScreenWidth() = 0;
    virtual int getScreenHeight() = 0;
};

}
}
