#include "net/CURLNetRequest.h"
#include "logging/RhoLog.h"
#include "common/RhoFile.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "net/URI.h"

#include <algorithm>

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Net"

extern "C" void rho_net_impl_network_indicator(int active);

namespace rho
{
namespace net
{
curl_slist *set_curl_options(bool trace, CURL *curl, const char *method, const String& strUrl, const String& strBody,
                             IRhoSession* pSession, Hashtable<String,String>* pHeaders, bool sslVerifyPeer);
CURLMcode do_curl_perform(CURLM *curlm, CURL *curl);
	
IMPLEMENT_LOGCLASS(CURLNetRequest, "Net");

class CURLNetResponseImpl : public INetResponse
{
    String m_data;
    int   m_nRespCode;
    String m_cookies;

public:
    CURLNetResponseImpl(String const &data, int nRespCode) : m_data(data), m_nRespCode(nRespCode){}

    virtual const char* getCharData()
    {
        return m_data.c_str();
    }

    virtual unsigned int getDataSize()
    {
        return m_data.size();
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

    void setCharData(const String &data)
    {
        m_data = data;
    }
    
    void setCookies(String s)
    {
        m_cookies = s;
    }
    
    String getCookies()
    {
        return m_cookies;
    }
};

CURLNetRequest::CURLNetRequest()
{
    m_bTraceCalls = rho_conf_getBool("net_trace");
    m_sslVerifyPeer = true;
    // Init curl
    curlm = curl_multi_init();
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, &errbuf);
}

CURLNetRequest::~CURLNetRequest()
{
    curl_easy_cleanup(curl);
    curl_multi_cleanup(curlm);
}

INetResponse *CURLNetRequest::makeResponse(String strBody, int nErrorCode)
{
    std::auto_ptr<CURLNetResponseImpl> resp(new CURLNetResponseImpl(strBody, nErrorCode));
    if (resp->isOK())
        resp->setCookies(makeCookies());
    return resp.release();
}

static size_t curlBodyFileCallback(void *ptr, size_t size, size_t nmemb, void *opaque)
{
    common::CRhoFile *pFile = (common::CRhoFile*)opaque;
    size_t nBytes = size*nmemb;
    RAWTRACE1("Received %d bytes (blob)", nBytes);
    return pFile->write(ptr, nBytes);
}

static size_t curlBodyStringCallback(void *ptr, size_t size, size_t nmemb, void *opaque)
{
    String *pStr = (String *)opaque;
    size_t nBytes = size*nmemb;
    RAWTRACE1("Received %d bytes (string)", nBytes);
    pStr->append((const char *)ptr, nBytes);
    return nBytes;
}

INetResponse* CURLNetRequest::pullData(const String& strUrl, IRhoSession* oSession )
{
    return doRequest("GET", strUrl, String(), oSession, null );
}

INetResponse* CURLNetRequest::pushData(const String& strUrl, const String& strBody, IRhoSession* oSession)
{
    return doRequest("POST", strUrl, strBody, oSession, null );
}

INetResponse* CURLNetRequest::pullCookies(const String& strUrl, const String& strBody, IRhoSession* oSession)
{
    INetResponse* pResp = doRequest("POST", strUrl, strBody, oSession, null );
    if ( pResp->isOK() )
        ((CURLNetResponseImpl*)pResp)->setCharData(pResp->getCookies());

    return pResp;
}
	
INetResponse* CURLNetRequest::doRequest( const char* method, const String& strUrl, const String& strBody, IRhoSession* oSession, Hashtable<String,String>* pHeaders )
{
    //TODO: doRequest - pHeaders
    m_bCancel = false;
    int nRespCode = -1;
    String strRespBody;

    RAWLOG_INFO1("Request url: %s", strUrl.c_str());

    rho_net_impl_network_indicator(1);

    curl_slist *hdrs = set_curl_options(method, strUrl, strBody, oSession, pHeaders);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strRespBody);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &curlBodyStringCallback);

    CURLMcode err = do_curl_perform(curlm, curl);
    curl_slist_free_all(hdrs);

    rho_net_impl_network_indicator(0);

    nRespCode = getResponseCode(err, strRespBody, oSession);
    if (nRespCode == 200)
    {
        RAWTRACE("RESPONSE-----");
        RAWTRACE(strRespBody.c_str());
        RAWTRACE("END RESPONSE-----");
    }

    return makeResponse(strRespBody, nRespCode);
}

