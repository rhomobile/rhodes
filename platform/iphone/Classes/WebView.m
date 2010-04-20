//
//  RhoMainView.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 26.03.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

//#import "RhoMainView.h"
#import "Rhodes.h"
#include "common/RhodesApp.h"
#include "ruby/ext/rho/rhoruby.h"

@interface RhoWebViewNavigateTask : NSObject {}
+ (void)run:(NSString*)url :(NSValue*)index;
@end

@implementation RhoWebViewNavigateTask

+ (void)run:(NSString*)url :(NSValue*)value {
    // Workaround:
    // Navigation MUST be done through 'redirect_to' - otherwise WebView does not
    // perform actual url loading from time to time
    int index;
    [value getValue:&index];
    [[[Rhodes sharedInstance] mainView] navigateRedirect:url tab:index];
}

@end

@interface RhoWebViewActiveTabTask : NSObject {}
+ (void)run:(NSValue*)value;
@end

@implementation RhoWebViewActiveTabTask
+ (void)run:(NSValue*)value {
    int *pIndex = [value pointerValue];
    *pIndex = [[[Rhodes sharedInstance] mainView] activeTab];
}
@end

@interface RhoWebViewReloadTask : NSObject {}
+ (void)run:(NSValue*)value;
@end

@implementation RhoWebViewReloadTask
+ (void)run:(NSValue*)value {
    int index;
    [value getValue:&index];
    [[[Rhodes sharedInstance] mainView] reload:index];
}
@end

@interface RhoWebViewExecuteJsTask : NSObject {}
+ (void)run:(NSString*)js :(NSValue*)value;
@end

@implementation RhoWebViewExecuteJsTask
+ (void)run:(NSString*)js :(NSValue*)value {
    int index;
    [value getValue:&index];
    [[[Rhodes sharedInstance] mainView] executeJs:js tab:index];
}
@end


void webview_navigate(char* url, int index) {
    id runnable = [RhoWebViewNavigateTask class];
    id arg1 = [NSString stringWithUTF8String:url];
    id arg2 = [NSValue valueWithBytes:&index objCType:@encode(int)];
    [Rhodes performOnUiThread:runnable arg:arg1 arg:arg2 wait:NO];
}

int webview_active_tab() {
    int index;
    id runnable = [RhoWebViewActiveTabTask class];
    id arg = [NSValue valueWithPointer:&index];
    [Rhodes performOnUiThread:runnable arg:arg wait:YES];
    return index;
}

void webview_refresh(int index) {
    id runnable = [RhoWebViewReloadTask class];
    id arg = [NSValue valueWithBytes:&index objCType:@encode(int)];
    [Rhodes performOnUiThread:runnable arg:arg wait:NO];
}

char* webview_execute_js(char* js, int index) {
    id runnable = [RhoWebViewExecuteJsTask class];
    id arg1 = [NSString stringWithUTF8String:js];
    id arg2 = [NSValue valueWithBytes:&index objCType:@encode(int)];
    [Rhodes performOnUiThread:runnable arg:arg1 arg:arg2 wait:NO];
    return "";
}

char* webview_current_location(int index) {
	return (char*)rho_rhodesapp_getcurrenturl(index);
}

void webview_set_menu_items(VALUE valMenu) {
	//TODO: webview_set_menu_items
	rho_rhodesapp_setViewMenu(valMenu);
}

/*
void perform_webview_refresh() {
	[[ServerHost sharedInstance] performRefreshView];														
}
*/
