#ifndef _RHOSIMCONF_H_
#define _RHOSIMCONF_H_

#ifdef __cplusplus

namespace rho{
namespace common{

class RhoSimSettings : public RhoSettings
{
public:
    const String& getRhodesPath(){return getString("rhodes_path");}
};

extern RhoSimSettings g_RhoSimSettings;

}
}
inline rho::common::RhoSimSettings& RHOSIMCONF(){ return rho::common::g_RhoSimSettings; }

#else
#endif //__cplusplus

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

const char* rho_simconf_getRhodesPath();
const char* rho_simconf_getstring(const char* name);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_RHOSIMCONF_H_
