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

#include "rhoruby.h"

#include "defs.h"
#include "Server.h"
#include "HttpContext.h"
#include "ServerHost.h"
#include "Dispatcher.h"
#include "AppManagerI.h"
#include "config.h"
#include "../rho/sync/syncthread.h"

extern char* get_current_location();

#pragma mark -
#pragma mark Constant Definitions

#define kServiceType	CFSTR("_http._tcp.")


#pragma mark -
#pragma mark Static Function Declarations

static void AcceptConnection(ServerRef server, CFSocketNativeHandle sock, CFStreamError* error, void* info);


/* static */ void
AcceptConnection(ServerRef server, CFSocketNativeHandle sock, CFStreamError* error, void* info) {
    
	if (sock == ((CFSocketNativeHandle)(-1))) {
        
		DBG(("AcceptConnection - Received an error (%d, %d)\n", (int)error->domain, (int)error->error));
		
		ServerInvalidate(server);
		ServerRelease(server);
		
		CFRunLoopStop(CFRunLoopGetCurrent());
	}
	else {
        
		HttpContextRef http = HttpContextCreate(NULL, sock);
		
		if ((http != NULL) && !HttpContextOpen(http))
			HttpContextRelease(http);
	}
}

#pragma mark -

static ServerHost* sharedSH = nil;

@implementation ServerHost

@synthesize actionTarget, onStartFailure, onStartSuccess, onRefreshView, onNavigateTo, onSetViewHomeUrl, onSetViewOptionsUrl, onTakePicture, onChoosePicture;

- (void)serverStarted:(NSString*)data {
	if(actionTarget && [actionTarget respondsToSelector:onStartSuccess]) {
		[actionTarget performSelector:onStartSuccess withObject:data];
	}
	// Do sync w/ remote DB 
	//wake_up_sync_engine();	
}

- (void)serverFailed:(void*)data {
	if(actionTarget && [actionTarget respondsToSelector:onStartFailure]) {
		[actionTarget performSelector:onStartFailure];
	}
}

- (void)refreshView {
	if(actionTarget && [actionTarget respondsToSelector:onRefreshView]) {
		[actionTarget performSelectorOnMainThread:onRefreshView withObject:NULL waitUntilDone:NO];
	}
}

- (void)navigateTo:(NSString*) url {
	if(actionTarget && [actionTarget respondsToSelector:onNavigateTo]) {
		[actionTarget performSelectorOnMainThread:onNavigateTo withObject:url waitUntilDone:NO];
	}
}

- (void)performRefreshView {
	[self performSelectorOnMainThread:@selector(refreshView)
						   withObject:NULL waitUntilDone:NO]; 
}

- (void)setViewHomeUrl:(NSString*)url {
	if(actionTarget && [actionTarget respondsToSelector:onSetViewHomeUrl]) {
		[actionTarget performSelector:onSetViewHomeUrl withObject:url];
	}	
}

- (void)takePicture:(NSString*) url {
	if(actionTarget && [actionTarget respondsToSelector:onTakePicture]) {
		[actionTarget performSelector:onTakePicture withObject:url];
	}
}

- (void)choosePicture:(NSString*) url {
	if(actionTarget && [actionTarget respondsToSelector:onChoosePicture]) {
		[actionTarget performSelector:onChoosePicture withObject:url];
	}
}

- (void)setViewOptionsUrl:(NSString*)url {
	if(actionTarget && [actionTarget respondsToSelector:onSetViewOptionsUrl]) {
		[actionTarget performSelector:onSetViewOptionsUrl withObject:url];
	}	
}

- (void)ServerHostThreadRoutine:(id)anObject {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
	DBG(("Initializing ruby\n"));
	RhoRubyStart();
	homeUrl = [NSString stringWithCString:config_getString("start_path") encoding:NSUTF8StringEncoding];
	optionsUrl = [NSString stringWithCString:config_getString("options_path") encoding:NSUTF8StringEncoding];
	DBG(("Start page: %s\n", [homeUrl UTF8String]));
	DBG(("Options page: %s\n", [optionsUrl UTF8String]));
	[[ServerHost sharedInstance] setViewHomeUrl:homeUrl];
	[[ServerHost sharedInstance] setViewOptionsUrl:optionsUrl];
	
    runLoop = CFRunLoopGetCurrent();
    ServerContext c = {NULL, NULL, NULL, NULL};
    ServerRef server = ServerCreate(NULL, AcceptConnection, &c);
	if (server != NULL && ServerConnect(server, NULL, kServiceType, 8080)) {
		DBG(("HTTP Server started and ready\n"));
		[self performSelectorOnMainThread:@selector(serverStarted:) 
							   withObject:homeUrl waitUntilDone:NO];
		
		DBG(("Create Sync"));
		rho_sync_create();
		
        [[NSRunLoop currentRunLoop] run];
        DBG(("Invalidating local server\n"));
        ServerInvalidate(server);
    } else {
        DBG(("Failed to start HTTP Server\n"));
		[self performSelectorOnMainThread:@selector(serverFailed:) 
							   withObject:NULL waitUntilDone:NO];
    }
	
	DBG(("Destroy Sync"));
	rho_sync_destroy();
	
	DBG(("Stopping ruby"));
	RhoRubyStop();
	
    DBG(("Server host thread routine is completed\n"));
    [pool release];
}
/*
- (int)initializeDatabaseConn {
    NSString *appRoot = [AppManager getApplicationsRootPath];
    NSString *path = [appRoot stringByAppendingPathComponent:@"../db/syncdb.sqlite"];
	return sqlite3_open([path UTF8String], &database);
}*/

extern void InitRhoLog(const char* szRootPath);
extern const char* RhoGetRootPath();

-(void) start {
	//Create 
	appManager = [AppManager instance]; 
	//Configure AppManager
	InitRhoLog(RhoGetRootPath());
	[appManager configure];
	//Init log and settings
	
	//Start Sync engine
	//[self initializeDatabaseConn];
	// Startup the sync engine thread
	//start_sync_engine(database);
	
	
	// Start server thread	
    [NSThread detachNewThreadSelector:@selector(ServerHostThreadRoutine:)
                             toTarget:self withObject:nil];
}

-(void) stop {
    CFRunLoopStop(runLoop);
	// Stop the sync engine
	//stop_sync_engine();
	//shutdown_database();
}

- (void)dealloc 
{
    [appManager release];
	[homeUrl release];
	[optionsUrl release];
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
void webview_refresh() {
	[[ServerHost sharedInstance] refreshView];
}

void webview_navigate(char* url) {
	[[ServerHost sharedInstance] navigateTo:[NSString stringWithCString:url]];
}

void perform_webview_refresh() {
	[[ServerHost sharedInstance] performRefreshView];														
}

char* webview_current_location() {
	return get_current_location();
}

void take_picture(char* callback_url) {
	[[ServerHost sharedInstance] takePicture:[NSString stringWithCString:callback_url]];		
}

void choose_picture(char* callback_url) {
	[[ServerHost sharedInstance] choosePicture:[NSString stringWithCString:callback_url]];		
}
