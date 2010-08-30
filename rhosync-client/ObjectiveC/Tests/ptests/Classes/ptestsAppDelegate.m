//
//  ptestsAppDelegate.m
//  ptests
//
//  Created by Vlad on 8/25/10.
//  Copyright __MyCompanyName__ 2010. All rights reserved.
//

#import "ptestsAppDelegate.h"
#import "ptestsViewController.h"

@implementation ptestsAppDelegate

@synthesize window;
@synthesize viewController;


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    
    // Override point for customization after app launch    
    [window addSubview:viewController.view];
    [window makeKeyAndVisible];
	
	return YES;
}


- (void)dealloc {
    [viewController release];
    [window release];
    [super dealloc];
}


@end