INetResponse* CURLNetRequest::pushMultipartData(const String& strUrl, VectorPtr<CMultipartItem*>& arItems, IRhoSession* oSession, Hashtable<String,String>* pHeaders)
{
    int nRespCode = -1;
    String strRespBody;
    
    rho_net_impl_network_indicator(1);
    
    curl_slist *hdrs = set_curl_options("POST", strUrl, String(), oSession, pHeaders);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strRespBody);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &curlBodyStringCallback);
	
    curl_httppost *post = NULL, *last = NULL;
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, NULL);

    for (size_t i = 0, lim = arItems.size(); i < lim; ++i) {
        CMultipartItem *mi = arItems[i];
        
        size_t cl;
        if (mi->m_strFilePath.empty())
            cl = mi->m_strBody.size();
        else {
            common::CRhoFile f;
            if (!f.open(mi->m_strFilePath.c_str(), common::CRhoFile::OpenReadOnly))
                cl = 0;
            else {
                cl = f.size();
                f.close();
            }
        }

        char buf[32];
        buf[sizeof(buf) - 1] = '\0';
        snprintf(buf, sizeof(buf) - 1, "Content-Length: %lu", (unsigned long)cl);
        curl_slist *fh = NULL;
        fh = curl_slist_append(fh, buf);

        const char *name = mi->m_strName.empty() ? "blob" : mi->m_strName.c_str();
        int opt = mi->m_strFilePath.empty() ? CURLFORM_COPYCONTENTS : CURLFORM_FILE;
        const char *data = mi->m_strFilePath.empty() ? mi->m_strBody.c_str() : mi->m_strFilePath.c_str();
        const char *ct = mi->m_strContentType.empty() ? NULL : mi->m_strContentType.c_str();
        if (ct) {
            curl_formadd(&post, &last,
                         CURLFORM_COPYNAME, name,
                         opt, data,
                         CURLFORM_CONTENTTYPE, ct,
                         CURLFORM_CONTENTHEADER, fh,
                         CURLFORM_END);
        }
        else {
            curl_formadd(&post, &last,
                         CURLFORM_COPYNAME, name,
                         opt, data,
                         CURLFORM_CONTENTHEADER, fh,
                         CURLFORM_END);
        }
    }
        
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
    
    CURLMcode err = do_curl_perform(curlm, curl);
	
    curl_slist_free_all(hdrs);
    curl_formfree(post);
    
    rho_net_impl_network_indicator(0);
    
    nRespCode = getResponseCode(err, strRespBody, oSession);
    return makeResponse(strRespBody, nRespCode);
}

INetResponse* CURLNetRequest::pushMultipartData(const String& strUrl, CMultipartItem& oItem, IRhoSession* oSession, Hashtable<String,String>* pHeaders)
{
    VectorPtr<CMultipartItem*> arItems;
    arItems.addElement(&oItem);

    INetResponse* pResp = pushMultipartData(strUrl, arItems, oSession, pHeaders);

    arItems[0] = 0; //do not delete item
    return pResp;
}

INetResponse* CURLNetRequest::pushFile(const String& strUrl, const String& strFilePath, IRhoSession* oSession, Hashtable<String,String>* pHeaders)
{
    m_bCancel = false;
    int nRespCode = -1;
    String strRespBody;
	
    RAWLOG_INFO2("Push file. Url: %s; File: %s", strUrl.c_str(), strFilePath.c_str());
	
    common::CRhoFile oFile;
    if ( !oFile.open(strFilePath.c_str(),common::CRhoFile::OpenReadOnly) ) 
    {
        LOG(ERROR) + "pushFile: cannot find file :" + strFilePath;
        return new CURLNetResponseImpl(strRespBody,nRespCode);
    }
	
    rho_net_impl_network_indicator(1);
	
    curl_slist *hdrs = set_curl_options("POST", strUrl, String(), oSession, pHeaders);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strRespBody);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &curlBodyStringCallback);
	
    curl_httppost *post = NULL, *last = NULL;
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, NULL);
    curl_formadd(&post, &last,
                 CURLFORM_COPYNAME, "blob",
                 CURLFORM_FILE, strFilePath.c_str(),
                 CURLFORM_CONTENTTYPE, "application/octet-stream",
                 CURLFORM_END);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
    
    CURLMcode err = do_curl_perform(curlm, curl);
	
    curl_slist_free_all(hdrs);
    curl_formfree(post);
	
    rho_net_impl_network_indicator(0);
    
    nRespCode = getResponseCode(err, strRespBody, oSession);
    return makeResponse(strRespBody, nRespCode);
}
	
