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

#import "RhoWebViewFabrique.h"
#import <WebKit/WebKit.h>

#import "RhoUIWebView.h"
#import "RhoWKWebView.h"

#import "common/RhoConf.h"
#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoWebViewFabrique"


@implementation RhoWebViewFabrique

+ (id<RhoWebView,NSObject>)createRhoWebViewWithFrame:(CGRect)frame {
    
    BOOL shouldMakeWkWebView = NO;
    BOOL isDirectRequestActivated = NO;
    BOOL isWKWebViewCLassExist = NO;
    if (rho_conf_is_property_exists("ios_direct_local_requests")!=0) {
        if (rho_conf_getBool("ios_direct_local_requests")!=0 ) {
            isDirectRequestActivated = YES;
        }
    }
    if (rho_conf_is_property_exists("ios_use_WKWebView")!=0) {
        if (rho_conf_getBool("ios_use_WKWebView")!=0 ) {
            shouldMakeWkWebView = YES;
        }
    }
    if (NSClassFromString(@"WKWebView")) {
        isWKWebViewCLassExist = YES;
    }
    
    id<RhoWebView,NSObject> webView = nil;
    
    if (shouldMakeWkWebView) {
        RAWLOG_INFO("Try to create WKWebView ...");
        if (isDirectRequestActivated) {
            RAWLOG_ERROR("can not make WKWebView because ios_direct_local_requests is ACTIVATED !!!");
        }
        else {
            if (isWKWebViewCLassExist) {
                webView = [[RhoWKWebView alloc] initWithFrame:frame];
                RAWLOG_INFO("WKWebView was created OK !");
            }
            else {
                RAWLOG_ERROR("can not make WKWebView because iOS version < 8.0 - can not find WKWebView class !!!");
            }
        }
    }
    if (webView == nil) {
        RAWLOG_INFO("Try to create UIWebView ...");
        webView = [[RhoUIWebView alloc] initWithFrame:frame];
        RAWLOG_INFO("UIWebView was created OK !");
    }

    return webView;
}

@end
