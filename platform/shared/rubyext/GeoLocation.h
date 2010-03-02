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

    common::CAutoPtr<net::INetRequest> m_NetRequest;
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

public:
    CGeoLocation();
    void init(common::IRhoClassFactory* pFactory);

    void setGeoCallback(const char *url, char* params, int timeout_sec, boolean bView);
    void callGeoCallback(boolean bError);
    int getGeoTimeoutSec();


    static void callback_geolocation(void *arg, String const &/*query*/ );
private:

    int getDefaultPingTimeoutSec();
    void setPingTimeoutSec( int nTimeout );
    void callGeoCallback(const CGeoNotification& oNotify, boolean bError);

    net::INetRequest& getNet(){ return *m_NetRequest; }
};

}
}

#endif //__cplusplus

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

void rho_geo_set_notification( const char *url, char* params, int timeout_sec);
void rho_geo_set_view_notification( const char *url, char* params, int timeout_sec);

void rho_geo_callcallback();
void rho_geo_callcallback_error();
int rho_geo_gettimeout_sec();

#ifdef __cplusplus
};
#endif //__cplusplus

#endif //_RHOGEOLOCATION_H_
