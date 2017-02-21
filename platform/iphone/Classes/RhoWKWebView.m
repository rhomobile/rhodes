/*------------------------------------------------------------------------
 * (The MIT License)
 *
 * Copyright (c) 2008-2016 TAU Technologies, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * http://tau-technologies.com
 *------------------------------------------------------------------------*/



#import "RhoWKWebView.h"
#import "RhoMainView.h"
#include "logging/RhoLog.h"
#include "common/RhoConf.h"


#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoWKWebView"


@implementation RhoWKWebView

@synthesize webview, delegate;

- (id)initWithFrame:(CGRect)frame {
    [self init];
    
    WKWebViewConfiguration *configuration = [[WKWebViewConfiguration alloc] init];
    if ( rho_conf_getBool("enable_media_playback_without_gesture") == 1 )
        configuration.mediaPlaybackRequiresUserAction = NO;
    
    WKWebView* w = [[WKWebView alloc] initWithFrame:frame configuration:configuration];
    //w.scalesPageToFit = YES;
    if ( !rho_conf_getBool("WebView.enableBounce") )
        [[w scrollView] setBounces:NO];
    w.userInteractionEnabled = YES;
    w.multipleTouchEnabled = YES;
    w.clipsToBounds = NO;
    //w.dataDetectorTypes = UIDataDetectorTypeNone;
    w.navigationDelegate = self;
    w.autoresizesSubviews = YES;
    //w.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    w.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    w.tag = RHO_TAG_WEBVIEW;
    //assert([w retainCount] == 1);
    self.webview = w;
    
    
    
    return self;
}

- (UIView*)view {
    return self.webview;
}

- (NSString*)currentLocation {
    return [webview.URL absoluteString];
}

- (void) setupDelegate:(id<RhoWebViewDelegate,NSObject>)delegate {
    self.delegate = delegate;
}

- (UIView*)containerView {
    return self.webview.scrollView;
}




//UIWebView

- (NSString *)stringByEvaluatingJavaScriptFromString:(NSString *)script {
        __block NSString *resultString = nil;
        __block BOOL finished = NO;
        
        [webview evaluateJavaScript:script completionHandler:^(id result, NSError *error) {
            if (error == nil) {
                if (result != nil) {
                    resultString = [NSString stringWithFormat:@"%@", result];
                }
            } else {
                RAWLOG_ERROR1("WKWebView.evaluateJavaScript error : %s", [[error localizedDescription] UTF8String]);
            }
            finished = YES;
        }];
        
        while (!finished)
        {
            [[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate distantFuture]];
        }
        
        return resultString;
}

- (void)loadRequest:(NSURLRequest *)request {
    [webview loadRequest:request];
}

- (void)loadHTMLString:(NSString *)string baseURL:(nullable NSURL *)baseURL {
    [webview loadHTMLString:string baseURL:baseURL];
}

- (void)stopLoading {
    [webview stopLoading];
}

- (void)reload {
    [webview reload];
}

- (void)goBack {
    [webview goBack];
}

- (void)goForward {
    [webview goForward];
}




// WKNavigationDelegate
//   UIWebViewDelegate -> WKNavigationDelegate migrate
//     didFailLoadWithError => didFailNavigation
//     webViewDidFinishLoad => didFinishNavigation
//     webViewDidStartLoad => didStartProvisionalNavigation
//     shouldStartLoadWithRequest => decidePolicyForNavigationAction

- (void)webView:(WKWebView *)webView decidePolicyForNavigationAction:(WKNavigationAction *)navigationAction decisionHandler:(void (^)(WKNavigationActionPolicy))decisionHandler {
    
    UIWebViewNavigationType navType = UIWebViewNavigationTypeOther;
    if (navigationAction.navigationType == WKNavigationTypeLinkActivated) {
        navType = UIWebViewNavigationTypeLinkClicked;
    }
    if (navigationAction.navigationType == WKNavigationTypeFormSubmitted) {
        navType = UIWebViewNavigationTypeFormSubmitted;
    }
    if (navigationAction.navigationType == WKNavigationTypeBackForward) {
        navType = UIWebViewNavigationTypeBackForward;
    }
    if (navigationAction.navigationType == WKNavigationTypeReload) {
        navType = UIWebViewNavigationTypeReload;
    }
    if (navigationAction.navigationType == WKNavigationTypeFormResubmitted) {
        navType = UIWebViewNavigationTypeFormResubmitted;
    }
    BOOL result = YES;
    if (delegate != nil) {
        result = [delegate shouldStartLoadWithRequest:self request:navigationAction.request navigationType:navType];
    }
    if (result) {
        decisionHandler(WKNavigationActionPolicyAllow);
    }
    else {
        decisionHandler(WKNavigationActionPolicyCancel);
    }
}

- (void)webView:(WKWebView *)webView didStartProvisionalNavigation:(null_unspecified WKNavigation *)navigation {
    [delegate webViewDidStartLoad:self];
}

- (void)webView:(WKWebView *)webView didFinishNavigation:(null_unspecified WKNavigation *)navigation {
    // force Ajax CommonAPI calls
    NSString* jscode = @"window['__rho_nativeBridgeType']='ajax'";
    [self stringByEvaluatingJavaScriptFromString:jscode];
    
    [delegate webViewDidFinishLoad:self];
}

- (void)webView:(WKWebView *)webView didFailNavigation:(null_unspecified WKNavigation *)navigation withError:(NSError *)error {
    [delegate didFailLoadWithError:self error:error];
}

@end

