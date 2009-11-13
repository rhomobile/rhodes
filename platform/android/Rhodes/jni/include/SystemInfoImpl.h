#ifndef RHO_SYSTEM_INFO_1e6790d99aed4df58608cf8626773edf
#define RHO_SYSTEM_INFO_1e6790d99aed4df58608cf8626773edf

#include <common/ISystemInfo.h>

namespace rho
{
namespace common
{

class CSystemInfoImpl : public ISystemInfo
{
public:
    String getPlatform() {return "Android";}
};

} // namespace common
} // namespace rho

#endif // RHO_SYSTEM_INFO_1e6790d99aed4df58608cf8626773edf

