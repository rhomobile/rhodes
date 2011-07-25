/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

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
