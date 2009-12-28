#pragma once

#include "common/ISystemInfo.h"

extern "C" int rho_sys_get_screen_width();
extern "C" int rho_sys_get_screen_height();

namespace rho{
namespace common{

class CSystemInfoImpl : public ISystemInfo
{
public:
    String getPlatform() { return "Iphone"; }
    int getScreenWidth() { return rho_sys_get_screen_width(); }
    int getScreenHeight() { return rho_sys_get_screen_height(); }
};

}
}
