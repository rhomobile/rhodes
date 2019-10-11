
#import "CRhoWKURLProtocol.h"
//#import "CRhoURLProtocol.h"
#import "api_generator/iphone/CJSEntryPoint.h"

#import "net/http_parser.h"

#import "common/RhoConf.h"

#import "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoWKURLProtocol"

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







extern int rho_http_started();
extern int rho_http_get_port();
extern int rho_nodejs_get_port();
extern const char* rho_native_rhopath();

const char* rho_http_direct_request( const char* method, const char* uri, const char* query, const void* headers, const char* body, int bodylen, int* responseLength );
void rho_http_free_response( const char* data );

void* rho_http_init_headers_list();
void rho_http_add_header( void* list, const char* name, const char* value );
void rho_http_free_headers_list( void* list );



static int on_http_header(http_parser* parser, const char *at, size_t length);
static int on_http_header_value(http_parser* parser, const char *at, size_t length);
static int on_http_body(http_parser* parser, const char *at, size_t length);
static int on_http_status(http_parser* parser, const char *at, size_t length);

static int on_http_data_cb(http_parser* parser, const char *at, size_t length) { return 0; }
static int on_http_cb(http_parser* parser) { return 0; }



@interface CRhoWKURLResponse : NSHTTPURLResponse {

}

@property NSInteger statusCode;

@end


@implementation CRhoWKURLProtocol


-(const char*)selfIDstring {
    return [[NSString stringWithFormat:@"<%p>", self] UTF8String];
}

+ (const char*)requestInfo:(NSURLRequest*)req {
    return [[NSString stringWithFormat:@"<NSURLRequest:<%p>, URL:[ %@ ], Headers:[ %@ ]", req, req.URL, req.allHTTPHeaderFields] UTF8String];
}

+ (const char*)responseHttpInfo:(NSURLResponse*)res {
    if ([res isKindOfClass:[NSHTTPURLResponse class]]) {
        return [[NSString stringWithFormat:@"<NSHTTPURLResponse:<%p>, URL:[ %@ ], Headers:[ %@ ]", res, [res URL], [[(NSHTTPURLResponse*)res allHeaderFields] description]] UTF8String];
    }
    else {
        return [[NSString stringWithFormat:@"<NSHTTPURLResponse:<%p>, URL:[ %@ ], Responce isnot HTTP !", res, [res URL]] UTF8String];
    }
}


- (void)dealloc
{
  self.httpBody = nil;
  self.httpHeaderName = nil;
  self.httpHeaders = nil;

  [super dealloc];
}


- (void) freeAllResources {
    mUrlTask = nil;
    self.isStopped = false;
    self.httpBody = nil;
    self.httpHeaderName = nil;
    self.httpHeaders = nil;
    self.mRedirectRequest = nil;
}

- (void) informAboutRedirect:(NSHTTPURLResponse*)response redirectRequest:(NSURLRequest*)redirectRequest {
    if (is_net_trace()) {
      RAWTRACE2("$NetWKRequestProcess$ CRhoWKURLProtocol %s :: informAboutRedirect { %s }", [self selfIDstring], [CRhoWKURLProtocol responseHttpInfo:response]);
    }

    NSString* t01 = @"Redirection";
    NSString* t02 = @"did";
    NSString* selname = [NSString stringWithFormat:@"_%@%@%@:newRequest:", t02, @"Perform", t01];

    SEL selector = NSSelectorFromString(selname);

    NSObject* obj = (NSObject*)mUrlTask;
    [obj performSelector:selector withObject:response withObject:redirectRequest];
}

- (void) informAboutResponse:(NSURLResponse*)response data:(NSData*)data {
    if (is_net_trace()) {
            RAWTRACE2("$NetWKRequestProcess$ CRhoWKURLProtocol %s :: informAboutResponse { %s }", [self selfIDstring], [CRhoWKURLProtocol responseHttpInfo:(NSHTTPURLResponse*)response]);
    }

    if (!self.isStopped) {
        [mUrlTask didReceiveResponse:response];
    }
    if (data != nil) {
        if (!self.isStopped) {
            [mUrlTask didReceiveData:data];
        }
    }
    if (!self.isStopped) {
        [mUrlTask didFinish];
    }

    [self freeAllResources];
}

