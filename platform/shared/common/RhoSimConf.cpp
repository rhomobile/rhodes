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

const char* rho_simconf_getstring(const char* name)
{
    return RHOSIMCONF().getString(name).c_str();
}

}

#endif // RHODES_EMULATOR