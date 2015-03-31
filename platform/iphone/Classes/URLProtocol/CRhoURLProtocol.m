
#import "CRhoURLProtocol.h"
#import "api_generator/iphone/CJSEntryPoint.h"

#import "net/http_parser.h"

#import "common/RhoConf.h"

extern int rho_http_started();
extern int rho_http_get_port();

const char* rho_http_direct_request( const char* method, const char* uri, const char* query, const void* headers, const char* body, int* responseLength );
void rho_http_free_response( const char* data );

void* rho_http_init_headers_list();
void rho_http_add_header( void* list, const char* name, const char* value );
void rho_http_free_headers_list( void* list );



int on_http_header(http_parser* parser, const char *at, size_t length);
int on_http_header_value(http_parser* parser, const char *at, size_t length);
int on_http_body(http_parser* parser, const char *at, size_t length);
int on_http_status(http_parser* parser, const char *at, size_t length);


int on_http_data_cb(http_parser* parser, const char *at, size_t length) { return 0; }
int on_http_cb(http_parser* parser) { return 0; }


@interface CRhoURLResponse : NSHTTPURLResponse {
    
}

@property NSInteger statusCode;

@end


@implementation CRhoURLProtocol

- (void)dealloc
{
  [httpBody release];
  [httpHeaderName release];
  [httpHeaders release];
  
  [super dealloc];
}

+ (void)registerPGHttpURLProtocol {}

+ (void)registerURLProtocol {}

+ (void) initAndRegister {
    [NSURLProtocol registerClass:[CRhoURLProtocol class]];
}

 
+ (BOOL)canInitWithRequest:(NSURLRequest*)theRequest
{
    NSURL* theUrl = [theRequest URL];
    if ([[theUrl path] isEqualToString:@"/!__rhoNativeApi"]) {
        
        NSString* jsonRequestTest = [theRequest valueForHTTPHeaderField:@"__rhoNativeApiCall"];
        if (jsonRequestTest != nil) {
            //NSLog(@"$$$ process Request: [%@:%@]", [theUrl absoluteString], jsonRequestTest);
            return YES;
        }
    }
#if defined(RHO_NO_RUBY_API) && defined(RHO_NO_HTTP_SERVER)
    if ([theRequest.URL.scheme isEqualToString:@"file"]) {
        return YES;
    }
#endif

    bool canHandle = (rho_conf_is_property_exists("ios_direct_local_requests")!=0) && (rho_conf_getBool("ios_direct_local_requests")!=0);
  
    if ( canHandle && [CRhoURLProtocol isLocalURL:theUrl] ) {
      return YES;
    }

    return NO;
}

+ (NSURLRequest*)canonicalRequestForRequest:(NSURLRequest*)request
{
    return request;
}

