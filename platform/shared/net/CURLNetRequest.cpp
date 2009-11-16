#include "net/CURLNetRequest.h"
#include "logging/RhoLog.h"
#include "common/RhoFile.h"
#include "common/RhodesApp.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Net"

extern "C" void rho_net_impl_network_indicator(int active);

namespace rho
{
namespace net
{

IMPLEMENT_LOGCLASS(CURLNetRequest, "Net");

class CURLNetResponseImpl : public INetResponse
{
    char* m_pData;
    int   m_nRespCode;

public:
    CURLNetResponseImpl(char* data, int nRespCode) : m_pData(data), m_nRespCode(nRespCode){}
    ~CURLNetResponseImpl()
    {
      if (m_pData)
        free(m_pData);
    }

    virtual const char* getCharData()
    {
      return m_pData ? m_pData : "";
    }

    virtual unsigned int getDataSize()
    {
      return m_pData ? strlen(m_pData) : 0;
    }

    void setRespCode(int nRespCode) 
    {
      m_nRespCode = nRespCode;
    }

    virtual int getRespCode() 
    {
      return m_nRespCode;
    }

    virtual boolean isOK()
    {
      return m_nRespCode == 200;
    }
    virtual boolean isUnathorized()
    {
        return m_nRespCode == 401;
    }

    virtual boolean isResponseRecieved(){ return m_nRespCode!=-1;}

    void setCharData(const char* pData)
    {
        if (m_pData)
        {
            free(m_pData);
            m_pData = 0;
        }
        m_pData = strdup(pData);
    }
};

CURLNetRequest::CURLNetRequest()
{
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, &errbuf);
}

CURLNetRequest::~CURLNetRequest()
{
    curl_easy_cleanup(curl);
}

static bool isLocalHost(const String& strUrl)
{
    return strUrl.find_first_of("http://localhost") == 0 ||
        strUrl.find_first_of("http://127.0.0.1") == 0;
}

static size_t curlBodyCallback(void *ptr, size_t size, size_t nmemb, void *opaque)
{
    String *pStr = (String *)opaque;
    size_t nBytes = size*nmemb;
    RAWTRACE1("Received %d bytes", nBytes);
    pStr->append((const char *)ptr, nBytes);
    return nBytes;
}

static size_t curlHeaderCallback(void *ptr, size_t size, size_t nmemb, void *opaque)
{
    Vector<String> *headers = (Vector<String>*)opaque;
    size_t nBytes = size*nmemb;
    String header((const char *)ptr, nBytes);
    RAWTRACE1("Received header: %s", header.c_str());
    headers->push_back(header);
    return nBytes;
}

static void set_curl_options(CURL *curl, const char *method, const String& strUrl,
                             const String& session, String& result)
{
    curl_easy_reset(curl);
    if (strcasecmp(method, "GET") == 0)
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    else if (strcasecmp(method, "POST") == 0)
        curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
    if (!session.empty())
        curl_easy_setopt(curl, CURLOPT_COOKIE, session.c_str());
    // Just to enable cookie parser
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 180);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &curlBodyCallback);

    // Set very large timeout in case of local requests
    // It is required because otherwise requests (especially requests to writing!)
    // could repeated twice or more times
    if (isLocalHost(strUrl))
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10*24*60*60);
}

static void set_curl_options(CURL *curl, const char *method, const String& strUrl,
                             const String& strBody, const String& session, String& result)
{
    set_curl_options(curl, method, strUrl, session, result);
    if (strcasecmp(method, "POST") == 0) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strBody.size());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strBody.c_str());
    }
}

char* CURLNetRequest::request(const char *method, const String& strUrl, const String& strBody,
                           int *pnRespCode, IRhoSession* oSession)
{
    if (pnRespCode)
        *pnRespCode = -1;
    String session;
    if (oSession)
        session = oSession->getSession();
    if (session.empty() && !isLocalHost(strUrl))
        return NULL;

    RAWLOG_INFO1("Request url: %s", strUrl.c_str());

    rho_net_impl_network_indicator(1);

    String result;
    set_curl_options(curl, method, strUrl, strBody, session, result);
    curl_easy_perform(curl);

    rho_net_impl_network_indicator(0);

    long statusCode = 0;
    if (curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode) != 0)
        statusCode = 500;
    if (pnRespCode)
        *pnRespCode = (int)statusCode;

    char* respData = NULL;
    if (statusCode != 500 && statusCode != 422)
        respData = str_assign((char*)result.c_str());

    if (statusCode != 200) {
        RAWLOG_ERROR2("Request failed. HTTP Code: %d returned. HTTP Response: %s",
                      (int)statusCode, respData ? respData : "<null>");
        if (statusCode == 401)
            if (oSession)
                oSession->logout();
    }
    else {
        RAWTRACE("RESPONSE-----");
        RAWTRACE(respData);
        RAWTRACE("END RESPONSE-----");
    }

    return respData;
}

