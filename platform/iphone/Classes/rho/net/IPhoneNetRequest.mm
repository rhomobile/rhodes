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
#import "logging/RhoLog.h"


#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "iPhoneNetRequest"

extern "C" void rho_net_impl_network_indicator(int active);


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




@interface NetRequestDelegateContext : NSObject
{
    NSCondition* netDelegateThreadStartCond;
@public
    NSThread* netRequestDelegateThread;
}

@property (nonatomic, retain) NSThread* netRequestDelegateThread;
@property (nonatomic, retain) NSCondition* netDelegateThreadStartCond;

+ (id) sharedInstance;

@end

@implementation NetRequestDelegateContext

@synthesize netRequestDelegateThread, netDelegateThreadStartCond;

+ (instancetype)sharedInstance
{
    static NetRequestDelegateContext *sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[NetRequestDelegateContext alloc] init];
    });
    return sharedInstance;
}

- (id) init
{
  self = [super init];
  
  netDelegateThreadStartCond= [[NSCondition alloc]init];
  
  [netDelegateThreadStartCond lock];
  
  [NSThread detachNewThreadSelector:@selector(netRequestDelegateThreadProc) toTarget:self withObject:nil];
  
  [netDelegateThreadStartCond wait];
  [netDelegateThreadStartCond unlock];

  return self;
}

- (void) netRequestDelegateThreadProc
{
  netRequestDelegateThread = [NSThread currentThread];
  
  [netDelegateThreadStartCond lock];
  [netDelegateThreadStartCond signal];
  [netDelegateThreadStartCond unlock];
  
  NSRunLoop* loop = [NSRunLoop currentRunLoop];
  
  [loop addPort:[NSMachPort port] forMode:NSDefaultRunLoopMode]; // adding some input source, that is required for runLoop to runing
  
  while ([loop runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]]); // starting infinite loop which can be stopped by changing the shouldKeepRunning's value
}


@end

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
  virtual const rho::net::INetRequestImpl& getOriginalRequest() const = 0;
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

- (void)connectionDidFinishLoading:(NSURLConnection *)connection;

- (void)connection:(NSURLConnection *)connection willSendRequestForAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge;

- (nullable NSURLRequest *)connection:(NSURLConnection *)connection willSendRequest:(NSURLRequest *)request redirectResponse:(nullable NSURLResponse *)response;
- (void)connection:(NSURLConnection *)connection   didSendBodyData:(NSInteger)bytesWritten
 totalBytesWritten:(NSInteger)totalBytesWritten
totalBytesExpectedToWrite:(NSInteger)totalBytesExpectedToWrite;



//- (NSURLRequest *)connection:(NSURLConnection *)connection willSendRequest:(NSURLRequest *)request redirectResponse:(NSURLResponse *)response;

@property (retain)NSHTTPURLResponse* response;
@property (retain)NSError* error;
@property (retain)NSMutableData* data;

@end

