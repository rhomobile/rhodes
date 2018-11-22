/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
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
* http://rhomobile.com
*------------------------------------------------------------------------*/

//#import "RhoMainView.h"
#import "Rhodes.h"
#include "common/RhodesApp.h"
#include "ruby/ext/rho/rhoruby.h"
#include "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "WebView"

int rho_rhodesapp_check_mode();

#define SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(v)  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedAscending)


static int getIndex(NSValue *value)
{
    int index;
    [value getValue:&index];
    if (index < 0)
        index = [[[Rhodes sharedInstance] mainView] activeTab];
    return index;
}

@interface RhoWebViewNavigateTask : NSObject {}
+ (void)run:(NSString*)url :(NSValue*)index;
@end

@implementation RhoWebViewNavigateTask
+ (void)run:(NSString*)url :(NSValue*)value {
    // Workaround:
    // Navigation MUST be done through 'redirect_to' - otherwise WebView does not
    // perform actual url loading from time to time
	//[[Rhodes sharedInstance] hideSplash];
    int index = getIndex(value);
    [[[Rhodes sharedInstance] mainView] navigateRedirect:url tab:index];
}
@end

@interface RhoWebViewNavigateBackTask : NSObject {}
+ (void)run:(NSValue*)value;
@end

@implementation RhoWebViewNavigateBackTask
+ (void)run:(NSValue*)value {
    int index = getIndex(value);
    id mainView = [[Rhodes sharedInstance] mainView];
    [mainView back:index];
}
@end

@interface RhoWebViewReloadTask : NSObject {}
+ (void)run:(NSValue*)value;
@end

@implementation RhoWebViewReloadTask
+ (void)run:(NSValue*)value {
    int index = getIndex(value);
    [[[Rhodes sharedInstance] mainView] reload:index];
}
@end

@interface RhoWebViewExecuteJsTask : NSObject {}
+ (void)run:(NSString*)js :(NSValue*)value;
@end

@implementation RhoWebViewExecuteJsTask
+ (void)run:(NSString*)js :(NSValue*)value {
    int index = getIndex(value);
    [[[Rhodes sharedInstance] mainView] executeJs:js tab:index];
}
@end

@interface RhoWebViewSetCookieTask : NSObject {}
+ (void)run:(NSString*)url :(NSString*)cookie;
@end

@implementation RhoWebViewSetCookieTask
+ (void)run:(NSString *)url :(NSString *)cookie {
    [[Rhodes sharedInstance] setCookie:cookie forUrl:url];
}
@end


void rho_webview_navigate(const char* url, int index) 
{
    if (!rho_rhodesapp_check_mode())
        return;
    
    if ( !url )
    {
        RAWLOG_ERROR("WebView.navigate failed: url is nil");
        return;
    }
#if defined(RHO_NO_RUBY_API) && defined(RHO_NO_HTTP_SERVER)
    NSString* rho_path = [NSString stringWithUTF8String:rho_native_rhopath()];
    NSString* str_url = nil;
    if (url[0] == '/') {
        str_url = [NSString stringWithUTF8String:(url+1)];
    }
    else {
        str_url = [NSString stringWithUTF8String:url];
    }
    NSString* final_path = [rho_path stringByAppendingString:@"apps/"];
    final_path = [final_path stringByAppendingString:str_url];

    //id arg1 = [NSString stringWithUTF8String:url];
    id arg1 = final_path;
#else
    int len = rho_app_canonicalize_rho_url(url, NULL, 0);
    char* buf = malloc(len+2);
    rho_app_canonicalize_rho_url(url, buf, len+2);
    id arg1 = [NSString stringWithUTF8String:buf];
    free(buf);
#endif
    id runnable = [RhoWebViewNavigateTask class];
    id arg2 = [NSValue valueWithBytes:&index objCType:@encode(int)];
    [Rhodes performOnUiThread:runnable arg:arg1 arg:arg2 wait:NO];
}

int rho_webview_active_tab() {
    return [[[Rhodes sharedInstance] mainView] activeTab];
}

void rho_webview_refresh(int index) {
    if (!rho_rhodesapp_check_mode())
        return;
    id runnable = [RhoWebViewReloadTask class];
    id arg = [NSValue valueWithBytes:&index objCType:@encode(int)];
    [Rhodes performOnUiThread:runnable arg:arg wait:NO];
}

const char* rho_webview_execute_js(const char* js, int index) {
    if (!rho_rhodesapp_check_mode())
        return "";
    id runnable = [RhoWebViewExecuteJsTask class];
    id arg1 = [NSString stringWithUTF8String:js];
  
    if ( arg1 == nil )
    {
      arg1 = [[NSString alloc] initWithCString:js encoding:NSASCIIStringEncoding];
    }
  
    id arg2 = [NSValue valueWithBytes:&index objCType:@encode(int)];
    [Rhodes performOnUiThread:runnable arg:arg1 arg:arg2 wait:NO];
    return "";
}

const char* rho_webview_execute_js_sync(const char* js, int index) {
    BOOL isUIThread = [NSThread isMainThread];
    //NSAssert(isUIThread, @"Method called using a thread other than main!");
    if (isUIThread)
    {
        [[[Rhodes sharedInstance] mainView] executeJs:[NSString stringWithUTF8String:js] tab:index];
    }
    return "";
}

