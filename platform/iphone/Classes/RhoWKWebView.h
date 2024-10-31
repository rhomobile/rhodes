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


#import <UIKit/UIKit.h>
#import <WebKit/WebKit.h>
#import <WebKit/WKWebViewConfiguration.h>
#import "RhoWebView.h"

@interface RhoWKWebView : NSObject<RhoWebView, WKNavigationDelegate, WKUIDelegate> {
    WKWebView *webview;
    id<RhoWebViewDelegate,NSObject> delegate;
}

@property (nonatomic,retain) WKWebView *webview;
@property (nonatomic,retain) id<RhoWebViewDelegate,NSObject> delegate;

- (id)initWithFrame:(CGRect)frame;

- (void) setupDelegate:(id<RhoWebViewDelegate,NSObject>)delegate;

- (UIView*)view;

- (UIView*)containerView;

- (NSString*)currentLocation;

// UIWebView

- (NSString *)stringByEvaluatingJavaScriptFromString:(NSString *)script wantAnswer:(BOOL)wantAnswer;

- (void)loadRequest:(NSURLRequest *)request;

- (void)loadHTMLString:(NSString *)string baseURL:(nullable NSURL *)baseURL;

- (void)stopLoading;

- (void)reload;

- (void)goBack;

- (void)goForward;



// WKNavigationDelegate
//   UIWebViewDelegate -> WKNavigationDelegate migrate
//     didFailLoadWithError => didFailNavigation
//     webViewDidFinishLoad => didFinishNavigation
//     webViewDidStartLoad => didStartProvisionalNavigation
//     shouldStartLoadWithRequest => decidePolicyForNavigationAction

- (void)webView:(WKWebView *)webView decidePolicyForNavigationAction:(WKNavigationAction *)navigationAction decisionHandler:(void (^)(WKNavigationActionPolicy))decisionHandler;
//- (void)webView:(WKWebView *)webView decidePolicyForNavigationResponse:(WKNavigationResponse *)navigationResponse decisionHandler:(void (^)(WKNavigationResponsePolicy))decisionHandler;
- (void)webView:(WKWebView *)webView didStartProvisionalNavigation:(null_unspecified WKNavigation *)navigation;
//- (void)webView:(WKWebView *)webView didReceiveServerRedirectForProvisionalNavigation:(null_unspecified WKNavigation *)navigation;
//- (void)webView:(WKWebView *)webView didFailProvisionalNavigation:(null_unspecified WKNavigation *)navigation withError:(NSError *)error;
//- (void)webView:(WKWebView *)webView didCommitNavigation:(null_unspecified WKNavigation *)navigation;
- (void)webView:(WKWebView *)webView didFinishNavigation:(null_unspecified WKNavigation *)navigation;
- (void)webView:(WKWebView *)webView didFailNavigation:(null_unspecified WKNavigation *)navigation withError:(NSError *)error;
- (void)webView:(WKWebView *)webView didReceiveAuthenticationChallenge:(NSURLAuthenticationChallenge *)challenge
completionHandler:(void (^)(NSURLSessionAuthChallengeDisposition disposition, NSURLCredential *credential))completionHandler;

//- (void)webView:(WKWebView *)webView
//authenticationChallenge:(NSURLAuthenticationChallenge *)challenge
//shouldAllowDeprecatedTLS:(void (^)(BOOL))decisionHandler;

- (void)webView:(WKWebView *)webView runJavaScriptAlertPanelWithMessage:(NSString *)message initiatedByFrame:(WKFrameInfo *)frame completionHandler:(void (^)())completionHandler;
- (void)webView:(WKWebView *)webView runJavaScriptConfirmPanelWithMessage:(NSString *)message initiatedByFrame:(WKFrameInfo *)frame completionHandler:(void (^)(BOOL result))completionHandler;
- (void)webView:(WKWebView *)webView runJavaScriptTextInputPanelWithPrompt:(NSString *)prompt defaultText:(NSString *)defaultText initiatedByFrame:(WKFrameInfo *)frame completionHandler:(void (^)(NSString *result))completionHandler;

@end
