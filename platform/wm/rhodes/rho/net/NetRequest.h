#pragma once

#include "net/INetRequest.h"

namespace rho {
namespace net {

class CNetRequest : public INetRequest
{
    DEFINE_LOGCLASS;
public:
    CNetRequest(void){}
    virtual ~CNetRequest(void){}

    virtual INetData* pullData(const String& strUrl );
    virtual boolean pushData(const String& strUrl, const String& strBody);
    virtual boolean pushFile(const String& strUrl, const String& strFilePath);
    virtual boolean pullCookies(const String& strUrl, const String& strBody);
    //if strUrl.length() == 0 delete all cookies if possible
    virtual void deleteCookie(const String& strUrl);

    virtual String resolveUrl(const String& strUrl);

    virtual void cancelAll();

protected:
    virtual INetData* doRequest( const char* method, const String& strUrl, const String& strBody );
};

}
}

extern "C" int rho_net_has_network();
