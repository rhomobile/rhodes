#include "INetRequest.h"

#include "common/RhoFile.h"
#include "common/RhodesAppBase.h"

namespace rho {
namespace net {

void CNetRequestHolder::setRequest(INetRequestImpl* pReq)
{
    synchronized(m_mxReq)
    {
        m_pReq = pReq;

        if ( pReq )
        {
            m_bCancel = false;
            m_pReq->setSslVerifyPeer(m_sslVerifyPeer);
        }
    }
}

void CNetRequestHolder::cancel()
{
    synchronized(m_mxReq)
    {
        m_bCancel = true;
        if ( m_pReq != 0 )
            m_pReq->cancel();

        m_pReq = 0;
    }
}

boolean CNetRequestHolder::isCancelled()
{ 
    synchronized(m_mxReq)
    {
        return m_bCancel;
    }
}

/*static*/ String CNetRequestHolder::resolveUrl(const String& strUrl)
{
    return RHODESAPPBASE().canonicalizeRhoUrl(strUrl);
}

CNetRequestWrapper::CNetRequestWrapper(INetRequestImpl* pImpl, CNetRequestHolder* pHolder) : m_pReqImpl(pImpl), m_pHolder(pHolder)
{
    if ( m_pHolder )
        m_pHolder->setRequest(pImpl);
}

CNetRequestWrapper::~CNetRequestWrapper()
{ 
    if ( m_pHolder )
        m_pHolder->setRequest(0); 
}

INetResponse* CNetRequestWrapper::pullData(const String& strUrl, IRhoSession* oSession )
{
    return doRequest("GET",strUrl,String(),oSession,null);
}

INetResponse* CNetRequestWrapper::pushData(const String& strUrl, const String& strBody, IRhoSession* oSession)
{
    return doRequest("POST",strUrl,strBody,oSession,null);
}

INetResponse* CNetRequestWrapper::pullCookies(const String& strUrl, const String& strBody, IRhoSession* oSession)
{
    INetResponse* pResp = doRequest("POST", strUrl, strBody, oSession, null );
    if ( pResp->getRespCode() == 200 )
        pResp->setCharData(pResp->getCookies().c_str());
		
    return pResp;
}

INetResponse* CNetRequestWrapper::doRequest( const char* method, const String& strUrl, const String& strBody, IRhoSession* oSession, Hashtable<String,String>* pHeaders )
{
    return m_pReqImpl->doRequest(method, strUrl, strBody, oSession, pHeaders );
}

INetResponse* CNetRequestWrapper::pushMultipartData(const String& strUrl, VectorPtr<CMultipartItem*>& arItems, IRhoSession* oSession, Hashtable<String,String>* pHeaders)
{
    return m_pReqImpl->pushMultipartData(strUrl, arItems, oSession, pHeaders);
}

INetResponse* CNetRequestWrapper::pushMultipartData(const String& strUrl, CMultipartItem& oItem, IRhoSession* oSession, Hashtable<String,String>* pHeaders)
{
    VectorPtr<CMultipartItem*> arItems;
    arItems.addElement(&oItem);

    INetResponse* pResp = pushMultipartData(strUrl, arItems, oSession, pHeaders);

    arItems[0] = 0; //do not delete item
    return pResp;
}

INetResponse* CNetRequestWrapper::pullFile(const String& strUrl, const String& strFilePath, IRhoSession* oSession, Hashtable<String,String>* pHeaders)
{
    common::CRhoFile oFile;
    if ( !oFile.open(strFilePath.c_str(),common::CRhoFile::OpenForAppend) ) 
    {
        LOGC(ERROR, "Net") + "pullFile: cannot create file :" + strFilePath;
        return m_pReqImpl->createEmptyNetResponse();
    }

    return m_pReqImpl->pullFile( strUrl, oFile, oSession, pHeaders );
}

String CNetRequestWrapper::resolveUrl(const String& strUrl)
{
    return RHODESAPPBASE().canonicalizeRhoUrl(strUrl);
}
}
}

rho::net::CNetRequestWrapper getNetRequest(rho::net::CNetRequestHolder* pHolder/* = 0*/)
{
    return rho::net::CNetRequestWrapper( rho_get_RhoClassFactory()->createNetRequestImpl(), pHolder );
}
