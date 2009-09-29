#include "NetRequest.h"
#include "common/AutoPointer.h"
#include "common/RhoFile.h"

extern "C" {
	
char* HTTPResolveUrl(char* url);
char* rho_net_impl_request(const char* szMethod, const char* szUrl, const char* szBody, int* pnRespCode, FSAVECONNDATA fSave, void* pThis );
char* rho_net_impl_requestCookies(const char* szMethod, const char* szUrl, const char* szBody, int* pnRespCode, FSAVECONNDATA fSave, void* pThis );
//int   rho_net_impl_pushFile(const char* szUrl, const char* szFilePath, int* pbRespRecieved, FSAVECONNDATA fSave, void* pThis);
char*  rho_net_impl_pullFile(const char* szUrl, int* pnRespCode, int (*writeFunc)(void* pThis, void* pData, int nSize), void* pThisFile, FSAVECONNDATA fSave, void* pThis);
char*  rho_net_impl_pushMultipartData(const char* url, const char* data, size_t data_size, int* pnRespCode, FSAVECONNDATA fSave, void* pThis);	
void rho_net_impl_deleteAllCookies();
void rho_net_impl_cancel(void* pConnData);
}

namespace rho {
namespace net {
IMPLEMENT_LOGCLASS(CNetRequest,"Net");

class CNetResponseImpl : public INetResponse
{
	char* pData;
	int   m_nRespCode;
	
public:
	CNetResponseImpl(char* data, int nRespCode) : pData(data), m_nRespCode(nRespCode){}
	~CNetResponseImpl()
	{
		if (pData)
			free(pData);
	}
			
	virtual const char* getCharData()
	{
		return pData ? pData : "";
	}
			
	virtual unsigned int getDataSize()
	{
		return pData ? strlen(pData) : 0;
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
};

extern "C" void saveConnData(void* pThis, void* pData)
{
	CNetRequest* pNetRequest = (CNetRequest*)pThis;
	pNetRequest->m_pConnData = pData;
}

INetResponse* CNetRequest::doRequestTry(const char* method, const String& strUrl, const String& strBody, Frho_net_impl_request func )
{
	int nRespCode = -1;
	int nTry = 0;
	m_bCancel = false;
	char* response = 0;
	
	do{
		response = (*func)(method, strUrl.c_str(), strBody.c_str(), &nRespCode, saveConnData, this );
		nTry++;
	}while( !m_bCancel && nRespCode<0 && nTry < MAX_NETREQUEST_RETRY);
	
	return new CNetResponseImpl(response, nRespCode);
}
	
INetResponse* CNetRequest::pullData(const String& strUrl )
{
	return doRequestTry("GET", strUrl, String(), rho_net_impl_request );
}
	
INetResponse* CNetRequest::pushData(const String& strUrl, const String& strBody)
{
	return doRequestTry("POST", strUrl, strBody, rho_net_impl_request );
}

INetResponse* CNetRequest::pullCookies(const String& strUrl, const String& strBody)
{
	return doRequestTry("POST", strUrl, strBody, rho_net_impl_requestCookies );
}
	
static const char* szMultipartPrefix = 
"------------A6174410D6AD474183FDE48F5662FCC5\r\n"
"Content-Disposition: form-data; name=\"blob\"; filename=\"doesnotmatter.png\"\r\n"
"Content-Type: application/octet-stream\r\n\r\n";
static const char* szMultipartPostfix = 
"\r\n------------A6174410D6AD474183FDE48F5662FCC5--";

//static const char* szMultipartContType = 
//"multipart/form-data; boundary=----------A6174410D6AD474183FDE48F5662FCC5\r\n";
	
INetResponse* CNetRequest::pushFile(const String& strUrl, const String& strFilePath)
{
    common::CRhoFile oFile;
    if ( !oFile.open(strFilePath.c_str(),common::CRhoFile::OpenReadOnly) ) 
    {
        LOG(ERROR) + "pushFile: cannot find file :" + strFilePath;
        return new CNetResponseImpl(0,-1);
    }

	//TODO: call rho_net_impl_pushFile
	int nFileSize = oFile.size();
	int nDataLen = nFileSize+strlen(szMultipartPrefix)+strlen(szMultipartPostfix);
	char* data = (char*)malloc(nDataLen);
	memcpy(data, szMultipartPrefix, strlen(szMultipartPrefix) );
	oFile.readData(data,strlen(szMultipartPrefix),nFileSize);
	memcpy(data+nFileSize+strlen(szMultipartPrefix), szMultipartPostfix, strlen(szMultipartPostfix) );
	
	int nRespCode = -1;
	int nTry = 0;
	m_bCancel = false;
	char* response = 0;
	
	do{
		response = rho_net_impl_pushMultipartData(strUrl.c_str(), data, nDataLen, &nRespCode, saveConnData, this );
		nTry++;
	}while( !m_bCancel && nRespCode<0 && nTry < MAX_NETREQUEST_RETRY);
	
	return new CNetResponseImpl(response, nRespCode);
}

extern "C"	int writeToFile(void* pThis, void* pData, int nSize)
{
	common::CRhoFile& oFile = *((common::CRhoFile*)pThis);
	return oFile.write(pData, nSize);
}
	
INetResponse* CNetRequest::pullFile(const String& strUrl, const String& strFilePath)
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
		response = rho_net_impl_pullFile(strUrl.c_str(), &nRespCode, writeToFile, &oFile, saveConnData, this );
		nTry++;
	}while( !m_bCancel && nRespCode<0 && nTry < MAX_NETREQUEST_RETRY);
	
	return new CNetResponseImpl(response,nRespCode);
}

//if strUrl.length() == 0 delete all cookies if possible
void CNetRequest::deleteCookie(const String& strUrl)
{
	if ( strUrl.length() == 0 )
		rho_net_impl_deleteAllCookies();
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
	if ( m_pConnData != null )
		rho_net_impl_cancel(m_pConnData);
}

}
}
