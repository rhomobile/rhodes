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

@implementation ServerHost

@synthesize actionTarget, onStartFailure, onStartSuccess;

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

- (void)ServerHostThreadRoutine:(id)anObject {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
	DBG(("Initializing ruby\n"));
	RhoRubyStart();
	char* start_page = callGetStartPage();
	DBG(("Start page: %s\n", start_page));
	NSString* ref_start_page = [NSString stringWithCString:start_page encoding:NSUTF8StringEncoding];
	
    runLoop = CFRunLoopGetCurrent();
    ServerContext c = {NULL, NULL, NULL, NULL};
    ServerRef server = ServerCreate(NULL, AcceptConnection, &c);
	if (server != NULL && ServerConnect(server, NULL, kServiceType, 8080)) {
		DBG(("HTTP Server started and ready\n"));
		[self performSelectorOnMainThread:@selector(serverStarted:) 
							   withObject:ref_start_page waitUntilDone:NO];
        [[NSRunLoop currentRunLoop] run];
        DBG(("Invalidating local server\n"));
        ServerInvalidate(server);
    } else {
        DBG(("Failed to start HTTP Server\n"));
		[self performSelectorOnMainThread:@selector(serverFailed:) 
							   withObject:NULL waitUntilDone:NO];
    }
    
	[ref_start_page release];
	
	DBG(("Stopping ruby"));
	RhoRubyStop();
	
    DBG(("Server host thread routine is completed\n"));
    [pool release];
}

- (int)initializeDatabaseConn {
    NSString *appRoot = [AppManager getApplicationsRootPath];
    NSString *path = [appRoot stringByAppendingPathComponent:@"../db/syncdb.sqlite"];
	return sqlite3_open([path UTF8String], &database);
}

-(void) start {
	//Create and configure AppManager
	appManager = [AppManager instance]; 
	[appManager configure];

	//Start Sync engine
	[self initializeDatabaseConn];
	// Startup the sync engine thread
	start_sync_engine(database);
	
	
	// Start server thread	
    [NSThread detachNewThreadSelector:@selector(ServerHostThreadRoutine:)
                             toTarget:self withObject:nil];
}

-(void) stop {
    CFRunLoopStop(runLoop);
	// Stop the sync engine
	stop_sync_engine();
	shutdown_database();
}

- (void)dealloc 
{
    [appManager release];
	[super dealloc];
}

@end
