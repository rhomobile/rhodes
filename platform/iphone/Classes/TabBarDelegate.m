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

@synthesize tabBarController, mainWindow, tabBar;

- (void)dealloc {
	[tabBarController release];
	[super dealloc];
}

- (void)loadTabBarItemFirstPage:(NSString*)location itemIndex:(int)index {
	NSString* escapedUrl = [location stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]; 
	escapedUrl = [escapedUrl stringByReplacingOccurrencesOfString: @"&" withString: @"%26"];
	NSString* startLocation = [@"http://localhost:8080/system/redirect_to?url=" stringByAppendingString:escapedUrl];
	UIWebView* wView = (UIWebView*)[[tabBarController.viewControllers objectAtIndex:index] view];
	[wView loadRequest:[NSURLRequest requestWithURL: [NSURL URLWithString:startLocation]]];
}

- (void)createTabBar:(UIWindow*)window {
	self.mainWindow = window;
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
			UIWebView *wView = [[[UIWebView alloc] init] autorelease];
			NSString *imagePath = [[AppManager getApplicationsRootPath] stringByAppendingPathComponent:itemImage];	
			NSLog(@"PATH: %@", imagePath);
			subController.title = itemLabel;
			subController.tabBarItem.image = [UIImage imageWithContentsOfFile:imagePath];
			subController.view = wView;
			[tabs addObject:subController];
			[subController release];
		}
	}
	self.tabBarController.viewControllers = tabs;
	self.tabBarController.customizableViewControllers = nil;
	self.tabBarController.view.hidden = NO;
	[self.mainWindow addSubview:tabBarController.view];
	[tabs release];
}

- (void)tabBarController:(UITabBarController *)tabBarController didSelectViewController:(UIViewController *)viewController {
	printf("selected index: %i\n", self.tabBarController.selectedIndex);
	[self loadTabBarItemFirstPage:(NSString*)[self.tabBar.barItems objectAtIndex:(self.tabBarController.selectedIndex*3)+1]
						itemIndex:self.tabBarController.selectedIndex];
}

@end
