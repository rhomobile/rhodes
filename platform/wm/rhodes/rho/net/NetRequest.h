#pragma once

#include "net/INetRequest.h"

namespace rho {
namespace net {

class CNetRequestImpl;
class CNetRequest : public INetRequest
{
    DEFINE_LOGCLASS;
    boolean m_bCancel;
public:
    CNetRequestImpl* m_pCurNetRequestImpl;

    CNetRequest(void) : m_pCurNetRequestImpl(null), m_bCancel(false){}
    virtual ~CNetRequest(void){}

    virtual INetResponse* pullData(const String& strUrl, IRhoSession* oSession );
    virtual INetResponse* pushData(const String& strUrl, const String& strBody, IRhoSession* oSession);
    virtual INetResponse* pushFile(const String& strUrl, const String& strFilePath, IRhoSession* oSession);
    virtual INetResponse* pullFile(const String& strUrl, const String& strFilePath, IRhoSession* oSession);
    virtual INetResponse* pullCookies(const String& strUrl, const String& strBody, IRhoSession* oSession);

    virtual INetResponse* doRequest( const char* method, const String& strUrl, const String& strBody, IRhoSession* oSession, Hashtable<String,String>* pHeaders );

    virtual String resolveUrl(const String& strUrl);

    virtual void cancel();
    virtual boolean isCancelled(){return m_bCancel;}

protected:
};

}
}

extern "C" int rho_net_has_network();
