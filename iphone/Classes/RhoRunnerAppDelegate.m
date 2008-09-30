//
//  BrowserAppDelegate.m
//  Browser
//
//  Created by adam blum on 9/4/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import "RhoRunnerAppDelegate.h"
#import "WebViewController.h"

@implementation RhoRunnerAppDelegate

@synthesize window;
@synthesize webViewController;

- (void)applicationDidFinishLaunching:(UIApplication *)application {	
	
    //Create local server and start it
    [serverHost = [[ServerHost alloc] init] start];
    
    //Create View
	[window addSubview:webViewController.view];
    [window makeKeyAndVisible];
}

- (void)applicationWillTerminate:(UIApplication *)application {
    [serverHost stop];
    printf("Runner will terminate\n");
}

- (void)dealloc {
    [serverHost release];
	[webViewController release];
	[window release];
	[super dealloc];
}


@end
