
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

extern int rho_http_get_port();
extern int rho_nodejs_get_port();


@implementation CRhoWKURLProtocol





- (void)dealloc
{
  [super dealloc];
}


- (void) freeAllResources {
    mUrlTask = nil;
    self.mRedirectRequest = nil;
    [super freeAllResources];
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
    
    self.mRedirectRequest = [redirectRequest retain];
    [self startLoading];
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


    [self processCommonAPIrequest:theUrl];

    [self startLoadingInThread];
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

@synthesize mRedirectRequest;


@end


