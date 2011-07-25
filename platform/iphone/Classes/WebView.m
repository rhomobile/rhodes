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
+ (void)run;
@end

@implementation RhoWebViewNavigateBackTask
+ (void)run {
    id mainView = [[Rhodes sharedInstance] mainView];
    [mainView back:[mainView activeTab]];
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

    id runnable = [RhoWebViewNavigateTask class];
    id arg1 = [NSString stringWithUTF8String:url];
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
    id arg2 = [NSValue valueWithBytes:&index objCType:@encode(int)];
    [Rhodes performOnUiThread:runnable arg:arg1 arg:arg2 wait:NO];
    return "";
}

const char* rho_webview_current_location(int index) {
    return rho_rhodesapp_getcurrenturl(index);
}

void rho_webview_set_menu_items(VALUE valMenu) {
    rho_rhodesapp_setViewMenu(valMenu);
}

void rho_webview_navigate_back()
{
    if (!rho_rhodesapp_check_mode())
        return;
    id runnable = [RhoWebViewNavigateBackTask class];
    [Rhodes performOnUiThread:runnable wait:NO];
}

void rho_webview_full_screen_mode(int enable)
{
    if (!rho_rhodesapp_check_mode())
        return;
    [Rhodes setStatusBarHidden:enable];
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
