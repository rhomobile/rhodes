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
    INetResponse *pushFile(const String& strUrl, const String& strFileName, IRhoSession *oSession, Hashtable<String,String>* pHeaders);
    INetResponse* pushMultipartData(const String& strUrl, VectorPtr<CMultipartItem*>& arItems, IRhoSession* oSession, Hashtable<String,String>* pHeaders);
    INetResponse* pushMultipartData(const String& strUrl, CMultipartItem& oItem, IRhoSession* oSession, Hashtable<String,String>* pHeaders);
    INetResponse *pullFile(const String& strUrl, const String& strFilePath, IRhoSession *oSession, Hashtable<String,String>* pHeaders);
    INetResponse *pullCookies(const String& strUrl, const String& strBody, IRhoSession *oSession);
    INetResponse* doRequest( const char* method, const String& strUrl, const String& strBody, IRhoSession* oSession, Hashtable<String,String>* pHeaders );

    boolean sslVerifyPeer() {return m_sslVerifyPeer;}
    void sslVerifyPeer(boolean mode) {m_sslVerifyPeer = mode;}
    
    String resolveUrl(const String& strUrl);

    void cancel();
    virtual boolean isCancelled(){return m_bCancel;}

private:
    curl_slist *set_curl_options(const char *method, const String& strUrl, const String& strBody,
                                 IRhoSession* pSession, Hashtable<String,String>* pHeaders);
    int getResponseCode(CURLMcode err, const String& strRespBody, IRhoSession* oSession);
    String makeCookies();
    INetResponse *makeResponse(String strBody, int nErrorCode);
    
    boolean m_bCancel;
    CURLM *curlm;
    CURL *curl;
    char *errbuf[CURL_ERROR_SIZE];
    boolean m_bTraceCalls;
    boolean m_sslVerifyPeer;
};

} // namespace net
} // namespace rho

#endif // RHO_CURL_NET_REQUEST_041be6266e3e424c8a8e7b8a412f01f0

