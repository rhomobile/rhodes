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



#import "RhoWKWebView.h"
#import "RhoMainView.h"
#include "logging/RhoLog.h"
#include "common/RhoConf.h"

#import "Rhodes.h"

#import "URLProtocol/CRhoWKURLProtocol.h"


#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoWKWebView"

#import "common/app_build_capabilities.h"

#ifdef APP_BUILD_CAPABILITY_IOS_WKWEBVIEW_HTTP_DIRECT_PROCESSING

#import <objc/runtime.h>



@interface WKWebView (RhoAltMethod)
+ (BOOL)bogushandlesURLScheme:(NSString *)urlScheme;
@end

@implementation WKWebView (RhoAltMethod)

+ (BOOL)bogushandlesURLScheme:(NSString *)urlScheme {
    return false;
}

@end


static void dumpClassInfo(Class c, int inheritanceDepth)
{
    Class superClass = class_getSuperclass(c);
    if (superClass != Nil)
    {
        dumpClassInfo(superClass, (inheritanceDepth + 1));
    }

    int i = 0;
    unsigned int mc = 0;

    const char* className = class_getName(c);

    Method* mlist = class_copyMethodList(c, &mc);
    for (i = 0; i < mc; i++)
    {
        Method method = mlist[i];
        SEL methodSelector = method_getName(method);
        const char* methodName = sel_getName(methodSelector);

        const char *typeEncodings = method_getTypeEncoding(method);

        char returnType[80];
        method_getReturnType(method, returnType, 80);

        NSLog(@"%2.2d %s ==> %s (%s)", inheritanceDepth, className, methodName, (typeEncodings == Nil) ? "" : typeEncodings);

        int ac = method_getNumberOfArguments(method);
        int a = 0;
        for (a = 0; a < ac; a++) {
            char argumentType[80];
            method_getArgumentType(method, a, argumentType, 80);
            NSLog(@"   Argument no #%d: %s", a, argumentType);
        }
    }
}

#endif //APP_BUILD_CAPABILITY_IOS_WKWEBVIEW_HTTP_DIRECT_PROCESSING





@implementation RhoWKWebView

@synthesize webview, delegate;

