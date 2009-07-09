#include "NetRequest.h"
#include "common/AutoPointer.h"
#include "common/RhoFile.h"

extern "C" {
char* HTTPResolveUrl(char* url);
char* rho_net_impl_request(const char* szMethod, const char* szUrl, const char* szBody, int* pbRespRecieved );
int   rho_net_impl_requestCookies(const char* szMethod, const char* szUrl, const char* szBody, int* pbRespRecieved );
int   rho_net_impl_pushFile(const char* szUrl, const char* szFilePath, int* pbRespRecieved);
	int  rho_net_impl_pullFile(const char* szUrl, int* pbRespRecieved, int (*writeFunc)(void* pThis, void* pData, int nSize), void* pThis);
int   rho_net_impl_pushData(const char* url, const char* data, size_t data_size,const char* contentType);	
void rho_net_impl_deleteAllCookies();
void rho_net_impl_cancelAll();
}

namespace rho {
namespace net {
IMPLEMENT_LOGCLASS(CNetRequest,"Net");

class CNetData : public INetData
{
	char* pData;
			
public:
	CNetData(char* data) : pData(data){}
	~CNetData()
	{
		if (pData)
			free(pData);
	}
			
	virtual const char* getCharData()
	{
		return pData;
	}
			
	virtual unsigned int getDataSize()
	{
		return pData ? strlen(pData) : 0;
	}
};
	
INetData* CNetRequest::pullData(const String& strUrl )
{
    int bRespRecieved = 0;
	int nTry = 0;
	m_bCancel = false;
	char* response = 0;
	
	do{
		response = rho_net_impl_request("GET", strUrl.c_str(), "", &bRespRecieved );
		nTry++;
	}while( !m_bCancel && !bRespRecieved && nTry < MAX_NETREQUEST_RETRY);
	
	return new CNetData(response);
}
	
boolean CNetRequest::pushData(const String& strUrl, const String& strBody)
{
    int bRespRecieved = 0;
	int nTry = 0;
	m_bCancel = false;
	char* response = 0;
	
	do{
		response = rho_net_impl_request("POST", strUrl.c_str(), strBody.c_str(), &bRespRecieved );
		nTry++;
	}while( !m_bCancel && !bRespRecieved && nTry < MAX_NETREQUEST_RETRY);
		
	boolean bRet = response!=0;
	if ( response!=0 )
		free(response);
	
	return bRet;
}

static const char* szMultipartPrefix = 
"------------A6174410D6AD474183FDE48F5662FCC5\r\n"
"Content-Disposition: form-data; name=\"blob\"; filename=\"doesnotmatter.png\"\r\n"
"Content-Type: application/octet-stream\r\n\r\n";
static const char* szMultipartPostfix = 
"\r\n------------A6174410D6AD474183FDE48F5662FCC5--";

static const char* szMultipartContType = 
"multipart/form-data; boundary=----------A6174410D6AD474183FDE48F5662FCC5\r\n";
	
boolean CNetRequest::pushFile(const String& strUrl, const String& strFilePath)
{
    common::CRhoFile oFile;
    if ( !oFile.open(strFilePath.c_str(),common::CRhoFile::OpenReadOnly) ) 
    {
        LOG(ERROR) + "pushFile: cannot find file :" + strFilePath;
        return false;
    }
	
	int nFileSize = oFile.size();
	int nDataLen = nFileSize+strlen(szMultipartPrefix)+strlen(szMultipartPostfix);
	char* data = (char*)malloc(nDataLen);
	memcpy(data, szMultipartPrefix, strlen(szMultipartPrefix) );
	oFile.readData(data,strlen(szMultipartPrefix),nFileSize);
	memcpy(data+nFileSize-strlen(szMultipartPostfix), szMultipartPostfix, strlen(szMultipartPostfix) );
	
	boolean bRet = rho_net_impl_pushData(strUrl.c_str(), data, nDataLen, szMultipartContType ) != 0;					
	free(data);
	
	return bRet;
/*    int bRespRecieved = 0;
	int nTry = 0;
	boolean bRet = false;
	
	do{
		bRet = rho_net_impl_pushFile(strUrl.c_str(), strFileName.c_str(), &bRespRecieved ) != 0;
		nTry++;
	}while( !bRespRecieved && nTry < MAX_NETREQUEST_RETRY);
		
	return bRet;*/
}

extern "C"	int writeToFile(void* pThis, void* pData, int nSize)
{
	common::CRhoFile& oFile = *((common::CRhoFile*)pThis);
	return oFile.write(pData, nSize);
}
	
boolean CNetRequest::pullFile(const String& strUrl, const String& strFilePath)
{
    common::CRhoFile oFile;
    if ( !oFile.open(strFilePath.c_str(),common::CRhoFile::OpenForWrite) ) 
    {
        LOG(ERROR) + "pullFile: cannot create file :" + strFilePath;
        return false;
    }
	
	int bRespRecieved = 0;
	int nTry = 0;
	m_bCancel = false;
	boolean bRes = false;
	do{
		bRes = rho_net_impl_pullFile(strUrl.c_str(), &bRespRecieved, writeToFile, &oFile ) != 0 ? true : false;
		nTry++;
	}while( !m_bCancel && !bRespRecieved && nTry < MAX_NETREQUEST_RETRY);
	
	return bRes;
}

boolean CNetRequest::pullCookies(const String& strUrl, const String& strBody)
{
    int bRespRecieved = 0;
	int nTry = 0;
	m_bCancel = false;
	boolean bRet = false;
	
	do{
		bRet = rho_net_impl_requestCookies("POST", strUrl.c_str(), strBody.c_str(), &bRespRecieved ) != 0;
		nTry++;
	}while( !m_bCancel && !bRespRecieved && nTry < MAX_NETREQUEST_RETRY);
	
	return bRet;
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

void CNetRequest::cancelAll()
{
	m_bCancel = true;
	rho_net_impl_cancelAll();
}

}
}