- (void) informAboutError:(NSError*)error {
    if (is_net_trace()) {
      RAWTRACE2("$NetWKRequestProcess$ CRhoWKURLProtocol %s :: informAboutERROR { %@ }", [self selfIDstring], [error localizedDescription]);
    }

    if (!self.isStopped) {
        [mUrlTask didFailWithError:error];
    }

    [self freeAllResources];
}


+ (BOOL) isLocalURL:(NSURL*)url
{
    if (is_net_trace()) {
        RAWTRACE1("$NetWKRequestProcess$ isLocalURL BEGIN : URL: [ %s ] ", [[url absoluteString] UTF8String]);
    }
    if ( [[url absoluteString] isEqualToString:@""] )
    {
      if (is_net_trace()) {
          RAWTRACE("isLocalURL END : return NO");
      }
      return NO;
    }

    const char* scheme = [[url scheme] UTF8String];
    if (scheme != 0) {
        if ((strcmp(scheme, "http") !=0 ) && (strcmp(scheme, "https") !=0 )) {
            if (is_net_trace()) {
                RAWTRACE("$NetWKRequestProcess$ isLocalURL END : return NO");
            }
            return NO;
        }
    }

    const char* host = [[url host] UTF8String];

    if ( 0 == host )
    {
      if (is_net_trace()) {
          RAWTRACE("$NetWKRequestProcess$ isLocalURL END : return YES");
      }
        return YES;
    }

    NSNumber* p = [url port];
    int port = (nil==p)?80:[[url port] intValue];

    int rhoPort = rho_http_get_port();

    // path for Rhodes JS API from WebView in case of Node.js application
    int rhoNodeJSPort = rho_nodejs_get_port();
    NSString* path = [url path];
    if (![@"/system/js_api_entrypoint" isEqualToString:path]) {
        if (![@"/system/rholib_callback" isEqualToString:path]) {
            rhoNodeJSPort= -521;
        }
    }


    BOOL ret = (
      ((port == rhoPort) || (port == rhoNodeJSPort))
      && ( (strcmp(host,"127.0.0.1")==0) || (strcmp(host,"localhost")==0)  )
    );
    if (is_net_trace()) {
        RAWTRACE1("$NetWKRequestProcess$ isLocalURL END : return [%s]", [[[NSNumber numberWithBool:ret] stringValue] UTF8String]);
    }

    return ret;
}


+ (BOOL)canInitWithRequest:(NSURLRequest*)theRequest
{
    if (is_net_trace()) {
        RAWTRACE1("$NetWKRequestProcess$ canInitWithRequest BEGIN: { %s }", [CRhoWKURLProtocol requestInfo:theRequest]);
    }

    NSURL* theUrl = [theRequest URL];
    if ([[theUrl path] isEqualToString:@"/!__rhoNativeApi"]) {

        NSString* jsonRequestTest = [theRequest valueForHTTPHeaderField:@"__rhoNativeApiCall"];
        if (jsonRequestTest != nil) {
            if (is_net_trace()) {
                RAWTRACE("$NetRequestProcess$ canInitWithRequest END: return YES by !__rhoNativeApi prefix");
            }
            return YES;
        }
    }
#if defined(RHO_NO_RUBY_API) && defined(RHO_NO_HTTP_SERVER)
    if ([theRequest.URL.scheme isEqualToString:@"file"]) {
        if (is_net_trace()) {
            RAWTRACE("$NetRequestProcess$ canInitWithRequest END: return YES by file scheme when no Ruby API server");
        }
        return YES;
    }
#endif

    bool canHandle = true;
    if (rho_conf_is_property_exists("ios_direct_local_requests")!=0) {
        canHandle = rho_conf_getBool("ios_direct_local_requests")!=0;
    }

    if ( canHandle ) {
        if (is_net_trace()) {
            RAWTRACE("$NetWKRequestProcess$ canInitWithRequest: ios_direct_local_requests = true !");
        }
        if ([CRhoWKURLProtocol isLocalURL:theUrl]) {
            if (is_net_trace()) {
                RAWTRACE("$NetWKRequestProcess$ canInitWithRequest END: return YES URL is local !");
            }
            return YES;
        }
    }

    if (is_net_trace()) {
        RAWTRACE("$NetWKRequestProcess$ canInitWithRequest END: return NO");
    }
    return NO;
}

