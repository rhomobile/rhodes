#pragma once

#ifdef __cplusplus

#include "logging/RhoLog.h"
#include "common/RhoThread.h"

struct shttpd_ctx;

namespace rho {
namespace common {

class CRhodesApp : public common::CRhoThread
{
public:
    DEFINE_LOGCLASS;
private:

    static CRhodesApp* m_pInstance;
    CRhodesApp(const String& strRootPath);
    boolean m_bExit;

    String m_strListeningPorts;
    struct shttpd_ctx * m_shttpdCtx;
    String m_strRhoRootPath, m_strLoadingPagePath, m_strBlobsDirPath;
    String m_strHomeUrl, m_strStartUrl, m_strOptionsUrl, m_strRhobundleReloadUrl, m_strCurrentUrl;
    StringW m_strStartUrlW, m_strOptionsUrlW, m_strCurrentUrlW;

public:
    ~CRhodesApp(void);

    static CRhodesApp* Create(const String& strRootPath);
    static void Destroy();
    static CRhodesApp* getInstance(){ return m_pInstance; }
	void startApp();
    void stopApp();

    String canonicalizeRhoUrl(const String& strUrl) ;
    const StringW& getStartUrlW();
    const StringW& getOptionsUrlW();
    const String& getRhobundleReloadUrl();
    void  keepLastVisitedUrl(String strUrl);
    void  keepLastVisitedUrlW(StringW strUrlW);

    void navigateToUrl( const String& strUrl);
    const String& getStartUrl(){return m_strStartUrl;}
    const String& getOptionsUrl(){return m_strOptionsUrl;}
    const String& getCurrentUrl(){ return m_strCurrentUrl; }
    const StringW& getCurrentUrlW(){ return m_strCurrentUrlW; }

    const String& getLoadingPagePath(){return m_strLoadingPagePath; }
    const String& getBlobsDirPath(){return m_strBlobsDirPath; }
    const String& getRhoRootPath(){return m_strRhoRootPath;}

private:
	virtual void run();

    void initHttpServer();
    void initAppUrls();
    String getFirstStartUrl();

    const char* getFreeListeningPort();

    void callAppActiveCallback();
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
const char* rho_native_rhopath();
	
const char* rho_rhodesapp_getstarturl();
const char* rho_rhodesapp_getoptionsurl();
void rho_rhodesapp_keeplastvisitedurl(const char* szUrl);
const char* rho_rhodesapp_getcurrenturl();

const char* rho_rhodesapp_getloadingpagepath();
const char* rho_rhodesapp_getblobsdirpath();
	
void rho_http_redirect(void* httpContext, const char* szUrl);
void rho_http_senderror(void* httpContext, int nError, const char* szMsg);
void rho_http_sendresponse(void* httpContext, const char* szBody);

char* rho_http_normalizeurl(const char* szUrl);
void rho_http_free(void* data);
	
#ifdef __cplusplus
};
#endif //__cplusplus
