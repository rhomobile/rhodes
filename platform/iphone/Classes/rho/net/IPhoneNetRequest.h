//
//  NetRequestImpl.h
//  RhoAppBaseLib
//
//  Created by Alex Epifanoff on 17.02.15.
//
//

#pragma once

#include "net/INetRequest.h"

namespace rho {
namespace net {

class CIphoneNetRequestHolder;

class CIPhoneNetRequest : public INetRequestImpl {

public:
  CIPhoneNetRequest();
  virtual ~CIPhoneNetRequest(void);

  virtual INetResponse* doRequest( const char* method, const String& strUrl, const String& strBody, IRhoSession* oSession, Hashtable<String,String>* pHeaders );
  virtual INetResponse* pullFile(const String& strUrl, common::CRhoFile& oFile, IRhoSession* oSession, Hashtable<String,String>* pHeaders);
  virtual INetResponse* pushMultipartData(const String& strUrl, VectorPtr<CMultipartItem*>& arItems, IRhoSession* oSession, Hashtable<String,String>* pHeaders);

  virtual void cancel();

  virtual boolean getSslVerifyPeer();
  virtual void setSslVerifyPeer(boolean mode);
  
  virtual boolean getFollowRedirects();
  virtual void setFollowRedirects(boolean follow);

  virtual INetResponse* createEmptyNetResponse();
    
  virtual void setCallback(INetRequestCallback*);
  

private:
  common::CAutoPtr<CIphoneNetRequestHolder> m_pHolder;
  INetRequestCallback* m_pCallback;
};


}
}

