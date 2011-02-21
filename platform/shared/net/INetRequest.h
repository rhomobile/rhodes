#pragma once

#include "common/RhoStd.h"
#include "common/InputStream.h"
#include "common/AutoPointer.h"
#include "common/RhoMutexLock.h"

namespace rho {
namespace common {
class CRhoFile;
}

namespace net {

struct INetResponse
{
    virtual ~INetResponse(void){;}
    virtual const char* getCharData() = 0;
    virtual unsigned int getDataSize() = 0;
    virtual int getRespCode() = 0;
    virtual String getCookies() = 0;

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
};

typedef rho::common::CAutoPtr<rho::net::INetResponse> NetResponsePtr;

class CNetResponseWrapper
{
    NetResponsePtr m_netResp;
public:
    CNetResponseWrapper( INetResponse* resp = 0) : m_netResp(resp)  {}

    const char* getCharData(){ return m_netResp->getCharData(); }
    unsigned int getDataSize(){ return m_netResp->getDataSize(); }
    int getRespCode(){ return m_netResp->getRespCode(); }
    String getCookies(){ return m_netResp->getCookies(); }

	boolean isOK()
	{
		return getRespCode() == 200 || getRespCode() == 206;
	}

    boolean isUnathorized()
    {
        return getRespCode() == 401;
    }

    boolean isRedirect()
    {
       return getRespCode() == 301 || getRespCode() ==302;
    }

    boolean isSuccess()
    {
        return getRespCode() > 0 && getRespCode() < 400;
    }

    boolean isResponseRecieved(){ return getRespCode() != -1; }
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
};

class CNetRequestWrapper
{
    common::CAutoPtr<INetRequestImpl>  m_pReqImpl;
    CNetRequestHolder*                 m_pHolder;
public:
    CNetRequestWrapper(INetRequestImpl* pImpl, CNetRequestHolder* pHolder);
    ~CNetRequestWrapper();

    INetResponse* pullData(const String& strUrl, IRhoSession* oSession );
    INetResponse* pushData(const String& strUrl, const String& strBody, IRhoSession* oSession);
    INetResponse* pullCookies(const String& strUrl, const String& strBody, IRhoSession* oSession);
    INetResponse* doRequest( const char* method, const String& strUrl, const String& strBody, IRhoSession* oSession, Hashtable<String,String>* pHeaders );
    INetResponse* pushMultipartData(const String& strUrl, VectorPtr<CMultipartItem*>& arItems, IRhoSession* oSession, Hashtable<String,String>* pHeaders);
    INetResponse* pushMultipartData(const String& strUrl, CMultipartItem& oItem, IRhoSession* oSession, Hashtable<String,String>* pHeaders);
    INetResponse* pullFile(const String& strUrl, const String& strFilePath, IRhoSession* oSession, Hashtable<String,String>* pHeaders);

    String resolveUrl(const String& strUrl);

};

}
}

typedef rho::net::CNetResponseWrapper NetResponse;
typedef rho::net::CNetRequestHolder NetRequest;

rho::net::CNetRequestWrapper getNetRequest(rho::net::CNetRequestHolder* pHolder = 0);