char* CURLNetRequest::requestCookies(const char *method, const String& strUrl, const String& strBody,
                                  int *pnRespCode, IRhoSession* oSession)
{
    if (pnRespCode)
        *pnRespCode = -1;
    String session;
    if (oSession)
        session = oSession->getSession();
    if (!session.empty()) {
        RAWLOG_INFO("Found existing session for url...");

        if (pnRespCode)
            *pnRespCode = 200;

        return NULL;
    }

    RAWLOG_INFO1("Request cookies by Url: %s", strUrl.c_str());

    rho_net_impl_network_indicator(1);

    String result;
    set_curl_options(curl, method, strUrl, strBody, session, result);
    curl_easy_perform(curl);

    rho_net_impl_network_indicator(0);

    long statusCode = 0;
    if (curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode) != 0)
        statusCode = 500;
    if (pnRespCode)
        *pnRespCode = (int)statusCode;

    char* respData = NULL;
    if (statusCode != 500 && statusCode != 422)
        respData = str_assign((char*)result.c_str());

    if (statusCode != 200) {
        RAWLOG_ERROR2("Request cookies failed. HTTP Code: %d returned. HTTP Response: %s", 
                      (int)statusCode, respData ? respData : "<null>");
        if (statusCode == 401)
            if (oSession)
                oSession->logout();
    }
    else {
        cookies.clear();
        curl_slist *rcookies = NULL;
        if (curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &rcookies) != 0) {
            RAWLOG_ERROR1("Error when retrieving cookies: %s", errbuf);
        }
        else {
            for(curl_slist *cookie = rcookies; cookie; cookie = cookie->next) {
                char *data = cookie->data;

                // TODO: possible buffer overrun!!!
                char domain[512];
                char tailmatch[32];
                char path[512];
                char secure[32];
                char expires[32];
                char name[256];
                char value[512];
                sscanf(data, "%s\t%s\t%s\t%s\t%s\t%s\t%s", domain, tailmatch, path, secure, expires, name, value);

                cookies += name;
                cookies += "=";
                cookies += value;
                cookies += ";";
            }
            curl_slist_free_all(rcookies);
        }
    }

    return respData;
}

char* CURLNetRequest::pullMultipartData(const String& strUrl, int* pnRespCode, void* oFile, IRhoSession *oSession)
{
    // TODO:
    rho_net_impl_network_indicator(1);
    rho_net_impl_network_indicator(0);
    return NULL;
}

char* CURLNetRequest::pushMultipartData(const String& strUrl, const String& strFilePath, int* pnRespCode, IRhoSession *oSession)
{
    if (pnRespCode)
        *pnRespCode = -1;

    String session;
    if (oSession)
        session = oSession->getSession();

    char *respData = NULL;
    if (!session.empty()) {
        RAWLOG_INFO2("Push file. Url: %s; File: %s", strUrl.c_str(), strFilePath.c_str());

        rho_net_impl_network_indicator(1);

        String result;
        set_curl_options(curl, "POST", strUrl, session, result);
        //curl_slist *hdrs = NULL;
        // Disable "Expect: 100-continue"
        //hdrs = curl_slist_append(hdrs, "Expect:");
        //curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hdrs);

        curl_httppost *post = NULL, *last = NULL;
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, NULL);
        curl_formadd(&post, &last,
                     CURLFORM_COPYNAME, "blob",
                     CURLFORM_FILE, strFilePath.c_str(),
                     CURLFORM_CONTENTTYPE, "application/octet-stream",
                     CURLFORM_END);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
        curl_easy_perform(curl);

        //curl_slist_free_all(hdrs);
        curl_formfree(post);

        rho_net_impl_network_indicator(0);

        long statusCode = 0;
        if (curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode) != 0)
            statusCode = 500;
        if (pnRespCode)
            *pnRespCode = (int)statusCode;

        if (statusCode != 500 && statusCode != 422)
            respData = str_assign((char*)result.c_str());

        if (statusCode != 200) {
            RAWLOG_ERROR2("Request failed. HTTP Code: %d returned. HTTP Response: %s",
                          (int)statusCode, respData ? respData : "<null>");
            if (statusCode == 401)
                if (oSession)
                    oSession->logout();
        }
        else {
            RAWTRACE("RESPONSE-----");
            RAWTRACE(respData);
            RAWTRACE("END RESPONSE-----");
        }
    }

    return respData;
}

