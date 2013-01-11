
#include <windows.h>
#include <string>

#include <ruby/ext/rho/rhoruby.h>
#include <common/RhoDefs.h>
#include <logging/RhoLog.h>

RHO_GLOBAL double rho_geo_latitude()
{
	return 0;
}

RHO_GLOBAL double rho_geo_longitude()
{
	return 0;
}

RHO_GLOBAL double rho_geo_altitude() 
{
	return 0.0;
}

RHO_GLOBAL float rho_geo_accuracy()
{
	return 0;
}

RHO_GLOBAL void rho_geoimpl_turngpsoff()
{
}

RHO_GLOBAL int rho_geo_known_position()
{
	return 0;
}

RHO_GLOBAL void rho_geoimpl_settimeout(int nTimeoutSec)
{
	
}

RHO_GLOBAL int rho_geo_is_available()
{
	return 0;
}

RHO_GLOBAL void rho_geoimpl_do_geocoding(rho_param* p, const char* callback, int callback_tag)
{
}