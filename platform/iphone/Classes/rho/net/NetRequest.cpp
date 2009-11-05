#include "NetRequest.h"
#include "common/AutoPointer.h"
#include "common/RhoFile.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Net"

extern "C" {

char* HTTPResolveUrl(char* url);
#ifdef RHO_NET_NEW_IMPL
void rho_net_impl_network_indicator(int active);
#else
char* rho_net_impl_request(const char* szMethod, const char* szUrl, const char* szBody, int* pnRespCode, FSAVECONNDATA fSave, void* pThis );
char* rho_net_impl_requestCookies(const char* szMethod, const char* szUrl, const char* szBody, int* pnRespCode, FSAVECONNDATA fSave, void* pThis );
//int   rho_net_impl_pushFile(const char* szUrl, const char* szFilePath, int* pbRespRecieved, FSAVECONNDATA fSave, void* pThis);
char*  rho_net_impl_pullFile(const char* szUrl, int* pnRespCode, int (*writeFunc)(void* pThis, void* pData, int nSize), void* pThisFile, FSAVECONNDATA fSave, void* pThis);
char*  rho_net_impl_pushMultipartData(const char* url, const char* data, size_t data_size, int* pnRespCode, FSAVECONNDATA fSave, void* pThis);	
void rho_net_impl_deleteAllCookies();
void rho_net_impl_cancel(void* pConnData);
#endif // RHO_NET_NEW_IMPL
}

namespace rho {
namespace net {
IMPLEMENT_LOGCLASS(CNetRequest,"Net");

class CNetResponseImpl : public INetResponse
{
	char* m_pData;
	int   m_nRespCode;
	
public:
	CNetResponseImpl(char* data, int nRespCode) : m_pData(data), m_nRespCode(nRespCode){}
	~CNetResponseImpl()
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

extern "C" void saveConnData(void* pThis, void* pData)
{
	CNetRequest* pNetRequest = (CNetRequest*)pThis;
	pNetRequest->m_pConnData = pData;
}

CNetRequest::CNetRequest()
{
#ifdef RHO_NET_NEW_IMPL
	curl = ::curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, &errbuf);
	
	//String cookiefile = String(RhoGetRootPath()) + "/cookies.txt";
	//curl_easy_setopt(curl, CURLOPT_COOKIEJAR, cookiefile.c_str());
#endif // RHO_NET_NEW_IMPL
}

CNetRequest::~CNetRequest()
{
#ifdef RHO_NET_NEW_IMPL
	::curl_easy_cleanup(curl);
#endif // RHO_NET_NEW_IMPL
}

#ifdef RHO_NET_NEW_IMPL
bool isLocalHost(const String& strUrl)
{
	return strUrl.find_first_of("http://localhost") == 0 ||
		strUrl.find_first_of("http://127.0.0.1") == 0;
}

size_t curlBodyCallback(void *ptr, size_t size, size_t nmemb, void *opaque)
{
	String *pStr = (String *)opaque;
	size_t nBytes = size*nmemb;
	RAWTRACE1("Received %d bytes", nBytes);
	pStr->append((const char *)ptr, nBytes);
	return nBytes;
}

size_t curlHeaderCallback(void *ptr, size_t size, size_t nmemb, void *opaque)
{
	Vector<String> *headers = (Vector<String>*)opaque;
	size_t nBytes = size*nmemb;
	String header((const char *)ptr, nBytes);
	RAWTRACE1("Received header: %s", header.c_str());
	headers->push_back(header);
	return nBytes;
}

void set_curl_options(CURL *curl, const char *method, const String& strUrl, const String& session, String& result)
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
}

void set_curl_options(CURL *curl, const char *method, const String& strUrl, const String& strBody, const String& session, String& result)
{
	set_curl_options(curl, method, strUrl, session, result);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strBody.size());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strBody.c_str());
}
	
