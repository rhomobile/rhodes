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

#ifndef _RHOGEOLOCATION_H_
#define _RHOGEOLOCATION_H_

#ifdef __cplusplus

#include "logging/RhoLog.h"
#include "net/INetRequest.h"
#include "common/IRhoClassFactory.h"

namespace rho {
namespace rubyext {

class CGeoLocation
{
public:
    DEFINE_LOGCLASS;
private:

    NetRequest m_NetRequest;
	int m_nGeoPingTimeoutSec;

    struct CGeoNotification
    {
        String m_strUrl, m_strParams;
        
        CGeoNotification(){}
        CGeoNotification(String strUrl, String strParams)
        { 
        	m_strUrl = strUrl; 
        	m_strParams = strParams; 
        }
    };
    CGeoNotification m_ViewNotify, m_Notify;
    common::CMutex   m_mxNotify;

    static CGeoLocation* m_pInstance;

    CGeoLocation();

public:

    static CGeoLocation* Create();
    static void Destroy();
    static CGeoLocation* getInstance();

    void setGeoCallback(const char *url, const char* params, int timeout_sec, boolean bView);
    void callGeoCallback(const char* pszError = 0, boolean bRunInThread=false);
    int getGeoTimeoutSec();


    static void callback_geolocation(void *arg, String const &/*query*/ );
private:

    int getDefaultPingTimeoutSec();
    void setPingTimeoutSec( int nTimeout );
    void callGeoCallback(const CGeoNotification& oNotify, const char* pszError, boolean bRunInThread);

    net::CNetRequestWrapper getNet(){ return getNetRequest(&m_NetRequest); }
};

}
}

#endif //__cplusplus

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

double rho_geo_latitude();
double rho_geo_longitude();
float  rho_geo_accuracy();

void rho_geo_set_notification( const char *url, char* params, int timeout_sec);
void rho_geo_set_view_notification( const char *url, char* params, int timeout_sec);

void rho_geo_callcallback();
void rho_geo_callcallback_error();
void rho_geo_callcallback_stop();
int rho_geo_gettimeout_sec();

double rho_geo_haversine_distance(double lat1, double lon1, double lat2, double lon2);

#ifdef __cplusplus
};
#endif //__cplusplus

#endif //_RHOGEOLOCATION_H_
