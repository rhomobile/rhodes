#include "RhoConf.h"

#ifdef RHODES_EMULATOR

namespace rho{
namespace common{

RhoSimSettings g_RhoSimSettings;

}	
}

extern "C"{

const char* rho_simconf_getRhodesPath()
{
    return RHOSIMCONF().getRhodesPath().c_str();
}

const char* rho_simconf_getString(const char* name)
{
    return RHOSIMCONF().getString(name).c_str();
}

const int rho_simconf_getBool(const char* name)
{
    return RHOSIMCONF().getBool(name) ? 1 : 0;
}

}

#endif // RHODES_EMULATOR