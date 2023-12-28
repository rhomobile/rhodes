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
 * http://tau-platform.com
 *------------------------------------------------------------------------*/

#import "common/app_build_capabilities.h"
#ifdef APP_BUILD_CAPABILITY_IOS_UIWEBVIEW

#import "RhoUIWebView.h"
#import "RhoMainView.h"

#include "logging/RhoLog.h"
#include "common/RhoConf.h"


#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoUIWebView"




@implementation RhoUIWebView

@synthesize webview, delegate;


- (id)initWithFrame:(CGRect)frame {
    [self init];
    UIWebView* w = [[UIWebView alloc] initWithFrame:frame];
    w.scalesPageToFit = YES;
    if ( !rho_conf_getBool("WebView.enableBounce") )
        [[w scrollView] setBounces:NO];
    if ( rho_conf_getBool("enable_media_playback_without_gesture") == 1 )
        w.mediaPlaybackRequiresUserAction = NO;

    if (rho_conf_is_property_exists("WebView.keyboardDisplayRequiresUserAction")) {
        int v = rho_conf_getBool("WebView.keyboardDisplayRequiresUserAction");
        w.keyboardDisplayRequiresUserAction = (v != 0);
    }

    if (rho_conf_is_property_exists("WebView.enableDragAndDrop")) {
        int v = rho_conf_getBool("WebView.enableDragAndDrop");
        if (!v) {
#ifdef __IPHONE_11_0
            if (SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"11.0")) {
                w.scrollView.subviews[0].interactions = @[];
            }
#endif
        }
    }

    w.userInteractionEnabled = YES;
    w.multipleTouchEnabled = YES;
    w.clipsToBounds = NO;
    w.dataDetectorTypes = UIDataDetectorTypeNone;
    w.delegate = self;
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
    return [[webview.request mainDocumentURL] absoluteString];
}

- (void) setupDelegate:(id<RhoWebViewDelegate,NSObject>)delegate {
    self.delegate = delegate;
}

- (UIView*)containerView {
    return self.webview.scrollView;
}




//UIWebView

- (NSString *)stringByEvaluatingJavaScriptFromString:(NSString *)script wantAnswer:(BOOL)wantAnswer{
    return [webview stringByEvaluatingJavaScriptFromString:script];
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







// UIWebViewDelegate

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType {
    if (delegate != nil) {
        return [delegate shouldStartLoadWithRequest:self request:request];
    }
    return YES;
}

- (void)webViewDidStartLoad:(UIWebView *)webView {
    [delegate webViewDidStartLoad:self];
}

- (void)webViewDidFinishLoad:(UIWebView *)webView {
    [delegate webViewDidFinishLoad:self];
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error {
    [delegate didFailLoadWithError:self error:error];
}

@end


#endif //APP_BUILD_CAPABILITY_IOS_UIWEBVIEW
