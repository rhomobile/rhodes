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

    virtual INetData* pullData(const String& strUrl );
    virtual boolean pushData(const String& strUrl, const String& strBody);
    virtual boolean pushFile(const String& strUrl, const String& strFilePath);
    virtual boolean pullFile(const String& strUrl, const String& strFilePath);
    virtual boolean pullCookies(const String& strUrl, const String& strBody);
    //if strUrl.length() == 0 delete all cookies if possible
    virtual void deleteCookie(const String& strUrl);

    virtual String resolveUrl(const String& strUrl);

    virtual void cancel();

protected:
    virtual INetData* doRequest( const char* method, const String& strUrl, const String& strBody );
};

}
}

extern "C" int rho_net_has_network();
