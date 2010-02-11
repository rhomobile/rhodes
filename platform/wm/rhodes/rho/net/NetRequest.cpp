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
    return doRequest("GET",strUrl,String(),oSession,null);
}

INetResponse* CNetRequest::pushData(const String& strUrl, const String& strBody, IRhoSession* oSession)
{
    return doRequest("POST",strUrl,strBody,oSession,null);
}

INetResponse* CNetRequest::pullCookies(const String& strUrl, const String& strBody, IRhoSession* oSession)
{
    CNetRequestImpl oImpl(this, "POST",strUrl,oSession,null);
    CNetResponseImpl* resp = oImpl.sendString(strBody);

    if ( resp && resp->isOK() )
    {
        ((CNetResponseImpl*)resp)->getRawData() = oImpl.makeRhoCookie();
        //((CNetResponseImpl*)resp)->getRawData() = "exists";
    }

    return resp;
}

String CNetRequest::resolveUrl(const String& strUrl)
{
    return RHODESAPP().canonicalizeRhoUrl(strUrl);
}

void CNetRequest::cancel()
{
    m_bCancel = true;
    if ( m_pCurNetRequestImpl != null )
        m_pCurNetRequestImpl->cancel();
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

        CNetRequestImpl oImpl(this, "POST",strUrl,oSession,null);
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

        CNetRequestImpl oImpl(this, "GET",strUrl,oSession,null);
        pResp = oImpl.downloadFile(oFile);
        nTry++;

    }while( !m_bCancel && !pResp->isResponseRecieved() && nTry < MAX_NETREQUEST_RETRY );

    return pResp;
}

INetResponse* CNetRequest::doRequest( const char* method, const String& strUrl, const String& strBody, IRhoSession* oSession, Hashtable<String,String>* pHeaders )
{
    int nTry = 0;
    m_bCancel = false;
    CNetResponseImpl* pResp = 0;
    do
    {
        if ( pResp )
            delete pResp;

        CNetRequestImpl oImpl(this, method,strUrl,oSession,pHeaders);
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