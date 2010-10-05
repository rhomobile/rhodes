#include "stdafx.h"

#include "NetRequest.h"
#include "common/AutoPointer.h"
#include "common/RhoFile.h"
#include "common/RhoFilePath.h"
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
	INetResponse* pResp = doRequest("POST", strUrl, strBody, oSession, null );
	if ( pResp->isOK() )
        ((CNetResponseImpl*)pResp)->getRawData() = pResp->getCookies();
		
	return pResp;
}

INetResponse* CNetRequest::doRequest( const char* method, const String& strUrl, const String& strBody, IRhoSession* oSession, Hashtable<String,String>* pHeaders )
{
    m_bCancel = false;
    CNetRequestImpl oImpl(this, method,strUrl,oSession,pHeaders, m_sslVerifyPeer);
    return oImpl.sendString(strBody);
}


String CNetRequest::resolveUrl(const String& strUrl)
{
    return RHODESAPPBASE().canonicalizeRhoUrl(strUrl);
}

void CNetRequest::cancel()
{
    m_bCancel = true;
    if ( m_pCurNetRequestImpl != null )
        m_pCurNetRequestImpl->cancel();
}

INetResponse* CNetRequest::pushMultipartData(const String& strUrl, VectorPtr<CMultipartItem*>& arItems, IRhoSession* oSession, Hashtable<String,String>* pHeaders)
{
    m_bCancel = false;
    CNetRequestImpl oImpl(this, "POST",strUrl,oSession,pHeaders,m_sslVerifyPeer);
    CNetResponseImpl* pResp = oImpl.sendMultipartData(arItems);
    return pResp;
}

INetResponse* CNetRequest::pushMultipartData(const String& strUrl, CMultipartItem& oItem, IRhoSession* oSession, Hashtable<String,String>* pHeaders)
{
    m_bCancel = false;
    VectorPtr<CMultipartItem*> arItems;
    arItems.addElement(&oItem);

    INetResponse* pResp = pushMultipartData(strUrl, arItems, oSession, pHeaders);

    arItems[0] = 0; //do not delete item
    return pResp;
}

INetResponse* CNetRequest::pullFile(const String& strUrl, const String& strFilePath, IRhoSession* oSession, Hashtable<String,String>* pHeaders)
{
    m_bCancel = false;
    common::CRhoFile oFile;
    if ( !oFile.open(strFilePath.c_str(),common::CRhoFile::OpenForAppend) ) 
    {
        LOG(ERROR) + "pullFile: cannot create file :" + strFilePath;
        return new CNetResponseImpl();
    }

    CNetRequestImpl oImpl(this, "GET",strUrl,oSession,pHeaders,m_sslVerifyPeer);
    CNetResponseImpl* pResp = oImpl.downloadFile(oFile);
    return pResp;
}

}
}
