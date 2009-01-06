//
//  BrowserAppDelegate.m
//  Browser
//
//  Created by adam blum on 9/4/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//
#import "defs.h"
#import "RhoRunnerAppDelegate.h"
#import "WebViewController.h"
#import "AppManager.h"

@implementation RhoRunnerAppDelegate

@synthesize window;
@synthesize webViewController;

- (void)onServerStarted:(NSString*)data {
	printf("Server Started notification is recived\n");
	NSString* location = [@"http://localhost:8080/" stringByAppendingPathComponent:(NSString*)data];
	[webViewController navigate:location];
}

- (void)applicationDidFinishLaunching:(UIApplication *)application {	

    //Create local server and start it
    serverHost = [[ServerHost alloc] init];
	serverHost->actionTarget = self;
	serverHost->onStartSuccess = @selector(onServerStarted:);
    [serverHost start];
	
    //Create View
	[window addSubview:webViewController.view];
    [window makeKeyAndVisible];
}

- (void)applicationWillTerminate:(UIApplication *)application {
    DBG(("Runner will terminate\n"));
	//Stop HTTP server host 
    [serverHost stop];
}

- (void)dealloc {
    [serverHost release];
	[webViewController release];
	[window release];
	[super dealloc];
}


@end
