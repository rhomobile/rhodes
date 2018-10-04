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

#ifdef __cplusplus

#include "logging/RhoLog.h"
#include "common/RhoThread.h"
#include "common/IRhoClassFactory.h"
#include "push/RhoPushManager.h"
#include "api_generator/MethodResult.h"
#include "common/app_build_capabilities.h"
#include "common/app_build_configs.h"

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

    String m_strRhoRootPath, m_strBlobsDirPath, m_strDBDirPath, m_strAppRootPath, m_strAppUserPath, m_strRuntimePath;//, m_strRhodesPath;
#if defined(OS_WP8) || defined(OS_UWP)
	static String m_strHomeUrl;
#else
	String m_strHomeUrl;
#endif

    boolean m_bSendingLog;
	
	mutable common::CMutex m_mxPushCallback;
    String m_strPushCallback, m_strPushCallbackParams;
    PushManager m_appPushMgr;
    bool m_bJSApplication;
    bool m_bNodeJSApplication;

    CRhodesAppBase(const String& strRootPath, const String& strUserPath, const String& strRuntimePath);
public:
    static CRhodesAppBase* Create(const String& strRootPath, const String& strUserPath, const String& strRuntimePath);
    static void Destroy();
    static CRhodesAppBase* getInstance(){ return m_pInstance; }

    String resolveDBFilesPath(const String& strFilePath);
    String getRelativeDBFilesPath(const String& strFilePath);
	String getDBFileRoot();
    const String& getRhoRootPath()const{return m_strRhoRootPath;}
    const String& getRhoUserPath(){return m_strAppUserPath;}
    const String& getRhoRuntimePath() const{return m_strRuntimePath;}
    //const String& getRhodesPath(){return m_strRhodesPath;}
    //void setRhodesPath(const String& path){m_strRhodesPath = path;}
    const String& getAppRootPath()const{return m_strAppRootPath;}
    const String& getBlobsDirPath(){return m_strBlobsDirPath; }
    const String& getDBDirPath(){return m_strDBDirPath; }
    const String& getHomeUrl(){ return m_strHomeUrl; }

    String canonicalizeRhoUrl(const String& strUrl) const;

    boolean isBaseUrl(const String& strUrl);
    
    void setSendingLog(boolean bSending){m_bSendingLog = bSending; }
    boolean sendLog(rho::apiGenerator::CMethodResult& oResult);
    boolean sendLogInSameThread();
	
    void setJSApplication(bool bJSApp){ m_bJSApplication = bJSApp; }
    bool isJSApplication()const{ return m_bJSApplication; }

    void setNodeJSApplication(bool bJSApp){ m_bNodeJSApplication = bJSApp; }
    bool isNodeJSApplication()const{ return m_bNodeJSApplication; }

    // Deprecated
    boolean callPushCallback(const String& strData) const;

protected:
    virtual void run(){}

    void initAppUrls();
};
    
    template <typename T>
    class CRhoCallInThread : public common::CRhoThread
    {
    public:
        CRhoCallInThread(T* cb)
        :CRhoThread(), m_cb(cb)
        {
            start(epNormal);
        }
        
    private:
        virtual void run()
        {
            m_cb->run(*this);
        }
        
        virtual void runObject()
        {
            common::CRhoThread::runObject();
            delete this;
        }
        
    private:
        common::CAutoPtr<T> m_cb;
    };
    
    template <typename T>
    void rho_rhodesapp_call_in_thread(T *cb)
    {
        new CRhoCallInThread<T>(cb);
    }

}
}

inline rho::common::CRhodesAppBase& RHODESAPPBASE(){ return *rho::common::CRhodesAppBase::getInstance(); }


#endif //__cplusplus

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

const char* rho_native_reruntimepath();
const char* rho_native_rhopath();
const char* rho_native_get_appname();
const char* rho_native_rhouserpath();
const char* rho_native_rhodbpath();    

const char* rho_rhodesapp_getplatform();
//const char* rho_rhodesapp_getrhodespath();
void rho_file_recursive_createdir(const char* szPath, const char* szBasePath);

int rho_unzip_file(const char* szZipPath, const char* psw);

void rho_free_callbackdata(void* pData);
	
int rho_base64_encode(const char *src, int srclen, char *dst);
int rho_base64_decode(const char *src, int srclen, char *dst);	
    
int rho_sys_set_do_not_bakup_attribute(const char* path, int value);
	
void rho_net_request_with_data(const char *url, const char *str_body);
    
int rho_app_canonicalize_rho_url(const char* url, char* result, int max_len);

int rho_decrypt_file(const char* filebuf, int filebuf_len, char* decrypted_buf, int maxlen);
	
#ifdef __cplusplus
};
#endif //__cplusplus
