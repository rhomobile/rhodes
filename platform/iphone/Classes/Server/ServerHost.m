//
//  ServerHost.m
//  Browser
//
//  Created by adam blum on 9/10/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#pragma mark Includes

#include <assert.h>
#include <unistd.h>

#include "common/RhoPort.h"
#include "common/rhoparams.h"
//#include "Server.h"
//#include "HttpContext.h"
#include "ServerHost.h"
//#include "Dispatcher.h"
#include "AppManagerI.h"
#include "common/RhoConf.h"
#include "logging/RhoLogConf.h"
//#include "sync/syncthread.h"
#include "common/RhodesApp.h"
#import "ParamsWrapper.h"
#import "DateTime.h"
#import "NativeBar.h"
#import "MapViewController.h"
#include "ruby/ext/rho/rhoruby.h"

#import "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "ServerHost"

extern void rho_geoimpl_init();

#pragma mark -
#pragma mark Constant Definitions

#define kServiceType	CFSTR("_http._tcp.")


#pragma mark -
#pragma mark Static Function Declarations

//static void AcceptConnection(ServerRef server, CFSocketNativeHandle sock, CFStreamError* error, void* info);


/* static */ //void
/*AcceptConnection(ServerRef server, CFSocketNativeHandle sock, CFStreamError* error, void* info) {
    
	if (sock == ((CFSocketNativeHandle)(-1))) {
        
		RAWLOG_INFO2("AcceptConnection - Received an error (%d, %d)", (int)error->domain, (int)error->error );
		
		ServerInvalidate(server);
		ServerRelease(server);
		
		CFRunLoopStop(CFRunLoopGetCurrent());
	}
	else {
        
		HttpContextRef http = HttpContextCreate(NULL, sock);
		
		if ((http != NULL) && !HttpContextOpen(http))
			HttpContextRelease(http);
	}
}*/

#pragma mark -

static ServerHost* sharedSH = nil;

@implementation ServerHost

@synthesize actionTarget, onTakePicture, onChoosePicture, onChooseDateTime, onCreateMap;

- (void)takePicture:(NSString*) url {
	if(actionTarget && [actionTarget respondsToSelector:onTakePicture]) {
		[actionTarget performSelectorOnMainThread:onTakePicture withObject:url waitUntilDone:NO];
	}
}

- (void)choosePicture:(NSString*) url {
	if(actionTarget && [actionTarget respondsToSelector:onChoosePicture]) {
		[actionTarget performSelectorOnMainThread:onChoosePicture withObject:url waitUntilDone:NO];
	}
}

- (void)chooseDateTime:(NSString*)url title:(NSString*)title initialTime:(long)initial_time format:(int)format data:(NSString*)data {
	if(actionTarget && [actionTarget respondsToSelector:onChooseDateTime]) {
		DateTime* dateTime = [[DateTime alloc] init];
		dateTime.url = url;
		dateTime.title = title;
		dateTime.initialTime = initial_time;
		dateTime.format = format;
		dateTime.data = data;
		[actionTarget performSelectorOnMainThread:onChooseDateTime withObject:dateTime waitUntilDone:YES];
		[dateTime release];
	}
}

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

//ruby extension hooks

void take_picture(char* callback_url) {
	[[ServerHost sharedInstance] takePicture:[NSString stringWithUTF8String:callback_url]];		
}

void choose_picture(char* callback_url) {
	[[ServerHost sharedInstance] choosePicture:[NSString stringWithUTF8String:callback_url]];		
}

void choose_datetime(char* callback, char* title, long initial_time, int format, char* data) {
	[[ServerHost sharedInstance] chooseDateTime:[NSString stringWithUTF8String:callback] 
										  title:[NSString stringWithUTF8String:title]
									initialTime:initial_time 
										 format:format
										   data:[NSString stringWithUTF8String:data]];
}


