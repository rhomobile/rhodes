#include "stdafx.h"

#include "NetRequest.h"
#include "common/AutoPointer.h"
#include "common/RhoFile.h"
#include "NetRequestImpl.h"

extern "C" char* HTTPResolveUrl(char* url);

namespace rho {
namespace net {
IMPLEMENT_LOGCLASS(CNetRequest,"Net");

INetData* CNetRequest::pullData(const String& strUrl )
{
    return doRequest("GET",strUrl,String());
}

boolean CNetRequest::pushData(const String& strUrl, const String& strBody)
{
    common::CAutoPtr<INetData> ptrData = doRequest("POST",strUrl,strBody);
    return ptrData->getCharData() != null;
}

boolean CNetRequest::pullCookies(const String& strUrl, const String& strBody)
{
    common::CAutoPtr<INetData> ptrData = doRequest("POST",strUrl,strBody);
    return ptrData->getCharData() != null;
}

//if strUrl.length() == 0 delete all cookies if possible
void CNetRequest::deleteCookie(const String& strUrl)
{
    if ( strUrl.length() > 0 )
        ::InternetSetCookieA(strUrl.c_str(), NULL, "");
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
    CNetRequestImpl::cancel();
}

boolean CNetRequest::pushFile(const String& strUrl, const String& strFilePath)
{
    common::CRhoFile oFile;
    if ( !oFile.open(strFilePath.c_str(),common::CRhoFile::OpenReadOnly) ) 
    {
        LOG(ERROR) + "pushFile: cannot find file :" + strFilePath;
        return false;
    }

    int nTry = 0;
    m_bCancel = false;
    CNetDataImpl* pData = 0;
    do
    {
        if ( pData )
            delete pData;

        CNetRequestImpl oImpl("POST",strUrl);
        pData = oImpl.sendStream(oFile.getInputStream());
        nTry++;

    }while( !m_bCancel && !pData->isResponseRecieved() && nTry < MAX_NETREQUEST_RETRY );

    boolean bRet = pData->getCharData() != null;
    delete pData;
    return bRet;
}

INetData* CNetRequest::doRequest( const char* method, const String& strUrl, const String& strBody )
{
    int nTry = 0;
    m_bCancel = false;
    CNetDataImpl* pData = 0;
    do
    {
        if ( pData )
            delete pData;

        CNetRequestImpl oImpl(method,strUrl);
        pData = oImpl.sendString(strBody);
        nTry++;

    }while( !m_bCancel && !pData->isResponseRecieved() && nTry < MAX_NETREQUEST_RETRY );

    return pData;
}

}
}

extern "C" {

int g_rho_net_has_network = 1;

int rho_net_has_network()
{
	return g_rho_net_has_network > 0 ? 1 : 0;
}

}