+ (NSURLRequest*)canonicalRequestForRequest:(NSURLRequest*)request
{
    return request;
}

- (void)startLoadingInThread
{
    if (is_net_trace()) {
        RAWTRACE2("$NetWKRequestProcess$ CRhoWKURLProtocol %s :: startLoadingInThread BEGIN: { %s }", [self selfIDstring], [CRhoWKURLProtocol requestInfo:[self request]]);
    }
    NSURL* theUrl = [[self request] URL];

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

            CRhoWKURLResponse* response =
            [[CRhoWKURLResponse alloc] initWithURL:[[self request] URL]
                                        MIMEType:@"text/plain"
                           expectedContentLength:[data length]
                                textEncodingName:@"UTF-8"];
            response.statusCode = 200;

            [self informAboutResponse:response data:data];

            return;
        }
    }
#endif

  if ( [CRhoWKURLProtocol isLocalURL:theUrl] )
  {
    if (is_net_trace()) {
        RAWTRACE1("$NetWKRequestProcess$ CRhoWKURLProtocol %s :: URL is local !", [self selfIDstring]);
    }
    NSURL* url = theUrl;
    CRhoWKURLResponse* resp = nil;

    resp = [self makeDirectHttpRequest:url];

    if ( resp != nil )
    {
      if (is_net_trace()) {
          RAWTRACE1("$NetWKRequestProcess$ CRhoWKURLProtocol %s :: startLoading has Responce from local server", [self selfIDstring]);
      }
      if ( ((self.httpStatusCode==301)||(self.httpStatusCode==302)) && ( [self.httpHeaders objectForKey:@"location"] != nil ) ) {

        if (is_net_trace()) {
            RAWTRACE1("$NetWKRequestProcess$ CRhoWKURLProtocol %s :: startLoading we have REDIRECT from local server !", [self selfIDstring]);
        }
        NSString* loc = [self.httpHeaders objectForKey:@"location"];

        NSString* escaped = [loc stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];

        url = [NSURL URLWithString:escaped];

        if ( url != nil )
        {
            //our URL;
            if ( ([url host] == nil) && ([url port] == nil ) && ( [url scheme]==nil) && ( [url path] != nil ) )
            {
                NSMutableString* s = nil;//[NSMutableString stringWithFormat:@"https://127.0.0.1:%d%@",rho_http_get_port(),[url path]];

                bool force_https = false;
                if (rho_conf_is_property_exists("ios_https_local_server")!=0) {
                    force_https = rho_conf_getBool("ios_https_local_server")!=0;
                }

                NSString* spath = [url path];
                spath = [spath stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];


                if (force_https) {
                    s = [NSMutableString stringWithFormat:@"%@://127.0.0.1:%d%@",@"https",rho_http_get_port(),spath];
                }
                else {
                    s = [NSMutableString stringWithFormat:@"%@://127.0.0.1:%d%@",@"http", rho_http_get_port(),spath];
                }


                NSString* squery = [url query];
                if ( squery != nil )
                {
                    // decode query back to original state
                    squery = [squery stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
                    [s appendFormat:@"?%@", squery];
                }

                if ( [url fragment] != nil )
                {
                    [s appendFormat:@"#%@",[url fragment]];
                }

                url = [NSURL URLWithString:s];
            }


            NSMutableURLRequest* redirReq = [NSMutableURLRequest requestWithURL:url];

            // clone headers from original request to redirect request
            NSDictionary* headers = [[self request] allHTTPHeaderFields];
            for (NSString* key in headers) {
              NSString* value = [headers objectForKey:key];
              [redirReq setValue:value forHTTPHeaderField:key];
            }

            if (is_net_trace()) {
              RAWTRACE2("$NetWKRequestProcess$ CRhoWKURLProtocol %s :: startLoading wasRedirectedToRequest with Request { %s }", [self selfIDstring], [CRhoWKURLProtocol requestInfo:redirReq]);
            }
            if (!self.isStopped) {
                CRhoWKURLResponse* redir_resp = nil;
                redir_resp = [self makeDirectHttpRequest:url];
                NSHTTPURLResponse* respo = [[NSHTTPURLResponse alloc] initWithURL:theUrl statusCode:self.httpStatusCode HTTPVersion:@"HTTP/1.1" headerFields:@{@"Location":[[redirReq URL] absoluteString]}];

                [self informAboutRedirect:resp redirectRequest:redirReq];

                self.mRedirectRequest = [redirReq retain];
                [self startLoading];
            }
            if (is_net_trace()) {
              RAWTRACE1("$NetWKRequestProcess$ CRhoWKURLProtocol %s :: startLoading END", [self selfIDstring]);
          }
          return;
        }
        else {
            RAWLOG_ERROR2("$NetWKRequestProcess$ CRhoWKURLProtocol %s :: startLoading Redirect response has no URL ! Initial request: { %s }", [self selfIDstring], [CRhoWKURLProtocol requestInfo:[self request]]);
        }
      }
      else
      {
        if (is_net_trace()) {
            RAWTRACE1("$NetWKRequestProcess$ CRhoWKURLProtocol %s :: startLoading just request - no redirection", [self selfIDstring]);
        }
          [self informAboutResponse:resp data:self.httpBody];

        if (is_net_trace()) {
            RAWTRACE1("$NetWKRequestProcess$ CRhoWKURLProtocol %s :: startLoading END", [self selfIDstring]);
        }
        return;
      }
    }
    else {
        RAWLOG_ERROR2("$NetWKRequestProcess$ CRhoWKURLProtocol %s :: startLoading has NO Responce from local server !!! Initial request: { %s }", [self selfIDstring], [CRhoWKURLProtocol requestInfo:[self request]]);
    }
  }

    NSString* body = @"error";
    [self sendResponseWithResponseCode:401 data:[body dataUsingEncoding:NSUTF8StringEncoding]];
    if (is_net_trace()) {
        RAWTRACE1("$NetWKRequestProcess$ CRhoWKURLProtocol %s :: startLoadingInThread END", [self selfIDstring]);
    }
}

