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
    String m_strHomeUrl, m_strHomeUrlLocalHost;
    boolean m_bSendingLog;

    CRhodesAppBase(const String& strRootPath, const String& strUserPath, const String& strRuntimePath);
public:
    static CRhodesAppBase* Create(const String& strRootPath, const String& strUserPath, const String& strRuntimePath);
    static void Destroy();
    static CRhodesAppBase* getInstance(){ return m_pInstance; }

    String resolveDBFilesPath(const String& strFilePath);
    String getRelativeDBFilesPath(const String& strFilePath);
	String getDBFileRoot();
    const String& getRhoRootPath(){return m_strRhoRootPath;}
    const String& getRhoUserPath(){return m_strAppUserPath;}
    const String& getRhoRuntimePath(){return m_strRuntimePath;}
    //const String& getRhodesPath(){return m_strRhodesPath;}
    //void setRhodesPath(const String& path){m_strRhodesPath = path;}
    const String& getAppRootPath(){return m_strAppRootPath;}
    const String& getBlobsDirPath(){return m_strBlobsDirPath; }
    const String& getDBDirPath(){return m_strDBDirPath; }
    const String& getHomeUrl(){ return m_strHomeUrl; }

    String canonicalizeRhoUrl(const String& strUrl) const;

    boolean isBaseUrl(const String& strUrl);
    
    void setSendingLog(boolean bSending){m_bSendingLog = bSending; }
    boolean sendLog(const String& strCallbackUrl);
    boolean sendLogInSameThread();


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

	
#ifdef __cplusplus
};
#endif //__cplusplus