const char* rho_webview_current_location(int index) {
    //return rho_rhodesapp_getcurrenturl(index);
    NSString* str = [[[Rhodes sharedInstance] mainView] currentLocation:index];
    if (str == nil) {
        return "";
    }
    return [str UTF8String];
}

void rho_webview_navigate_back_with_tab(int index)
{
    if (!rho_rhodesapp_check_mode())
        return;
    id runnable = [RhoWebViewNavigateBackTask class];
    id arg = [NSValue valueWithBytes:&index objCType:@encode(int)];
    [Rhodes performOnUiThread:runnable arg:arg wait:NO];
}

void rho_webview_navigate_back() {
    rho_webview_navigate_back_with_tab(-1);
}


void rho_webview_full_screen_mode(int enable)
{
    if (!rho_rhodesapp_check_mode())
        return;
    [Rhodes setStatusBarHidden:enable];
}

BOOL rho_webview_get_full_screen_mode() {
    if (!rho_rhodesapp_check_mode())
        return NO;
    return [Rhodes getStatusBarHidden];
}

BOOL rho_webview_get_KeyboardDisplayRequiresUserAction() {
    if (!rho_rhodesapp_check_mode())
        return YES;
    UIView *webv_v = [[[[Rhodes sharedInstance] mainView] getRhoWebView:-1] view];
    UIWebView* webv = (UIWebView*)webv_v;

    if (webv != nil) {
        return webv.keyboardDisplayRequiresUserAction;
    }

    return YES;
}

void rho_webview_set_KeyboardDisplayRequiresUserAction(BOOL value) {
    if (!rho_rhodesapp_check_mode())
        return;
    UIView *webv_v = [[[[Rhodes sharedInstance] mainView] getRhoWebView:-1] view];
    UIWebView* webv = (UIWebView*)webv_v;

    if (webv != nil) {
        webv.keyboardDisplayRequiresUserAction = value;
    }
}

BOOL rho_webview_get_EnableDragAndDrop() {
    if (!rho_rhodesapp_check_mode())
        return YES;
    UIView *webv_v = [[[[Rhodes sharedInstance] mainView] getRhoWebView:-1] view];
    UIWebView* webv = (UIWebView*)webv_v;

    if (webv != nil) {
        // TODO: detect current state
        return YES;
    }

    return YES;
}

void rho_webview_set_EnableDragAndDrop(BOOL value) {
    if (!rho_rhodesapp_check_mode())
        return;
    UIView *webv_v = [[[[Rhodes sharedInstance] mainView] getRhoWebView:-1] view];
    UIWebView* webv = (UIWebView*)webv_v;

    if (webv != nil) {
        if (value) {
            // TODO: restore interactions
        }
        else {
#ifdef __IPHONE_11_0
            if (SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"11.0")) {
                dispatch_async( dispatch_get_main_queue(), ^{
                    webv.scrollView.subviews[0].interactions = @[];
                });
            }
#endif
        }
    }
}




void rho_webview_set_cookie(const char *u, const char *c)
{
    if (!rho_rhodesapp_check_mode())
        return;
    id runnable = [RhoWebViewSetCookieTask class];
    NSString *url = [NSString stringWithUTF8String:u];
    NSString *cookie = [NSString stringWithUTF8String:c];
    [Rhodes performOnUiThread:runnable arg:url arg:cookie wait:NO];
}

void rho_webview_get_cookies( NSString* url, NSDictionary** retVal )
{
  if (!rho_rhodesapp_check_mode())
      return;

    *retVal = [[Rhodes sharedInstance] getCookies:url];
}

BOOL rho_webview_remove_cookie( NSString* url, NSString* name )
{
  if (!rho_rhodesapp_check_mode())
      return false;
    
  return [[Rhodes sharedInstance] removeCookie:url name:name];
}

BOOL rho_webview_remove_all_cookies() {
    if (!rho_rhodesapp_check_mode())
        return false;
    
    return [[Rhodes sharedInstance] removeAllCookies];
}


void rho_webview_save(const char* format, const char* path, int tab_index)
{
    UIView *webvw = [[[[Rhodes sharedInstance] mainView] getRhoWebView:tab_index] view];

    webvw.frame=CGRectMake(webvw.frame.origin.x, webvw.frame.origin.y, webvw.frame.size.width, webvw.frame.size.height);
    UIGraphicsBeginImageContextWithOptions(webvw.frame.size, NO, 0.0f);
    [webvw.layer renderInContext:UIGraphicsGetCurrentContext()];
    UIImage *screenshot = UIGraphicsGetImageFromCurrentImageContext();

    NSString *pathStr = [NSString stringWithUTF8String:path];
    [UIImagePNGRepresentation(screenshot) writeToFile:pathStr atomically:YES];

    UIGraphicsEndImageContext();
}

NSString* rho_webview_get_current_url(int tab_index) {
    NSString* str = [[[Rhodes sharedInstance] mainView] get_current_url:tab_index];
    if (str == nil) {
        return @"";
    }
    return str;
}

