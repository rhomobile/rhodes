#pragma once

#include "RhodesAppBase.h"

#ifdef __cplusplus

#include "net/INetRequest.h"

#include "net/HttpServer.h"
#include "SplashScreen.h"
#include "AppMenu.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhodesApp"

namespace rho {
namespace common {

class CRhodesApp : public CRhodesAppBase
{
public:
    DEFINE_LOGCLASS;

private:

    CRhodesApp(const String& strRootPath);
    boolean m_bExit;

    common::CAutoPtr<common::IRhoClassFactory> m_ptrFactory;
    common::CAutoPtr<net::INetRequest> m_NetRequest;

    String m_strListeningPorts;

    common::CAutoPtr<net::CHttpServer> m_httpServer;
    CSplashScreen m_oSplashScreen;
    CAppMenu m_oAppMenu;

    String m_strLoadingPagePath, m_strLoadingPngPath;
    String m_strStartUrl, m_strOptionsUrl, m_strRhobundleReloadUrl, m_strFirstStartUrl;
    String m_strRhoMessage;

    int m_currentTabIndex;
    String m_currentUrls[5];

    String m_strAppBackUrl;
    Vector<ICallbackObject*> m_arCallbackObjects;

    common::CMutex m_mxPushCallback;
    String m_strPushCallback, m_strPushCallbackParams;
	
    common::CMutex m_mxScreenRotationCallback;
    String m_strScreenRotationCallback, m_strScreenRotationCallbackParams;


public:
    ~CRhodesApp(void);

    static CRhodesApp* Create(const String& strRootPath);
    static void Destroy();
    static CRhodesApp* getInstance(){ return (CRhodesApp*)m_pInstance; }
    void startApp();
    void stopApp();

    void  keepLastVisitedUrl(String strUrl);
    void navigateToUrl( const String& strUrl);
    void navigateBack();

    const String& getRhobundleReloadUrl();
    const String& getStartUrl();
    const String& getOptionsUrl();
    const String& getCurrentUrl(int index = 0);
    const String& getFirstStartUrl();

	const String& getLoadingPngPath(){return m_strLoadingPngPath;}
    const String& getLoadingPagePath(){return m_strLoadingPagePath; }

    const String& getAppBackUrl(){return m_strAppBackUrl;}
    void setAppBackUrl(const String& url);
    String getAppTitle();

    void callCameraCallback(String strCallbackUrl, const String& strImagePath, const String& strError, boolean bCancel );
    void callDateTimeCallback(String strCallbackUrl, long lDateTime, const char* szData, int bCancel );
    void callAppActiveCallback(boolean bActive);
    void callPopupCallback(String strCallbackUrl, const String &id, const String &title);

    CAppMenu& getAppMenu (void) { return m_oAppMenu; }
    CSplashScreen& getSplashScreen(){return m_oSplashScreen;}

    boolean sendLog();

    String addCallbackObject(ICallbackObject* pCallbackObject, String strName);
    unsigned long getCallbackObject(int nIndex);

    const String& getRhoMessage(int nError, const char* szName);

    void runCallbackInThread(const String& strCallback, const String& strBody);

    void setPushNotification(String strUrl, String strParams );
    boolean callPushCallback(String strData);
	
    void setScreenRotationNotification(String strUrl, String strParams);
    void callScreenRotationCallback(int width, int height, int degrees);

    void loadUrl(String url);

protected:
    virtual void run();

    void initHttpServer();
    void initAppUrls();

    const char* getFreeListeningPort();
    net::INetRequest& getNet(){ return *m_NetRequest; }
};

}
}

inline rho::common::CRhodesApp& RHODESAPP(){ return *rho::common::CRhodesApp::getInstance(); }

#endif //__cplusplus

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
	
void rho_rhodesapp_create(const char* szRootPath);
void rho_rhodesapp_start();	
void rho_rhodesapp_destroy();
	
const char* rho_rhodesapp_getstarturl();
const char* rho_rhodesapp_getfirststarturl();

const char* rho_rhodesapp_gethomeurl();
const char* rho_rhodesapp_getoptionsurl();
void rho_rhodesapp_keeplastvisitedurl(const char* szUrl);
const char* rho_rhodesapp_getcurrenturl(int tabindex);

const char* rho_rhodesapp_getloadingpagepath();
const char* rho_rhodesapp_getblobsdirpath();
	
void rho_http_redirect(void* httpContext, const char* szUrl);
void rho_http_senderror(void* httpContext, int nError, const char* szMsg);
void rho_http_sendresponse(void* httpContext, const char* szBody);
int	rho_http_snprintf(char *buf, size_t buflen, const char *fmt, ...);

void rho_rhodesapp_navigate_back();
	
char* rho_http_normalizeurl(const char* szUrl);
void rho_http_free(void* data);

void rho_rhodesapp_callCameraCallback(const char* strCallbackUrl, const char* strImagePath, 
    const char* strError, int bCancel );
void rho_rhodesapp_callDateTimeCallback(const char* strCallbackUrl, long lDateTime, const char* szData, int bCancel );
void rho_rhodesapp_callAppActiveCallback(int nActive);
unsigned long rho_rhodesapp_GetCallbackObject(int nIndex);

void rho_rhodesapp_setViewMenu(unsigned long valMenu);
const char* rho_rhodesapp_getappbackurl();

int rho_rhodesapp_callPushCallback(const char* szData);

void rho_rhodesapp_callScreenRotationCallback(int width, int height, int degrees);
void rho_rhodesapp_callPopupCallback(const char *strCallbackUrl, const char *id, const char *title);

int rho_conf_send_log();

int rho_base64_encode(const char *src, int srclen, char *dst);
int rho_base64_decode(const char *src, int srclen, char *dst);

void rho_net_request(const char *url);

void rho_rhodesapp_load_url(const char *url);

#ifdef __cplusplus
};
#endif //__cplusplus