char* CNetRequest::request(const char *method, const String& strUrl, const String& strBody,
						   int *pnRespCode, FSAVECONNDATA fSave, IRhoSession* oSession)
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

char* CNetRequest::requestCookies(const char *method, const String& strUrl, const String& strBody,
								  int *pnRespCode, FSAVECONNDATA fSave, IRhoSession* oSession)
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

char* CNetRequest::pullMultipartData(const String& strUrl, int* pnRespCode, void* oFile, FSAVECONNDATA fSave, IRhoSession *oSession)
{
	// TODO:
	rho_net_impl_network_indicator(1);
	rho_net_impl_network_indicator(0);
	return NULL;
}

char* CNetRequest::pushMultipartData(const String& strUrl, const String& strFilePath, int* pnRespCode, FSAVECONNDATA fSave, IRhoSession *oSession)
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
		curl_slist *hdrs = NULL;
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

#endif // RHO_NET_NEW_IMPL

INetResponse* CNetRequest::doRequestTry(const char* method, const String& strUrl, const String& strBody,
										Frho_net_impl_request func, IRhoSession* oSession )
{
	int nRespCode = -1;
	int nTry = 0;
	m_bCancel = false;
	char* response = 0;

	do{
#ifdef RHO_NET_NEW_IMPL
		response = (this->*func)(method, strUrl, strBody, &nRespCode, saveConnData, oSession);
#else
		response = (*func)(method, strUrl.c_str(), strBody.c_str(), &nRespCode, saveConnData, this );
#endif // RHO_NET_NEW_IMPL
		nTry++;
	}while( !m_bCancel && nRespCode<0 && nTry < MAX_NETREQUEST_RETRY);
	
	return new CNetResponseImpl(response, nRespCode);
}
	
INetResponse* CNetRequest::pullData(const String& strUrl, IRhoSession* oSession )
{
#ifdef RHO_NET_NEW_IMPL
	Frho_net_impl_request func = &CNetRequest::request;
#else
	Frho_net_impl_request func = &rho_net_impl_request;
#endif // RHO_NET_NEW_IMPL
	return doRequestTry("GET", strUrl, String(), func, oSession );
}
	
INetResponse* CNetRequest::pushData(const String& strUrl, const String& strBody, IRhoSession* oSession)
{
#ifdef RHO_NET_NEW_IMPL
	Frho_net_impl_request func = &CNetRequest::request;
#else
	Frho_net_impl_request func = &rho_net_impl_request;
#endif // RHO_NET_NEW_IMPL
	return doRequestTry("POST", strUrl, strBody, func, oSession );
}

INetResponse* CNetRequest::pullCookies(const String& strUrl, const String& strBody, IRhoSession* oSession)
{
#ifdef RHO_NET_NEW_IMPL
	Frho_net_impl_request func = &CNetRequest::requestCookies;
#else
	Frho_net_impl_request func = &rho_net_impl_requestCookies;
#endif // RHO_NET_NEW_IMPL
	INetResponse* resp = doRequestTry("POST", strUrl, strBody, func, oSession );
    if ( resp && resp->isOK() )
        ((CNetResponseImpl*)resp)->setCharData(cookies.c_str());
	
	return resp;
}

#if !defined(RHO_NET_NEW_IMPL)
static const char* szMultipartPrefix = 
"------------A6174410D6AD474183FDE48F5662FCC5\r\n"
"Content-Disposition: form-data; name=\"blob\"; filename=\"doesnotmatter.png\"\r\n"
"Content-Type: application/octet-stream\r\n\r\n";
static const char* szMultipartPostfix = 
"\r\n------------A6174410D6AD474183FDE48F5662FCC5--";
#endif // !defined(RHO_NET_NEW_IMPL)

//static const char* szMultipartContType = 
//"multipart/form-data; boundary=----------A6174410D6AD474183FDE48F5662FCC5\r\n";
	
