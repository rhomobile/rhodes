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
#include "JSString.h"
#import "WebViewUrl.h"
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
@synthesize onShowPopup, onVibrate, onPlayFile, onStopPlaying, onSysCall, onMapLocation, onCreateMap, onActiveTab, onShowLog;

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

- (void)refreshView:(int)index {
	if(actionTarget && [actionTarget respondsToSelector:onRefreshView]) {
		[actionTarget performSelectorOnMainThread:onRefreshView withObject:(void*)index waitUntilDone:NO];
	}
}

- (void)navigateTo:(WebViewUrl*) wvUrl {
	if(actionTarget && [actionTarget respondsToSelector:onNavigateTo]) {
		[actionTarget performSelectorOnMainThread:onNavigateTo withObject:wvUrl waitUntilDone:NO];
	}
}

- (void)executeJs:(JSString*) js {
	if(actionTarget && [actionTarget respondsToSelector:onExecuteJs]) {
		[actionTarget performSelectorOnMainThread:onExecuteJs withObject:js waitUntilDone:YES];
	}
}

- (void)performRefreshView {
	[self performSelectorOnMainThread:@selector(refreshView)
						   withObject:NULL waitUntilDone:NO]; 
}
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

- (void)stopPlaying {
    if (actionTarget && [actionTarget respondsToSelector:onStopPlaying]) {
        [actionTarget performSelectorOnMainThread:onStopPlaying withObject:nil waitUntilDone:NO];
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
void webview_refresh(int index) {
	[[ServerHost sharedInstance] refreshView:index];
}

void webview_navigate(char* url, int index) {
	WebViewUrl *webViewUrl = [[[WebViewUrl alloc] init] autorelease];
	//char* szNormUrl = rho_http_normalizeurl(url);
	webViewUrl.url = [NSString stringWithUTF8String:url];
	//rho_http_free(szNormUrl);
	webViewUrl.webViewIndex = index;
	[[ServerHost sharedInstance] navigateTo:webViewUrl];
}

char* webview_execute_js(char* js, int index) {
	char * retval;
	JSString *javascript = [[[JSString alloc] init] autorelease];
	javascript.inputJs = [NSString stringWithUTF8String:js];
    javascript->index = index;
	[[ServerHost sharedInstance] executeJs:javascript];
	// TBD: Does ruby GC pick this up?
	retval = strdup([[javascript outputJs] cStringUsingEncoding:[NSString defaultCStringEncoding]]);
	return retval;
}

void perform_webview_refresh() {
	[[ServerHost sharedInstance] performRefreshView];														
}

void rho_conf_show_log() {
    [[ServerHost sharedInstance] showLog];
}

char* webview_current_location(int index) {
	return (char*)rho_rhodesapp_getcurrenturl(index);
}

void webview_set_menu_items(VALUE valMenu) {
	//TODO: webview_set_menu_items
	rho_rhodesapp_setViewMenu(valMenu);
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

VALUE rho_ringtone_manager_get_all()
{
    // No API to get ringtones
    return rho_ruby_get_NIL();
}

void rho_ringtone_manager_play(char *file_name)
{
    if (file_name == NULL) {
        RAWLOG_ERROR("RingtoneManager.play - please specify file name to play");
        return;
    }
    
    [[ServerHost sharedInstance] playFile:[NSString stringWithUTF8String:file_name]
                                mediaType:NULL];
}

void rho_ringtone_manager_stop()
{
    [[ServerHost sharedInstance] stopPlaying];
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

void create_nativebar(int bar_type, rho_param *p)
{
    if (p->type != RHO_PARAM_ARRAY) {
        RAWLOG_ERROR("Unexpected parameter type for create_nativebar, should be Array");
        return;
    }
    
    int size = p->v.array->size;
    NSMutableArray *items = [NSMutableArray arrayWithCapacity:size];
    for (int i = 0; i < size; ++i) {
        rho_param *hash = p->v.array->value[i];
        if (hash->type != RHO_PARAM_HASH) {
            RAWLOG_ERROR("Unexpected type of array item for create_nativebar, should be Hash");
            return;
        }
        
        const char *label = NULL;
        const char *action = NULL;
        const char *icon = NULL;
        const char *reload = NULL;
        
        for (int j = 0, lim = hash->v.hash->size; j < lim; ++j) {
            const char *name = hash->v.hash->name[j];
            rho_param *value = hash->v.hash->value[j];
            if (value->type != RHO_PARAM_STRING) {
                RAWLOG_ERROR1("Unexpected '%s' type, should be String", name);
                return;
            }
            
            if (strcasecmp(name, "label") == 0)
                label = value->v.string;
            else if (strcasecmp(name, "action") == 0)
                action = value->v.string;
            else if (strcasecmp(name, "icon") == 0)
                icon = value->v.string;
            else if (strcasecmp(name, "reload") == 0)
                reload = value->v.string;
        }
        
        if (label == NULL && bar_type == TOOLBAR_TYPE)
            label = "";
        
        if (label == NULL || action == NULL) {
            RAWLOG_ERROR("Illegal argument for create_nativebar");
            return;
        }
        
        [items addObject:[NSString stringWithUTF8String:label]];
        [items addObject:[NSString stringWithUTF8String:action]];
        [items addObject:[NSString stringWithUTF8String:(icon ? icon : "")]];
        [items addObject:[NSString stringWithUTF8String:(reload ? reload : "false")]];
    }
    [[ServerHost sharedInstance] createNativeBar:bar_type dataArray:items];
}

void remove_nativebar() {
    [[ServerHost sharedInstance] removeNativeBar];
}

void nativebar_switch_tab(int index) {
    [[ServerHost sharedInstance] switchTab:index];
}

int webview_active_tab() {
	return [[ServerHost sharedInstance] activeTab];
}