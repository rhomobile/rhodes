/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#include "common/RhoPort.h"
#import "AppManager.h"
#import "AppLoader.h"
#import "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "AppLoader"

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
	RAWLOG_ERROR("AppLoader: Failed Load Application");
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