- (void)startLoading
{
    if (is_net_trace()) {
        RAWTRACE1("$NetWKRequestProcess$ CRhoWKURLProtocol %s :: startLoading()", [self selfIDstring]);
    }

    NSURL* theUrl = [[self request] URL];

    if (is_net_trace()) {
        if (theUrl != nil) {
            RAWTRACE1("$NetWKRequestProcess$ CRhoWKURLProtocol :: startLoading() URL = %s", [[theUrl absoluteString] UTF8String]);
        } else {
            RAWTRACE("$NetWKRequestProcess$ CRhoWKURLProtocol :: startLoading() URL = NIL !!!");
        }
    }

    if (![CRhoWKURLProtocol canInitWithRequest:[self request]]) {
        // extern URL !
        if (is_net_trace()) {
            RAWTRACE2("$NetWKRequestProcess$ CRhoWKURLProtocol %s :: startLoading EXTERNAL REQUEST: { %s }", [self selfIDstring], [CRhoWKURLProtocol requestInfo:[self request]]);
        }

        NSData* __block _data = nil;
        NSURLResponse* __block _response = nil;
        NSError* __block _error = nil;

        dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);
        [[[NSURLSession sharedSession] dataTaskWithRequest:[self request] completionHandler:^(NSData * data, NSURLResponse * response, NSError * error) {
            _data = [data retain];
            _response = [response retain];
            _error = [error retain];
            dispatch_semaphore_signal(semaphore);
            if (is_net_trace()) {
                RAWTRACE2("$NetWKRequestProcess$ CRhoWKURLProtocol %s :: startLoading EXTERNAL REQUEST get response in thread: { %s }", [self selfIDstring], [CRhoWKURLProtocol responseHttpInfo:_response]);
            }
        }] resume];
        dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);

        // check for error
        if (_error != nil) {
            [self informAboutError:_error];
            return;
        }

        NSInteger _statusCode = 0;

        NSHTTPURLResponse *_httpResponse = nil;
        if ([_response isKindOfClass:[NSHTTPURLResponse class]]) {
           _httpResponse = (NSHTTPURLResponse*)_response;
           _statusCode = _httpResponse.statusCode;

            if (is_net_trace()) {
                RAWTRACE2("$NetWKRequestProcess$ CRhoWKURLProtocol %s :: startLoading EXTERNAL RESPONSE: { %s }", [self selfIDstring], [CRhoWKURLProtocol responseHttpInfo:_httpResponse]);
            }
        }
        // check for redirect
        if (((_statusCode == 301) || (_statusCode == 302) || (_statusCode == 303) || (_statusCode == 307) || (_statusCode == 308)) && (_httpResponse != nil) ) {

            NSDictionary* _headers = [_httpResponse allHeaderFields];

            NSString* loc = [_headers objectForKey:@"location"];
            if (loc == nil) {
                loc = [_headers objectForKey:@"Location"];
            }
            if (loc != nil) {
                NSString* escaped = [loc stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
                NSURL* _url = [NSURL URLWithString:escaped];

                NSMutableURLRequest* _redirReq = [NSMutableURLRequest requestWithURL:_url];

                // clone headers from original request to redirect request
                NSDictionary* _headers = [[self request] allHTTPHeaderFields];
                for (NSString* key in _headers) {
                  NSString* value = [_headers objectForKey:key];
                  [_redirReq setValue:value forHTTPHeaderField:key];
                }

                [self informAboutRedirect:_httpResponse redirectRequest:_redirReq];

                self.mRedirectRequest = [_redirReq retain];
                [self startLoading];

                return;
            }
        }

        // other answer
        [self informAboutResponse:_response data:_data];

        [_data release];
        [_response release];
        [_error release];

        return;
    }


    if ([[theUrl path] isEqualToString:@"/!__rhoNativeApi"]) {
        if (is_net_trace()) {
            RAWTRACE1("$NetWKRequestProcess$ CRhoWKURLProtocol %s :: startLoading URL has !__rhoNativeApi", [self selfIDstring]);
        }
        NSString* jsonRequestTest = [[self request] valueForHTTPHeaderField:@"__rhoNativeApiCall"];
        if (jsonRequestTest != nil) {
            NSString* responseStr = [CJSEntryPoint js_entry_point:jsonRequestTest];
            if (responseStr != nil) {
                [self sendResponseWithResponseCode:200 data:[responseStr dataUsingEncoding:NSUTF8StringEncoding]];
                if (is_net_trace()) {
                    RAWTRACE1("$NetWKRequestProcess$ CRhoWKURLProtocol %s :: startLoading END", [self selfIDstring]);
                }
                return;
            }
        }

    }

    //dispatch_async( dispatch_get_global_queue( DISPATCH_QUEUE_PRIORITY_DEFAULT, 0 ), ^{
        [self startLoadingInThread];
    //});
 }


