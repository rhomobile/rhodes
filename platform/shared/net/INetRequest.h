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

#include "common/RhoStd.h"
#include "common/InputStream.h"
#include "common/AutoPointer.h"
#include "common/RhoMutexLock.h"
#include "common/RhoTime.h"

namespace rho {
namespace common {
class CRhoFile;
}

namespace net {

  enum AuthMethod {
    AUTH_NONE,
    AUTH_BASIC,
    AUTH_DIGEST
  };

struct INetResponse
{
    virtual ~INetResponse(void){;}
    virtual const char* getCharData() const = 0;
    virtual unsigned int getDataSize() const = 0;
    virtual int getRespCode() const = 0;
    virtual String getCookies() const = 0;
    virtual String getErrorMessage() const = 0;

    virtual void setCharData(const char* szData) = 0;
};

struct IRhoSession
{
	virtual void logout()=0;
	virtual const String& getSession()=0;
    virtual const String& getContentType()=0;
};

struct CMultipartItem
{
    //mutually exclusive
    String m_strFilePath;
    String m_strBody;
    //

    String m_strName, m_strFileName, m_strContentType;

    String m_strDataPrefix;
};

class INetRequestCallback;

struct INetRequestImpl
{
    virtual ~INetRequestImpl(void){;}

    virtual INetResponse* doRequest( const char* method, const String& strUrl, const String& strBody, IRhoSession* oSession, Hashtable<String,String>* pHeaders )=0;
    virtual INetResponse* pullFile(const String& strUrl, common::CRhoFile& oFile, IRhoSession* oSession, Hashtable<String,String>* pHeaders)=0;
    virtual INetResponse* pushMultipartData(const String& strUrl, VectorPtr<CMultipartItem*>& arItems, IRhoSession* oSession, Hashtable<String,String>* pHeaders)=0;

    virtual void cancel() = 0;

    virtual boolean getSslVerifyPeer() = 0;
    virtual void setSslVerifyPeer(boolean mode) = 0;

    virtual INetResponse* createEmptyNetResponse() = 0;
    
    virtual void setCallback(INetRequestCallback*) = 0;

    virtual void setAuthMethod( AuthMethod method ) = 0;
    virtual AuthMethod getAuthMethod() const = 0;
    
    virtual void setAuthUser( const String& user ) = 0;
    virtual const String& getAuthUser() const = 0;
    
    virtual void setAuthPassword( const String& password ) = 0;
    virtual const String& getAuthPassword() const = 0;
};

class CNetRequestBase : public INetRequestImpl
{
  friend class CNetRequestHolder;
public:
    CNetRequestBase()
      : m_pCallback(0)
      , m_authMethod( AUTH_NONE )
    {

    }

    virtual void setCallback(INetRequestCallback* cb ) { m_pCallback = cb; }
    virtual void setAuthMethod( AuthMethod method ) { m_authMethod = method; }
    virtual AuthMethod getAuthMethod() const { return m_authMethod; }
    virtual void setAuthUser( const String& user ) { m_authUser = user; }
    virtual const String& getAuthUser() const { return m_authUser; }
    virtual void setAuthPassword( const String& password ) { m_authPassword = password; }
    virtual const String& getAuthPassword() const { return m_authPassword; }

protected:
  INetRequestCallback* m_pCallback;
  
  AuthMethod m_authMethod;
  String m_authUser;
  String m_authPassword;
};

typedef rho::common::CAutoPtr<rho::net::INetResponse> NetResponsePtr;

class CNetResponseWrapper
{
    NetResponsePtr m_netResp;
public:
    CNetResponseWrapper( INetResponse* resp = 0) : m_netResp(resp)  {}

    const char* getCharData() const { return m_netResp->getCharData(); }
    void setCharData(const char* szData){ return m_netResp->setCharData(szData); }
    unsigned int getDataSize() const { return m_netResp->getDataSize(); }
    int getRespCode() const { return m_netResp->getRespCode(); }
    String getCookies() const { return m_netResp->getCookies(); }
    String getErrorMessage() const { return m_netResp->getErrorMessage(); }

	boolean isOK() const
	{
		return getRespCode() == 200 || getRespCode() == 206;
	}

    boolean isUnathorized() const
    {
        return getRespCode() == 401;
    }

    boolean isRedirect() const
    {
       return getRespCode() == 301 || getRespCode() ==302;
    }

    boolean isSuccess() const
    {
        return getRespCode() > 0 && getRespCode() < 400;
    }

    boolean isResponseRecieved() const { return getRespCode() != -1; }
};

class CNetRequestHolder
{
    common::CMutex m_mxReq;
    INetRequestImpl*   m_pReq;
    boolean        m_bCancel;
    boolean        m_sslVerifyPeer;

public:

    CNetRequestHolder() : m_pReq(0), m_bCancel(false), m_sslVerifyPeer(true){}

    void setRequest(INetRequestImpl* pReq);
    void cancel();
    boolean isCancelled();
    static String resolveUrl(const String& strUrl);

    boolean getSslVerifyPeer() {return m_sslVerifyPeer;}
    void setSslVerifyPeer(boolean mode) {m_sslVerifyPeer = mode;}
    
    bool isInsideRequest() const;

    void setAuthMethod( AuthMethod method );
    void setAuthUser( const String& user );
    void setAuthPassword( const String& password );

};

class CNetRequestWrapper
{
    common::CAutoPtr<INetRequestImpl>  m_pReqImpl;
    CNetRequestHolder*                 m_pHolder;
public:
    CNetRequestWrapper(INetRequestImpl* pImpl, CNetRequestHolder* pHolder);
    ~CNetRequestWrapper();

    rho::net::CNetResponseWrapper pullData(const String& strUrl, IRhoSession* oSession );
    rho::net::CNetResponseWrapper pushData(const String& strUrl, const String& strBody, IRhoSession* oSession);
    rho::net::CNetResponseWrapper pullCookies(const String& strUrl, const String& strBody, IRhoSession* oSession);
    rho::net::CNetResponseWrapper doRequest( const char* method, const String& strUrl, const String& strBody, IRhoSession* oSession, Hashtable<String,String>* pHeaders );
    rho::net::CNetResponseWrapper pushMultipartData(const String& strUrl, VectorPtr<CMultipartItem*>& arItems, IRhoSession* oSession, Hashtable<String,String>* pHeaders);
    rho::net::CNetResponseWrapper pushMultipartData(const String& strUrl, CMultipartItem& oItem, IRhoSession* oSession, Hashtable<String,String>* pHeaders);
    rho::net::CNetResponseWrapper pullFile(const String& strUrl, const String& strFilePath, IRhoSession* oSession, Hashtable<String,String>* pHeaders,bool overwriteFile,bool createFolders, bool* pFileExistsFlag = 0 );

    String resolveUrl(const String& strUrl);
    void cancel();
    
    void setCallback(INetRequestCallback*);
    void setAuthMethod( AuthMethod method );
    void setAuthUser( const String& user );
    void setAuthPassword( const String& password );

};



}
}

typedef rho::net::CNetResponseWrapper NetResponse;
typedef rho::net::CNetRequestHolder NetRequest;

rho::net::CNetRequestWrapper getNetRequest(rho::net::CNetRequestHolder* pHolder = 0);

namespace rho
{

namespace common
{
    class CRhoThread;
}

namespace net
{

class INetRequestBuilder
{
public:
  virtual INetRequestImpl* build() = 0;
};

class INetRequestCallback
{
  public:
    virtual ~INetRequestCallback() {}

    virtual void didReceiveResponse(NetResponse&, const Hashtable<String,String>* headers) = 0;
    virtual void didReceiveData(const char*, int) = 0;
    virtual void didFinishLoading() = 0;
    virtual void didFail(NetResponse&) = 0;

};

class CAsyncNetRequest : public INetRequestCallback, private common::CRhoTimer::ICallback
{

private:
  String m_method;
  String m_url;
  String m_body;
  net::IRhoSession* m_pSession;
  Hashtable<String,String> m_headers;
  INetRequestCallback* m_pCallback;

  /*net::CNetRequestWrapper*/NetRequest  m_request;

  common::CMutex m_mxCallbackAccess;

public:

  CAsyncNetRequest()
    : m_pSession(0)
    , m_pCallback(0)
    //, m_request(::getNetRequest())
  {
  }

  ~CAsyncNetRequest();
  
  void setMethod(const String& method) { m_method = method; }
  void setUrl(const String& url) { m_url = url; }
  void setBody(const String& body) { m_body = body; }
  void setSession( IRhoSession* pSession ) { m_pSession = pSession; }
  void setCallback( INetRequestCallback* pCallback )
  { 
    synchronized(m_mxCallbackAccess)
    {
      m_pCallback = pCallback; 
    }
  }
  void setHeaders( const Hashtable<String, String> headers ) { m_headers = headers; }

  void run(common::CRhoThread &);
  void cancel();
  void requestCancel(); //will perform cancel in timer thread

  virtual void didReceiveResponse(NetResponse& resp, const Hashtable<String,String>* headers);
  virtual void didReceiveData(const char* ptr, int len);
  virtual void didFinishLoading();
  virtual void didFail(NetResponse& resp);

private:
  virtual bool onTimer();

};

}
}


