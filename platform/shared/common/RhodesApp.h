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
    String m_strRhoRootPath;
    String m_strHomeUrl, m_strStartUrl, m_strOptionsUrl, m_strRhobundleReloadUrl;
    StringW m_strStartUrlW, m_strOptionsUrlW;

public:
    ~CRhodesApp(void);

    static CRhodesApp* Create(const String& strRootPath);
    static void Destroy();
    static CRhodesApp* getInstance(){ return m_pInstance; }
    void exitApp();

    String canonicalizeRhoUrl(const String& strUrl) ;
    const StringW& getStartUrlW();
    const StringW& getOptionsUrlW();
    const String& getRhobundleReloadUrl();
    void  keepLastVisitedUrl(String strUrl);
    void navigateToUrl( const String& strUrl);

private:
	virtual void run();

    void initHttpServer();
    void initAppUrls();
    String getFirstStartUrl();

    const char* getFreeListeningPort();
    const String& getRhoRootPath();
    String getSystemRootPath();
    const char* getRelativeBlobsPath(); 
    const wchar_t* getRelativeBlobsPathW();
    String getLoadingPagePath();
    const String& getStartUrl();

};

}
}

inline rho::common::CRhodesApp& RHODESAPP(){ return *rho::common::CRhodesApp::getInstance(); }

#endif //__cplusplus

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
	
void rho_rhodesapp_create(const char* szRootPath);
void rho_rhodesapp_destroy();

void rho_http_redirect(void* httpContext, const char* szUrl);
void rho_http_senderror(void* httpContext, int nError, const char* szMsg);
void rho_http_sendresponse(void* httpContext, const char* szBody);

#ifdef __cplusplus
};
#endif //__cplusplus