- (CRhoWKURLResponse*) makeDirectHttpRequest:(NSURL*)theUrl
{

    if (is_net_trace()) {
        RAWTRACE2("$NetЦЛRequestProcess$ CRhoWKURLProtocol %s :: makeDirectHttpRequest : URL [ %s ]", [self selfIDstring], [[theUrl absoluteString] UTF8String]);
    }


  const char* uri = [[theUrl path] UTF8String];
  const char* method = [[[self request] HTTPMethod] UTF8String];
  const char* body = [[[self request] HTTPBody] bytes];
  int bodylen = (int)[[self request] HTTPBody].length;
  const char* query = [[theUrl query] UTF8String];

  NSDictionary* headers = [[self request] allHTTPHeaderFields];

  void* cHeaders = rho_http_init_headers_list();

  for (NSString* key in headers) {
    NSString* value = [headers objectForKey:key];

    rho_http_add_header(cHeaders, [key UTF8String], [value UTF8String]);

  }

  int len = 0;

  const char* response = rho_http_direct_request(method, uri, query, cHeaders, body, bodylen, &len);

  rho_http_free_headers_list(cHeaders);

  CRhoWKURLResponse* resp = nil;

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
    [[CRhoWKURLResponse alloc] initWithURL:
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
    mUrlTask = nil;
    self.isStopped = YES;
    if (is_net_trace()) {
        RAWTRACE1("$NetWKRequestProcess$ CRhoWKURLProtocol %s :: stopLoading()", [self selfIDstring]);
    }

}

- (NSURLRequest*) request {
    if (self.mRedirectRequest !=nil) {
        return self.mRedirectRequest;
    }
    return [mUrlTask request];
}


- (void)webView:(WKWebView *)webView startURLSchemeTask:(id <WKURLSchemeTask>)urlSchemeTask {
    mUrlTask = urlSchemeTask;
    self.isStopped = NO;
    self.mRedirectRequest = nil;
    [self startLoading];
}

- (void)webView:(WKWebView *)webView stopURLSchemeTask:(id <WKURLSchemeTask>)urlSchemeTask {
    [self stopLoading];
}


+ (BOOL)requestIsCacheEquivalent:(NSURLRequest*)requestA toRequest:(NSURLRequest*)requestB
{
    return NO;
}

- (void)sendResponseWithResponseCode:(NSInteger)statusCode data:(NSData*)data
{
    if (is_net_trace()) {
        RAWTRACE2("$NetWKRequestProcess$ CRhoWKURLProtocol %s :: sendResponseWithResponseCode : code: [ %d ] ", [self selfIDstring], (int)statusCode);
    }

    if (self.isStopped) {
        return;
    }
    CRhoWKURLResponse* response =
    [[CRhoWKURLResponse alloc] initWithURL:[[self request] URL]
                                   MIMEType:@"text/plain"
                      expectedContentLength:[data length]
                           textEncodingName:@"UTF-8"];
    response.statusCode = statusCode;

    [self informAboutResponse:response data:data];
    [response release];


}


@synthesize httpStatusCode;
@synthesize httpHeaderName;
@synthesize httpHeaders;
@synthesize httpBody;
@synthesize isStopped;
@synthesize mRedirectRequest;


@end

@implementation CRhoWKURLResponse
@synthesize statusCode;

- (NSDictionary*)allHeaderFields
{
    return nil;
}

@end


int on_http_header(http_parser* parser, const char *at, size_t length)
{
  CRhoWKURLProtocol* urlProtocol = (CRhoWKURLProtocol*)parser->data;
  NSData* data = [NSData dataWithBytes:at length:length];
  urlProtocol.httpHeaderName = [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] lowercaseString];

  return 0;
}

int on_http_header_value(http_parser* parser, const char *at, size_t length)
{
  CRhoWKURLProtocol* urlProtocol = (CRhoWKURLProtocol*)parser->data;
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
  CRhoWKURLProtocol* urlProtocol = (CRhoWKURLProtocol*)parser->data;
  urlProtocol.httpBody = [NSData dataWithBytes:at length:length];

  return 0;
}

int on_http_status(http_parser* parser, const char *at, size_t length)
{
  CRhoWKURLProtocol* urlProtocol = (CRhoWKURLProtocol*)parser->data;
  NSData* data = [NSData dataWithBytes:at length:length];
  NSString* str = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
  urlProtocol.httpStatusCode = [str integerValue];

  return 0;
}
