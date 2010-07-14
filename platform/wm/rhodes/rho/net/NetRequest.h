#pragma once

#include "net/INetRequest.h"
#include "logging/RhoLog.h"

namespace rho {
namespace net {

class CNetRequestImpl;
class CNetRequest : public INetRequest
{
    DEFINE_LOGCLASS;
    boolean m_bCancel;
    boolean m_sslVerifyPeer;
public:
    CNetRequestImpl* m_pCurNetRequestImpl;

    CNetRequest(void) : m_pCurNetRequestImpl(null), m_bCancel(false), m_sslVerifyPeer(true) {}
    virtual ~CNetRequest(void){}

    virtual INetResponse* pullData(const String& strUrl, IRhoSession* oSession );
    virtual INetResponse* pushData(const String& strUrl, const String& strBody, IRhoSession* oSession);
    virtual INetResponse* pushMultipartData(const String& strUrl, VectorPtr<CMultipartItem*>& arItems, IRhoSession* oSession, Hashtable<String,String>* pHeaders);
    virtual INetResponse* pushMultipartData(const String& strUrl, CMultipartItem& oItem, IRhoSession* oSession, Hashtable<String,String>* pHeaders);

    virtual INetResponse* pullFile(const String& strUrl, const String& strFilePath, IRhoSession* oSession, Hashtable<String,String>* pHeaders);
    virtual INetResponse* pullCookies(const String& strUrl, const String& strBody, IRhoSession* oSession);

    virtual INetResponse* doRequest( const char* method, const String& strUrl, const String& strBody, IRhoSession* oSession, Hashtable<String,String>* pHeaders );

    virtual boolean sslVerifyPeer() {return m_sslVerifyPeer;}
    virtual void sslVerifyPeer(boolean mode) {m_sslVerifyPeer = mode;}

    virtual String resolveUrl(const String& strUrl);

    virtual void cancel();
    virtual boolean isCancelled(){return m_bCancel;}

protected:
};

}
}

extern "C" int rho_net_has_network();
