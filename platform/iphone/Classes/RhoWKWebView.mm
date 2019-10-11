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

#import "URLProtocol/CRhoWKURLProtocol.h"


#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoWKWebView"



/*
@interface CRhoWKURLHandler : NSObject<WKURLSchemeHandler> {
}

- (void)webView:(WKWebView *)webView startURLSchemeTask:(id <WKURLSchemeTask>)urlSchemeTask;
- (void)webView:(WKWebView *)webView stopURLSchemeTask:(id <WKURLSchemeTask>)urlSchemeTask;

@end

@implementation CRhoWKURLHandler

- (void)webView:(WKWebView *)webView startURLSchemeTask:(id <WKURLSchemeTask>)urlSchemeTask {
    
}

- (void)webView:(WKWebView *)webView stopURLSchemeTask:(id <WKURLSchemeTask>)urlSchemeTask {
    
}

@end
*/


#import <objc/runtime.h>

void MethodSwizzle(Class aClass, SEL orig_sel, SEL alt_sel)
{
    Method orig_method = nil, alt_method = nil;

    // First, look for the methods
    orig_method = class_getInstanceMethod(aClass, orig_sel);
    alt_method = class_getInstanceMethod(aClass, alt_sel);

    // If both are found, swizzle them
    if ((orig_method != nil) && (alt_method != nil))
        {
        char *temp1;
        IMP temp2;

        /*
         temp1 = orig_method->method_types;
        orig_method->method_types = alt_method->method_types;
        alt_method->method_types = temp1;

        temp2 = orig_method->method_imp;
        orig_method->method_imp = alt_method->method_imp;
        alt_method->method_imp = temp2;
         */
        }
}


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




@interface RhoWKWebViewConfiguration : WKWebViewConfiguration {}

- (id<WKURLSchemeHandler>)urlSchemeHandlerForURLScheme:(NSString *)urlScheme;

@end


@implementation RhoWKWebViewConfiguration

- (id<WKURLSchemeHandler>)urlSchemeHandlerForURLScheme:(NSString *)urlScheme {
    if ([@"http" isEqualToString:urlScheme]) {
        return [super urlSchemeHandlerForURLScheme:@"nevercallscheme"];
    }
    if ([@"https" isEqualToString:urlScheme]) {
        return [super urlSchemeHandlerForURLScheme:@"nevercallscheme"];
    }
    return [super urlSchemeHandlerForURLScheme:urlScheme];
}

@end



@interface RhoCustomWKWebView : WKWebView {}

- (id<WKURLSchemeHandler>)urlSchemeHandlerForURLScheme:(NSString *)urlScheme;

@end


@implementation RhoCustomWKWebView

- (id<WKURLSchemeHandler>)urlSchemeHandlerForURLScheme:(NSString *)urlScheme {
    /*
     if ([@"http" isEqualToString:urlScheme]) {
        return [super urlSchemeHandlerForURLScheme:@"nevercallscheme"];
    }
    if ([@"https" isEqualToString:urlScheme]) {
        return [super urlSchemeHandlerForURLScheme:@"nevercallscheme"];
    }
     */
    return [super urlSchemeHandlerForURLScheme:urlScheme];
}

@end



@implementation RhoWKWebView

@synthesize webview, delegate;

- (id)initWithFrame:(CGRect)frame {
    [self init];
    
    RhoWKWebViewConfiguration *configuration = [[RhoWKWebViewConfiguration alloc] init];
    if ( rho_conf_getBool("enable_media_playback_without_gesture") == 1 )
        configuration.mediaPlaybackRequiresUserAction = NO;

    ///* MOHUS
    CRhoWKURLProtocol *schemeHandler = [[CRhoWKURLProtocol alloc] init];
    //[configuration setURLSchemeHandler:schemeHandler forURLScheme:@"nevercallscheme"];
    
    
    /*
     MethodSwizzle([WKWebView class],
                  @selector(handlesURLScheme:),
                  @selector(handlesURLSchemeAlt:));
    */
    
    
    
    
    // replace original static method to our bogus method
    Method bogusHandle = class_getClassMethod([WKWebView class], @selector(bogushandlesURLScheme:));
    Method handleOriginal = class_getClassMethod([WKWebView class], @selector(handlesURLScheme:));
    method_exchangeImplementations(bogusHandle, handleOriginal);
    
    
    [configuration setURLSchemeHandler:schemeHandler forURLScheme:@"http"];
    [configuration setURLSchemeHandler:schemeHandler forURLScheme:@"https"];

    // return original static method
    method_exchangeImplementations(bogusHandle, handleOriginal);

    
    //NSMutableDictionary *handlers = [configuration valueForKey:@"_urlSchemeHandlers"];
    //NSMutableDictionary *handlers = [configuration _urlSchemeHandlers];
    //handlers[@"http"] = schemeHandler;
    //handlers[@"https"] = schemeHandler;
    //*/
     
     
    RhoCustomWKWebView* w = [[RhoCustomWKWebView alloc] initWithFrame:frame configuration:configuration];
    
    
    /*
    Class c =  object_getClass(w);
    dumpClassInfo(c, 0);
    
    unsigned int outCount, i;

    objc_property_t *properties = class_copyPropertyList([WKWebView class], &outCount);
    for(i = 0; i < outCount; i++) {
        objc_property_t property = properties[i];
        const char *propName = property_getName(property);
        if(propName) {
                //const char *propType = getPropertyType(property);
                NSString *propertyName = [NSString stringWithUTF8String:propName];
                NSLog(@"$$$$$   %@", propertyName);
                //NSString *propertyType = [NSString stringWithUTF8String:propType];
        }
    }
    free(properties);
    //*/
    
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
    
    NSLog(@"$$$$$ NavigationAction with URL = %@", [[navigationAction request] URL]);
    
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

