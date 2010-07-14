#include "GeoLocation.h"
#include "common/RhodesApp.h"
#include "common/RhoMutexLock.h"
#include "common/IRhoClassFactory.h"
#include "common/RhoConf.h"
#include "net/INetRequest.h"
#include <math.h>

extern "C" {
double rho_geo_latitude();
double rho_geo_longitude();
int rho_geo_known_position();	
void rho_geoimpl_settimeout(int nTimeoutSec);
int rho_geo_is_available();
}

namespace rho {
using namespace common;
namespace rubyext{

IMPLEMENT_LOGCLASS(CGeoLocation,"GeoLocation");
CGeoLocation* CGeoLocation::m_pInstance = 0;

/*static*/ CGeoLocation* CGeoLocation::Create(common::IRhoClassFactory* factory)
{
    if ( m_pInstance ) 
        return m_pInstance;

    m_pInstance = new CGeoLocation(factory);
    return m_pInstance;
}

/*static*/void CGeoLocation::Destroy()
{
    if ( m_pInstance )
        delete m_pInstance;

    m_pInstance = 0;
}

CGeoLocation::CGeoLocation(common::IRhoClassFactory* pFactory)
{
    m_nGeoPingTimeoutSec = 0;
    m_NetRequest = pFactory->createNetRequest();
}

void CGeoLocation::callGeoCallback(const CGeoNotification& oNotify, boolean bError, boolean bRunInThread)
{
	if (oNotify.m_strUrl.length() == 0)
		return;
	
	String strFullUrl = getNet().resolveUrl(oNotify.m_strUrl);
	String strBody = "rho_callback=1";
	if (bError && rho_geo_is_available() )
		strBody += "&status=error&error_code=" + convertToStringA(12);//RhoRuby.ERR_GEOLOCATION;
	else	
		strBody += "&status=ok";

	strBody += "&available=" + convertToStringA( (rho_geo_is_available() ? 1 : 0) );
	strBody += "&known_position=" + convertToStringA(rho_geo_known_position());
	strBody += "&latitude=" + convertToStringA(rho_geo_latitude());
	strBody += "&longitude=" + convertToStringA(rho_geo_longitude());

    if ( oNotify.m_strParams.length() > 0 )
        strBody += "&" + oNotify.m_strParams;

    if ( bRunInThread )
        RHODESAPP().runCallbackInThread(strFullUrl, strBody);
    else
    {
        NetRequest( getNet().pushData( strFullUrl, strBody, null ) );
    }
}

void CGeoLocation::callGeoCallback(boolean bError, boolean bRunInThread)
{
    synchronized(m_mxNotify)
    {
        callGeoCallback(m_Notify, bError, bRunInThread);
        m_Notify = CGeoNotification();
        callGeoCallback(m_ViewNotify, bError, bRunInThread);
        if ( bError )
            m_ViewNotify = CGeoNotification();
    }
}

void CGeoLocation::setGeoCallback(const char *url, char* params, int timeout_sec, boolean bView)
{
    if ( bView)
        m_ViewNotify = CGeoNotification(url?url:"",params?params:"");
    else
        m_Notify = CGeoNotification(url?url:"",params?params:"");

    setPingTimeoutSec(timeout_sec);

    if ( !rho_geo_is_available() )
        callGeoCallback(true, true);
}

int CGeoLocation::getDefaultPingTimeoutSec()
{
	int nPingTimeoutSec = RHOCONF().getInt("gps_ping_timeout_sec");
	if (nPingTimeoutSec==0)
		nPingTimeoutSec = 10;
	
	return nPingTimeoutSec;
}

void CGeoLocation::setPingTimeoutSec( int nTimeout )
{
	int nNewTimeout = nTimeout;
	if (nNewTimeout == 0)
		nNewTimeout = getDefaultPingTimeoutSec();
	
	if ( nNewTimeout != m_nGeoPingTimeoutSec)
	{
		m_nGeoPingTimeoutSec = nNewTimeout;
		rho_geoimpl_settimeout(m_nGeoPingTimeoutSec);
	}
}

int CGeoLocation::getGeoTimeoutSec()
{
	if (m_nGeoPingTimeoutSec==0)
		m_nGeoPingTimeoutSec = getDefaultPingTimeoutSec();
	
	return m_nGeoPingTimeoutSec;
}

/*static*/ void CGeoLocation::callback_geolocation(void *arg, String const &/*query*/ )
{
    if ( !rho_geo_is_available() )
    {
        rho_http_sendresponse(arg, "Unavailable;Unavailable;Unavailable");
        return;
    }

    if (!rho_geo_known_position())
    {
        rho_http_sendresponse(arg, "Reading;Reading;Reading");
        return;
    }
    
    double latitude = rho_geo_latitude();
    double longitude = rho_geo_longitude();

    char location[256];
    sprintf(location,"%.4f\xc2\xb0 %s, %.4f\xc2\xb0 %s;%f;%f",
        fabs(latitude),latitude < 0 ? "South" : "North",
        fabs(longitude),longitude < 0 ? "West" : "East",
        latitude,longitude);

    LOGC(INFO,CRhodesApp::getLogCategory())+ "Location: " + location;
    rho_http_sendresponse(arg, location);
}

}
}

extern "C" {

using namespace rho::rubyext;

void rho_geo_set_notification( const char *url, char* params, int timeout_sec)
{
    if ( url && *url )
        rho_geo_known_position();

    CGeoLocation::getInstance()->setGeoCallback(url, params, timeout_sec, false);
}

void rho_geo_set_view_notification( const char *url, char* params, int timeout_sec)
{
    if ( url && *url )
        rho_geo_known_position();

    CGeoLocation::getInstance()->setGeoCallback(url, params, timeout_sec, true);
}

void rho_geo_callcallback()
{
    CGeoLocation::getInstance()->callGeoCallback(false);
}

void rho_geo_callcallback_error()
{
    CGeoLocation::getInstance()->callGeoCallback(true);
}

int rho_geo_gettimeout_sec()
{
    return CGeoLocation::getInstance()->getGeoTimeoutSec();
}

double rho_geo_haversine_distance(double lat1, double lon1, double lat2, double lon2)
{
	if ((lat1 == lat2) && (lon1 == lon2))
		return 0;

	const double RAD_PER_DEG = 0.017453293; //PI/180
	const int Rmiles = 3956;                //radius of the great circle in miles

	double dlon = lon2 - lon1;
	double dlat = lat2 - lat1;
	double dlon_rad = dlon * RAD_PER_DEG;
	double dlat_rad = dlat * RAD_PER_DEG;
	double lat1_rad = lat1 * RAD_PER_DEG;
	//double lon1_rad = lon1 * RAD_PER_DEG;
	double lat2_rad = lat2 * RAD_PER_DEG;
	//double lon2_rad = lon2 * RAD_PER_DEG;

	double a = pow((sin(dlat_rad/2)), 2) + cos(lat1_rad) * cos(lat2_rad) * pow(sin(dlon_rad/2), 2);
	double c = 2 * atan2(sqrt(a), sqrt(1-a));
	double dMi = Rmiles * c; //delta between the two points in mile

	return dMi;
}

} //extern "C"