- (void)startLoading
{
    NSURL* theUrl = [[self request] URL];
    
    
    if ([[theUrl path] isEqualToString:@"/!__rhoNativeApi"]) {
        
        NSString* jsonRequestTest = [[self request] valueForHTTPHeaderField:@"__rhoNativeApiCall"];
        if (jsonRequestTest != nil) {
            NSString* responseStr = [CJSEntryPoint js_entry_point:jsonRequestTest];
            if (responseStr != nil) {
                //NSLog(@"$$$ send responce for[%@:%@] = [%@]", [theUrl absoluteString], jsonRequestTest, responseStr);
                [self sendResponseWithResponseCode:200 data:[responseStr dataUsingEncoding:NSUTF8StringEncoding]];
                return;
            }
        }
        
    }
#if defined(RHO_NO_RUBY_API) && defined(RHO_NO_HTTP_SERVER)
    if ([theUrl.scheme isEqualToString:@"file"]) {
        NSString* rho_path = [NSString stringWithUTF8String:rho_native_rhopath()];
        NSString* str_url = [theUrl path];
        NSString* final_path = [rho_path stringByAppendingString:@"apps/"];
        
        if ([str_url hasPrefix:final_path]) {
            final_path = str_url;
        }
        else {
            final_path = [final_path stringByAppendingString:str_url];
        }
        
        if ([[NSFileManager defaultManager] isReadableFileAtPath:final_path]) {
        
            NSData* data = [NSData dataWithContentsOfFile:final_path];
            
            CRhoURLResponse* response =
            [[CRhoURLResponse alloc] initWithURL:[[self request] URL]
                                        MIMEType:@"text/plain"
                           expectedContentLength:[data length]
                                textEncodingName:@"UTF-8"];
            response.statusCode = 200;
            
            [[self client] URLProtocol:self didReceiveResponse:response cacheStoragePolicy:NSURLCacheStorageNotAllowed];
            if (data != nil) {
                [[self client] URLProtocol:self didLoadData:data];
            }
            [[self client] URLProtocolDidFinishLoading:self];
            
            
            return;
        }
    }
#endif

  if ( [CRhoURLProtocol isLocalURL:theUrl] )
  {
    NSURL* url = theUrl;
    CRhoURLResponse* resp = nil;
    
    resp = [self makeDirectHttpRequest:url];
    
    if ( resp != nil )
    {
      if ( ((self.httpStatusCode==301)||(self.httpStatusCode==302)) && ( [self.httpHeaders objectForKey:@"location"] != nil ) ) {
        NSString* loc = [self.httpHeaders objectForKey:@"location"];
        
        NSString* escaped = [loc stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
        
        url = [NSURL URLWithString:escaped];
        
        if ( url != nil )
        {
          //our URL;
          if ( ([url host] == nil) && ([url port] == nil ) && ( [url scheme]==nil) && ( [url path] != nil ) )
          {
            NSMutableString* s = [NSMutableString stringWithFormat:@"http://127.0.0.1:%d%@",rho_http_get_port(),[url path]];
            
            if ( [url query] != nil )
            {
              [s appendFormat:@"?%@",[url query]];
            }
            
            if ( [url fragment] != nil )
            {
              [s appendFormat:@"#%@",[url fragment]];
            }
          
            url = [NSURL URLWithString:s];
          }
        
        
          NSURLRequest* redirReq = [NSURLRequest requestWithURL:url];
          [[self client] URLProtocol:self wasRedirectedToRequest:redirReq redirectResponse:resp];
          return;
        }
      }
      else
      {
        [[self client] URLProtocol:self didReceiveResponse:resp cacheStoragePolicy:NSURLCacheStorageNotAllowed];
        
        if (self.httpBody != nil) {
          [[self client] URLProtocol:self didLoadData:self.httpBody];
        }
        
        [[self client] URLProtocolDidFinishLoading:self];
        return;
      }
    }
  }
  
    //NSLog(@"$$$ responce ERROR: [%@]", [theUrl absoluteString]);
    NSString* body = @"error";
    [self sendResponseWithResponseCode:401 data:[body dataUsingEncoding:NSUTF8StringEncoding]];
 
}

- (CRhoURLResponse*) makeDirectHttpRequest:(NSURL*)theUrl
{
  NSLog(@"Will make local request to %@", [theUrl absoluteString]);
  
  const char* uri = [[theUrl path] UTF8String];
  const char* method = [[[self request] HTTPMethod] UTF8String];
  const char* body = [[[self request] HTTPBody] bytes];
  const char* query = [[theUrl query] UTF8String];
  
  NSDictionary* headers = [[self request] allHTTPHeaderFields];
  
  void* cHeaders = rho_http_init_headers_list();
  
  for (NSString* key in headers) {
    NSString* value = [headers objectForKey:key];
    
    rho_http_add_header(cHeaders, [key UTF8String], [value UTF8String]);
    
  }
  
  int len = 0;
  
  const char* response = rho_http_direct_request(method, uri, query, cHeaders, body, &len);
  
  rho_http_free_headers_list(cHeaders);
  
  CRhoURLResponse* resp = nil;
  
  if ( response != 0 ) {
    
    self.httpStatusCode = 0;
    self.httpBody = nil;
    self.httpHeaderName = nil;
    self.httpHeaders = nil;
    
    http_parser_settings settings;
    settings.on_header_field = on_http_header;
    settings.on_header_value = on_http_header_value;
    settings.on_body = on_http_body;
    settings.on_status = on_http_status;
    
    settings.on_headers_complete = on_http_cb;
    settings.on_message_begin = on_http_cb;
    settings.on_message_complete = on_http_cb;
    settings.on_url = on_http_data_cb;
    
    http_parser *parser = malloc(sizeof(http_parser));
    parser->data = self;
    http_parser_init(parser, HTTP_RESPONSE);
    http_parser_execute(parser, &settings, response, len);
    
    NSString* strHttpVer = [NSString stringWithFormat:@"%d.%d",parser->http_major,parser->http_minor];
    
    self.httpStatusCode = parser->status_code;
    
    free(parser);
    
    rho_http_free_response(response);
    
    resp =
    [[CRhoURLResponse alloc] initWithURL:
     [[self request] URL]
                              statusCode:self.httpStatusCode
                             HTTPVersion:strHttpVer
                            headerFields:self.httpHeaders
     ];
    
    resp.statusCode = self.httpStatusCode;
  }
  
  return resp;
}

- (void)stopLoading
{

}

+ (BOOL)requestIsCacheEquivalent:(NSURLRequest*)requestA toRequest:(NSURLRequest*)requestB
{
    return NO;
}

- (void)sendResponseWithResponseCode:(NSInteger)statusCode data:(NSData*)data
{

    CRhoURLResponse* response =
    [[CRhoURLResponse alloc] initWithURL:[[self request] URL]
                                   MIMEType:@"text/plain"
                      expectedContentLength:[data length]
                           textEncodingName:@"UTF-8"];
    response.statusCode = statusCode;
    
    [[self client] URLProtocol:self didReceiveResponse:response cacheStoragePolicy:NSURLCacheStorageNotAllowed];
    if (data != nil) {
        [[self client] URLProtocol:self didLoadData:data];
    }
    [[self client] URLProtocolDidFinishLoading:self];
}

+ (BOOL) isLocalURL:(NSURL*)url
{
    if ( [[url absoluteString] isEqualToString:@""] )
    {
      return NO;
    }

    const char* host = [[url host] UTF8String];
  
    if ( 0 == host )
    {
      return YES;
    }
  
    const char* scheme = [[url scheme] UTF8String];
  
    NSNumber* p = [url port];
    int port = (nil==p)?80:[[url port] intValue];
  
    int rhoPort = rho_http_get_port();

    return (
      ( (0==scheme) || (strcmp(scheme, "http") ==0 ))
      && ((port == rhoPort))
      && ( (strcmp(host,"127.0.0.1")==0) || (strcmp(host,"localhost")==0)  )
    );
}


@synthesize httpStatusCode;
@synthesize httpHeaderName;
@synthesize httpHeaders;
@synthesize httpBody;


@end

@implementation CRhoURLResponse
@synthesize statusCode;

- (NSDictionary*)allHeaderFields
{
    return nil;
}

@end


int on_http_header(http_parser* parser, const char *at, size_t length)
{
  CRhoURLProtocol* urlProtocol = (CRhoURLProtocol*)parser->data;
  NSData* data = [NSData dataWithBytes:at length:length];
  urlProtocol.httpHeaderName = [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] lowercaseString];
  
  return 0;
}

int on_http_header_value(http_parser* parser, const char *at, size_t length)
{
  CRhoURLProtocol* urlProtocol = (CRhoURLProtocol*)parser->data;
  if ( urlProtocol.httpHeaders == nil ) {
    urlProtocol.httpHeaders = [[NSMutableDictionary alloc]init];
  }
  
  NSData* data = [NSData dataWithBytes:at length:length];
  NSString* val = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
  NSString* key = urlProtocol.httpHeaderName;
  
  NSMutableDictionary* headers = urlProtocol.httpHeaders;

  [headers setValue:val forKey:key];
  
  return 0;
}

int on_http_body(http_parser* parser, const char *at, size_t length)
{
  CRhoURLProtocol* urlProtocol = (CRhoURLProtocol*)parser->data;
  urlProtocol.httpBody = [NSData dataWithBytes:at length:length];
  
  return 0;
}

int on_http_status(http_parser* parser, const char *at, size_t length)
{
  CRhoURLProtocol* urlProtocol = (CRhoURLProtocol*)parser->data;
  NSData* data = [NSData dataWithBytes:at length:length];
  NSString* str = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
  urlProtocol.httpStatusCode = [str integerValue];
  
  return 0;
}
