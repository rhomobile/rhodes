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

@synthesize actionTarget, /*onStartFailure,*/ onStartSuccess, onRefreshView, onNavigateTo, onExecuteJs; 
@synthesize /*onSetViewHomeUrl, onSetViewOptionsUrl,*/ onTakePicture, onChoosePicture, onChooseDateTime;
@synthesize onCreateNativeBar, onRemoveNativeBar, onSwitchTab;
@synthesize onShowPopup, onVibrate, onPlayFile, onSysCall, onMapLocation, onCreateMap, onActiveTab, onShowLog;

- (void)serverStarted:(NSString*)data {
	if(actionTarget && [actionTarget respondsToSelector:onStartSuccess]) {
		[actionTarget performSelector:onStartSuccess withObject:data];
	}
	// Do sync w/ remote DB 
	//wake_up_sync_engine();	
}
/*
- (void)serverFailed:(void*)data {
	if(actionTarget && [actionTarget respondsToSelector:onStartFailure]) {
		[actionTarget performSelector:onStartFailure];
	}
}*/

/*
- (void)setViewHomeUrl:(NSString*)url {
	if(actionTarget && [actionTarget respondsToSelector:onSetViewHomeUrl]) {
		[actionTarget performSelector:onSetViewHomeUrl withObject:url];
	}	
}*/

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

/*
- (void)createNativeBar:(int)barType dataArray:(NSArray*)dataArray {
	if(actionTarget && [actionTarget respondsToSelector:onCreateNativeBar]) {
		NativeBar* nativeBar = [[NativeBar alloc] init];
		nativeBar.barType = barType;
		nativeBar.barItemDataArray = dataArray;
		[actionTarget performSelectorOnMainThread:onCreateNativeBar withObject:nativeBar waitUntilDone:YES];
		[nativeBar release];
	}
}

- (void)removeNativeBar {
    if (actionTarget && [actionTarget respondsToSelector:onRemoveNativeBar]) {
        [actionTarget performSelectorOnMainThread:onRemoveNativeBar withObject:nil waitUntilDone:YES];
    }
}

- (void)switchTab:(int)index {
    if (actionTarget && [actionTarget respondsToSelector:onSwitchTab]) {
        NSValue* value = [NSValue valueWithPointer:&index];
        [actionTarget performSelectorOnMainThread:onSwitchTab withObject:value waitUntilDone:YES];
        [value release];
    }
}
*/

- (void)showLog {
    if (actionTarget && [actionTarget respondsToSelector:onShowLog]) {
        [actionTarget performSelectorOnMainThread:onShowLog withObject:nil waitUntilDone:NO];
    }
}

/*
- (void)setViewOptionsUrl:(NSString*)url {
	if(actionTarget && [actionTarget respondsToSelector:onSetViewOptionsUrl]) {
		[actionTarget performSelector:onSetViewOptionsUrl withObject:url];
	}	
}*/

- (void)showPopup:(NSString*) message {
	if(actionTarget && [actionTarget respondsToSelector:onShowPopup]) {
		[actionTarget performSelectorOnMainThread:onShowPopup withObject:message waitUntilDone:NO];
	}
}

- (void)vibrate:(int) duration {
	if(actionTarget && [actionTarget respondsToSelector:onVibrate]) {
		[actionTarget performSelectorOnMainThread:onVibrate withObject:(void*)duration waitUntilDone:NO];
	}
}

- (void)playFile:(NSString*) fileName mediaType:(NSString*) media_type {
	if(actionTarget && [actionTarget respondsToSelector:onPlayFile]) {
		[actionTarget performSelectorOnMainThread:onPlayFile withObject:fileName waitUntilDone:NO];
	}
}

- (void)mapLocation:(NSString*) query {
	if(actionTarget && [actionTarget respondsToSelector:onMapLocation]) {
		[actionTarget performSelectorOnMainThread:onMapLocation withObject:query waitUntilDone:NO];
	}
}

- (void)createMap:(rho_param*)p {
	if(actionTarget && [actionTarget respondsToSelector:onCreateMap]) {
        NSValue *value = [NSValue valueWithPointer:p];
        if (!value) return;
		[actionTarget performSelectorOnMainThread:onCreateMap withObject:value waitUntilDone:NO];
	}
}

- (int)activeTab {
	int retval = 0;
	if(actionTarget && [actionTarget respondsToSelector:onActiveTab]) {
		NSValue* result = [NSValue valueWithPointer: &retval];
		if (!result) return 0;
		[actionTarget performSelectorOnMainThread:onActiveTab withObject:result waitUntilDone:YES];
	}
	return retval;
}

- (void)sysCall:(PARAMS_WRAPPER*)params {
}

- (void)doSysCall:(PARAMS_WRAPPER*)params {
	ParamsWrapper* pw = [ParamsWrapper wrap:params]; 	
	if(actionTarget && [actionTarget respondsToSelector:onSysCall]) {
		[actionTarget performSelectorOnMainThread:onSysCall withObject:pw waitUntilDone:NO];
	}
}