- (id)initWithFrame:(CGRect)frame {
    [self init];

    WKWebViewConfiguration *configuration = [[WKWebViewConfiguration alloc] init];
    if ( rho_conf_getBool("enable_media_playback_without_gesture") == 1 ) {
        configuration.mediaPlaybackRequiresUserAction = NO;
    }

    if ( rho_conf_getBool("WebView.allowsInlineMediaPlayback") == 1 ) {
        configuration.allowsInlineMediaPlayback = YES;
    }


    BOOL isDirectRequestActivated = NO;
    if (rho_conf_is_property_exists("ios_direct_local_requests")!=0) {
        if (rho_conf_getBool("ios_direct_local_requests")!=0 ) {
            isDirectRequestActivated = YES;
        }
    }
    BOOL isDirectRequestCustomProtocol = NO;
    if (rho_conf_is_property_exists("ios_direct_local_requests_with_custom_protocol")!=0) {
        if (rho_conf_getBool("ios_direct_local_requests_with_custom_protocol")!=0 ) {
            isDirectRequestCustomProtocol = YES;
        }
    }

    if (isDirectRequestActivated || isDirectRequestCustomProtocol) {
        
        BOOL isDirectProcessingActivated = NO;
        
            
        if (!isDirectProcessingActivated) {
            if (isDirectRequestCustomProtocol) {
                
                CRhoWKURLProtocol *schemeHandler = [[CRhoWKURLProtocol alloc] init];
                [configuration setURLSchemeHandler:schemeHandler forURLScheme:@"rhoctp"];
                isDirectProcessingActivated = YES;
            }
        }

#ifdef APP_BUILD_CAPABILITY_IOS_WKWEBVIEW_HTTP_DIRECT_PROCESSING_METHOD_3
        if (!isDirectProcessingActivated) {
            
            try {
                NSArray *privateStrArr = @[@"Handlers", @"Scheme", @"url", @"_"];
                NSString *keyName =  [[[privateStrArr reverseObjectEnumerator] allObjects] componentsJoinedByString:@""];
                
                CRhoWKURLProtocol *schemeHandler = [[CRhoWKURLProtocol alloc] init];
                [configuration setURLSchemeHandler:schemeHandler forURLScheme:@"rhoapiprotocol"];
                NSMutableDictionary *handlers = [configuration valueForKey:keyName];
                if (handlers != nil) {
                    handlers[@"http"] = schemeHandler;
                    handlers[@"https"] = schemeHandler;
                    isDirectProcessingActivated = YES;
                }
            } catch(id anException)  {
                int o = 9;
                o = 7;

            }
        }
#endif

#ifdef APP_BUILD_CAPABILITY_IOS_WKWEBVIEW_HTTP_DIRECT_PROCESSING_METHOD_2
        if (!isDirectProcessingActivated) {

            try {

                NSArray *privateStrArr = @[@"Controller", @"Context", @"Browsing", @"K", @"W"];
                NSString *className =  [[[privateStrArr reverseObjectEnumerator] allObjects] componentsJoinedByString:@""];
                NSArray *privateStr2Arr = @[@":", @"Protocol", @"Custom", @"For", @"Scheme", @"register"];
                NSString *selName =  [[[privateStr2Arr reverseObjectEnumerator] allObjects] componentsJoinedByString:@""];

                Class cls = NSClassFromString(className);
                SEL sel = NSSelectorFromString(selName);
                
                if (cls && sel) {
                    if ([(id)cls respondsToSelector:sel]) {
                        [(id)cls performSelector:sel withObject:@"http"];
                        [(id)cls performSelector:sel withObject:@"https"];
                        isDirectProcessingActivated = YES;
                    }
                }
                // already registered when app started
                //[NSURLProtocol registerClass:[CRhoURLProtocol class]];
                
            } catch(id anException)  {
                int o = 9;
                o = 7;

            }

        }
#endif

#ifdef APP_BUILD_CAPABILITY_IOS_WKWEBVIEW_HTTP_DIRECT_PROCESSING

        if (!isDirectProcessingActivated) {
            try {

                CRhoWKURLProtocol *schemeHandler = [[CRhoWKURLProtocol alloc] init];

                // replace original static method to our bogus method
                Method bogusHandle = class_getClassMethod([WKWebView class], @selector(bogushandlesURLScheme:));
                Method handleOriginal = class_getClassMethod([WKWebView class], @selector(handlesURLScheme:));
                method_exchangeImplementations(bogusHandle, handleOriginal);


                [configuration setURLSchemeHandler:schemeHandler forURLScheme:@"http"];
                [configuration setURLSchemeHandler:schemeHandler forURLScheme:@"https"];

                // return original static method
                method_exchangeImplementations(bogusHandle, handleOriginal);
                isDirectProcessingActivated = YES;
            } catch(id anException)  {
                int o = 9;
                o = 7;
            }
        }
#endif
        
        if (!isDirectProcessingActivated) {
            RAWLOG_ERROR("You can not enable ios_direct_local_requests if you not added IOS_WKWEBVIEW_HTTP_DIRECT_PROCESSING or/and APP_BUILD_CAPABILITY_IOS_WKWEBVIEW_HTTP_DIRECT_PROCESSING_METHOD_2 or/and APP_BUILD_CAPABILITY_IOS_WKWEBVIEW_HTTP_DIRECT_PROCESSING_METHOD_3 capability/s to build.yml !!!");
        }
    }
    
    configuration.preferences.javaScriptCanOpenWindowsAutomatically = YES;
    //configuration.limitsNavigationsToAppBoundDomains = NO;
    
    WKWebView* w = [[WKWebView alloc] initWithFrame:frame configuration:configuration];

    //w.scalesPageToFit = YES;
    if ( !rho_conf_getBool("WebView.enableBounce") )
        [[w scrollView] setBounces:NO];
    w.userInteractionEnabled = YES;
    w.multipleTouchEnabled = YES;
    w.clipsToBounds = NO;
    //w.dataDetectorTypes = UIDataDetectorTypeNone;
    w.navigationDelegate = self;
    w.UIDelegate= self;
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

- (NSString *)stringByEvaluatingJavaScriptFromString:(NSString *)script wantAnswer:(BOOL)wantAnswer{
        __block NSString *resultString = nil;
        __block BOOL finished = NO;

    if (!wantAnswer) {
        [webview evaluateJavaScript:script completionHandler:nil];
        return nil;
    }
    
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

        while (!finished && wantAnswer)
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

    //NSLog(@"$$$$$ NavigationAction with URL = %@", [[navigationAction request] URL]);

    BOOL result = YES;
    if (delegate != nil) {
        result = [delegate shouldStartLoadWithRequest:self request:navigationAction.request];
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
    [self stringByEvaluatingJavaScriptFromString:jscode wantAnswer:NO];

    [delegate webViewDidFinishLoad:self];
}

- (void)webView:(WKWebView *)webView didFailNavigation:(null_unspecified WKNavigation *)navigation withError:(NSError *)error {
    [delegate didFailLoadWithError:self error:error];
}



- (void)webView:(WKWebView *)webView runJavaScriptAlertPanelWithMessage:(NSString *)message initiatedByFrame:(WKFrameInfo *)frame completionHandler:(void (^)())completionHandler
{
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:nil
                                                                   message:message
                                                            preferredStyle:UIAlertControllerStyleAlert];

    [alert addAction:[UIAlertAction actionWithTitle:NSLocalizedString(@"OK", nil) style:UIAlertActionStyleCancel handler:^(UIAlertAction *action) {
        completionHandler();
    }]];

    [[[[Rhodes sharedInstance] mainView] getMainViewController] presentViewController:alert animated:YES completion:nil];
}

- (void)webView:(WKWebView *)webView runJavaScriptConfirmPanelWithMessage:(NSString *)message initiatedByFrame:(WKFrameInfo *)frame completionHandler:(void (^)(BOOL result))completionHandler
{
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:nil
                                                                   message:message
                                                            preferredStyle:UIAlertControllerStyleAlert];

    [alert addAction:[UIAlertAction actionWithTitle:NSLocalizedString(@"Cancel", nil) style:UIAlertActionStyleCancel handler:^(UIAlertAction *action) {
        completionHandler(NO);
    }]];

    [alert addAction:[UIAlertAction actionWithTitle:NSLocalizedString(@"OK", nil) style:UIAlertActionStyleDefault handler:^(UIAlertAction *action) {
        completionHandler(YES);
    }]];

    [[[[Rhodes sharedInstance] mainView] getMainViewController] presentViewController:alert animated:YES completion:nil];
}

- (void)webView:(WKWebView *)webView runJavaScriptTextInputPanelWithPrompt:(NSString *)prompt defaultText:(NSString *)defaultText initiatedByFrame:(WKFrameInfo *)frame completionHandler:(void (^)(NSString *result))completionHandler
{
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:nil
                                                                   message:prompt
                                                            preferredStyle:UIAlertControllerStyleAlert];

    [alert addTextFieldWithConfigurationHandler:^(UITextField *textField) {
        textField.placeholder = prompt;
        textField.secureTextEntry = NO;
        textField.text = defaultText;
    }];

    [alert addAction:[UIAlertAction actionWithTitle:NSLocalizedString(@"Cancel", nil) style:UIAlertActionStyleCancel handler:^(UIAlertAction *action) {
        completionHandler(nil);
    }]];

    [alert addAction:[UIAlertAction actionWithTitle:NSLocalizedString(@"OK", nil) style:UIAlertActionStyleDefault handler:^(UIAlertAction *action) {
        completionHandler([alert.textFields.firstObject text]);
    }]];

    [[[[Rhodes sharedInstance] mainView] getMainViewController] presentViewController:alert animated:YES completion:nil];
}


@end
