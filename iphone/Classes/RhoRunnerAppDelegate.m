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

NSString *localhost = @"http://localhost:8080/";

- (void)onServerStarted:(NSString*)data {
	printf("Server Started notification is recived\n");
	NSString* location = [localhost stringByAppendingPathComponent:(NSString*)data];
	[webViewController navigate:location];
}

- (void)onRefreshView {
	[webViewController refresh];
}

- (void)onSetViewHomeUrl:(NSString *)url {
	[webViewController setViewHomeUrl:[localhost stringByAppendingPathComponent:url]];
}

- (void)applicationDidFinishLaunching:(UIApplication *)application {	

    //Create local server and start it
    //serverHost = [[ServerHost alloc] init];
	serverHost = [ServerHost sharedInstance];
	serverHost->actionTarget = self;
	serverHost->onStartSuccess = @selector(onServerStarted:);
	serverHost->onRefreshView = @selector(onRefreshView);
	serverHost->onSetViewHomeUrl = @selector(onSetViewHomeUrl:);
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
