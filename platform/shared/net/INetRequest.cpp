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
#include "common/RhodesApp.h"
#include "common/RhoFilePath.h"
#include "net/URI.h"

#include "common/RhoConf.h"
#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "NetRequestTimer"


static bool is_net_trace() {
    static int res = -1;
    if (res == -1) {
        if (rho_conf_getBool("net_trace") ) {
            res = 1;
        }
        else {
            res = 0;
        }
    }
    return res == 1;
}



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
    if (is_net_trace()) {
        RAWTRACE("$NetRequestProcess$ PRE SYNCHRONIZED CNetRequestHolder::cancel() TIMER");
    }
    synchronized(m_mxReq)
    {
        m_bCancel = true;
        if ( m_pReq != 0 )
            m_pReq->cancel();
        if (is_net_trace()) {
            RAWTRACE("$NetRequestProcess$ POST SYNCHRONIZED CNetRequestHolder::cancel() TIMER");
        }
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

void CNetRequestHolder::setAuthMethod( AuthMethod method )
{
  if ( m_pReq ) {
    m_pReq->setAuthMethod(method);
  }
}

void CNetRequestHolder::setAuthUser( const String& user )
{
  if ( m_pReq ) {
    m_pReq->setAuthUser(user);
  }
}

void CNetRequestHolder::setAuthPassword( const String& password )
{
    if ( m_pReq ) {
    m_pReq->setAuthPassword(password);
  }
}


/*static*/ String CNetRequestHolder::resolveUrl(const String& strUrl)
{
    return RHODESAPPBASE().canonicalizeRhoUrl(strUrl);
}

bool CNetRequestHolder::isInsideRequest() const
{
	return (m_pReq != 0);
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

rho::net::CNetResponseWrapper CNetRequestWrapper::pullData(const String& strUrl, IRhoSession* oSession )
{
    return doRequest("GET",strUrl,String(),oSession,NULL);
}

rho::net::CNetResponseWrapper CNetRequestWrapper::pushData(const String& strUrl, const String& strBody, IRhoSession* oSession)
{
    return doRequest("POST",strUrl,strBody,oSession,NULL);
}

rho::net::CNetResponseWrapper CNetRequestWrapper::pullCookies(const String& strUrl, const String& strBody, IRhoSession* oSession)
{
    rho::net::CNetResponseWrapper oResp = doRequest("POST", strUrl, strBody, oSession, NULL );
    if ( oResp.getRespCode() == 200 )
        oResp.setCharData(oResp.getCookies().c_str());
		
    return oResp;
}

rho::net::CNetResponseWrapper CNetRequestWrapper::doRequest( const char* method, const String& strUrl, const String& strBody, IRhoSession* oSession, Hashtable<String,String>* pHeaders )
{
    return m_pReqImpl->doRequest(method, strUrl, strBody, oSession, pHeaders );
}

rho::net::CNetResponseWrapper CNetRequestWrapper::pushMultipartData(const String& strUrl, VectorPtr<CMultipartItem*>& arItems, IRhoSession* oSession, Hashtable<String,String>* pHeaders)
{
    return m_pReqImpl->pushMultipartData(strUrl, arItems, oSession, pHeaders);
}

rho::net::CNetResponseWrapper CNetRequestWrapper::pushMultipartData(const String& strUrl, CMultipartItem& oItem, IRhoSession* oSession, Hashtable<String,String>* pHeaders)
{
    VectorPtr<CMultipartItem*> arItems;
    arItems.addElement(&oItem);

    rho::net::CNetResponseWrapper oResp = pushMultipartData(strUrl, arItems, oSession, pHeaders);

    arItems[0] = 0; //do not delete item
    return oResp;
}

rho::net::CNetResponseWrapper CNetRequestWrapper::pullFile(const String& strUrl, const String& strFilePath, IRhoSession* oSession, Hashtable<String,String>* pHeaders,bool overwriteFile,bool createFolders, bool* pFileExistsFlag )
{
    if (!overwriteFile && common::CRhoFile::isFileExist(strFilePath.c_str())) {
        LOGC(WARNING,"Net") + "pullFile: " + strFilePath + " already exists, won't download since overwrite flag is not set";
        
        if ( pFileExistsFlag != 0 ) {
            *pFileExistsFlag = true;
        }
        
        return m_pReqImpl->createEmptyNetResponse();
    }
    
    if ( pFileExistsFlag != 0 ) {
        *pFileExistsFlag = false;
    }
    
    if ( createFolders ) {
        String targetDir = common::CFilePath(strFilePath).getFolderName();
        
        if ( !common::CRhoFile::isDirectory(targetDir.c_str()) ) {
            common::CRhoFile::recursiveCreateDir(targetDir.c_str(), "");
        }
    }
    
    String tmpfilename = strFilePath + ".rhodownload";
    String modfilename = strFilePath + ".modtime";
    
    common::CRhoFile tmpFile;
    common::CRhoFile::EOpenModes openMode = common::CRhoFile::OpenForWrite;
    
    //don't request headers of will overwrite anyway
    if (!overwriteFile) {
        Hashtable<String, String> h;
        ::getNetRequest().doRequest("HEAD", strUrl, "", oSession, &h );
    
        if ( h.containsKey("last-modified") )
        {
            if ( common::CRhoFile::isFileExist(modfilename.c_str()) ) {
                String modDate;
                common::CRhoFile fModDate;
                if ( fModDate.open(modfilename.c_str(), common::CRhoFile::OpenReadOnly)) {
                    fModDate.readString(modDate);
                    if (modDate == h.get("last-modified")) {
                        openMode = common::CRhoFile::OpenForAppend;
                    }
                }
            } else {
                common::CRhoFile fModDate;
                if ( fModDate.open(modfilename.c_str(), common::CRhoFile::OpenForWrite) ) {
                    const String& modDate = h.get("last-modified");
                    fModDate.write((void*)modDate.c_str(), modDate.length());
                }
            }
        }
    }
    
    if ( !tmpFile.open(tmpfilename.c_str(),openMode) )
    {
        LOGC(ERROR, "Net") + "pullFile: cannot create file :" + tmpfilename;
        return m_pReqImpl->createEmptyNetResponse();
    }
    
    INetResponse* pResp = m_pReqImpl->pullFile( strUrl, tmpFile, oSession, pHeaders );
    
    tmpFile.close();
    
    if ( (pResp->getRespCode() == 200) || (pResp->getRespCode() == 206) ) 
    {
        common::CRhoFile::deleteFile(strFilePath.c_str());

        if ( common::CRhoFile::renameFile( tmpfilename.c_str(), strFilePath.c_str() ) != 0 ) 
        {
            LOGC(ERROR, "Net") + "pullFile: cannot rename file :" + tmpfilename + " to " + strFilePath;
            return m_pReqImpl->createEmptyNetResponse();
        }
        
        common::CRhoFile::deleteFile(modfilename.c_str());
    }
    
    return pResp;
}

String CNetRequestWrapper::resolveUrl(const String& strUrl)
{
    return RHODESAPPBASE().canonicalizeRhoUrl(strUrl);
}

void CNetRequestWrapper::cancel()
{
  if ( m_pHolder != 0 )
  {
    m_pHolder->cancel();
  }
}

void CNetRequestWrapper::setAuthMethod( AuthMethod method )
{
  m_pReqImpl->setAuthMethod(method);
}

void CNetRequestWrapper::setAuthUser( const String& user )
{
  m_pReqImpl->setAuthUser(user);
}

void CNetRequestWrapper::setAuthPassword( const String& password )
{
  m_pReqImpl->setAuthPassword(password);
}


void CNetRequestWrapper::setCallback(rho::net::INetRequestCallback* callback )
{
  m_pReqImpl->setCallback(callback);
}


CAsyncNetRequest::~CAsyncNetRequest()
{
  RHODESAPP().getTimer().stopNativeTimer(this);
}

void CAsyncNetRequest::run(common::CRhoThread &)
{
  CNetRequestWrapper net = ::getNetRequest(&m_request);
  net.setCallback(this);
  net.doRequest(m_method.c_str(),m_url,m_body,m_pSession,&m_headers);
}

void CAsyncNetRequest::cancel()
{
  m_request.cancel();
}

void CAsyncNetRequest::requestCancel()
{
    RHODESAPP().getTimer().addNativeTimer(0,this);
}

bool CAsyncNetRequest::onTimer()
{
  cancel();
  return true;
}


  void CAsyncNetRequest::didReceiveResponse(NetResponse& resp, const Hashtable<String,String>* headers)
  {
    synchronized(m_mxCallbackAccess)
    {
      if ( m_pCallback != 0 )
      {
        m_pCallback->didReceiveResponse(resp,headers);
      }
    }
  }

  void CAsyncNetRequest::didReceiveData(const char* ptr, int len) 
  {
    synchronized(m_mxCallbackAccess)
    {
      if ( m_pCallback != 0 )
      {
        m_pCallback->didReceiveData(ptr,len);
      }
    }
  }

  void CAsyncNetRequest::didFinishLoading()
  {
    synchronized(m_mxCallbackAccess)
    {
      if ( m_pCallback != 0 )
      {
        m_pCallback->didFinishLoading();
      }
    }
  }

  void CAsyncNetRequest::didFail(NetResponse& resp)
  {
    synchronized(m_mxCallbackAccess)
    {
      if ( m_pCallback != 0 )
      {
        m_pCallback->didFail(resp);
      }
    }
  }




}
}

rho::net::CNetRequestWrapper getNetRequest(rho::net::CNetRequestHolder* pHolder/* = 0*/)
{
    return rho::net::CNetRequestWrapper( rho_get_RhoClassFactory()->createNetRequestImpl(), pHolder );
}
