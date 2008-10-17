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

- (void)onServerStarted {
	printf("Server Started notification is recived\n");
	[webViewController navigate:@"http://localhost:8080/"];
	// Do sync w/ remote DB 
	wake_up_sync_engine();
}

- (int)initializeDatabaseConn {
    NSString *appRoot = [AppManager getApplicationsRootPath];
    NSString *path = [appRoot stringByAppendingPathComponent:@"lib/syncdb.sqlite"];
	return sqlite3_open([path UTF8String], &database);
}

- (void)applicationDidFinishLaunching:(UIApplication *)application {	
	//Start Sync engine
	[self initializeDatabaseConn];
	// Startup the sync engine thread
	start_sync_engine(database);
	
    //Create local server and start it
    serverHost = [[ServerHost alloc] init];
	serverHost->actionTarget = self;
	serverHost->onStartSuccess = @selector(onServerStarted);
    [serverHost start];
	
    //Create View
	[window addSubview:webViewController.view];
    [window makeKeyAndVisible];
}

- (void)applicationWillTerminate:(UIApplication *)application {
    DBG(("Runner will terminate\n"));
	//Stop HTTP server host 
    [serverHost stop];
	// Stop the sync engine
	stop_sync_engine();

}

- (void)dealloc {
    [serverHost release];
	[webViewController release];
	[window release];
	[super dealloc];
}


@end