INetResponse* CURLNetRequest::pullFile(const String& strUrl, const String& strFilePath, IRhoSession* oSession, Hashtable<String,String>* pHeaders)
{
    m_bCancel = false;
    int nRespCode = -1;
    String strRespBody;
	
    RAWLOG_INFO2("Pull file. Url: %s; File: %s", strUrl.c_str(), strFilePath.c_str());
	
    common::CRhoFile oFile;
    if ( !oFile.open(strFilePath.c_str(),common::CRhoFile::OpenForWrite) ) 
    {
        RAWLOG_ERROR1("pullFile: cannot create file: %s", strFilePath.c_str());
        return new CURLNetResponseImpl(strRespBody,nRespCode);
    }

    rho_net_impl_network_indicator(1);
    
	curl_slist *hdrs = set_curl_options("GET", strUrl, String(), oSession, pHeaders);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &oFile);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &curlBodyFileCallback);
	
	CURLMcode err = do_curl_perform(curlm, curl);
	curl_slist_free_all(hdrs);
	
    rho_net_impl_network_indicator(0);
	
	nRespCode = getResponseCode(err, strRespBody, oSession );
    return makeResponse(strRespBody, nRespCode);
}
	
int CURLNetRequest::getResponseCode(CURLMcode err, const String& strRespBody, IRhoSession* oSession )	
{    
	if (err != CURLM_OK) {
		RAWLOG_ERROR1("Error when calling curl_multi_perform: %d", err);
		return -1;
	}
	
    long statusCode = 0;
    if (curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode) != 0)
        statusCode = 500;
    
    if (statusCode != 200) {
        RAWLOG_ERROR2("Request failed. HTTP Code: %d returned. HTTP Response: %s",
                      (int)statusCode, strRespBody.c_str());
        if (statusCode == 401)
            if (oSession)
                oSession->logout();
    }
	
	return (int)statusCode;
}

String CURLNetRequest::resolveUrl(const String& strUrl)
{
    return RHODESAPP().canonicalizeRhoUrl(strUrl);
}

void CURLNetRequest::cancel()
{
    m_bCancel = true;
	curl_multi_remove_handle(curlm, curl);
}

String CURLNetRequest::makeCookies()
{
	String cookies;
	curl_slist *rcookies = NULL;
	if (curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &rcookies) != 0) {
        // No cookies
        RAWTRACE("No cookies");
        return cookies;
    }
	
    for(curl_slist *cookie = rcookies; cookie; cookie = cookie->next) {
        char *data = cookie->data;
        
        // Parse cookie which is in Netscape format:
        // domain <Tab> tailmatch <Tab> path <Tab> secure <Tab> expires <Tab> name <Tab> value
        char *s = data;
        
        // Skip 'domain'
        for (; *s == '\t'; ++s);
        for (; *s != '\t' && *s != '\0'; ++s);
        if (*s == '\0') continue;
        
        // Skip 'tailmatch'
        for (; *s == '\t'; ++s);
        for (; *s != '\t' && *s != '\0'; ++s);
        if (*s == '\0') continue;
        
        // Skip 'path'
        for (; *s == '\t'; ++s);
        for (; *s != '\t' && *s != '\0'; ++s);
        if (*s == '\0') continue;
        
        // Skip 'secure'
        for (; *s == '\t'; ++s);
        for (; *s != '\t' && *s != '\0'; ++s);
        if (*s == '\0') continue;
        
        // Skip 'expires'
        for (; *s == '\t'; ++s);
        for (; *s != '\t' && *s != '\0'; ++s);
        if (*s == '\0') continue;
        
        // Parse 'name'
        for (; *s == '\t'; ++s);
        char *name_start = s;
        for (; *s != '\t' && *s != '\0'; ++s);
        if (*s == '\0') continue;
        char *name_finish = s;
        String name(name_start, name_finish);
        
        // Parse 'value'
        for (; *s == '\t'; ++s);
        char *value = s;
        
        cookies += name;
        cookies += "=";
        cookies += value;
        cookies += ";";
    }
    curl_slist_free_all(rcookies);
	
	return cookies;
}

static int curl_trace(CURL *curl, curl_infotype type, char *data, size_t size, void *opaque)
{
    const char *text = "";
    switch (type) {
        case CURLINFO_TEXT:         text = "== Info"; break;
        case CURLINFO_HEADER_IN:    text = "<= Recv headers"; break;
        case CURLINFO_HEADER_OUT:   text = "=> Send headers"; break;
        case CURLINFO_DATA_IN:      text = "<= Recv data"; break;
        case CURLINFO_DATA_OUT:     text = "=> Send data"; break;
        case CURLINFO_SSL_DATA_IN:  text = "<= Recv SSL data"; break;
        case CURLINFO_SSL_DATA_OUT: text = "=> Send SSL data"; break;
        default: return 0;
    }
    
    String strData(data, size);
    RAWLOG_INFO3("%s (%d bytes): %s", text, size, strData.c_str());
    return 0;
}

