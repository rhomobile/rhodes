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

#pragma once

#include "RhodesAppBase.h"

#ifdef __cplusplus

#include "common/ThreadQueue.h"

#include "net/INetRequest.h"

#include "net/HttpServer.h"
#include "SplashScreen.h"
#include "AppMenu.h"
#include "ExtManager.h"
#include "push/RhoPushManager.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhodesApp"

namespace rho {
namespace common {
	
	enum enNetworkStatus {
        networkStatusUnknown,
        networkStatusConnected,
        networkStatusDisconnected
    };
		
    class INetworkStatusReceiver
    {
    public:
        virtual ~INetworkStatusReceiver() {}
        virtual void onNetworkStatusChanged(enNetworkStatus currentStatus) = 0;
    };
	
	class INetworkStatusMonitor
    {
    public:
        virtual ~INetworkStatusMonitor() {}
        virtual void setPollInterval(int pollInterval) = 0;
		virtual void setNetworkStatusReceiver(INetworkStatusReceiver* receiver) = 0;
    };
	
	class NetworkStatusReceiver : public INetworkStatusReceiver
    {
    private:
		enNetworkStatus m_prevStatus;
        String m_callbackUrl;
		common::CMutex& m_mxAccess;
		
    public:
		NetworkStatusReceiver( common::CMutex& mxAccess );
        virtual void onNetworkStatusChanged(enNetworkStatus currentStatus);
        void setCallbackUrl( const String& url ) { m_callbackUrl = url; }
		
	private:
		static String networkStatusToString( enNetworkStatus status );
    };

class CRhodesApp : public CRhodesAppBase
{
public:
    DEFINE_LOGCLASS;

private:

    CRhodesApp(const String& strRootPath, const String& strUserPath, const String& strRuntimePath);
    boolean m_bExit, m_bRestartServer;

    String m_strListeningPorts;

    common::CAutoPtr<net::CHttpServer> m_httpServer;
    CSplashScreen m_oSplashScreen;
    CAppMenu m_oAppMenu;
    CRhoTimer m_oTimer;

    String m_strLoadingPagePath, m_strLoadingPngPath;
    String m_strStartUrl, m_strOptionsUrl, m_strRhobundleReloadUrl;//, m_strFirstStartUrl;
    static String m_strStartParameters;
    static boolean m_bSecurityTokenNotPassed;
    String m_strRhoMessage;
    String m_EmptyString;

    Vector<String> m_currentUrls;

    Vector<String> m_arAppBackUrl, m_arAppBackUrlOrig;
    Vector<ICallbackObject*> m_arCallbackObjects;

    mutable common::CMutex m_mxPushCallback;
    String m_strPushCallback, m_strPushCallbackParams;
    PushManager m_appPushMgr;
	
    common::CMutex m_mxScreenRotationCallback;
    String m_strScreenRotationCallback, m_strScreenRotationCallbackParams;
    
    boolean m_bDeactivationMode;
    //int m_activateCounter;

    common::CAutoPtr<common::CThreadQueue> m_appCallbacksQueue;
    CExtManager* m_pExtManager;
	
	common::CMutex m_mxNetworkStatus;
	NetworkStatusReceiver m_networkStatusReceiver;
    INetworkStatusMonitor* m_pNetworkStatusMonitor;
	static const int c_defaultNetworkStatusPollInterval = 20;

public:
    ~CRhodesApp(void);
#if defined(WINDOWS_PLATFORM)
	bool m_cameraOpened;
#endif
    static CRhodesApp* Create(const String& strRootPath, const String& strUserPath, const String& strRuntimePath);
    static void Destroy();
    static CRhodesApp* getInstance(){ return (CRhodesApp*)m_pInstance; }
    void startApp();
    void stopApp();
    void restartLocalServer(common::CThreadQueue& waitThread);

    void  keepLastVisitedUrl(String strUrl);
    void navigateToUrl( const String& strUrl);
    void navigateBack();
    
    boolean deactivationMode() const {return m_bDeactivationMode;}

