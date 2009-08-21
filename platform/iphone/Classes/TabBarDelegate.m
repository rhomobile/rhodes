//
//  TabBarDelegate.m
//  rhorunner
//
//  Created by lars on 8/14/09.
//  Copyright 2009 Home. All rights reserved.
//

#import "TabBarDelegate.h"
#import "WebViewController.h"


@implementation TabBarDelegate

@synthesize tabBar, tabBarController, mainWindow;

- (void)dealloc {
	[tabBar release];
	[tabBarController release];
	[super dealloc];
}

- (void)loadTabBarItemFirstPage:(NSString*)location {
	NSString* escapedUrl = [location stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]; 
	escapedUrl = [escapedUrl stringByReplacingOccurrencesOfString: @"&" withString: @"%26"];
	NSString* startLocation = [@"http://localhost:8080/system/redirect_to?url=" stringByAppendingString:escapedUrl];
	UIWebView* currentWebView = (UIWebView*)[[tabBarController.viewControllers objectAtIndex:0] view];
	
	[currentWebView loadRequest:[NSURLRequest requestWithURL: [NSURL URLWithString:startLocation]]];
	[self.mainWindow addSubview:currentWebView];
}

- (void)createTabBar:(UIWindow*)window {
	self.mainWindow = window;
	CGRect mainViewBounds = [[UIScreen mainScreen] applicationFrame];
	
	// Load the tab bar (just one of them)
	if(!self.tabBarController) {
		self.tabBarController = [[UITabBarController alloc] initWithNibName:nil bundle:nil];
		self.tabBarController.delegate = self;
	}
	
	tabBarController.moreNavigationController.navigationBar.barStyle = UIBarStyleBlackOpaque;
	NSMutableArray *tabs = [[NSMutableArray alloc] init];
	int barSize = [tabBar.barItems count] / 3;
	
	// Setup each tabbar item with elements in specific order:
	// label, action, icon
	for(int i=0; i < barSize; i++) {
		NSString* itemLabel = (NSString*)[tabBar.barItems objectAtIndex:i*3];
		NSString* itemLocation = (NSString*)[tabBar.barItems objectAtIndex:(i*3)+1];
		NSString* itemImage = (NSString*)[tabBar.barItems objectAtIndex:(i*3)+2];
		if (itemLabel && itemLocation && itemImage) {
			UIViewController *subController = [[WebViewController alloc] initWithNibName:nil bundle:nil];
			UIWebView *wView = [[[UIWebView alloc] initWithFrame:mainViewBounds] autorelease];
			subController.title = itemLabel;
			subController.tabBarItem.image = [UIImage imageNamed:@"home_btn.png"];
			subController.view = wView;
			[tabs addObject:subController];
			[subController release];
		}
	}
	self.tabBarController.viewControllers = tabs;
	self.tabBarController.customizableViewControllers = nil;
	self.tabBarController.view.hidden = NO;
	[self.mainWindow addSubview:tabBarController.view];
	
	// Load location for tab #0
	[self loadTabBarItemFirstPage:(NSString*)[tabBar.barItems objectAtIndex:1]];
	[tabs release];
}

- (void)tabBarController:(UITabBarController *)tabBarController didSelectViewController:(UIViewController *)viewController {
	printf("selected index: %i\n", self.tabBarController.selectedIndex);
	[self loadTabBarItemFirstPage:(NSString*)[self.tabBar.barItems objectAtIndex:(self.tabBarController.selectedIndex*3)+1]];
}

@end
