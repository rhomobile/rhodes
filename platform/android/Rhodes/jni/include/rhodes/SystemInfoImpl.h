#ifndef RHO_SYSTEM_INFO_1e6790d99aed4df58608cf8626773edf
#define RHO_SYSTEM_INFO_1e6790d99aed4df58608cf8626773edf

#include <common/ISystemInfo.h>

extern "C" int rho_sys_get_screen_width();
extern "C" int rho_sys_get_screen_height();

namespace rho
{
namespace common
{

class CSystemInfoImpl : public ISystemInfo
{
public:
    String getPlatform() {return "Android";}
	int getScreenWidth() {return rho_sys_get_screen_width();}
	int getScreenHeight() {return rho_sys_get_screen_height();}
};

} // namespace common
} // namespace rho

#endif // RHO_SYSTEM_INFO_1e6790d99aed4df58608cf8626773edf

