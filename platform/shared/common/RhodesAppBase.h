#pragma once

#ifdef __cplusplus

#include "logging/RhoLog.h"
#include "common/RhoThread.h"
#include "common/IRhoClassFactory.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhodesApp"

namespace rho {
namespace common {

class CRhodesAppBase : public common::CRhoThread
{
public:
    DEFINE_LOGCLASS;

protected:

    static CRhodesAppBase* m_pInstance;

    String m_strRhoRootPath, m_strBlobsDirPath, m_strDBDirPath;
    String m_strHomeUrl;

    CRhodesAppBase(const String& strRootPath);
public:
    static CRhodesAppBase* Create(const String& strRootPath);
    static void Destroy();
    static CRhodesAppBase* getInstance(){ return m_pInstance; }

    String resolveDBFilesPath(const String& strFilePath);
    const String& getRhoRootPath(){return m_strRhoRootPath;}
    const String& getBlobsDirPath(){return m_strBlobsDirPath; }
    const String& getDBDirPath(){return m_strDBDirPath; }
    const String& getHomeUrl(){ return m_strHomeUrl; }

    String canonicalizeRhoUrl(const String& strUrl) ;

protected:
    virtual void run(){}

    void initAppUrls();
};

}
}

inline rho::common::CRhodesAppBase& RHODESAPPBASE(){ return *rho::common::CRhodesAppBase::getInstance(); }

#endif //__cplusplus

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

const char* rho_native_rhopath();
const char* rho_rhodesapp_getplatform();
int rho_unzip_file(const char* szZipPath);

void rho_free_callbackdata(void* pData);
	
#ifdef __cplusplus
};
#endif //__cplusplus
