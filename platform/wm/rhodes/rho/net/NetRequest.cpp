#include "stdafx.h"

#include "NetRequest.h"
#include "common/AutoPointer.h"
#include "common/RhoFile.h"
#include "NetRequestImpl.h"
#include "common/RhodesApp.h"

namespace rho {
namespace net {
IMPLEMENT_LOGCLASS(CNetRequest,"Net");

INetResponse* CNetRequest::pullData(const String& strUrl, IRhoSession* oSession )
{
    return doRequest("GET",strUrl,String(),oSession);
}

INetResponse* CNetRequest::pushData(const String& strUrl, const String& strBody, IRhoSession* oSession)
{
    return doRequest("POST",strUrl,strBody,oSession);
}

INetResponse* CNetRequest::pullCookies(const String& strUrl, const String& strBody, IRhoSession* oSession)
{
    INetResponse* resp = doRequest("POST",strUrl,strBody,oSession);
    if ( resp && resp->isOK() )
        ((CNetResponseImpl*)resp)->getRawData() = "exists";

    return resp;
}

//if strUrl.length() == 0 delete all cookies if possible
void CNetRequest::deleteCookie(const String& strUrl)
{
    if ( strUrl.length() > 0 )
        ::InternetSetCookieA(strUrl.c_str(), NULL, "");
}

String CNetRequest::resolveUrl(const String& strUrl)
{
    return RHODESAPP().canonicalizeRhoUrl(strUrl);
}

void CNetRequest::cancel()
{
    m_bCancel = true;
    if ( m_pCurNetRequestImpl != null )
        m_pCurNetRequestImpl->close();
}

INetResponse* CNetRequest::pushFile(const String& strUrl, const String& strFilePath, IRhoSession* oSession)
{
    common::CRhoFile oFile;
    if ( !oFile.open(strFilePath.c_str(),common::CRhoFile::OpenReadOnly) ) 
    {
        LOG(ERROR) + "pushFile: cannot find file :" + strFilePath;
        return new CNetResponseImpl();
    }

    int nTry = 0;
    m_bCancel = false;
    CNetResponseImpl* pResp = 0;
    do
    {
        if ( pResp )
            delete pResp;

        CNetRequestImpl oImpl(this, "POST",strUrl);
        pResp = oImpl.sendStream(oFile.getInputStream());
        nTry++;

    }while( !m_bCancel && !pResp->isResponseRecieved() && nTry < MAX_NETREQUEST_RETRY );

    return pResp;
}

INetResponse* CNetRequest::pullFile(const String& strUrl, const String& strFilePath, IRhoSession* oSession)
{
    common::CRhoFile oFile;
    if ( !oFile.open(strFilePath.c_str(),common::CRhoFile::OpenForWrite) ) 
    {
        LOG(ERROR) + "pullFile: cannot create file :" + strFilePath;
        return new CNetResponseImpl();
    }

    int nTry = 0;
    m_bCancel = false;
    CNetResponseImpl* pResp = 0;
    do
    {
        if ( pResp )
            delete pResp;

        CNetRequestImpl oImpl(this, "GET",strUrl);
        pResp = oImpl.downloadFile(oFile);
        nTry++;

    }while( !m_bCancel && !pResp->isResponseRecieved() && nTry < MAX_NETREQUEST_RETRY );

    return pResp;
}

INetResponse* CNetRequest::doRequest( const char* method, const String& strUrl, const String& strBody, IRhoSession* oSession )
{
    int nTry = 0;
    m_bCancel = false;
    CNetResponseImpl* pResp = 0;
    do
    {
        if ( pResp )
            delete pResp;

        CNetRequestImpl oImpl(this, method,strUrl);
        pResp = oImpl.sendString(strBody);
        nTry++;

    }while( !m_bCancel && !pResp->isResponseRecieved() && nTry < MAX_NETREQUEST_RETRY );

    return pResp;
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