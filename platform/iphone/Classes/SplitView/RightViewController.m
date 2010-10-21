    //
//  TabbedMainView.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 26.03.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "RightViewController.h"
#import "SimpleMainView.h"
#import "Rhodes.h"
#import "AppManager.h"

#include "common/RhodesApp.h"
#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RightViewController"




@interface RhoRightItem : NSObject {
@public
    NSString *url;
	SimpleMainView* view;
    BOOL loaded;
    BOOL reload;
}

@property (retain) NSString *url;
@property (retain) SimpleMainView *view;
@property (assign) BOOL loaded;
@property (assign) BOOL reload;

- (id)init;
- (void)dealloc;

@end

@implementation RhoRightItem

@synthesize url, loaded, reload, view;

- (id)init {
    url = nil;
	view = nil;
    loaded = NO;
    reload = NO;
    return self;
}

- (void)dealloc {
    [url release];
	[view release];
    [super dealloc];
}

@end





@implementation RightViewController

@synthesize itemsData, tabindex;

- (id)initWithItems:(NSArray*)items parent:(SplittedMainView*)parent {
	self = [self init];
	self.view = [[UIView alloc] init];
	
	CGRect rect = CGRectMake(0,0,200,200);//self.view.frame;

    int count = [items count]/4;

    NSMutableArray *tabs = [[NSMutableArray alloc] initWithCapacity:count];
    
    NSString *initUrl = nil;
    
    for (int i = 0; i < count; ++i) {
        int index = i*4 - 1;
        NSString *label = [items objectAtIndex:++index];
        NSString *url = [items objectAtIndex:++index];
        NSString *icon = [items objectAtIndex:++index];
        NSString *reload = [items objectAtIndex:++index];
        
        if (!initUrl)
            initUrl = url;
        
        if (label && url && icon) {
            RhoRightItem *td = [[RhoRightItem alloc] init];
            td.url = url;
            td.reload = [reload isEqualToString:@"true"];
            
            SimpleMainView *subController = [[SimpleMainView alloc] initWithParentView:parent frame:rect];
            
			subController.title = label;
			subController.view.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
            
			td.view = subController;
            
            [tabs addObject:td];
            
            [td release];
            [subController release];
        }
    }
    
    self.itemsData = tabs;
    [tabs release];
	
	self.view.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
	
    self.tabindex = 0;
    if (initUrl){
        [self navigateRedirect:initUrl tab:0];
		RhoRightItem *ri = [self.itemsData objectAtIndex:tabindex];
		ri.loaded = YES;
	}
	// set first tab
	SimpleMainView* v = [[self.itemsData objectAtIndex:0] view];
	if (v != NULL) {
		[v navigateRedirect:initUrl tab:0];
		[self.view addSubview:v.view];
		[self.view setNeedsLayout];
		[v.view setNeedsDisplay];
	}
	
	return self;
}


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // MUST return YES to allow all orientations
    return YES;
}



- (SimpleMainView*) getSimpleView:(int)index {
	if (index == -1) {
		index = self.tabindex;
	}
	return [[self.itemsData objectAtIndex:index] view];
}


- (UIWebView*)detachWebView {
	SimpleMainView* v = [self getSimpleView:-1];
	return [v detachWebView];
}

- (void)loadHTMLString:(NSString*)data {
	SimpleMainView* v = [self getSimpleView:-1];
	[v loadHTMLString:data];
}

- (void)back:(int)index {
	SimpleMainView* v = [self getSimpleView:index];
	[v back:index];
}

- (void)forward:(int)index {
	SimpleMainView* v = [self getSimpleView:index];
	[v forward:index];
}

- (void)navigate:(NSString*)url tab:(int)index {
	SimpleMainView* v = [self getSimpleView:index];
	[v navigate:url tab:index];
}

- (void)navigateRedirect:(NSString*)url tab:(int)index {
	SimpleMainView* v = [self getSimpleView:index];
	[v navigateRedirect:url tab:index];
}

- (void)reload:(int)index {
	SimpleMainView* v = [self getSimpleView:index];
	[v reload:index];
}

- (void)executeJs:(NSString*)js tab:(int)index {
	SimpleMainView* v = [self getSimpleView:index];
	[v executeJs:js tab:index];
}

- (NSString*)currentLocation:(int)index {
	SimpleMainView* v = [self getSimpleView:index];
	return [v currentLocation:index];
}

- (void)switchTabCommand:(SimpleMainView*)new_v {
	int index = 0;
	int i;
	for (i = 0; i < [self.itemsData count]; i++) {
		if ([[self.itemsData objectAtIndex:i] view] == new_v) {
			index = i;
		}
	}
	SimpleMainView* cur_v = [self getSimpleView:tabindex];
	if (cur_v == new_v) {
		return;
	}
	tabindex = index;
    RhoRightItem *ri = [self.itemsData objectAtIndex:tabindex];
    if (!ri.loaded || ri.reload) {
        const char *s = [ri.url UTF8String];
        rho_rhodesapp_load_url(s);
        ri.loaded = YES;
    }
	CGRect myframe = self.view.bounds;
	new_v.view.frame = myframe;
	[cur_v.view removeFromSuperview];
	[self.view addSubview:new_v.view];
	[self.view setNeedsLayout];
	[self.view layoutSubviews];
	[self.view.superview setNeedsLayout];
	[new_v.view setNeedsDisplay];
}

- (void)switchTab:(int)index {
	SimpleMainView* new_v = [self getSimpleView:index];
	[self performSelectorOnMainThread:@selector(switchTabCommand:) withObject:new_v waitUntilDone:NO];	
}

- (int)activeTab {
	return tabindex;
}

- (void)addNavBar:(NSString*)title left:(NSArray*)left right:(NSArray*)right {
	SimpleMainView* v = [self getSimpleView:index];
	[v addNavBar:title left:left right:right];
}

- (void)removeNavBar {
	SimpleMainView* v = [self getSimpleView:index];
	[v removeNavBar];
}

- (UIWebView*)getWebView:(int)tab_index {
	SimpleMainView* v = [self getSimpleView:index];
	return [v getWebView:tab_index];
}



- (void)dealloc {
    [super dealloc];
}








@end
