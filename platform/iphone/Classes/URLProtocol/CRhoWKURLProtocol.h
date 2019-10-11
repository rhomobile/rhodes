
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <WebKit/WebKit.h>
#import <WebKit/WKWebViewConfiguration.h>
#import "CRhoURLProtocol.h"


@interface CRhoWKURLProtocol : CRhoURLProtocol<WKURLSchemeHandler> {
    
    id <WKURLSchemeTask> mUrlTask;
}


@property (nonatomic, retain) NSMutableURLRequest* mRedirectRequest;


- (NSURLRequest*) request;
- (void)webView:(WKWebView *)webView startURLSchemeTask:(id <WKURLSchemeTask>)urlSchemeTask;
- (void)webView:(WKWebView *)webView stopURLSchemeTask:(id <WKURLSchemeTask>)urlSchemeTask;

@end
