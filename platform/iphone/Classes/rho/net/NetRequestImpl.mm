//
//  NetRequestImpl.cpp
//  RhoAppBaseLib
//
//  Created by Alex Epifanoff on 17.02.15.
//
//

#import "NetRequestImpl.h"

namespace rho {

namespace net {


INetResponse* NetRequestImpl::doRequest( const char* method, const String& strUrl, const String& strBody, IRhoSession* oSession, Hashtable<String,String>* pHeaders )
{
  return 0;
}

INetResponse* NetRequestImpl::pullFile(const String& strUrl, common::CRhoFile& oFile, IRhoSession* oSession, Hashtable<String,String>* pHeaders)
{
  return 0;
}

INetResponse* NetRequestImpl::pushMultipartData(const String& strUrl, VectorPtr<CMultipartItem*>& arItems, IRhoSession* oSession, Hashtable<String,String>* pHeaders)
{
  return 0;
}

void NetRequestImpl::cancel()
{
}

boolean NetRequestImpl::getSslVerifyPeer()
{
  return false;
}

void NetRequestImpl::setSslVerifyPeer(boolean mode)
{
}
  
boolean NetRequestImpl::getFollowRedirects()
{
  return false;
}

void NetRequestImpl::setFollowRedirects(boolean follow)
{
}

INetResponse* NetRequestImpl::createEmptyNetResponse()
{
  return 0;
}
    
void NetRequestImpl::setCallback(INetRequestCallback*)
{
}

}

}