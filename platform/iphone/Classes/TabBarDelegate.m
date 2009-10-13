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

@implementation TabBarDelegate

@synthesize tabBarController, mainWindow, tabBar, barItems, activeTab;

- (void)dealloc {
	[tabBarController release];
	[barItems release];
	[tabBar release];
	[super dealloc];
}

- (void)loadTabBarItemFirstPage:(BarItem*)item {
	if (item.loaded == NO || item.reload == YES) {
		NSString* escapedUrl = [item.location stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]; 
		escapedUrl = [escapedUrl stringByReplacingOccurrencesOfString: @"&" withString: @"%26"];
		NSString* startLocation = [@"http://localhost:8080/system/redirect_to?url=" stringByAppendingString:escapedUrl];
		[(UIWebView*)[item.viewController view] loadRequest:[NSURLRequest requestWithURL: [NSURL URLWithString:startLocation]]];
		item.loaded = YES;
	}
}

- (void)loadTabBarItemLocation:(BarItem*)item url:(NSString*)url {
	[item.viewController navigateRedirect:url];
}

- (void)createTabBar:(UIWindow*)window {
	self.mainWindow = window;
	// Load the tab bar (just one of them)
	if(!self.tabBarController) {
		self.tabBarController = [[UITabBarController alloc] initWithNibName:nil bundle:nil];
		self.tabBarController.delegate = self;
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
		item.label = (NSString*)[tabBar.barItemDataArray objectAtIndex:i*4];
		item.location = (NSString*)[tabBar.barItemDataArray objectAtIndex:(i*4)+1];
		item.icon = (NSString*)[tabBar.barItemDataArray objectAtIndex:(i*4)+2];
		item.reload = [(NSString*)[tabBar.barItemDataArray objectAtIndex:(i*4)+3] isEqualToString:@"true"] ? YES : NO;
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

- (void)tabBarController:(UITabBarController *)tabBarController didSelectViewController:(UIViewController *)viewController {
	if(self.tabBarController.selectedIndex > barItems.count) {
		[NSException raise:@"Exception" format:@"Rhodes currently only supports up to 5 tabs.  Please change your tabs array and try again."];
	} else {
		[self loadTabBarItemFirstPage:(BarItem*)[barItems objectAtIndex:self.tabBarController.selectedIndex]];
	}
	self.activeTab = self.tabBarController.selectedIndex;
}

@end
