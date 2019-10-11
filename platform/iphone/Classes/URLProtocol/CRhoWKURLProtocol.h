
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <WebKit/WebKit.h>
#import <WebKit/WKWebViewConfiguration.h>


@interface CRhoWKURLProtocol : NSObject<WKURLSchemeHandler> {
    
    id <WKURLSchemeTask> mUrlTask;
}


+ (void) initAndRegister;

- (void)sendResponseWithResponseCode:(NSInteger)statusCode data:(NSData*)data;

@property (nonatomic) NSInteger httpStatusCode;
@property (retain) NSString* httpHeaderName;
@property (retain) NSMutableDictionary* httpHeaders;
@property (retain) NSData* httpBody;
@property (nonatomic) BOOL isStopped;
@property (nonatomic, retain) NSMutableURLRequest* mRedirectRequest;


- (NSURLRequest*) request;
- (void)webView:(WKWebView *)webView startURLSchemeTask:(id <WKURLSchemeTask>)urlSchemeTask;
- (void)webView:(WKWebView *)webView stopURLSchemeTask:(id <WKURLSchemeTask>)urlSchemeTask;

@end
