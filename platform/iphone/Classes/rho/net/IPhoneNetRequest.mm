//
//  NetRequestImpl.cpp
//  RhoAppBaseLib
//
//  Created by Alex Epifanoff on 17.02.15.
//
//

#import "IPhoneNetRequest.h"

class IURLRequestDelegate
{
public:
  virtual ~IURLRequestDelegate() {}
  
  virtual void start() = 0;
  virtual void done() = 0;
};

@interface ConnDelegate : NSObject<NSURLConnectionDataDelegate>

{
@public
  IURLRequestDelegate* m_pCppDelegate;
  
  NSHTTPURLResponse* response;
  NSError* error;
  NSData* data;
}

- (id) init:(IURLRequestDelegate*)delegate;

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error;

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response;

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data;

@property (retain)NSHTTPURLResponse* response;
@property (retain)NSError* error;
@property (retain)NSData* data;

@end

@implementation ConnDelegate

  - (id) init:(IURLRequestDelegate*)delegate
  {
    self = [super init];
    
    m_pCppDelegate = delegate;
    
    return self;
  }

  - (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)err
  {
    self.error = err;
    m_pCppDelegate->done();
  }

  - (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)resp
  {
    self.response = resp;
  }

  - (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)d
  {
    self.data = d;
  }

  - (void)connectionDidFinishLoading:(NSURLConnection *)connection
  {
    m_pCppDelegate->done();
  }

  - (void)startAsyncConnection
  {
    m_pCppDelegate->start();
  }

  @synthesize error;
  @synthesize response;
  @synthesize data;


@end

namespace rho {

namespace net {


  class CIPhoneNetResponseImpl : public INetResponse
  {
    String m_data;
    int   m_nRespCode;
    String m_cookies;
    
  public:
    CIPhoneNetResponseImpl(char const *data, size_t size, int nRespCode)
      :m_nRespCode(nRespCode)
    {
      m_data.assign(data, size);
    }
    
    virtual const char* getCharData()
    {
      return m_data.c_str();
    }
    
    virtual unsigned int getDataSize()
    {
      return (unsigned int)m_data.size();
    }
    
    virtual int getRespCode()
    {
      return m_nRespCode;
    }
    
    virtual String getCookies()
    {
      return m_cookies;
    }
    
    virtual void setCharData(const char* szData)
    {
      m_data = szData;
    }
    
    void setRespCode(int nRespCode)
    {
      m_nRespCode = nRespCode;
    }
    
    boolean isOK()
    {
      return m_nRespCode == 200 || m_nRespCode == 206;
    }
    
    boolean isUnathorized()
    {
      return m_nRespCode == 401;
    }
    
    boolean isSuccess()
    {
      return m_nRespCode > 0 && m_nRespCode < 400;
    }
    
    boolean isResponseRecieved(){ return m_nRespCode!=-1;}
    
    void setCharData(const String &data)
    {
      m_data = data;
    }
    
    void setCookies(String s)
    {
      m_cookies = s;
    }
  };
  
class CIphoneNetRequestHolder : public IURLRequestDelegate
{
public:
  CIphoneNetRequestHolder() :
     m_pReq( [NSMutableURLRequest alloc] )
    ,m_pConn( [NSURLConnection alloc] )
    ,m_pPerformCond( [NSCondition alloc] )
    ,m_pConnDelegate( [ConnDelegate alloc] )
  {
    [m_pPerformCond init];
    [m_pConnDelegate init:this];
  }
  
  virtual ~CIphoneNetRequestHolder()
  {
    [m_pReq release];
    [m_pConn release];
    [m_pPerformCond release];
    [m_pConnDelegate release];
  }
  
