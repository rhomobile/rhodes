//
//  TabBarDelegate.m
//  rhorunner
//
//  Created by lars on 8/14/09.
//  Copyright 2009 Home. All rights reserved.
//

#import "TabBarDelegate.h"
#import "WebViewController.h"
#import "AppManager.h"

#import "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "TabBarDelegate"

@implementation TabBarDelegate

@synthesize tabBarController, mainWindow, tabBar, barItems, activeTab;

- (id)init {
    [super init];
    tabBarController.selectedIndex = 0;
    return self;
}

- (void)dealloc {
	[tabBarController release];
	[barItems release];
	[tabBar release];
	[super dealloc];
}

- (int)getSelectedIndex {
    int index = tabBarController.selectedIndex;
    int count = [barItems count];
    if (index >= count) index = count - 1;
    if (index < 0) index = 0;
    tabBarController.selectedIndex = index;
    return index;
}

- (void)loadTabBarItemFirstPage:(BarItem*)item {
	if (item.loaded == NO || item.reload == YES) {
        [item.viewController navigateRedirect:item.location];
		item.loaded = YES;
	}
}

- (void)loadTabBarItemLocation:(BarItem*)item url:(NSString*)url {
    int index = [self getSelectedIndex];
    BarItem *activeBar = (BarItem*)[barItems objectAtIndex:index];
    if (activeBar == item || item.loaded == YES)
        [item.viewController navigateRedirect:url];
    else
        item.location = url;
}

- (void)refresh:(BarItem*)item {
    int index = [self getSelectedIndex];
    BarItem *activeBar = (BarItem*)[barItems objectAtIndex:index];
    if (activeBar == item || item.loaded == YES)
        [item.viewController refresh];
    else
        item.reload = YES;
}

- (void)executeJs:(BarItem*)item js:(JSString*)js {
    [item.viewController executeJs:js];
}

- (void)createTabBar:(UIWindow*)window {
	self.mainWindow = window;
	// Load the tab bar (just one of them)
	if(!self.tabBarController) {
		self.tabBarController = [[UITabBarController alloc] initWithNibName:nil bundle:nil];
		self.tabBarController.delegate = self;
        self.tabBarController.selectedIndex = 0;
	}
	
	tabBarController.moreNavigationController.navigationBar.barStyle = UIBarStyleBlackOpaque;
	int barSize = [tabBar.barItemDataArray count] / 4;
	NSMutableArray *tabs = [NSMutableArray arrayWithCapacity:barSize];
	
	if(!self.barItems) {
		self.barItems = [NSMutableArray arrayWithCapacity:barSize];
	}
	
	// Setup each tabbar item with elements in specific order:
	// label, action, icon, reload
	for(int i=0; i < barSize; i++) {
		BarItem* item = [[BarItem alloc] init];
        int index = i*4;
		item.label = (NSString*)[tabBar.barItemDataArray objectAtIndex:index++];
		item.location = (NSString*)[tabBar.barItemDataArray objectAtIndex:index++];
		item.icon = (NSString*)[tabBar.barItemDataArray objectAtIndex:index++];
		item.reload = [(NSString*)[tabBar.barItemDataArray objectAtIndex:index++] isEqualToString:@"true"] ? YES : NO;
		if (item.label && item.location && item.icon) {
			WebViewController *subController = [[WebViewController alloc] initWithNibName:nil bundle:nil];
			UIWebView *wView = [[UIWebView alloc] init];
			NSString *imagePath = [[AppManager getApplicationsRootPath] stringByAppendingPathComponent:item.icon];	
			subController.title = item.label;
			subController.tabBarItem.image = [UIImage imageWithContentsOfFile:imagePath];
			//TODO: Figure out why view and webView need to point to the same object!
			subController.view = wView;
			subController.webView = wView;
			item.viewController = subController;
			[barItems addObject:item];
			[tabs addObject:subController];
			[item release];
			[subController release];
		}
	}
	self.tabBarController.viewControllers = tabs;
	self.tabBarController.customizableViewControllers = nil;
	self.tabBarController.view.hidden = NO;
	[self.mainWindow addSubview:tabBarController.view];
}

- (void)deleteTabBar {
    [barItems release];
    barItems = nil;
    [tabBarController.view removeFromSuperview];
    [tabBarController release];
    tabBarController = nil;
}

- (void)tabBarController:(UITabBarController *)tabBarController didSelectViewController:(UIViewController *)viewController {
	if(self.tabBarController.selectedIndex > barItems.count) {
		[NSException raise:@"Exception" format:@"Rhodes currently only supports up to 5 tabs.  Please change your tabs array and try again."];
	} else {
        int index = [self getSelectedIndex];
		[self loadTabBarItemFirstPage:(BarItem*)[barItems objectAtIndex:index]];
	}
	self.activeTab = self.tabBarController.selectedIndex;
}

- (void)switchTab:(int)index {
    self.tabBarController.selectedIndex = index;
    self.tabBarController.selectedIndex = [self getSelectedIndex];
    self.activeTab = self.tabBarController.selectedIndex;
}

@end
