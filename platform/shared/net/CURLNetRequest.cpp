#include "net/CURLNetRequest.h"
#include "logging/RhoLog.h"
#include "common/RhoFile.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"

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
    m_bTraceCalls = rho_conf_getBool("net_trace");
	curlm = curl_multi_init();
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, &errbuf);
}

CURLNetRequest::~CURLNetRequest()
{
    curl_easy_cleanup(curl);
	curl_multi_cleanup(curlm);
}

static bool isLocalHost(const String& strUrl)
{
    return strUrl.find_first_of("http://localhost") == 0 ||
        strUrl.find_first_of("http://127.0.0.1") == 0;
}

static size_t curlBodyStringCallback(void *ptr, size_t size, size_t nmemb, void *opaque)
{
    String *pStr = (String *)opaque;
    size_t nBytes = size*nmemb;
    RAWTRACE1("Received %d bytes (string)", nBytes);
    pStr->append((const char *)ptr, nBytes);
    return nBytes;
}

static size_t curlBodyFileCallback(void *ptr, size_t size, size_t nmemb, void *opaque)
{
    common::CRhoFile *pFile = (common::CRhoFile*)opaque;
    size_t nBytes = size*nmemb;
    RAWTRACE1("Received %d bytes (blob)", nBytes);
    return pFile->write(ptr, nBytes);
}

#if 0
static size_t curlHeaderCallback(void *ptr, size_t size, size_t nmemb, void *opaque)
{
    Vector<String> *headers = (Vector<String>*)opaque;
    size_t nBytes = size*nmemb;
    String header((const char *)ptr, nBytes);
    RAWTRACE1("Received header: %s", header.c_str());
    headers->push_back(header);
    return nBytes;
}
#endif

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

static curl_slist *set_curl_options(bool trace, CURL *curl, const char *method, const String& strUrl,
                             const String& session)
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
    if (!session.empty()) {
        //RAWTRACE1("Set cookie: %s", session.c_str());
        curl_easy_setopt(curl, CURLOPT_COOKIE, session.c_str());
    }
    
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 180);
	curl_easy_setopt(curl, CURLOPT_TCP_NODELAY, 1);

    // Set very large timeout in case of local requests
    // It is required because otherwise requests (especially requests to writing!)
    // could repeated twice or more times
    if (isLocalHost(strUrl))
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10*24*60*60);
	
	curl_slist *hdrs = NULL;
	// Disable "Expect: 100-continue"
	hdrs = curl_slist_append(hdrs, "Expect:");
	// Add Keep-Alive header
	hdrs = curl_slist_append(hdrs, "Connection: Keep-Alive");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hdrs);
    
    if (trace) {
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, &curl_trace);
        curl_easy_setopt(curl, CURLOPT_DEBUGDATA, NULL);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    }
	
	return hdrs;
}

static curl_slist *set_curl_options(bool trace, CURL *curl, const char *method, const String& strUrl,
                             const String& strBody, const String& session)
{
    curl_slist *retval = set_curl_options(trace, curl, method, strUrl, session);
    if (strcasecmp(method, "POST") == 0) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strBody.size());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strBody.c_str());
    }
	return retval;
}

static curl_slist *set_curl_options(bool trace, CURL *curl, const char *method, const String& strUrl,
                             const String& strBody, const String& session, const String& result)
{
    curl_slist *retval = set_curl_options(trace, curl, method, strUrl, strBody, session);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &curlBodyStringCallback);
    return retval;
}

static curl_slist *set_curl_options(bool trace, CURL *curl, const char *method, const String& strUrl,
                            const String& strBody, const String& session, const common::CRhoFile *pFile)
{
    curl_slist *retval = set_curl_options(trace, curl, method, strUrl, strBody, session);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, pFile);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &curlBodyFileCallback);
    return retval;
}