  void init( const char* method, const String& url, const String& body, Hashtable<String,String>* pHeaders )
  {
    [m_pReq initWithURL:
      [NSURL URLWithString:
        [NSString stringWithUTF8String:url.c_str()]
      ]
    ];
    
    if ( method != 0 )
    {
      [m_pReq setHTTPMethod:[NSString stringWithUTF8String:method]];
    }
    
    if ( body.length() > 0 )
    {
      [m_pReq setHTTPBody:[NSData dataWithBytes:(const void*)body.c_str() length:body.length()]];
    }
    
    if ( pHeaders != 0 )
    {
      for ( Hashtable<String,String>::iterator it = pHeaders->begin(); it != pHeaders->end(); ++it )
      {
        [m_pReq setValue:
          [NSString stringWithUTF8String: it->second.c_str() ]
          forHTTPHeaderField: [NSString stringWithUTF8String: it->first.c_str()]
        ];
      }
    }
  }
  
  INetResponse* perform()
  {
    [m_pPerformCond lock];

    [m_pConnDelegate performSelectorOnMainThread:@selector(startAsyncConnection) withObject:nil waitUntilDone:NO];
  
    [m_pPerformCond wait];
    [m_pPerformCond unlock];
    
    INetResponse* ret = 0;
    
    if ( m_pConnDelegate.error != nil )
    {
      ret = new CIPhoneNetResponseImpl("",0,-1);
    } else {
    
      NSData* d = m_pConnDelegate.data;

    
      ret = new CIPhoneNetResponseImpl( d==nil?"":(const char*)[d bytes], d==nil?0:[d length], (int)[m_pConnDelegate.response statusCode]  );
    
    }
    
    return ret;
    
  }
  
  virtual void start()
  {
    [m_pConn initWithRequest:m_pReq delegate:m_pConnDelegate];
  }
  
  void cancel()
  {
    [m_pPerformCond lock];
    [m_pPerformCond signal];
    [m_pPerformCond unlock];
  }
  
  virtual void done()
  {
    [m_pPerformCond lock];
    [m_pPerformCond signal];
    [m_pPerformCond unlock];
  }

private:

    NSMutableURLRequest* m_pReq;
    NSURLConnection* m_pConn;
    NSCondition* m_pPerformCond;
    ConnDelegate* m_pConnDelegate;
};



CIPhoneNetRequest::CIPhoneNetRequest() :
   m_pCallback(0), m_pHolder( new CIphoneNetRequestHolder() )
{
}

CIPhoneNetRequest::~CIPhoneNetRequest()
{
}


INetResponse* CIPhoneNetRequest::doRequest( const char* method, const String& strUrl, const String& strBody, IRhoSession* oSession, Hashtable<String,String>* pHeaders )
{

  m_pHolder->init(method, strUrl, strBody, pHeaders);
  
  return m_pHolder->perform();
}

INetResponse* CIPhoneNetRequest::pullFile(const String& strUrl, common::CRhoFile& oFile, IRhoSession* oSession, Hashtable<String,String>* pHeaders)
{
  m_pHolder->init(0, strUrl, "", pHeaders);
  
  m_pHolder->perform();


  return 0;
}

INetResponse* CIPhoneNetRequest::pushMultipartData(const String& strUrl, VectorPtr<CMultipartItem*>& arItems, IRhoSession* oSession, Hashtable<String,String>* pHeaders)
{

  m_pHolder->init(0, strUrl, "", pHeaders);

  m_pHolder->perform();

  return 0;
}

void CIPhoneNetRequest::cancel()
{
  m_pHolder->cancel();
}

boolean CIPhoneNetRequest::getSslVerifyPeer()
{
  return false;
}

void CIPhoneNetRequest::setSslVerifyPeer(boolean mode)
{
}
  
boolean CIPhoneNetRequest::getFollowRedirects()
{
  return false;
}

void CIPhoneNetRequest::setFollowRedirects(boolean follow)
{
}

INetResponse* CIPhoneNetRequest::createEmptyNetResponse()
{
  return new CIPhoneNetResponseImpl("",0,-1);
}
    
void CIPhoneNetRequest::setCallback(INetRequestCallback* cb)
{
  m_pCallback = cb;
}


}

}