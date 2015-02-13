
#import "CRhoURLProtocol.h"
#import "api_generator/iphone/CJSEntryPoint.h"

extern int rho_http_started();
extern int rho_http_get_port();


@interface CRhoURLResponse : NSHTTPURLResponse {
    
}

@property (nonatomic) NSInteger statusCode;

@end


@implementation CRhoURLProtocol

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

    if ( [CRhoURLProtocol isLocalURL:theUrl] ) {
      return NO;//YES;
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
      NSLog(@"Will make local request to %@", [theUrl absoluteString]);
    }
    
    //NSLog(@"$$$ responce ERROR: [%@]", [theUrl absoluteString]);
    NSString* body = @"error";
    [self sendResponseWithResponseCode:401 data:[body dataUsingEncoding:NSUTF8StringEncoding]];
 
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
    const char* host = [[url host] UTF8String];
    int port = [[url port] intValue];

    return (
      (rho_http_started()!=0) &&
      (port == rho_http_get_port()) &&
      ( (strcmp(host,"127.0.0.1")==0) || (strcmp(host,"localhost")==0)  )
    );
}

@end

@implementation CRhoURLResponse
@synthesize statusCode;

- (NSDictionary*)allHeaderFields
{
    return nil;
}

@end