INetResponse* CURLNetRequest::doRequestTry(const char* method, const String& strUrl, const String& strBody,
                                        Frho_net_impl_request func, IRhoSession* oSession )
{
    int nRespCode = -1;
    int nTry = 0;
    m_bCancel = false;
    char* response = 0;

    do{
        response = (this->*func)(method, strUrl, strBody, &nRespCode, oSession);
        nTry++;
    }while( !m_bCancel && nRespCode<0 && nTry < MAX_NETREQUEST_RETRY);

    return new CURLNetResponseImpl(response, nRespCode);
}

INetResponse* CURLNetRequest::pullData(const String& strUrl, IRhoSession* oSession )
{
    return doRequestTry("GET", strUrl, String(), &CURLNetRequest::request, oSession );
}

INetResponse* CURLNetRequest::pushData(const String& strUrl, const String& strBody, IRhoSession* oSession)
{
    return doRequestTry("POST", strUrl, strBody, &CURLNetRequest::request, oSession );
}

INetResponse* CURLNetRequest::pullCookies(const String& strUrl, const String& strBody, IRhoSession* oSession)
{
    INetResponse* resp = doRequestTry("POST", strUrl, strBody, &CURLNetRequest::requestCookies, oSession );
    if ( resp && resp->isOK() )
        ((CURLNetResponseImpl*)resp)->setCharData(cookies.c_str());

    return resp;
}

INetResponse* CURLNetRequest::pushFile(const String& strUrl, const String& strFilePath, IRhoSession* oSession)
{
    common::CRhoFile oFile;
    if ( !oFile.open(strFilePath.c_str(),common::CRhoFile::OpenReadOnly) ) 
    {
        LOG(ERROR) + "pushFile: cannot find file :" + strFilePath;
        return new CURLNetResponseImpl(0,-1);
    }

    int nRespCode = -1;
    int nTry = 0;
    m_bCancel = false;
    char* response = 0;

    do{
        response = pushMultipartData(strUrl, strFilePath, &nRespCode, oSession);
        nTry++;
    }while( !m_bCancel && nRespCode<0 && nTry < MAX_NETREQUEST_RETRY);

    return new CURLNetResponseImpl(response, nRespCode);
}

INetResponse* CURLNetRequest::pullFile(const String& strUrl, const String& strFilePath, IRhoSession* oSession)
{
    common::CRhoFile oFile;
    if ( !oFile.open(strFilePath.c_str(),common::CRhoFile::OpenForWrite) ) 
    {
        LOG(ERROR) + "pullFile: cannot create file :" + strFilePath;
        return new CURLNetResponseImpl(0,-1);
    }

    int nRespCode = -1;
    int nTry = 0;
    m_bCancel = false;
    char* response = 0;
    do{
        response = pullMultipartData(strUrl, &nRespCode, &oFile, oSession);
        nTry++;
    }while( !m_bCancel && nRespCode<0 && nTry < MAX_NETREQUEST_RETRY);

    return new CURLNetResponseImpl(response,nRespCode);
}

void CURLNetRequest::deleteCookie(const String& strUrl)
{
    // Not implemented
}

String CURLNetRequest::resolveUrl(const String& strUrl)
{
    return RHODESAPP().canonicalizeRhoUrl(strUrl);
}

void CURLNetRequest::cancel()
{
    m_bCancel = true;
    // TODO
    //curl_easy_pause(curl, CURLPAUSE_ALL);
}

} // namespace net
} // namespace rho

