/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "GeoLocation.h"
#include "common/RhodesApp.h"
#include "common/RhoMutexLock.h"
#include "common/IRhoClassFactory.h"
#include "common/RhoConf.h"
#include "net/INetRequest.h"
#include <math.h>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "GeoLocation"

extern "C" {
double rho_geo_latitude();
double rho_geo_longitude();
float  rho_geo_accuracy();
int rho_geo_known_position();
void rho_geoimpl_settimeout(int nTimeoutSec);
int rho_geo_is_available();
}

namespace rho {
using namespace common;
namespace rubyext{

IMPLEMENT_LOGCLASS(CGeoLocation,"GeoLocation");
CGeoLocation* CGeoLocation::m_pInstance = 0;

/*static*/ CGeoLocation* CGeoLocation::Create()
{
    if ( m_pInstance ) 
        return m_pInstance;

    RAWLOG_INFO("Creating singleton instance.");
    m_pInstance = new CGeoLocation();
    return m_pInstance;
}

/*static*/void CGeoLocation::Destroy()
{
    if ( m_pInstance )
    {
        RAWLOG_INFO("Destroying singleton instance.");
        delete m_pInstance;
    }

    m_pInstance = 0;
}

/*static*/CGeoLocation* CGeoLocation::getInstance()
{
    if (!m_pInstance)
    {
        RAWLOG_FATAL("No singleton instance!!!!");
    }
    return m_pInstance;
}

CGeoLocation::CGeoLocation()
{
    m_nGeoPingTimeoutSec = 0;
}

void CGeoLocation::callGeoCallback(const CGeoNotification& oNotify, const char* pszError, boolean bRunInThread)
{
    if ((oNotify.m_strUrl.length() == 0))
        return;

    String strFullUrl = getNet().resolveUrl(oNotify.m_strUrl);
    String strBody = "rho_callback=1";
    if (pszError) {
        strBody += "&status=";
        strBody += pszError;
        if (!strcmp(pszError, "error")) {
            strBody += "&error_code=" + convertToStringA(12);//RhoRuby.ERR_GEOLOCATION;
        }
        if (rho_geo_is_available()) {
            strBody += "&available=1&known_position=" + convertToStringA(rho_geo_known_position());
            strBody += "&latitude=" + convertToStringA(rho_geo_latitude());
            strBody += "&longitude=" + convertToStringA(rho_geo_longitude());
            strBody += "&accuracy=" + convertToStringA(rho_geo_accuracy());
        } else {
            strBody += "&available=0&known_position=0&latitude=0.0&longitude=0.0&accuracy=0.0";
        }
    } else {
        strBody += "&status=ok";
        strBody += "&available=" + convertToStringA( (rho_geo_is_available() ? 1 : 0) );
        strBody += "&known_position=" + convertToStringA(rho_geo_known_position());
        strBody += "&latitude=" + convertToStringA(rho_geo_latitude());
        strBody += "&longitude=" + convertToStringA(rho_geo_longitude());
        strBody += "&accuracy=" + convertToStringA(rho_geo_accuracy());
    }

    if ( oNotify.m_strParams.length() > 0 )
        strBody += "&" + oNotify.m_strParams;

    //if ( bRunInThread )
        RHODESAPP().runCallbackInThread(strFullUrl, strBody);
    //else
    //{
    //    getNet().pushData( strFullUrl, strBody, null );
    //}
}

void CGeoLocation::callGeoCallback(const char* pszError, boolean bRunInThread)
{
    synchronized(m_mxNotify)
    {
        RAWTRACE4("Call geo callback: %s, %s, status=%s, runInThread=%d.",
                     m_Notify.m_strUrl.c_str(), m_Notify.m_strParams.c_str(), pszError?pszError:"ok", (int)bRunInThread);
        callGeoCallback(m_Notify, pszError, bRunInThread);
        if(pszError && !strcmp(pszError, "stop")) {
            RAWTRACE("Reset geo notification callback to default.");
            m_Notify = CGeoNotification();
        }
        RAWTRACE4("Call geo view callback: %s, %s, status=%s, runInThread=%d.",
                     m_ViewNotify.m_strUrl.c_str(), m_ViewNotify.m_strParams.c_str(), pszError?pszError:"ok", (int)bRunInThread);
        callGeoCallback(m_ViewNotify, pszError, bRunInThread);
        if (pszError) {
            RAWTRACE("Reset geo view notification callback to default.");
            m_ViewNotify = CGeoNotification();
        }
    }
}

void CGeoLocation::setGeoCallback(const char *url, const char* params, int timeout_sec, boolean bView)
{
    synchronized(m_mxNotify)
    {
        if ( bView) {
            RAWTRACE2("Set geo view notification callback: %s, %s", url, params);
            m_ViewNotify = CGeoNotification(url?url:"",params?params:"");
        } else {
            RAWTRACE2("Set geo notification callback: %s, %s", url, params);
            m_Notify = CGeoNotification(url?url:"",params?params:"");
        }
    }

    if ( url && *url )
    {
        RAWTRACE1("Set new geo ping timeout %d sec", timeout_sec);

        setPingTimeoutSec(timeout_sec);

        if ( !rho_geo_is_available() )
            callGeoCallback("error", true);
    }
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
	
	m_nGeoPingTimeoutSec = nNewTimeout;
	rho_geoimpl_settimeout(m_nGeoPingTimeoutSec);
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
    CGeoLocation::getInstance()->callGeoCallback();
}

void rho_geo_callcallback_error()
{
    CGeoLocation::getInstance()->callGeoCallback("error");
}

void rho_geo_callcallback_stop()
{
    CGeoLocation::getInstance()->callGeoCallback("stop");
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