    const String& getRhobundleReloadUrl();
    const String& getBaseUrl();
    void  setBaseUrl(const String& strBaseUrl);
    const String& getStartUrl();
    const String& getOptionsUrl();
    const String& getCurrentUrl(int index = 0);
    boolean isOnStartPage();

	const String& getLoadingPngPath(){return m_strLoadingPngPath;}
    const String& getLoadingPagePath(){return m_strLoadingPagePath; }

    static void setStartParameters(const char* szParams ){ m_strStartParameters = (szParams ? szParams : ""); }
    static void setSecurityTokenNotPassed(boolean is_not_passed) {m_bSecurityTokenNotPassed = is_not_passed;}
    static boolean isSecurityTokenNotPassed() {return m_bSecurityTokenNotPassed;}
    static const String& getStartParameters(){ return m_strStartParameters; }

    const String& getAppBackUrl();
    void setAppBackUrl(const String& url);
    String getAppTitle();
    String getAppName();
    StringW getAppNameW();

    void callBarcodeCallback(String strCallbackUrl, const String& strBarcode, bool isError);
    void callCallbackWithData(String strCallbackUrl, String strBody, const String& strCallbackData, bool bWaitForResponse);
    void callCallbackWithJsonBody( const char* szCallback, const char* szCallbackBody, const char* szCallbackData, bool bWaitForResponse);
    void callCameraCallback(String strCallbackUrl, const String& strImagePath, const String& strError, boolean bCancel );
    void callSignatureCallback(String strCallbackUrl, const String& strSignaturePath, const String& strError, boolean bCancel );
    void callDateTimeCallback(String strCallbackUrl, long lDateTime, const char* szData, int bCancel );
	void callBluetoothCallback(String strCallbackUrl, const char* body);
    void callAppActiveCallback(boolean bActive);
    void callUiCreatedCallback();
    void callUiDestroyedCallback();
    void callPopupCallback(String strCallbackUrl, const String &id, const String &title);
    boolean callTimerCallback(const String& strUrl, const String& strData);

    CAppMenu& getAppMenu (void) { return m_oAppMenu; }
    CSplashScreen& getSplashScreen(){return m_oSplashScreen;}
    CRhoTimer&     getTimer(){ return m_oTimer; }

    void setExtManager( CExtManager* pExtManager ){m_pExtManager = pExtManager; }
    CExtManager&   getExtManager(){ return *m_pExtManager; }

    String addCallbackObject(ICallbackObject* pCallbackObject, String strName);
    unsigned long getCallbackObject(int nIndex);

    const String& getRhoMessage(int nError, const char* szName);

    void runCallbackInThread(const String& strCallback, const String& strBody);

    IRhoPushClient* getDefaultPushClient() {return m_appPushMgr.getDefaultClient(); }
    void addPushClient(IRhoPushClient* pClient) { m_appPushMgr.addClient(pClient); }
    void initPushClients();
    void setPushNotification(const String& strUrl, const String& strParams, const String& types);
    //void checkPushRegistration(const String& strUrl, const String& strParams, const String& type);

    // Deprecated
    boolean callPushCallback(const String& strData) const;
    boolean callPushCallbackWithJsonBody(const String& strUrl, const String& strData, const String& strParams);

    boolean callPushCallback(const String& strType, const String& strJson, const String& strData);
	
    void setScreenRotationNotification(String strUrl, String strParams);
    void callScreenRotationCallback(int width, int height, int degrees);

    void loadUrl(String url);

    void notifyLocalServerStarted();
    const char* getFreeListeningPort();
    int determineFreeListeningPort();
	
    void setNetworkStatusNotify(const String& url, int poll_interval);
    void clearNetworkStatusNotify();
    void setNetworkStatusMonitor(INetworkStatusMonitor* netMonitor);

protected:
    virtual void run();

    void initHttpServer();
    void initAppUrls();
};

#if defined(WINDOWS_PLATFORM)
extern "C" void rho_wm_impl_performOnUiThread(rho::common::IRhoRunnable* pTask);

template <typename FUNCTYPE, typename PARAMTYPE>
void rho_callInUIThread( FUNCTYPE pFunc, PARAMTYPE param )
{
    rho_wm_impl_performOnUiThread( new rho::common::CStaticClassFunctor<FUNCTYPE,PARAMTYPE>(pFunc, param) );
}
#endif //WINDOWS_PLATFORM

}
}

