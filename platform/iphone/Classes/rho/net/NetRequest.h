#pragma once

typedef void (*FSAVECONNDATA)(void* pThis, void* pData);

#ifdef __cplusplus

#include "RhoDefs.h"

#ifdef RHO_NET_NEW_IMPL
#include "curl/curl.h"
#endif // NEW_NET_IMPL

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
	
    CNetRequest();
    virtual ~CNetRequest();

    virtual INetResponse* pullData(const String& strUrl, IRhoSession* oSession );
    virtual INetResponse* pushData(const String& strUrl, const String& strBody, IRhoSession* oSession);
    virtual INetResponse* pushFile(const String& strUrl, const String& strFileName, IRhoSession* oSession);
    virtual INetResponse* pullFile(const String& strUrl, const String& strFilePath, IRhoSession* oSession);
    virtual INetResponse* pullCookies(const String& strUrl, const String& strBody, IRhoSession* oSession);
    //if strUrl.length() == 0 delete all cookies if possible
    virtual void deleteCookie(const String& strUrl);

    virtual String resolveUrl(const String& strUrl);

    virtual void cancel();
private:
#ifdef RHO_NET_NEW_IMPL
	char* request(const char *method, const String& strUrl, const String& strBody,
				  int *pnRespCode, FSAVECONNDATA fSave, IRhoSession* oSession);
	char* requestCookies(const char *method, const String& strUrl, const String& strBody,
						 int *pnRespCode, FSAVECONNDATA fSave, IRhoSession* oSession);
	
	typedef char* (CNetRequest::*Frho_net_impl_request)(const char* method, const String& strUrl,
					const String& strBody, int* pnRespCode, FSAVECONNDATA fSave, IRhoSession* oSession);
	
	char* pullMultipartData(const String& strUrl, int* pnRespCode, void* oFile, FSAVECONNDATA fSave, IRhoSession *oSession);
	char* pushMultipartData(const String& strUrl, const String& strFilePath, int* pnRespCode, FSAVECONNDATA fSave, IRhoSession *oSession);
#else	
	typedef char* (*Frho_net_impl_request)(const char* szMethod, const char* szUrl, const char* szBody, int* pnRespCode, FSAVECONNDATA fSave, void* pThis );
#endif // RHO_NET_NEW_IMPL
	
	INetResponse* doRequestTry(const char* method, const String& strUrl, const String& strBody, Frho_net_impl_request func, IRhoSession* oSession );

#ifdef RHO_NET_NEW_IMPL
	CURL *curl;
	char *errbuf[CURL_ERROR_SIZE];
	String cookies;
#endif // RHO_NET_NEW_IMPL
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
	