#if defined(RHO_USE_OWN_HTTPD) && !defined(RHO_HTTPD_COMMON_IMPL)
- (void)ServerHostThreadRoutine:(id)anObject {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	runLoop = CFRunLoopGetCurrent();
	m_geoThread = [NSThread currentThread];
	geo_init();
	
	RAWLOG_INFO("Initializing ruby");
	RhoRubyStart();
	
    ServerContext c = {NULL, NULL, NULL, NULL};
    ServerRef server = ServerCreate(NULL, AcceptConnection, &c);
	if (server != NULL && ServerConnect(server, NULL, kServiceType, 8080)) {
		RAWLOG_INFO("HTTP Server started and ready");
		
		RAWLOG_INFO("Create Sync");
		rho_sync_create();
		RhoRubyInitApp();
		rho_ruby_activateApp();
		
		[self performSelectorOnMainThread:@selector(serverStarted:) 
							   withObject:NULL waitUntilDone:NO];
		
        [[NSRunLoop currentRunLoop] run];
	
	
	    RAWLOG_INFO("Invalidating local server");
        ServerInvalidate(server);
    } else {
        RAWLOG_INFO("Failed to start HTTP Server");
		[self performSelectorOnMainThread:@selector(serverFailed:) 
							   withObject:NULL waitUntilDone:NO];
    }
	
	RAWLOG_INFO("Destroy Sync");
	rho_sync_destroy();
	
	RAWLOG_INFO("Stopping ruby");
	RhoRubyStop();
	
    RAWLOG_INFO("Server host thread routine is completed");
	[pool release];
}
#else // defined(RHO_USE_OWN_HTTPD) && !defined(RHO_HTTPD_COMMON_IMPL)
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
#endif // defined(RHO_USE_OWN_HTTPD) && !defined(RHO_HTTPD_COMMON_IMPL)

/*
- (int)initializeDatabaseConn {
    NSString *appRoot = [AppManager getApplicationsRootPath];
    NSString *path = [appRoot stringByAppendingPathComponent:@"../db/syncdb.sqlite"];
	return sqlite3_open([path UTF8String], &database);
}*/

//extern const char* RhoGetRootPath();

-(void) create {
	//Create 
	appManager = [AppManager instance]; 
	//Configure AppManager
	[appManager configure];
	//Init log and settings
	
	//Start Sync engine
	//[self initializeDatabaseConn];
	// Startup the sync engine thread
	//start_sync_engine(database);
	
	// Start server thread	
    [NSThread detachNewThreadSelector:@selector(ServerHostThreadRoutine:)
                             toTarget:self withObject:nil];
	rho_rhodesapp_create(rho_native_rhopath());	
}

-(void)start {
#if !defined(RHO_USE_OWN_HTTPD) || defined(RHO_HTTPD_COMMON_IMPL)
	rho_rhodesapp_start();
#endif
}

void* rho_nativethread_start()
{
	return [[NSAutoreleasePool alloc] init];
}

void rho_nativethread_end(void* pData)
{
    NSAutoreleasePool *pool = (NSAutoreleasePool *)pData;	
    [pool release];	
}

-(void) stop {
	rho_rhodesapp_destroy();
	CFRunLoopStop(runLoop);
	// Stop the sync engine
	//stop_sync_engine();
	//shutdown_database();
}
/*
//Sync all sources
- (void) doSync {
	rho_sync_doSyncAllSources(TRUE);
}

- (void) doSyncFor:(NSString*)url {
	rho_sync_doSyncSourceByUrl([url cStringUsingEncoding:[NSString defaultCStringEncoding]]);
}*/

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

void rho_conf_show_log() {
    [[ServerHost sharedInstance] showLog];
}

void Init_RingtoneManager()
{
	//TODO: implement Ringtone
}

void alert_show_popup(char* message) {
	if (message==NULL) {
		RAWLOG_ERROR("Alert.show_popup - wrong arguments");
	} else {
		[[ServerHost sharedInstance] showPopup:[NSString stringWithUTF8String:message]];
	}
}

void alert_vibrate(int duration) {
	[[ServerHost sharedInstance] vibrate:duration];
}

void alert_play_file(char* file_name, char* media_type) {
	if (file_name==NULL) {
		RAWLOG_ERROR("Alert.play_file - please specify file name to play");
	} else {
		[[ServerHost sharedInstance] playFile:[NSString stringWithUTF8String:file_name] 
								mediaType:media_type?[NSString stringWithUTF8String:media_type]:NULL];
	}
}

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

void rho_map_location(char* query) {
	[[ServerHost sharedInstance] mapLocation:[NSString stringWithUTF8String:query]];
}

void mapview_create(rho_param *p) {
#ifdef __IPHONE_3_0
	[[ServerHost sharedInstance] createMap:rho_param_dup(p)];
#endif	
}

void _rho_ext_syscall(PARAMS_WRAPPER* params) {
	[[ServerHost sharedInstance] doSysCall:params];
}
