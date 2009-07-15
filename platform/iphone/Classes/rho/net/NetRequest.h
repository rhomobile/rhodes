#pragma once

#ifdef __cplusplus

#include "net/INetRequest.h"
#include "logging/RhoLog.h"

namespace rho {
namespace net {

class CNetRequest : public INetRequest
{
    DEFINE_LOGCLASS;
	boolean m_bCancel;
public:
	void* m_pConnData;
	
    CNetRequest(void) : m_pConnData(0){}
    virtual ~CNetRequest(void){}

    virtual INetResponse* pullData(const String& strUrl );
    virtual INetResponse* pushData(const String& strUrl, const String& strBody);
    virtual INetResponse* pushFile(const String& strUrl, const String& strFileName);
    virtual INetResponse* pullFile(const String& strUrl, const String& strFilePath);
    virtual INetResponse* pullCookies(const String& strUrl, const String& strBody);
    //if strUrl.length() == 0 delete all cookies if possible
    virtual void deleteCookie(const String& strUrl);

    virtual String resolveUrl(const String& strUrl);

    virtual void cancel();
};

}
}
#else
#include "common/RhoPort.h"
#endif //__cplusplus

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

int rho_net_has_network();
	
#ifdef __cplusplus
};
#endif //__cplusplus
	
