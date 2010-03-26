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

@interface RhoWebViewNavigateTask : NSObject<RhoRunnable>
{
    NSString *url;
    int index;
}

@property (nonatomic, copy) NSString *url;
@property (nonatomic, assign) int index;

- (id)initWithUrl:(NSString*)url andIndex:(int)index;
- (void)dealloc;

- (void)run;

@end

@implementation RhoWebViewNavigateTask

@synthesize url, index;

- (id)initWithUrl:(NSString*)u andIndex:(int)idx {
    self.url = u;
    self.index = idx;
    return self;
}

- (void)dealloc {
    [url release];
    [super dealloc];
}

- (void)run {
    [[[Rhodes sharedInstance] mainView] navigate:url tab:index];
}

@end

@interface RhoWebViewActiveTabTask : NSObject<RhoRunnable>
{
    int *pIndex;
}

- (id)init:(int*)p;
- (void)run;

@end

@implementation RhoWebViewActiveTabTask

- (id)init:(int*)p {
    pIndex = p;
    return self;
}

- (void)run {
    *pIndex = [[[Rhodes sharedInstance] mainView] activeTab];
}

@end

@interface RhoWebViewReloadTask : NSObject<RhoRunnable>
{
    int index;
}

- (id)initWithIndex:(int)index;
- (void)run;

@end

@implementation RhoWebViewReloadTask

- (id)initWithIndex:(int)idx {
    index = idx;
    return self;
}

- (void)run {
    [[[Rhodes sharedInstance] mainView] reload:index];
}

@end

@interface RhoWebViewExecuteJsTask : NSObject<RhoRunnable>
{
    NSString *js;
    int index;
}

@property (nonatomic, copy) NSString *js;
@property (nonatomic, assign) int index;

- (id)initWithJs:(NSString*)s andIndex:(int)index;
- (void)dealloc;
- (void)run;

@end

@implementation RhoWebViewExecuteJsTask

@synthesize js,index;

- (id)initWithJs:(NSString*)s andIndex:(int)idx {
    self.js = s;
    self.index = idx;
    return self;
}

- (void)dealloc {
    [js release];
    [super dealloc];
}

- (void)run {
    [[[Rhodes sharedInstance] mainView] executeJs:js tab:index];
}

@end


void webview_navigate(char* url, int index) {
    id task = [[[RhoWebViewNavigateTask alloc]
            initWithUrl:[NSString stringWithUTF8String:url] andIndex:index] autorelease];
    [Rhodes performOnUiThread:task wait:NO];
}

int webview_active_tab() {
    int index;
    id task = [[RhoWebViewActiveTabTask alloc] init:&index];
    [Rhodes performOnUiThread:task wait:YES];
    [task release];
    return index;
}

void webview_refresh(int index) {
    id task = [[[RhoWebViewReloadTask alloc] initWithIndex:index] autorelease];
    [Rhodes performOnUiThread:task wait:NO];
}

char* webview_execute_js(char* js, int index) {
    id task = [[[RhoWebViewExecuteJsTask alloc] initWithJs:[NSString stringWithUTF8String:js] andIndex:index] autorelease];
    [Rhodes performOnUiThread:task wait:NO];
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
