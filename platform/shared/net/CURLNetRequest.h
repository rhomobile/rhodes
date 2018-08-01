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

#ifndef RHO_CURL_NET_REQUEST_041be6266e3e424c8a8e7b8a412f01f0
#define RHO_CURL_NET_REQUEST_041be6266e3e424c8a8e7b8a412f01f0

//#include "curl/lib/setup.h"
#include "curl/curl.h"
#include "net/INetRequest.h"
#include "logging/RhoLog.h"
#include "common/RhoFile.h"

namespace rho
{
namespace net
{

class CURLNetRequest : public CNetRequestBase
{
    DEFINE_LOGCLASS;
    
    struct ProxySettings
    {
        void initFromConfig();
        
        String  host;
        int     port;
        String  username;
        String  password;
    };

    struct AuthSettings
    {
        AuthSettings( AuthMethod m, const String& u, const String& p )
        : method(m)
        , user(u)
        , password(p)
        {}

        const AuthMethod method;
        const String user;
        const String password;
    };
    
    class CURLHolder
    {
    public:
        CURLHolder();
        ~CURLHolder();
        
        CURL *curl() {return m_curl;}
        
        curl_slist *set_options(const char *method, const String& strUrl, const String& strBody,
                                IRhoSession* pSession, Hashtable<String,String>* pHeaders, const ProxySettings& pProxySettings, const AuthSettings& authSettings );
        CURLcode perform();
        
        void cancel() {deactivate();}
        
        boolean sslVerifyPeer() {return m_sslVerifyPeer;}
        void sslVerifyPeer(boolean mode) {m_sslVerifyPeer = mode;}
        
    private:
        void activate();
        void deactivate();
        
    private:
        CURL *m_curl;
        CURLM *m_curlm;
        char *errbuf[CURL_ERROR_SIZE];
#ifdef mutexSmartPointer
        mutexSmartPointer m_lock;
#else
        common::CMutex m_lock;
#endif
        int m_active;
        
        boolean m_bTraceCalls;
        long timeout;
        boolean m_sslVerifyPeer;
        
        String mStrMethod;
        String mStrUrl;
        String mStrBody;
    };
    
    struct RequestState
    {
        RequestState() : respCode(0), respChunk(0), request(0), pFile(0) {}
        
        int respCode;
        Hashtable<String, String>* headers;
        Vector<char>* respChunk;
        CURLNetRequest* request;
        common::CRhoFile* pFile;
    };
    
public:
    CURLNetRequest() {}
    
    virtual INetResponse* doRequest( const char* method, const String& strUrl, const String& strBody, IRhoSession* oSession, Hashtable<String,String>* pHeaders );
    virtual INetResponse* pullFile(const String& strUrl, common::CRhoFile& oFile, IRhoSession* oSession, Hashtable<String,String>* pHeaders);
    virtual INetResponse* pushMultipartData(const String& strUrl, VectorPtr<CMultipartItem*>& arItems, IRhoSession* oSession, Hashtable<String,String>* pHeaders);

    virtual void cancel();

    virtual boolean getSslVerifyPeer() {return m_curl.sslVerifyPeer();}
    virtual void setSslVerifyPeer(boolean mode){m_curl.sslVerifyPeer(mode);}

    virtual INetResponse* createEmptyNetResponse();
    
private:
    INetResponse* doPull(const char *method, const String &strUrl, const String &strBody, common::CRhoFile *oFile, IRhoSession *oSession, Hashtable<String,String>* pHeaders);
    int getResponseCode(CURLcode err, String const &body, IRhoSession* oSession);
    int getResponseCode(CURLcode err, Vector<char> const &body, IRhoSession* oSession);
    int getResponseCode(CURLcode err, char const *body, size_t bodysize, IRhoSession* oSession);
    String makeCookies();
    INetResponse *makeResponse(String const &body, int nErrorCode);
    INetResponse *makeResponse(Vector<char> const &body, int nErrorCode);
    INetResponse *makeResponse(char const *body, size_t bodysize, int nErrorCode);
	CURLcode doCURLPerform(const String& strUrl);
    
    static size_t curlHeaderCallback(void *ptr, size_t size, size_t nmemb, void *opaque);
    static size_t curlBodyDataCallback(void *ptr, size_t size, size_t nmemb, void *opaque);


    CURLHolder m_curl;
};

} // namespace net
} // namespace rho

#endif // RHO_CURL_NET_REQUEST_041be6266e3e424c8a8e7b8a412f01f0

