//
//  NetRequestImpl.cpp
//  RhoAppBaseLib
//
//  Created by Alex Epifanoff on 17.02.15.
//
//

#import "IPhoneNetRequest.h"

#import "common/RhoFile.h"
#import "common/RhoFilePath.h"
#import "common/StringConverter.h"
#import "common/RhoConf.h"

class IURLRequestDelegate
{
public:
  virtual ~IURLRequestDelegate() {}
  
  virtual void onDone() = 0;
  virtual bool verifySSLPeers() = 0;
  virtual void onResponse(NSHTTPURLResponse* r) = 0;
  virtual void onData(NSData* d) = 0;
  virtual bool allowRedirects() = 0;
  virtual bool shouldSaveData() = 0;
  virtual void start() = 0;
  virtual NSURLRequest* getRequest() = 0;
};

@interface ConnDelegate : NSObject<NSURLConnectionDelegate,NSURLConnectionDataDelegate>

{
@public
  IURLRequestDelegate* m_pCppDelegate;
  
  NSHTTPURLResponse* response;
  NSError* error;
  NSMutableData* data;
}

- (id) init:(IURLRequestDelegate*)delegate;

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error;

- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response;

- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data;

//- (void)connection:(NSURLConnection *)connection willSendRequestForAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge;

//- (NSURLRequest *)connection:(NSURLConnection *)connection willSendRequest:(NSURLRequest *)request redirectResponse:(NSURLResponse *)response;

@property (retain)NSHTTPURLResponse* response;
@property (retain)NSError* error;
@property (retain)NSMutableData* data;

@end

