
#import <Foundation/Foundation.h>

@interface CRhoURLResponse : NSHTTPURLResponse {
    
}

@property NSInteger statusCode;

@end


@interface CRhoURLProtocol : NSURLProtocol {}



@property (nonatomic) NSInteger httpStatusCode;
@property (retain) NSString* httpHeaderName;
@property (retain) NSMutableDictionary* httpHeaders;
@property (retain) NSData* httpBody;

@property (nonatomic) BOOL isStopped;


//NSURLProtocol specific
+ (void) initAndRegister;



//utils methods
+ (BOOL) isLocalURL:(NSURL*)url;
- (void)sendResponseWithResponseCode:(NSInteger)statusCode data:(NSData*)data;
- (const char*)selfIDstring;
+ (const char*)requestInfo:(NSURLRequest*)req;
+ (const char*)responseHttpInfo:(NSURLResponse*)res;
+ (BOOL)canInitWithRequest:(NSURLRequest*)theRequest;
+ (NSURLRequest*)canonicalRequestForRequest:(NSURLRequest*)request;
- (CRhoURLResponse*) makeDirectHttpRequest:(NSURL*)theUrl;
- (void) processCommonAPIrequest:(NSURL*)url;
- (void)startLoadingInThread;

// should be override !
- (void) freeAllResources;
- (void) informAboutRedirect:(NSHTTPURLResponse*)response redirectRequest:(NSURLRequest*)redirectRequest;
- (void) informAboutResponse:(NSURLResponse*)response data:(NSData*)data;
- (void) informAboutError:(NSError*)error;


@end
