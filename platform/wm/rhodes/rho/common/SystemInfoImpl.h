#pragma once

#include "common/ISystemInfo.h"

extern "C" int rho_sys_get_screen_width();
extern "C" int rho_sys_get_screen_height();
extern VALUE rho_sys_get_property(char* property);

namespace rho{
namespace common{

class CSystemInfoImpl : public ISystemInfo
{
public:
    String getPlatform() { return "Wince"; }
	int getScreenWidth() { return rho_sys_get_screen_width(); }
	int getScreenHeight() { return rho_sys_get_screen_height(); }
};

}
}
