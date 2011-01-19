#pragma once

#include "common/RhoStd.h"
#include "common/InputStream.h"
#include "common/AutoPointer.h"

namespace rho {
namespace net {

struct INetResponse
{
    virtual ~INetResponse(void){;}
    virtual const char* getCharData() = 0;
    virtual unsigned int getDataSize() = 0;
    virtual int getRespCode() = 0;
    virtual boolean isOK() = 0;
    virtual boolean isResponseRecieved() = 0;
    virtual boolean isUnathorized() = 0;
    virtual boolean isSuccess() = 0;
    virtual String getCookies() = 0;
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

struct INetRequest
{
    virtual ~INetRequest(void){;}

    virtual INetResponse* pullData(const String& strUrl, IRhoSession* oSession ) = 0;
    virtual INetResponse* pushData(const String& strUrl, const String& strBody, IRhoSession* oSession)=0;
    virtual INetResponse* pushMultipartData(const String& strUrl, VectorPtr<CMultipartItem*>& arItems, IRhoSession* oSession, Hashtable<String,String>* pHeaders)=0;
    virtual INetResponse* pushMultipartData(const String& strUrl, CMultipartItem& oItem, IRhoSession* oSession, Hashtable<String,String>* pHeaders)=0;

    virtual INetResponse* pullFile(const String& strUrl, const String& strFileName, IRhoSession* oSession, Hashtable<String,String>* pHeaders)=0;
    virtual INetResponse* pullCookies(const String& strUrl, const String& strBody, IRhoSession* oSession)=0;

    virtual INetResponse* doRequest( const char* method, const String& strUrl, const String& strBody, IRhoSession* oSession, Hashtable<String,String>* pHeaders )=0;

    virtual boolean sslVerifyPeer() = 0;
    virtual void sslVerifyPeer(boolean mode) = 0;
    
    virtual String resolveUrl(const String& strUrl)=0;

    virtual void cancel() = 0;
    virtual boolean isCancelled() = 0;
};

#define NetResponse(name,call)\
    rho::common::CAutoPtr<rho::net::INetResponse> p##name = call;\
    rho::net::INetResponse& name = *p##name;
#define NetRequest(call)\
    rho::common::CAutoPtr<rho::net::INetResponse> p__NetReq = call;p__NetReq;

}
}