@implementation ConnDelegate

  - (id) init:(IURLRequestDelegate*)delegate
  {
    NSLog(@">>>>>>>>> Starting net request: %@", self );
  
    self = [super init];
    m_pCppDelegate = delegate;
    self.data = [NSMutableData dataWithCapacity:0];
    return self;
  }

  - (void)dealloc
  {
    NSLog(@">>>>>>>>> Killing net request: %@", self );
  
  
    [response release];
    [error release];
    [data release];
    
    m_pCppDelegate = 0;
    
    [super dealloc];
  }

  - (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)err
  {
    NSLog(@">>>>>>>>> %@ didFailWithError: %@", self, err );
  
    self.error = err;
    m_pCppDelegate->onDone();
  }

  - (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)resp
  {
    NSLog(@">>>>>>>>> %@ didReceiveResponse: %@", self, resp );
  
    self.response = (NSHTTPURLResponse*)resp;
    m_pCppDelegate->onResponse((NSHTTPURLResponse*)resp);
  }

  - (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
  {
    NSLog(@">>>>>>>>> %@ didReceiveData: %@", self, data );
  
    if ( m_pCppDelegate->shouldSaveData() )
    {
      [self.data appendData:data];
    }
    m_pCppDelegate->onData(data);
  }

  - (void)connectionDidFinishLoading:(NSURLConnection *)connection
  {
    NSLog(@">>>>>>>>> %@ connectionDidFinishLoading: %@", self, connection );
  
    m_pCppDelegate->onDone();
  }

/*
  - (void)connection:(NSURLConnection *)connection willSendRequestForAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge
  {
  
    NSLog(@">>>>>>>>> %@ willSendRequestForAuthenticationChallenge: %@", self, challenge );
    
    if ( (!m_pCppDelegate->verifySSLPeers()) && [challenge.protectionSpace.authenticationMethod isEqualToString:NSURLAuthenticationMethodServerTrust] )
    {
      [challenge.sender cancelAuthenticationChallenge:challenge];
    }
    else
    {
      [challenge.sender performDefaultHandlingForAuthenticationChallenge:challenge];
    }
  }
*/
/*
  - (NSURLRequest *)connection:(NSURLConnection *)connection willSendRequest:(NSURLRequest *)request redirectResponse:(NSURLResponse *)response
  {
    if ( nil == response )
    {
      return request;
    }
    
    if ( m_pCppDelegate->allowRedirects() )
    {
      NSURL *newURL = [request URL];
      NSMutableURLRequest *newRequest = [m_pCppDelegate->getRequest() mutableCopy];
      [newRequest setURL: newURL];
      return newRequest;
    }
    
    m_pCppDelegate->onDone();
    return nil;
  }
*/

  -(void) startAsyncRequest
  {
    NSLog(@">>>>>>>>> willSendRequestForAuthenticationChallenge: %@", self );
  
    m_pCppDelegate->start();
  }

  @synthesize error;
  @synthesize response;
  @synthesize data;


@end


@interface MultipartBodyStream : NSInputStream
{
  rho::VectorPtr<rho::net::CMultipartItem*>* arItems;
}
-(id)init:(rho::VectorPtr<rho::net::CMultipartItem*>*) items;
@end

@implementation MultipartBodyStream
-(id)init:(rho::VectorPtr<rho::net::CMultipartItem*>*) items
{
  self = [super init];
  
  arItems = items;
  
  return self;
}
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
    
    CIPhoneNetResponseImpl(int nRespCode)
      :m_nRespCode(nRespCode)
    {
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
    ,m_verifyPeer(true)
    ,m_pCallback(0)
    ,m_allowRedirects(true)
    ,m_pFile(0)
    ,m_multipartTempPath(nil)
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
    
    if ( m_multipartTempPath != nil )
    {
      [[NSFileManager defaultManager] removeItemAtPath:m_multipartTempPath error:nil];
      [m_multipartTempPath release];
      m_multipartTempPath = nil;
    }

  }
  
  bool init( const char* method, const String& url, const String& body, Hashtable<String,String>* pHeaders, IRhoSession* pSession )
  {
    int timeout = RHOCONF().getInt("net_timeout");
    if (timeout == 0)
    {
      timeout = 30; // 30 seconds by default
    }

    [m_pReq initWithURL:
      [NSURL URLWithString:
        [NSString stringWithUTF8String:url.c_str()]
      ]
      cachePolicy:NSURLRequestUseProtocolCachePolicy
      timeoutInterval:timeout
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
    
    String session;
    if (pSession != 0)
    {
      session = pSession->getSession();
    }
    
    if (!session.empty())
    {
      [m_pReq setValue:[NSString stringWithUTF8String:session.c_str()] forHTTPHeaderField:@"Cookie"];
    }
    
    return true;
  }
  
  bool init( const char* method, const String& url, VectorPtr<CMultipartItem*>& arItems, Hashtable<String,String>* pHeaders, IRhoSession* pSession )
  {
    if ( !init(method,url,"",pHeaders,pSession) )
    {
      return false;
    }

    processMultipartItems(arItems);
    
    [m_pReq setValue:[NSString stringWithFormat:@"multipart/form-data; boundary=%s",m_sMultipartBoundary] forHTTPHeaderField:@"content-type"];

    
    m_multipartTempPath = getMultipartTempFile();
    if ( nil == m_multipartTempPath )
    {
      return false;
    }
    
    if ( !writeMultipartDataToFile(arItems ,m_multipartTempPath) )
    {
      return false;
    }
    
    [m_pReq setHTTPBodyStream:[NSInputStream inputStreamWithFileAtPath:m_multipartTempPath]];
 
    return true;
  }
  
  static bool writeMultipartDataToFile(VectorPtr<CMultipartItem*>& arItems, NSString* path)
  {
    NSOutputStream* out = [NSOutputStream outputStreamToFileAtPath:path append:NO];
    
    if ( nil == out )
    {
      return false;
    }
    
    size_t len = 0;

    //write all items
    for( int i = 0; i < (int)arItems.size(); i++ )
    {
      CMultipartItem& oItem = *arItems.elementAt(i);
      
      if ( oItem.m_strFilePath.length() > 0 )
      {
        common::CRhoFile oFile;
        if ( !oFile.open(oItem.m_strFilePath.c_str(),common::CRhoFile::OpenReadOnly) )
        {
          [out close];
          //[out release];
          return false;
        }
        common::InputStream* bodyStream = oFile.getInputStream();
        
        len = oItem.m_strDataPrefix.length();
        if ( [out write:(const uint8_t*)oItem.m_strDataPrefix.c_str() maxLength:len] != len )
        {
          [out close];
          //[out release];
          return false;
        }
        
        if ( bodyStream->available() > 0 )
        {
          Vector<char> buf;
          buf.assign(4096, 0);
          int nReaded = 0;
          
          do
          {
            nReaded = bodyStream->read(&buf[0],0,(int)buf.size());
            if ( nReaded > 0 )
            {
              if ( [out write:(const uint8_t*)&buf[0] maxLength:nReaded] != nReaded )
              {
                [out close];
                //[out release];
                return false;
              }
            }
          } while(nReaded > 0);
        }
        
      }
      else
      {
        len = oItem.m_strDataPrefix.length();
        if ( [out write:(const uint8_t*)oItem.m_strDataPrefix.c_str() maxLength:len] != len )
        {
          [out close];
          //[out release];
          return false;
        }
        
        len = oItem.m_strBody.length();
        if ( [out write:(const uint8_t*)oItem.m_strBody.c_str() maxLength:len] != len )
        {
          [out close];
          //[out release];
          return false;
        }
      }
    }
    
    len = strlen(m_sMultipartPostfix);
    if ( [out write:(const uint8_t*)m_sMultipartPostfix maxLength:len] != len )
    {
      [out close];
      //[out release];
      return false;
    }
      
    [out close];
    //[out release];

    return true;
  }

  int processMultipartItems( VectorPtr<CMultipartItem*>& arItems )
  {
    int nSize = 0;
    for( int i = 0; i < (int)arItems.size(); i++ )
    {
      CMultipartItem& oItem = *arItems.elementAt(i);
      
      if ( oItem.m_strName.length() == 0 )
      oItem.m_strName = "blob";
      
      if ( oItem.m_strFileName.length() == 0 )
      {
        if ( oItem.m_strFilePath.length() > 0 )
        {
          common::CFilePath oPath(oItem.m_strFilePath);
          oItem.m_strFileName = oPath.getBaseName();
        }
      }
      
      oItem.m_strDataPrefix = i > 0 ? "\r\n" : "";
      oItem.m_strDataPrefix += m_sMultipartBoundary;
      oItem.m_strDataPrefix += "\r\n";
      oItem.m_strDataPrefix += "Content-Disposition: form-data; name=\"";
      oItem.m_strDataPrefix += oItem.m_strName + "\"";
      if (oItem.m_strFileName.length()>0)
      oItem.m_strDataPrefix += "; filename=\"" + oItem.m_strFileName + "\"";
      oItem.m_strDataPrefix += "\r\n";
      if ( oItem.m_strContentType.length() > 0 )
      oItem.m_strDataPrefix += "Content-Type: " + oItem.m_strContentType + "\r\n";
      
      int nContentSize = 0;
      if ( oItem.m_strFilePath.length() > 0 )
      {
        common::CRhoFile oFile;
        if ( oFile.open(oItem.m_strFilePath.c_str(),common::CRhoFile::OpenReadOnly) )
        nContentSize = oFile.size();
      }
      else
      {
        nContentSize = (int)oItem.m_strBody.length();
      }
      
      if ( oItem.m_strContentType.length() > 0 )
      oItem.m_strDataPrefix += "Content-Length: " + common::convertToStringA(nContentSize) + "\r\n";
      
      oItem.m_strDataPrefix += "\r\n";
      
      nSize += oItem.m_strDataPrefix.length() + nContentSize;
      
    }
    
    nSize += strlen(m_sMultipartPostfix);
    
    return nSize;
  }

  static NSString* getMultipartTempFile()
  {
    NSString *tempFileTemplate = [NSTemporaryDirectory() stringByAppendingPathComponent:@"rho-multipart-XXXXXX.tmp"];
    
    const char *tempFileTemplateCString = [tempFileTemplate fileSystemRepresentation];

    char *tempFileNameCString = (char *)malloc(strlen(tempFileTemplateCString) + 1);
    strcpy(tempFileNameCString, tempFileTemplateCString);
    int fileDescriptor = mkstemps(tempFileNameCString, 4);

    // no need to keep it open
    close(fileDescriptor);
    
    if (fileDescriptor == -1) {
        NSLog(@"Error while creating tmp file");
        free(tempFileNameCString);
        return nil;
    }

    NSString *tempFileName = [[NSFileManager defaultManager]
        stringWithFileSystemRepresentation:tempFileNameCString
        length:strlen(tempFileNameCString)];

    free(tempFileNameCString);

    return tempFileName;
  }


  
  INetResponse* perform( Hashtable<String,String>* pHeaders, IRhoSession* pSession )
  {
  
    [m_pPerformCond lock];

    //start();
    [m_pConnDelegate performSelectorOnMainThread:@selector(startAsyncRequest) withObject:nil waitUntilDone:NO];

    [m_pPerformCond wait];
    [m_pPerformCond unlock];
  
    NSHTTPURLResponse* resp = nil;
    NSError* err = nil;
    NSData* data = nil;
    
    resp = m_pConnDelegate.response;
    err = m_pConnDelegate.error;
    data = m_pConnDelegate.data;
    
    if ( m_multipartTempPath != nil )
    {
      [[NSFileManager defaultManager] removeItemAtPath:m_multipartTempPath error:nil];
      [m_multipartTempPath release];
      m_multipartTempPath = nil;
    }
    
    INetResponse* ret = 0;
    
    if ( err != nil )
    {
      ret = new CIPhoneNetResponseImpl(-1);
      
      if ( m_pCallback != 0 )
      {
        NetResponse nrw = new CIPhoneNetResponseImpl(-1);
        m_pCallback->didFail( nrw );
      }
      
    } else {
    
      NSData* d = data;
      if ( (d!=nil) && (d.length>0) )
      {
        ret = new CIPhoneNetResponseImpl( (const char*)[d bytes], [d length], (int)[resp statusCode]  );
      } else {
        ret = new CIPhoneNetResponseImpl( (int)[resp statusCode]  );
      }
      
      if ( pHeaders != 0 )
      {
        pHeaders->clear();
        
        NSDictionary* respHeadrs = [resp allHeaderFields];
        
        for (NSString* key in respHeadrs)
        {
          NSString* val = [respHeadrs valueForKey:key];
          pHeaders->put( [key UTF8String], [val UTF8String] );
        }
      }
      
      if ( (pSession!=0) && ( [resp statusCode] == 401 ) )
      {
        pSession->logout();
      }
      
      if ( m_pCallback != 0 )
      {
        m_pCallback->didFinishLoading();
      }
    }
    
    return ret;
    
  }
  
  virtual void start()
  {
    [m_pConn initWithRequest:m_pReq delegate:m_pConnDelegate startImmediately:NO];
//    [m_pConn setDelegateQueue:[NSOperationQueue mainQueue]];
//    [m_pConn scheduleInRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
    [m_pConn start];
  }
  
  void cancel()
  {
    [m_pConn cancel];
    
    [m_pPerformCond lock];
    [m_pPerformCond signal];
    [m_pPerformCond unlock];
  }
  
  virtual void onDone()
  {
    [m_pPerformCond lock];
    [m_pPerformCond signal];
    [m_pPerformCond unlock];
  }
  
  virtual bool verifySSLPeers()
  {
    return m_verifyPeer;
  }
  
  void setVerifyPeers(bool verify)
  {
    m_verifyPeer = verify;
  }
  
  virtual void onResponse(NSHTTPURLResponse* r)
  {
    if ( m_pCallback != 0 )
    {
      NetResponse resp = new CIPhoneNetResponseImpl(0,0,(int)[r statusCode]);
      Hashtable<String, String> headers;
      
      for ( NSString* key in r.allHeaderFields )
      {
        NSString* val = [r.allHeaderFields valueForKey:key];
        headers.put( [key UTF8String], [val UTF8String] );
      }
      
      m_pCallback->didReceiveResponse(resp, &headers);
    }
  }
  
  virtual void onData(NSData* d)
  {
    if ( m_pFile != 0 )
    {
      m_pFile->write([d bytes], (int)[d length]);
      m_pFile->flush();
    }

    if ( m_pCallback != 0 )
    {
      m_pCallback->didReceiveData((const char*)[d bytes], (int)[d length]);
    }
  }

  void setCallback( INetRequestCallback* cb )
  {
    m_pCallback = cb;
  }
  
  virtual bool allowRedirects()
  {
    return m_allowRedirects;
  }
  
  void setAllowRedirects( bool allow )
  {
    m_allowRedirects = allow;
  }
  
  void setFile( common::CRhoFile* file )
  {
    m_pFile = file;
  }
  
  virtual bool shouldSaveData()
  {
    return 0==m_pFile;
  }
  
  virtual NSURLRequest* getRequest()
  {
    return m_pReq;
  }

private:

    NSMutableURLRequest* m_pReq;
    NSURLConnection* m_pConn;
    NSCondition* m_pPerformCond;
    ConnDelegate* m_pConnDelegate;
    bool m_verifyPeer;
    INetRequestCallback* m_pCallback;
    bool m_allowRedirects;
    common::CRhoFile* m_pFile;
    NSString* m_multipartTempPath;
  
    static const char* m_sMultipartBoundary;
    static const char* m_sMultipartPostfix;
};

const char* CIphoneNetRequestHolder::m_sMultipartBoundary = "----------A6174410D6AD474183FDE48F5662FCC5";
const char* CIphoneNetRequestHolder::m_sMultipartPostfix = "\r\n------------A6174410D6AD474183FDE48F5662FCC5--";


CIPhoneNetRequest::CIPhoneNetRequest() :
   m_pCallback(0), m_pHolder( new CIphoneNetRequestHolder() )
{
}

CIPhoneNetRequest::~CIPhoneNetRequest()
{
}


INetResponse* CIPhoneNetRequest::doRequest( const char* method, const String& strUrl, const String& strBody, IRhoSession* oSession, Hashtable<String,String>* pHeaders )
{
  if ( !m_pHolder->init(method, strUrl, strBody, pHeaders, oSession) )
  {
    return createEmptyNetResponse();
  }
  
  return m_pHolder->perform( pHeaders, oSession );
}

INetResponse* CIPhoneNetRequest::pullFile(const String& strUrl, common::CRhoFile& oFile, IRhoSession* oSession, Hashtable<String,String>* pHeaders)
{
  if ( !m_pHolder->init("GET", strUrl, "", pHeaders, oSession) )
  {
    return createEmptyNetResponse();
  }
  
  m_pHolder->setFile(&oFile);
  return m_pHolder->perform( pHeaders, oSession );
}

INetResponse* CIPhoneNetRequest::pushMultipartData(const String& strUrl, VectorPtr<CMultipartItem*>& arItems, IRhoSession* oSession, Hashtable<String,String>* pHeaders)
{
  return createEmptyNetResponse();

  if ( !m_pHolder->init("POST", strUrl, arItems, pHeaders, oSession) )
  {
    return createEmptyNetResponse();
  }
  
  return m_pHolder->perform( pHeaders, oSession );
}

void CIPhoneNetRequest::cancel()
{
  m_pHolder->cancel();
}

boolean CIPhoneNetRequest::getSslVerifyPeer()
{
  return m_pHolder->verifySSLPeers();
}

void CIPhoneNetRequest::setSslVerifyPeer(boolean mode)
{
  m_pHolder->setVerifyPeers(mode);
}
  
boolean CIPhoneNetRequest::getFollowRedirects()
{
  return m_pHolder->allowRedirects();
}

void CIPhoneNetRequest::setFollowRedirects(boolean follow)
{
  m_pHolder->setAllowRedirects(follow);
}

INetResponse* CIPhoneNetRequest::createEmptyNetResponse()
{
  return new CIPhoneNetResponseImpl("",0,-1);
}
    
void CIPhoneNetRequest::setCallback(INetRequestCallback* cb)
{
  m_pHolder->setCallback(cb);
}


}

}