static CURLMcode do_curl_perform(CURLM *curlm, CURL *curl)
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
    curl_slist *hdrs = set_curl_options(m_bTraceCalls, curl, method, strUrl, strBody, session, result);
	//curl_easy_perform(curl);
	CURLMcode err = do_curl_perform(curlm, curl);
	curl_slist_free_all(hdrs);
	
    rho_net_impl_network_indicator(0);
	
	if (err != CURLM_OK) {
		RAWLOG_ERROR1("Error when calling curl_multi_perform: %d", err);
		return NULL;
	}

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
    curl_slist *hdrs = set_curl_options(m_bTraceCalls, curl, method, strUrl, strBody, session, result);
    //curl_easy_perform(curl);
	CURLMcode err = do_curl_perform(curlm, curl);
	curl_slist_free_all(hdrs);
	
    rho_net_impl_network_indicator(0);
	
	if (err != CURLM_OK) {
		RAWLOG_ERROR1("Error when calling curl_multi_perform: %d", err);
		return NULL;
	}

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
        }
    }

    return respData;
}

char* CURLNetRequest::pullMultipartData(const String& strUrl, int* pnRespCode, void* oFile, IRhoSession *oSession)
{
    if (pnRespCode)
        *pnRespCode = -1;
    String session;
    if (oSession)
        session = oSession->getSession();
    //if (session.empty() && !isLocalHost(strUrl))
    //    return NULL;
    
    RAWLOG_INFO1("Request url: %s", strUrl.c_str());
    
    rho_net_impl_network_indicator(1);
    
    curl_slist *hdrs = set_curl_options(m_bTraceCalls, curl, "GET", strUrl, "", session, (common::CRhoFile*)oFile);
	//curl_easy_perform(curl);
	CURLMcode err = do_curl_perform(curlm, curl);
	curl_slist_free_all(hdrs);
	
    rho_net_impl_network_indicator(0);
	
	if (err != CURLM_OK) {
		RAWLOG_ERROR1("Error when calling curl_multi_perform: %d", err);
		return NULL;
	}
    
    long statusCode = 0;
    if (curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode) != 0)
        statusCode = 500;
    if (pnRespCode)
        *pnRespCode = (int)statusCode;
    
    if (statusCode != 200) {
        RAWLOG_ERROR1("Request failed. HTTP Code: %d returned", (int)statusCode);
        if (statusCode == 401)
            if (oSession)
                oSession->logout();
    }
    
    return str_assign("OK");
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
        curl_slist *hdrs = set_curl_options(m_bTraceCalls, curl, "POST", strUrl, "", session, result);

        curl_httppost *post = NULL, *last = NULL;
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, NULL);
        curl_formadd(&post, &last,
                     CURLFORM_COPYNAME, "blob",
                     CURLFORM_FILE, strFilePath.c_str(),
                     CURLFORM_CONTENTTYPE, "application/octet-stream",
                     CURLFORM_END);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, post);
		
        //curl_easy_perform(curl);
		CURLMcode err = do_curl_perform(curlm, curl);

        curl_slist_free_all(hdrs);
        curl_formfree(post);

        rho_net_impl_network_indicator(0);
		
		if (err != CURLM_OK) {
			RAWLOG_ERROR1("Error when calling curl_multi_perform: %d", err);
			return NULL;
		}

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
		//RAWLOG_INFO("start Net request");
        response = (this->*func)(method, strUrl, strBody, &nRespCode, oSession);
		//RAWLOG_INFO("end Net request");
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

INetResponse* CURLNetRequest::doRequest( const char* method, const String& strUrl, const String& strBody, IRhoSession* oSession, Hashtable<String,String>* pHeaders )
{
    //TODO: doRequest - pHeaders
    return doRequestTry(method, strUrl, strBody, &CURLNetRequest::request, oSession );
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
        RAWLOG_ERROR1("pullFile: cannot create file: %s", strFilePath.c_str());
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

String CURLNetRequest::resolveUrl(const String& strUrl)
{
    return RHODESAPP().canonicalizeRhoUrl(strUrl);
}

void CURLNetRequest::cancel()
{
    m_bCancel = true;
	curl_multi_remove_handle(curlm, curl);
}

} // namespace net
} // namespace rho

