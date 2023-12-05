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

#import <UIKit/UIKit.h>
#import "RhoWebView.h"

@interface RhoUIWebView : NSObject<RhoWebView, UIWebViewDelegate> {
    UIWebView *webview;
    id<RhoWebViewDelegate,NSObject> delegate;
}

@property (nonatomic,retain) UIWebView *webview;
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


// UIWebViewDelegate

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType;

- (void)webViewDidStartLoad:(UIWebView *)webView;

- (void)webViewDidFinishLoad:(UIWebView *)webView;

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error;

@end

#endif //APP_BUILD_CAPABILITY_IOS_UIWEBVIEW