INetResponse* CNetRequest::pushFile(const String& strUrl, const String& strFilePath, IRhoSession* oSession)
{
    common::CRhoFile oFile;
    if ( !oFile.open(strFilePath.c_str(),common::CRhoFile::OpenReadOnly) ) 
    {
        LOG(ERROR) + "pushFile: cannot find file :" + strFilePath;
        return new CNetResponseImpl(0,-1);
    }

#if !defined(RHO_NET_NEW_IMPL)
	//TODO: call rho_net_impl_pushFile
	int nFileSize = oFile.size();
	int nDataLen = nFileSize+strlen(szMultipartPrefix)+strlen(szMultipartPostfix);
	char* data = (char*)malloc(nDataLen);
	memcpy(data, szMultipartPrefix, strlen(szMultipartPrefix) );
	oFile.readData(data,strlen(szMultipartPrefix),nFileSize);
	memcpy(data+nFileSize+strlen(szMultipartPrefix), szMultipartPostfix, strlen(szMultipartPostfix) );
#endif // !defined(RHO_NET_NEW_IMPL)
	
	int nRespCode = -1;
	int nTry = 0;
	m_bCancel = false;
	char* response = 0;
	
	do{
#ifdef RHO_NET_NEW_IMPL
		response = pushMultipartData(strUrl, strFilePath, &nRespCode, saveConnData, oSession);
#else
		response = rho_net_impl_pushMultipartData(strUrl.c_str(), data, nDataLen, &nRespCode, saveConnData, this );
#endif // RHO_NET_NEW_IMPL
		nTry++;
	}while( !m_bCancel && nRespCode<0 && nTry < MAX_NETREQUEST_RETRY);
	
	return new CNetResponseImpl(response, nRespCode);
}

extern "C"	int writeToFile(void* pThis, void* pData, int nSize)
{
	common::CRhoFile& oFile = *((common::CRhoFile*)pThis);
	return oFile.write(pData, nSize);
}
	
INetResponse* CNetRequest::pullFile(const String& strUrl, const String& strFilePath, IRhoSession* oSession)
{
    common::CRhoFile oFile;
    if ( !oFile.open(strFilePath.c_str(),common::CRhoFile::OpenForWrite) ) 
    {
        LOG(ERROR) + "pullFile: cannot create file :" + strFilePath;
        return new CNetResponseImpl(0,-1);
    }
	
	int nRespCode = -1;
	int nTry = 0;
	m_bCancel = false;
	char* response = 0;
	do{
#ifdef RHO_NET_NEW_IMPL
		response = pullMultipartData(strUrl, &nRespCode, &oFile, saveConnData, oSession);
#else
		response = rho_net_impl_pullFile(strUrl.c_str(), &nRespCode, writeToFile, &oFile, saveConnData, this );
#endif // RHO_NET_NEW_IMPL
		nTry++;
	}while( !m_bCancel && nRespCode<0 && nTry < MAX_NETREQUEST_RETRY);
	
	return new CNetResponseImpl(response,nRespCode);
}

//if strUrl.length() == 0 delete all cookies if possible
void CNetRequest::deleteCookie(const String& strUrl)
{
#if !defined(RHO_NET_NEW_IMPL)
	if ( strUrl.length() == 0 )
		rho_net_impl_deleteAllCookies();
#endif // RHO_NET_NEW_IMPL
}

String CNetRequest::resolveUrl(const String& strUrl)
{
    char* url = HTTPResolveUrl( strdup(strUrl.c_str()) );
    String res = url;
    free(url);
    return res;
}

void CNetRequest::cancel()
{
	m_bCancel = true;
#ifdef RHO_NET_NEW_IMPL
	curl_easy_pause(curl, CURLPAUSE_ALL);
#else
	if ( m_pConnData != null )
		rho_net_impl_cancel(m_pConnData);
#endif // RHO_NET_NEW_IMPL
}

}
}
