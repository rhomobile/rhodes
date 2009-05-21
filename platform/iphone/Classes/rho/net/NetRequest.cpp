#include "NetRequest.h"
#include "common/AutoPointer.h"


extern "C" {
char* HTTPResolveUrl(char* url);
char* rho_net_impl_request(const char* szMethod, const char* szUrl, const char* szBody, int* pbRespRecieved );
int   rho_net_impl_requestCookies(const char* szMethod, const char* szUrl, const char* szBody, int* pbRespRecieved );
int   rho_net_impl_pushFile(const char* szUrl, const char* szFilePath, int* pbRespRecieved);
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
	char* response = 0;
	
	do{
		response = rho_net_impl_request("GET", strUrl.c_str(), "", &bRespRecieved );
		nTry++;
	}while( !bRespRecieved && nTry < MAX_NETREQUEST_RETRY);
	
	return new CNetData(response);
}
	
boolean CNetRequest::pushData(const String& strUrl, const String& strBody)
{
    int bRespRecieved = 0;
	int nTry = 0;
	char* response = 0;
	
	do{
		response = rho_net_impl_request("POST", strUrl.c_str(), strBody.c_str(), &bRespRecieved );
		nTry++;
	}while( !bRespRecieved && nTry < MAX_NETREQUEST_RETRY);
		
	boolean bRet = response!=0;
	if ( response!=0 )
		free(response);
	
	return bRet;
}

boolean CNetRequest::pushFile(const String& strUrl, const String& strFileName)
{
    int bRespRecieved = 0;
	int nTry = 0;
	boolean bRet = false;
	
	do{
		bRet = rho_net_impl_pushFile(strUrl.c_str(), strFileName.c_str(), &bRespRecieved ) != 0;
		nTry++;
	}while( !bRespRecieved && nTry < MAX_NETREQUEST_RETRY);
		
	return bRet;
}
	
boolean CNetRequest::pullCookies(const String& strUrl, const String& strBody)
{
    int bRespRecieved = 0;
	int nTry = 0;
	boolean bRet = false;
	
	do{
		bRet = rho_net_impl_requestCookies("POST", strUrl.c_str(), strBody.c_str(), &bRespRecieved ) != 0;
		nTry++;
	}while( !bRespRecieved && nTry < MAX_NETREQUEST_RETRY);
	
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
	rho_net_impl_cancelAll();
}

}
}