inline rho::common::CRhodesApp& RHODESAPP(){ return *rho::common::CRhodesApp::getInstance(); }

#endif //__cplusplus

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
	
void rho_rhodesapp_create(const char* szRootPath);
void rho_rhodesapp_create_with_separate_user_path(const char* szRootPath, const char* szUserPath);    
void rho_rhodesapp_create_with_separate_runtime(const char* szRootPath, const char* szRuntimePath);    
void rho_rhodesapp_start();	
void rho_rhodesapp_destroy();
	
const char* rho_rhodesapp_getstarturl();
//const char* rho_rhodesapp_getfirststarturl();

const char* rho_rhodesapp_gethomeurl();
const char* rho_rhodesapp_getoptionsurl();
void rho_rhodesapp_keeplastvisitedurl(const char* szUrl);
const char* rho_rhodesapp_getcurrenturl(int tabindex);

const char* rho_rhodesapp_getloadingpagepath();
const char* rho_rhodesapp_getblobsdirpath();
const char* rho_rhodesapp_getdbdirpath();
const char* rho_rhodesapp_getapprootpath();
const char* rho_rhodesapp_getuserrootpath();
	
void rho_http_redirect(void* httpContext, const char* szUrl);
void rho_http_senderror(void* httpContext, int nError, const char* szMsg);
void rho_http_sendresponse(void* httpContext, const char* szBody);
int	rho_http_snprintf(char *buf, size_t buflen, const char *fmt, ...);

void rho_rhodesapp_navigate_back();
	
char* rho_http_normalizeurl(const char* szUrl);
void rho_http_free(void* data);

void rho_rhodesapp_callCameraCallback(const char* strCallbackUrl, const char* strImagePath, 
    const char* strError, int bCancel );
void rho_rhodesapp_callSignatureCallback(const char* strCallbackUrl, const char* strSignaturePath, 
  const char* strError, int bCancel );
void rho_rhodesapp_callDateTimeCallback(const char* strCallbackUrl, long lDateTime, const char* szData, int bCancel );
void rho_rhodesapp_callBluetoothCallback(const char* strCallbackUrl, const char* body);
void rho_rhodesapp_callAppActiveCallback(int nActive);
void rho_rhodesapp_callUiCreatedCallback();
void rho_rhodesapp_callUiDestroyedCallback();
unsigned long rho_rhodesapp_GetCallbackObject(int nIndex);

void rho_rhodesapp_setViewMenu(unsigned long valMenu);
const char* rho_rhodesapp_getappbackurl();

int rho_rhodesapp_callPushCallback(const char* szData);
//int rho_rhodesapp_callPushCallbackWithJsonBody(const char* szUrl, const char* szData, const char* szParam);

void rho_rhodesapp_callScreenRotationCallback(int width, int height, int degrees);
void rho_rhodesapp_callPopupCallback(const char *strCallbackUrl, const char *id, const char *title);

int rho_conf_send_log(const char* callback_url);

void rho_net_request(const char *url);
void rho_net_request_with_data(const char *url, const char *str_body);
void rho_net_request_with_data_in_separated_thread(const char *url, const char *str_body);

void rho_rhodesapp_load_url(const char *url);

int rho_rhodesapp_check_mode();

int rho_rhodesapp_canstartapp(const char* szCmdLine, const char* szSeparators);

// return 1 only if "motorola_licence" and "motorola_licence" property is exist and correct !
int rho_is_motorola_licence_checked();    
    
int rho_can_app_started_with_current_licence();    
int rho_is_rho_elements_extension_can_be_used();    

    
// should be implemented in platforms code
void rho_platform_restart_application();
    
void rho_sys_set_network_status_notify(const char* url, int poll_interval);
void rho_sys_clear_network_status_notify();
    
#ifdef __cplusplus
};
#endif //__cplusplus
