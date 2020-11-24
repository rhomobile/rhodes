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
#ifndef JIN_NET_REQUEST
#define JIN_NET_REQUEST

#include <rhodes.h>
#include <jni.h>
#include <net/INetRequest.h>
#include <logging/RhoLog.h>
#include <common/RhoFile.h>


namespace rho
{
namespace net
{

class JNINetRequest : public CNetRequestBase
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

    //struct AuthSettings
    //{
    //    AuthSettings( AuthMethod m, const String& u, const String& p )
    //    : method(m)
    //    , user(u)
    //    , password(p)
    //    {}

    //    const AuthMethod method;
    //    const String user;
    //    const String password;
    //};

    jclass cls = nullptr;
    jclass clsHashMap = nullptr;
    jobject netRequestObject = nullptr;
    jmethodID midDoPull = nullptr;
    jmethodID midPut = nullptr;
    jmethodID midHashMapConstructor = nullptr;
    jmethodID midgetValuesFromResponseHeaders = nullptr;
    jmethodID midgetKeysFromResponseHeaders = nullptr;
    jmethodID midgetResponseBody = nullptr;
    long timeout = 0;
    
public:
    JNINetRequest();
    
    virtual INetResponse* doRequest( const char* method, const String& strUrl, const String& strBody, IRhoSession* oSession, Hashtable<String,String>* pHeaders ) override ;
    virtual INetResponse* pullFile(const String& strUrl, common::CRhoFile& oFile, IRhoSession* oSession, Hashtable<String,String>* pHeaders) override ;
    virtual INetResponse* pushMultipartData(const String& strUrl, VectorPtr<CMultipartItem*>& arItems, IRhoSession* oSession, Hashtable<String,String>* pHeaders) override ;

    virtual void cancel() override;

    virtual boolean getSslVerifyPeer() override { return false; }
    virtual void setSslVerifyPeer(boolean mode) override {  }

    virtual INetResponse* createEmptyNetResponse() override;
    
private:

    jobject makeJavaHashMap(const Hashtable<String,String>& table);
    INetResponse* doPull(const char *method, const String &strUrl, const String &strBody, common::CRhoFile *oFile, IRhoSession *oSession, Hashtable<String,String>* pHeaders);
    void set_options(const String &strUrl, const String &strBody, common::CRhoFile *oFile, IRhoSession *pSession, Hashtable<String,String> headers);

    INetResponse* makeResponse(rho::Vector<char> const &body, int nErrorCode);
    INetResponse* makeResponse(char const *body, size_t bodysize, int nErrorCode);
    INetResponse* makeResponse(rho::String const &body, int nErrorCode);

    void getResponseHeader(Hashtable<String,String>& headers);
    void JArraytoVectorString(const jharray& array, std::vector<rho::String>& v);
};

} // namespace net
} // namespace rho

#endif