static size_t curlHeaderCallback(void *ptr, size_t size, size_t nmemb, void *opaque)
{
    Hashtable<String,String>* pHeaders = (Hashtable<String,String>*)opaque;
    size_t nBytes = size*nmemb;
    String strHeader((const char *)ptr, nBytes);
    RAWTRACE1("Received header: %s", strHeader.c_str());
    
    int nSep = strHeader.find(':');
    if (nSep > 0 )
    {		
        String strName = String_trim(strHeader.substr(0, nSep));
        String lName;
        std::transform(strName.begin(), strName.end(), std::back_inserter(lName), &::tolower);

        String strValue = String_trim(strHeader.substr(nSep+1, strHeader.length() - (nSep+3) ));
    
        pHeaders->put(lName, strValue);
    }
    
    return nBytes;
}

curl_slist *CURLNetRequest::set_curl_options(const char *method, const String& strUrl, const String& strBody,
                             IRhoSession* pSession, Hashtable<String,String>* pHeaders)
{
    curl_easy_reset(curl);
    if (strcasecmp(method, "GET") == 0)
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    else if (strcasecmp(method, "POST") == 0)
        curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
    
    // Just to enable cookie parser
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
    // It will clear all stored cookies
    curl_easy_setopt(curl, CURLOPT_COOKIELIST, "ALL");
    String session;
    if (pSession)
        session = pSession->getSession();
    
    if (!session.empty()) {
        //RAWTRACE1("Set cookie: %s", session.c_str());
        curl_easy_setopt(curl, CURLOPT_COOKIE, session.c_str());
    }
    
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 180);
    curl_easy_setopt(curl, CURLOPT_TCP_NODELAY, 1);
    
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, (long)m_sslVerifyPeer);
    
    // Set very large timeout in case of local requests
    // It is required because otherwise requests (especially requests to writing!)
    // could repeated twice or more times
    if (net::URI::isLocalHost(strUrl))
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10*24*60*60);
    
    curl_slist *hdrs = NULL;
    // Disable "Expect: 100-continue"
    hdrs = curl_slist_append(hdrs, "Expect:");
    // Add Keep-Alive header
    hdrs = curl_slist_append(hdrs, "Connection: Keep-Alive");

    if (strcasecmp(method, "POST") == 0) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strBody.size());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strBody.c_str());
    }
    
    bool hasContentType = false;
    if (pHeaders)
    {
        for ( Hashtable<String,String>::iterator it = pHeaders->begin();  it != pHeaders->end(); ++it )
        {
            if (!hasContentType && strcasecmp(it->first.c_str(), "content-type") == 0)
                hasContentType = true;
            String strHeader = it->first + ": " + it->second;
            hdrs = curl_slist_append(hdrs, strHeader.c_str());
//            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hdrs);
        }
        
        //set header callback
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, pHeaders);
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, &curlHeaderCallback);
        
    }
    
    if (!hasContentType && strcasecmp(method, "POST") == 0 && strBody.length() > 0)
    {
        String strHeader = "Content-Type: ";
        if ( pSession )
            strHeader += pSession->getContentType().c_str();
        else
            strHeader += "application/x-www-form-urlencoded";
        hdrs = curl_slist_append(hdrs, strHeader.c_str());
    }
    
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hdrs);
    
    if (m_bTraceCalls) {
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, &curl_trace);
        curl_easy_setopt(curl, CURLOPT_DEBUGDATA, NULL);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    }
    
    return hdrs;
}

CURLMcode do_curl_perform(CURLM *curlm, CURL *curl)
{
    int running = 0;
    curl_multi_add_handle(curlm, curl);
    CURLMcode err;
    for(;;) {
        err = curl_multi_perform(curlm, &running);
        if (err == CURLM_CALL_MULTI_PERFORM)
            continue;
        if (err == CURLM_OK && running > 0) {
            //RAWTRACE("curl_multi_perform returns OK, but we still have active transfers");
            fd_set rfd, wfd, efd;
            int n = 0;
            FD_ZERO(&rfd);
            FD_ZERO(&wfd);
            FD_ZERO(&efd);
            curl_multi_fdset(curlm, &rfd, &wfd, &efd, &n);
            if (n < 0) n = 0;
            timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = 100000;
            select(n, &rfd, &wfd, &efd, &tv);
            continue;
        }
        break;
    }
    curl_multi_remove_handle(curlm, curl);
    return err;
}
	
} // namespace net
} // namespace rho

