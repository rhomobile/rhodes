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

#import "RhoWebViewFabrique.h"
#import <WebKit/WebKit.h>

#import "common/app_build_capabilities.h"

#ifdef APP_BUILD_CAPABILITY_IOS_UIWEBVIEW
#import "RhoUIWebView.h"
#endif

#import "RhoWKWebView.h"

#import "common/RhoConf.h"
#include "logging/RhoLog.h"


#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoWebViewFabrique"


@implementation RhoWebViewFabrique

+ (id<RhoWebView,NSObject>)createRhoWebViewWithFrame:(CGRect)frame {

    BOOL shouldMakeWkWebView = YES;
    BOOL isDirectRequestActivated = NO;
    BOOL isDirectRequestCustomProtocol = NO;
    BOOL isWKWebViewCLassExist = NO;
    if (rho_conf_is_property_exists("ios_direct_local_requests")!=0) {
        if (rho_conf_getBool("ios_direct_local_requests")!=0 ) {
            isDirectRequestActivated = YES;
        }
    }
    if (rho_conf_is_property_exists("ios_direct_local_requests_with_custom_protocol")!=0) {
        if (rho_conf_getBool("ios_direct_local_requests_with_custom_protocol")!=0 ) {
            isDirectRequestCustomProtocol = YES;
        }
    }
    if (rho_conf_is_property_exists("ios_use_WKWebView")!=0) {
        if (rho_conf_getBool("ios_use_WKWebView")==0 ) {
#ifdef APP_BUILD_CAPABILITY_IOS_UIWEBVIEW
            shouldMakeWkWebView = NO;
#else
            RAWLOG_ERROR("You can not use UIWebView without capability IOS_UIWEBVIEW in build.yml !!!");
#endif
        }
    }
    if (NSClassFromString(@"WKWebView")) {
        isWKWebViewCLassExist = YES;
    }

    id<RhoWebView,NSObject> webView = nil;

    if (shouldMakeWkWebView) {
        RAWLOG_INFO("Try to create WKWebView ...");
        if (isDirectRequestActivated && (!isDirectRequestCustomProtocol)) {
#ifdef APP_BUILD_CAPABILITY_IOS_WKWEBVIEW_HTTP_DIRECT_PROCESSING

#elif defined APP_BUILD_CAPABILITY_IOS_WKWEBVIEW_HTTP_DIRECT_PROCESSING_METHOD_2

#elif defined APP_BUILD_CAPABILITY_IOS_WKWEBVIEW_HTTP_DIRECT_PROCESSING_METHOD_3

#else
            RAWLOG_ERROR("You can not enable ios_direct_local_requests if you not added IOS_WKWEBVIEW_HTTP_DIRECT_PROCESSING or/and APP_BUILD_CAPABILITY_IOS_WKWEBVIEW_HTTP_DIRECT_PROCESSING_METHOD_2 or/and APP_BUILD_CAPABILITY_IOS_WKWEBVIEW_HTTP_DIRECT_PROCESSING_METHOD_3 capability to build.yml !!!");
#endif
        }
        if (isWKWebViewCLassExist) {
            webView = [[RhoWKWebView alloc] initWithFrame:frame];
            RAWLOG_INFO("WKWebView was created OK !");
        }
        else {
            RAWLOG_ERROR("can not make WKWebView  - can not find WKWebView class !!!");
        }
    }
#ifdef APP_BUILD_CAPABILITY_IOS_UIWEBVIEW
    if (webView == nil) {
        RAWLOG_INFO("Try to create UIWebView ...");
        webView = [[RhoUIWebView alloc] initWithFrame:frame];
        RAWLOG_INFO("UIWebView was created OK !");
    }
#endif
    return webView;
}

@end