@implementation ConnDelegate

  - (id) init:(IURLRequestDelegate*)delegate
  {
    self = [super init];
    m_pCppDelegate = delegate;
    self.data = [NSMutableData dataWithCapacity:0];
    return self;
  }

  - (void)dealloc
  {
    [response release];
    [error release];
    [data release];
    
    m_pCppDelegate = 0;
    
    [super dealloc];
  }

  - (BOOL)connectionShouldUseCredentialStorage:(NSURLConnection *)connection {
      return NO;
  }

  - (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)err
  {
      if (is_net_trace()) {
          RAWTRACE3("$NetRequestProcess$ DELEGATE IPhoneNetRequest::delegate::didFailWithError uri = %s  ERRCODE = %d,  ERRTEXT = %s", [[[[connection currentRequest] URL] absoluteString] UTF8String], (int)[err code], [err localizedDescription]);
      }
      
    self.error = err;
    m_pCppDelegate->onDone();
  }

  - (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)resp
  {
      if (is_net_trace()) {
          RAWTRACE1("$NetRequestProcess$ DELEGATE IPhoneNetRequest::delegate::didReceiveResponse uri = %s ", [[[[connection currentRequest] URL] absoluteString] UTF8String]);
      }
    self.response = (NSHTTPURLResponse*)resp;
    m_pCppDelegate->onResponse((NSHTTPURLResponse*)resp);
  }

  - (void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
  {
      if (is_net_trace()) {
          RAWTRACE1("$NetRequestProcess$ DELEGATE IPhoneNetRequest::delegate::didReceiveData uri = %s ", [[[[connection currentRequest] URL] absoluteString] UTF8String]);
      }
    if ( m_pCppDelegate->shouldSaveData() )
    {
      [self.data appendData:data];
    }
    m_pCppDelegate->onData(data);
  }

  - (void)connectionDidFinishLoading:(NSURLConnection *)connection
  {
      if (is_net_trace()) {
          RAWTRACE1("$NetRequestProcess$ DELEGATE IPhoneNetRequest::delegate::connectionDidFinishLoading uri = %s ", [[[[connection currentRequest] URL] absoluteString] UTF8String]);
      }
    m_pCppDelegate->onDone();
  }

  - (void)connection:(NSURLConnection *)connection willSendRequestForAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge
  {
      NSString* authMethod = challenge.protectionSpace.authenticationMethod;
      
      if (is_net_trace()) {
          
          
          RAWTRACE2("$NetRequestProcess$ DELEGATE IPhoneNetRequest::delegate::willSendRequestForAuthenticationChallenge uri = %s, auth method = %s", [[[[connection currentRequest] URL] absoluteString] UTF8String], [authMethod UTF8String]);
      }
      
    //will skip peer check if disabled for SSL
    if ( (!m_pCppDelegate->verifySSLPeers()) && [authMethod isEqualToString:NSURLAuthenticationMethodServerTrust] )
    {
      SecTrustRef trust = challenge.protectionSpace.serverTrust;
      NSURLCredential *cred = [NSURLCredential credentialForTrust:trust];
      [challenge.sender useCredential:cred forAuthenticationChallenge:challenge];
    }
    else if ( [authMethod isEqualToString:NSURLAuthenticationMethodHTTPDigest] )
    {
        if ( m_pCppDelegate->getOriginalRequest().getAuthMethod() != rho::net::AUTH_DIGEST )
        {
            [challenge.sender performDefaultHandlingForAuthenticationChallenge:challenge];
        }
        else
        {
            const rho::net::INetRequestImpl& originalReq = m_pCppDelegate->getOriginalRequest();
            
            if ([challenge previousFailureCount] == 0)
            {
                NSString* user = [NSString stringWithUTF8String:originalReq.getAuthUser().c_str()];
                NSString* password = [NSString stringWithUTF8String:originalReq.getAuthPassword().c_str()];
                
                [[challenge sender] useCredential:
                    [NSURLCredential
                     credentialWithUser:user
                     password:password
                     persistence:NSURLCredentialPersistenceNone]
                    forAuthenticationChallenge:challenge];
            }
            else
            {
                [[challenge sender] continueWithoutCredentialForAuthenticationChallenge:challenge];
            }
        }
    }
    else
    {
      [challenge.sender performDefaultHandlingForAuthenticationChallenge:challenge];
    }
  }

- (nullable NSURLRequest *)connection:(NSURLConnection *)connection willSendRequest:(NSURLRequest *)request redirectResponse:(nullable NSURLResponse *)response {
    if (is_net_trace()) {
        RAWTRACE1("$NetRequestProcess$ DELEGATE IPhoneNetRequest::delegate::willSendRequest uri = %s ", [[[[connection currentRequest] URL] absoluteString] UTF8String]);
    }
    return request;
}


- (void)connection:(NSURLConnection *)connection   didSendBodyData:(NSInteger)bytesWritten
 totalBytesWritten:(NSInteger)totalBytesWritten
totalBytesExpectedToWrite:(NSInteger)totalBytesExpectedToWrite {
    if (is_net_trace()) {
        RAWTRACE1("$NetRequestProcess$ DELEGATE IPhoneNetRequest::delegate::didSendBodyData uri = %s ", [[[[connection currentRequest] URL] absoluteString] UTF8String]);
    }
}



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
    String m_errorMessage;
    
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
    
    virtual const char* getCharData() const
    {
      return m_data.c_str();
    }
    
    virtual unsigned int getDataSize() const
    {
      return (unsigned int)m_data.size();
    }
    
    virtual int getRespCode() const
    {
      return m_nRespCode;
    }
    
    virtual String getCookies() const
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
      
    void setErrorMessage( const char* message )
    {
      m_errorMessage = message;
    }
      
    virtual String getErrorMessage() const
    {
        return m_errorMessage;
    }
      
    boolean isOK() const
    {
      return m_nRespCode == 200 || m_nRespCode == 206;
    }
    
    boolean isUnathorized() const
    {
      return m_nRespCode == 401;
    }
    
    boolean isSuccess() const
    {
      return m_nRespCode > 0 && m_nRespCode < 400;
    }
    
    boolean isResponseRecieved() const { return m_nRespCode!=-1;}
    
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
    CIphoneNetRequestHolder( INetRequestImpl& originalRequest ) :
     m_pReq( [NSMutableURLRequest alloc] )
    ,m_pConn( [NSURLConnection alloc] )
    ,m_pPerformCond( [NSCondition alloc] )
    ,m_pConnDelegate( [ConnDelegate alloc] )
    ,m_verifyPeer(true)
    ,m_pCallback(0)
    ,m_allowRedirects(true)
    ,m_pFile(0)
    ,m_multipartTempPath(nil)
    ,m_originalRequest(originalRequest)
  {
    [m_pPerformCond init];
    [m_pConnDelegate init:this];
    
    m_multipartBoundary = [[[NSUUID UUID] UUIDString] UTF8String];
    
    m_multipartPostfix = "\r\n";
    m_multipartPostfix += "--";
    m_multipartPostfix += m_multipartBoundary;
    m_multipartPostfix += "--\r\n";
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
      //[m_multipartTempPath release];
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

      NSString* encodedUrl = nil;

      encodedUrl = [NSString stringWithUTF8String:url.c_str()];

      // encode whole URL for NSURLComponents ( url with {id} not pass by NSURLComponents)
      encodedUrl = [encodedUrl stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLFragmentAllowedCharacterSet]];

      NSURLComponents* components = [NSURLComponents componentsWithString:encodedUrl];
      
      // decode query to initial state - all other parts still encoded
      @try {
          components.percentEncodedQuery = components.query;
      } @catch( NSException* e ) {
          RAWLOG_ERROR1( "Error decoding URL query: %s", [e.reason UTF8String] );
      }

      // make URL
      NSURL* nsurl = components.URL;
      
      [m_pReq initWithURL:nsurl
      cachePolicy:NSURLRequestUseProtocolCachePolicy
      timeoutInterval:timeout
    ];
    
    [m_pReq setHTTPMethod:(method!=0)?[NSString stringWithUTF8String:method]:@"NULL"];

    if ( body.length() > 0 )
    {
      [m_pReq setHTTPBody:[NSData dataWithBytes:(const void*)body.c_str() length:body.length()]];
    }
    
    bool hasContentType = false;
    
    if ( pHeaders != 0 )
    {
      for ( Hashtable<String,String>::iterator it = pHeaders->begin(); it != pHeaders->end(); ++it )
      {
        if (!hasContentType && strcasecmp(it->first.c_str(), "content-type") == 0)
        {
          hasContentType = true;
        }

        [m_pReq setValue:
          [NSString stringWithUTF8String: it->second.c_str() ]
          forHTTPHeaderField: [NSString stringWithUTF8String: it->first.c_str()]
        ];
      }
    }
    
    if (!hasContentType && /*strcasecmp(method, "POST") == 0 &&*/ body.length() > 0)
    {
      String ct = (pSession !=0)?pSession->getContentType():"application/x-www-form-urlencoded";
      [m_pReq setValue:[NSString stringWithUTF8String:ct.c_str()] forHTTPHeaderField:@"Content-Type"];
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
    
    //Commenting this as heroku reject request with empty value.
    //[m_pReq setValue:@"" forHTTPHeaderField:@"Expect"];
    [m_pReq setValue:@"Keep-Alive" forHTTPHeaderField:@"Connection"];
    
    return true;
  }
  
  bool init( const char* method, const String& url, VectorPtr<CMultipartItem*>& arItems, Hashtable<String,String>* pHeaders, IRhoSession* pSession )
  {
    if ( !init(method,url,"",pHeaders,pSession) )
    {
      return false;
    }

    processMultipartItems(arItems);
    
    [m_pReq setValue:[NSString stringWithFormat:@"multipart/form-data; boundary=%s",m_multipartBoundary.c_str()] forHTTPHeaderField:@"content-type"];

    
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
    
    NSDictionary *fileDictionary = nil;
    {
        NSString *fullPath = [m_multipartTempPath stringByResolvingSymlinksInPath];
        NSError *error = nil;
        fileDictionary = [[NSFileManager defaultManager] attributesOfItemAtPath:fullPath error:&error];
    }
    if (fileDictionary == nil) {
        return false;
    }
      
    unsigned long fileSize = [fileDictionary fileSize];
    
    [m_pReq setValue:[NSString stringWithFormat:@"%lu",fileSize] forHTTPHeaderField:@"content-length"];
 
    return true;
  }
  
  bool writeMultipartDataToFile(VectorPtr<CMultipartItem*>& arItems, NSString* path)
  {
    NSOutputStream* out = [NSOutputStream outputStreamToFileAtPath:path append:NO];
    
    if ( nil == out )
    {
      return false;
    }
    
    [out open];
    
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
          return false;
        }
        common::InputStream* bodyStream = oFile.getInputStream();
        
        len = oItem.m_strDataPrefix.length();
        int written = (int)[out write:(const uint8_t*)oItem.m_strDataPrefix.c_str() maxLength:len];
        if ( written != len )
        {
          [out close];
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
          return false;
        }
        
        len = oItem.m_strBody.length();
        if ( [out write:(const uint8_t*)oItem.m_strBody.c_str() maxLength:len] != len )
        {
          [out close];
          return false;
        }
      }
    }
    
    len = m_multipartPostfix.length();
    if ( [out write:(const uint8_t*)m_multipartPostfix.c_str() maxLength:len] != len )
    {
      [out close];
      return false;
    }
      
    [out close];

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
      oItem.m_strDataPrefix += "--";
      oItem.m_strDataPrefix += m_multipartBoundary;
      oItem.m_strDataPrefix += "\r\n";
      oItem.m_strDataPrefix += "Content-Disposition: form-data; name=\"";
      oItem.m_strDataPrefix += oItem.m_strName + "\"";
      if (oItem.m_strFileName.length()>0)
      {
        oItem.m_strDataPrefix += "; filename=\"" + oItem.m_strFileName + "\"";
      }
      
      oItem.m_strDataPrefix += "\r\n";
      if ( oItem.m_strContentType.length() > 0 )
      {
        oItem.m_strDataPrefix += "Content-Type: " + oItem.m_strContentType + "\r\n";
      }
      
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
      
      //if ( oItem.m_strContentType.length() > 0 )
      {
        oItem.m_strDataPrefix += "Content-Length: " + common::convertToStringA(nContentSize) + "\r\n";
      }
      
      oItem.m_strDataPrefix += "\r\n";
      
      nSize += oItem.m_strDataPrefix.length() + nContentSize;
      
    }
    
    nSize += m_multipartPostfix.length();
    
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
        RAWLOG_ERROR("Error while creating tmp file");
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
      if (is_net_trace()) {
          RAWLOG_INFO1("$NetRequestProcess$ PRE LOCK IPhoneNetRequest::perform uri = %s", [[[m_pReq URL] absoluteString] UTF8String]);
      }

      [m_pPerformCond lock];
      if (is_net_trace()) {
          RAWLOG_INFO1("$NetRequestProcess$ POST LOCK IPhoneNetRequest::perform uri = %s", [[[m_pReq URL] absoluteString] UTF8String]);
      }

    //start();
    rho_net_impl_network_indicator(1);

    [m_pConnDelegate performSelector:@selector(startAsyncRequest) onThread:[[NetRequestDelegateContext sharedInstance] netRequestDelegateThread] withObject:nil waitUntilDone:NO];

      if (is_net_trace()) {
          RAWTRACE1("$NetRequestProcess$ PRE WAIT IPhoneNetRequest::perform uri = %s", [[[m_pReq URL] absoluteString] UTF8String]);
      }
    [m_pPerformCond wait];
    [m_pPerformCond unlock];
      if (is_net_trace()) {
          RAWTRACE1("$NetRequestProcess$ POST WAIT IPhoneNetRequest::perform uri = %s", [[[m_pReq URL] absoluteString] UTF8String]);
      }
  
    NSHTTPURLResponse* resp = nil;
    NSError* err = nil;
    NSData* data = nil;
    
    resp = m_pConnDelegate.response;
    err = m_pConnDelegate.error;
    data = m_pConnDelegate.data;
    
    if ( m_multipartTempPath != nil )
    {
      [[NSFileManager defaultManager] removeItemAtPath:m_multipartTempPath error:nil];
      m_multipartTempPath = nil;
    }
    
    CIPhoneNetResponseImpl* ret = 0;
    
    if ( err != nil )
    {
      ret = new CIPhoneNetResponseImpl(-1);
      ret->setErrorMessage([[err localizedDescription] UTF8String]);
      
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
      
      NSDictionary* respHeadrs = [resp allHeaderFields];
      
      if ( pHeaders != 0 )
      {
        pHeaders->clear();
        
        for (NSString* key in respHeadrs)
        {
          NSString* val = [respHeadrs valueForKey:key];
          pHeaders->put( [key UTF8String], [val UTF8String] );
        }
      }
      
      String sCookies;
      
      NSArray* cookies = [NSHTTPCookie cookiesWithResponseHeaderFields:respHeadrs forURL:[m_pReq URL]];
      
      if ( cookies != nil )
      {
        for ( NSHTTPCookie* c in cookies )
        {
          sCookies += [[c name] UTF8String];
          sCookies += "=";
          sCookies += [[c value] UTF8String];
          sCookies += ";";
        }
        
        ret->setCookies(sCookies);
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
    rho_net_impl_network_indicator(0);
      
      if (is_net_trace()) {
          RAWTRACE1("$NetRequestProcess$ FINISH IPhoneNetRequest::perform uri = %s", [[[m_pReq URL] absoluteString] UTF8String]);
      }
      
    return ret;
    
  }
  
  virtual void start()
  {
    [m_pConn initWithRequest:m_pReq delegate:m_pConnDelegate startImmediately:NO];
//    [m_pConn setDelegateQueue:[NSOperationQueue mainQueue]];
    //[m_pConn scheduleInRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
      if (is_net_trace()) {
          RAWTRACE1("$NetRequestProcess$ PRE START IPhoneNetRequest::start uri = %s", [[[m_pReq URL] absoluteString] UTF8String]);
      }
      
    [m_pConn start];
      if (is_net_trace()) {
          RAWTRACE1("$NetRequestProcess$ POST START IPhoneNetRequest::start uri = %s", [[[m_pReq URL] absoluteString] UTF8String]);
      }
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
    
  virtual const INetRequestImpl& getOriginalRequest() const
  {
      return m_originalRequest;
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
  
    String m_multipartBoundary;
    String m_multipartPostfix;
    
    const INetRequestImpl& m_originalRequest;
};

CIPhoneNetRequest::CIPhoneNetRequest() :
   m_pHolder( new CIphoneNetRequestHolder( *this ) )
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


}

}
