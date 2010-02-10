#ifndef RHO_CURL_NET_REQUEST_041be6266e3e424c8a8e7b8a412f01f0
#define RHO_CURL_NET_REQUEST_041be6266e3e424c8a8e7b8a412f01f0

//#include "curl/lib/setup.h"
#include "curl/curl.h"
#include "net/INetRequest.h"
#include "logging/RhoLog.h"

namespace rho
{
namespace net
{

class CURLNetRequest : public INetRequest
{
    DEFINE_LOGCLASS;
public:
    CURLNetRequest();
    ~CURLNetRequest();

    INetResponse *pullData(const String& strUrl, IRhoSession *oSession);
    INetResponse *pushData(const String& strUrl, const String& strBody, IRhoSession *oSession);
    INetResponse *pushFile(const String& strUrl, const String& strFileName, IRhoSession *oSession);
    INetResponse *pullFile(const String& strUrl, const String& strFilePath, IRhoSession *oSession);
    INetResponse *pullCookies(const String& strUrl, const String& strBody, IRhoSession *oSession);
    INetResponse* doRequest( const char* method, const String& strUrl, const String& strBody, IRhoSession* oSession, Hashtable<String,String>* pHeaders );

    String resolveUrl(const String& strUrl);

    void cancel();
    virtual boolean isCancelled(){return m_bCancel;}

private:
    char* request(const char *method, const String& strUrl, const String& strBody,
                  int *pnRespCode, IRhoSession* oSession);
    char* requestCookies(const char *method, const String& strUrl, const String& strBody,
                  int *pnRespCode, IRhoSession* oSession);

    typedef char* (CURLNetRequest::*Frho_net_impl_request)(const char* method, const String& strUrl,
                  const String& strBody, int* pnRespCode, IRhoSession* oSession);

    char* pullMultipartData(const String& strUrl, int* pnRespCode, void* oFile, IRhoSession *oSession);
    char* pushMultipartData(const String& strUrl, const String& strFilePath, int* pnRespCode, IRhoSession *oSession);

    INetResponse* doRequestTry(const char* method, const String& strUrl, const String& strBody,
        Frho_net_impl_request func, IRhoSession* oSession );

private:
    boolean m_bCancel;
	CURLM *curlm;
    CURL *curl;
    char *errbuf[CURL_ERROR_SIZE];
    String cookies;
    boolean m_bTraceCalls;
};

} // namespace net
} // namespace rho

#endif // RHO_CURL_NET_REQUEST_041be6266e3e424c8a8e7b8a412f01f0

