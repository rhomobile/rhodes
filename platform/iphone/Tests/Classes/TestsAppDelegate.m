//
//  TestsAppDelegate.m
//  Tests
//
//  Created by evgeny vovchenko on 5/28/09.
//  Copyright RhoMobile 2009. All rights reserved.
//

#import "TestsAppDelegate.h"

@implementation TestsAppDelegate

@synthesize window;


- (void)applicationDidFinishLaunching:(UIApplication *)application {    

    // Override point for customization after application launch
    [window makeKeyAndVisible];
}


- (void)dealloc {
    [window release];
    [super dealloc];
}


@end

const char* RhoGetRootPath() {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	static bool loaded = FALSE;
	static char root[FILENAME_MAX];
	if (!loaded){
		NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
		NSString *documentsDirectory = //[paths objectAtIndex:0];
		[ [paths objectAtIndex:0] stringByAppendingString:@"/"];
		[documentsDirectory getFileSystemRepresentation:root maxLength:sizeof(root)];
		
		loaded = TRUE;
	}
	[pool drain];
	[pool release];
	
	return root;
}

void rhoSleep( int ms )
{
	[NSThread sleepForTimeInterval: ((float)ms)/1000];
}
