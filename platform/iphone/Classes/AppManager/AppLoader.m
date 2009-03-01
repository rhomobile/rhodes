//
//  AppLoader.m
//  rhorunner
//
//  Created by vlad on 9/24/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "defs.h"
#import "AppManager.h"
#import "AppLoader.h"

@implementation AppLoader

@synthesize actionTarget, onLoadSuccess, onLoadFailure;

- (void) performAsyncLoadWithURL:(NSURL*)url {
	NSAutoreleasePool * pool =[[NSAutoreleasePool alloc] init];
	
	NSError* loadError = nil;
	NSData*  appData = [NSData dataWithContentsOfURL:url options:NSMappedRead error:&loadError];
	
	if(appData) {
		[self performSelectorOnMainThread:@selector(loadDidFinishWithData:)
			withObject:appData waitUntilDone:YES];
	} else {
		[self performSelectorOnMainThread:@selector(loadDidFinishWithError:)
			withObject:loadError waitUntilDone:YES];
	}
	
	[pool release]; // appData will be released here
}

- (void)loadDidFinishWithData:(NSData*)appData {	
	isLoading = NO;
	//Install loaded application on the file system
	//TODO: add error handling
	[AppManager installApplication:appName data:appData];	
	
	//[loadUrl release]; 
	//[appName release];

	if(actionTarget && [actionTarget respondsToSelector:onLoadSuccess]) {
		[actionTarget performSelector:onLoadSuccess];
	}
}

- (void)loadDidFinishWithError:(NSError*)error {
	isLoading = NO;
	DBG(("AppLoader: Failed Load Application\n"));
	//[loadUrl release];
	//[appName release];
	
	if(actionTarget && [actionTarget respondsToSelector:onLoadFailure]) {
		[actionTarget performSelector:onLoadFailure];
	}
}

- (bool) loadApplication:(NSString*)applicationName {
	appName = [applicationName copy];
	loadUrl = [NSString stringWithFormat: @"%@%@%@", [AppManager getApplicationsRosterUrl], appName, @".zip"];
	
	NSError* loadError = nil;
	NSData*  appData = [NSData dataWithContentsOfURL:[NSURL URLWithString:loadUrl]
										options:NSMappedRead error:&loadError];
		
	if(appData) {
		return [AppManager installApplication:appName data:appData];
	} 
	
	return false;
}

- (bool) loadApplicationAsync:(NSString*)applicationName {
	if(isLoading) {
		return false;
	}
	isLoading = YES;
	appName = [applicationName copy];
	loadUrl = [NSString stringWithFormat: @"%@%@%@", [AppManager getApplicationsRosterUrl], appName, @".zip"];
	
	[NSThread detachNewThreadSelector:@selector(performAsyncLoadWithURL:) 
							 toTarget:self withObject:[NSURL URLWithString:loadUrl]];
	
	return true;
}

- (void)dealloc {
	//[loadUrl release];
	//[appName release];
	[super dealloc];
}

@end
