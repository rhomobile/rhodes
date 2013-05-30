
#import "CRhoURLProtocol.h"
#import "api_generator/iphone/CJSEntryPoint.h"


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

@end

@implementation CRhoURLResponse
@synthesize statusCode;

- (NSDictionary*)allHeaderFields
{
    return nil;
}

@end
