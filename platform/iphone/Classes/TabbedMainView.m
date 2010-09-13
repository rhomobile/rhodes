    //
//  TabbedMainView.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 26.03.10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "TabbedMainView.h"
#import "SimpleMainView.h"
#import "Rhodes.h"
#import "AppManager.h"
#import "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "TabbedMainView"

@interface RhoTabBarData : NSObject {
@public
    NSString *url;
    BOOL loaded;
    BOOL reload;
}

@property (retain) NSString *url;
@property (assign) BOOL loaded;
@property (assign) BOOL reload;

- (id)init;
- (void)dealloc;

@end

@implementation RhoTabBarData

@synthesize url, loaded, reload;

- (id)init {
    url = nil;
    loaded = NO;
    reload = NO;
    return self;
}

- (void)dealloc {
    [url release];
    [super dealloc];
}

@end


@implementation TabbedMainView

@synthesize tabbar, tabbarData, tabindex;

- (id)initWithMainView:(id<RhoMainView>)v parent:(UIWindow*)p tabs:(NSArray *)items {
    CGRect frame = [[v view] frame];
    
    tabbar = [[UITabBarController alloc] initWithNibName:nil bundle:nil];
    tabbar.delegate = [Rhodes sharedInstance];
    tabbar.view.frame = frame;
    tabbar.selectedIndex = 0;
    
    CGRect childFrame = frame;
    CGRect tbFrame = tabbar.tabBar.frame;
    childFrame.size.height -= tbFrame.size.height;
    
    int count = [items count]/4;
    NSMutableArray *views = [NSMutableArray arrayWithCapacity:count];
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
            RhoTabBarData *td = [[RhoTabBarData alloc] init];
            td.url = url;
            td.reload = [reload isEqualToString:@"true"];
            
            SimpleMainView *subController = [[SimpleMainView alloc] initWithParentView:tabbar.view frame:childFrame];
            subController.title = label;
            NSString *imagePath = [[AppManager getApplicationsRootPath] stringByAppendingPathComponent:icon];
            subController.tabBarItem.image = [UIImage imageWithContentsOfFile:imagePath];
            //[subController navigateRedirect:url tab:0];
            
            [tabs addObject:td];
            [views addObject:subController];
            
            [td release];
            [subController release];
        }
    }
    tabbar.viewControllers = views;
    tabbar.customizableViewControllers = NO;
    tabbar.view.hidden = NO;
    
    self.tabbarData = tabs;
    [tabs release];
    
    if (initUrl)
        [self navigateRedirect:initUrl tab:0];
    
    return self;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
}

- (void)dealloc {
    //[tabbar.view removeFromSuperview];
    [tabbarData release];
	[tabbar release];
	tabbar = nil;
    [super dealloc];
}

- (SimpleMainView*)subView:(int)index {
    return (SimpleMainView*)[tabbar.viewControllers objectAtIndex:index];
}

- (RhoTabBarData*)tabData:(int)index {
    return (RhoTabBarData*)[tabbarData objectAtIndex:index];
}

// RhoMainView implementation

- (UIView*)view {
    return tabbar.view;
}

- (UIWebView*)detachWebView {
    int n = [self activeTab];
    return [[self subView:n] detachWebView];
}

- (void)loadHTMLString:(NSString *)data {
    [[self subView:[self activeTab]] loadHTMLString:data];
}

- (void)back:(int)index {
    [[self subView:index] back:0];
}

- (void)forward:(int)index {
    [[self subView:index] forward:0];
}

- (void)navigate:(NSString *)url tab:(int)index {
    [[self subView:index] navigate:url tab:0];
}

- (void)navigateRedirect:(NSString *)url tab:(int)index {
    [[self subView:index] navigateRedirect:url tab:0];
}

- (void)reload:(int)index {
    [[self subView:index] reload:0];
}

- (void)executeJs:(NSString*)js tab:(int)index {
    RAWLOG_INFO1("Executing JS: %s", [js UTF8String]);
    [[self subView:index] executeJs:js tab:0];
}

- (NSString*)currentLocation:(int)index {
    return [[self subView:index] currentLocation:0];
}

- (void)switchTab:(int)index {
    tabindex = index;
    tabbar.selectedIndex = tabindex;
}

- (void)onSwitchTab {
    tabindex = tabbar.selectedIndex;
    RhoTabBarData *td = [self tabData:tabindex];
    if (!td.loaded || td.reload) {
        [self navigateRedirect:td.url tab:tabindex];
        td.loaded = YES;
    }
}

- (int)activeTab {
    return tabindex;
}

- (void)addNavBar:(NSString*)title left:(NSArray*)left right:(NSArray*)right {
    [[self subView:[self activeTab]] addNavBar:title left:left right:right];
}

- (void)removeNavBar {
    [[self subView:[self activeTab]] removeNavBar];
}

@end
