//
//  ServerHost.m
//  Browser
//
//  Created by adam blum on 9/10/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

//#include <assert.h>
//#include <unistd.h>

#include "ServerHost.h"

#import "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "ServerHost"

extern void rho_geoimpl_init();

static ServerHost* sharedSH = nil;

@implementation ServerHost

- (void)ServerHostThreadRoutine:(id)anObject {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	runLoop = CFRunLoopGetCurrent();
	m_geoThread = [NSThread currentThread];
	rho_geoimpl_init();
    [self performSelectorOnMainThread:@selector(serverStarted:) 
                           withObject:NULL waitUntilDone:NO];
	[[NSRunLoop currentRunLoop] run];
	
    RAWLOG_INFO("Server host thread routine is completed");
	[pool release];
}

- (void)dealloc 
{
    [appManager release];
	//[homeUrl release];
	//[optionsUrl release];
	[super dealloc];
}

+ (ServerHost *)sharedInstance {
    @synchronized(self) {
        if (sharedSH == nil) {
            [[self alloc] init]; // assignment not done here
        }
    }
    return sharedSH;
}

+ (id)allocWithZone:(NSZone *)zone {
    @synchronized(self) {
        if (sharedSH == nil) {
            sharedSH = [super allocWithZone:zone];
            return sharedSH;  // assignment and return on first allocation
        }
    }
    return nil; // on subsequent allocation attempts return nil
}

- (id)copyWithZone:(NSZone *)zone
{
    return self;
}

- (id)retain {
    return self;
}

- (unsigned)retainCount {
    return UINT_MAX;  // denotes an object that cannot be released
}

- (void)release {
    //do nothing
}

- (id)autorelease {
    return self;
}

@end
