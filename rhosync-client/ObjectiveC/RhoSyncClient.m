//
//  RhoSyncClient.m
//  SyncClientTest
//
//  Created by evgeny vovchenko on 8/23/10.
//  Copyright 2010 RhoMobile. All rights reserved.
//

#import "RhoSyncClient.h"

#include "sync/SyncThread.h"

@interface CCallbackData : NSObject {
}

@property(assign) NSThread* targetThread;
@property SEL     targetMethod;
@property(assign) id      targetObject;

- (id) init: (SEL) selector target: (id) target thread: (NSThread*) thread;
@end

@implementation CCallbackData
@synthesize targetThread;
@synthesize targetMethod;
@synthesize targetObject;
- (id) init: (SEL) selector target: (id) target thread: (NSThread*) thread
{
	self = [super init];
	
	targetMethod = selector;
	targetObject = target;
	targetThread = thread;
	return self;
}

@end

int callback_impl(const char* szNotify, void* data)
{
    RHO_SYNC_NOTIFY oNotify = {0};
    rho_syncclient_parsenotify(szNotify, &oNotify);
	
	RhoSyncNotify* notify =  [[RhoSyncNotify alloc] init: &oNotify];
	
	CCallbackData* callbackObj = data;
	
	[ callbackObj.targetObject performSelector:callbackObj.targetMethod onThread:callbackObj.targetThread withObject:notify waitUntilDone:FALSE];

	//[notify release];
	//[callbackObj release];
	
	return 0;
}

void rho_free_callbackdata(void* pData)
{
	CCallbackData* callbackObj = pData;
	[callbackObj release];
}

@implementation RhoSyncClient

@synthesize threaded_mode;
@synthesize poll_interval;
@synthesize sync_server;

- (id) init
{
	self = [super init];
	
	return self;
}

- (void)dealloc 
{
	rho_syncclient_destroy();
	
	[sync_server release];
    [super dealloc];
}

- (void) setSyncServer:(NSString *)server
{
	sync_server = [server retain];
	rho_sync_set_syncserver([server cStringUsingEncoding:[NSString defaultCStringEncoding]]);
}

- (void) setThreadedMode:(BOOL)mode
{
	threaded_mode = mode;
	rho_sync_set_threaded_mode(mode ? 1 : 0);
}

- (void) setPollInterval:(int)interval
{
	poll_interval = interval;
	rho_sync_set_pollinterval(interval);
}

- (void) addModels: (NSArray*)models
{
	RHOM_MODEL rhom_models[models.count];
	int nModel = 0;
	for (RhomModel* model in models) 
	{
		rho_syncclient_initmodel(&rhom_models[nModel]);
		rhom_models[nModel].name = [model.name cStringUsingEncoding:[NSString defaultCStringEncoding]];

		rhom_models[nModel].sync_type = model.sync_type;
		
		nModel++;
	}
	
    rho_syncclient_init(rhom_models, models.count);	
	
    [self setThreadedMode:FALSE];
	[self setPollInterval: 0];
}

- (void) database_full_reset_and_logout
{
	rho_syncclient_database_full_reset_and_logout();	
}

- (BOOL) is_logged_in
{
	return rho_sync_logged_in() == 1 ? TRUE : FALSE;
}

- (RhoSyncNotify*) loginWithUser: (NSString*) user pwd:(NSString*) pwd
{
	char* res = (char*)rho_sync_login( [user cStringUsingEncoding:[NSString defaultCStringEncoding]],
				   [pwd cStringUsingEncoding:[NSString defaultCStringEncoding]], "");
	
    RHO_SYNC_NOTIFY oNotify = {0};
    rho_syncclient_parsenotify(res, &oNotify);
	rho_sync_free_string(res);
	
	return [[RhoSyncNotify alloc] init: &oNotify];
}

- (void) loginWithUser: (NSString*) user pwd:(NSString*) pwd callback:(SEL) callback target:(id)target
{
	rho_sync_login_c( [user cStringUsingEncoding:[NSString defaultCStringEncoding]],
					[pwd cStringUsingEncoding:[NSString defaultCStringEncoding]], 
					 callback_impl, [[ CCallbackData alloc] init: callback target: target thread:[NSThread currentThread] ] 
				   );
}

+ (void) setNotification: (SEL) callback target:(id)target
{
	rho_sync_set_notification_c(-1, callback_impl, 
      [[ CCallbackData alloc] init: callback target: target thread:[NSThread currentThread] ] );
}

- (void) setNotification: (SEL) callback target:(id)target
{
	rho_sync_set_notification_c(-1, callback_impl, 
	  [[ CCallbackData alloc] init: callback target: target thread:[NSThread currentThread] ] );
}

- (void) clearNotification
{
	rho_sync_clear_notification(-1);
}

- (RhoSyncNotify*) syncAll
{
    char* res = (char*)rho_sync_doSyncAllSources(0);
	
    RHO_SYNC_NOTIFY oNotify = {0};
    rho_syncclient_parsenotify(res, &oNotify);
	rho_sync_free_string(res);
	
	return [[RhoSyncNotify alloc] init: &oNotify];
}

- (RhoSyncNotify*) search: (NSArray*)models from: (NSString*) from params: (NSString*)params sync_changes: (BOOL) sync_changes progress_step: (int) progress_step
{
    unsigned long ar_sources = rho_syncclient_strarray_create();
	for (RhomModel* model in models)
	{
		rho_syncclient_strarray_add(ar_sources, [model.name cStringUsingEncoding:[NSString defaultCStringEncoding]]);
	}
	
    char* res = (char*)rho_sync_doSearchByNames( ar_sources, 
		[from cStringUsingEncoding:[NSString defaultCStringEncoding]], 
		[params cStringUsingEncoding:[NSString defaultCStringEncoding]],
		sync_changes ? 1 : 0, 
		progress_step, "", "" );
	
    rho_syncclient_strarray_delete(ar_sources);

    RHO_SYNC_NOTIFY oNotify = {0};
    rho_syncclient_parsenotify(res, &oNotify);
	rho_sync_free_string(res);
	
	return [[RhoSyncNotify alloc] init: &oNotify];
}

void copyFromMainBundle( NSFileManager* fileManager,  NSString * source, NSString * target, BOOL remove );
const char* rho_native_rhopath() ;
+ (void) initDatabase
{
    NSFileManager *fileManager = [NSFileManager defaultManager];
    
	NSString *bundleRoot = [[NSBundle mainBundle] resourcePath];
	NSString *rhoRoot = [NSString stringWithUTF8String:rho_native_rhopath()];
	
    NSString *dirs[] = {@"db"};	
	copyFromMainBundle( fileManager,
					   [bundleRoot stringByAppendingPathComponent:dirs[0]],
					   [rhoRoot stringByAppendingPathComponent:dirs[0]],
					   NO);
}

@end

const char* rho_native_rhopath() 
{
	static bool loaded = FALSE;
	static char root[FILENAME_MAX];
	if (!loaded){
		NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
		NSString *documentsDirectory = //[paths objectAtIndex:0];
		[ [paths objectAtIndex:0] stringByAppendingString:@"/"];
		[documentsDirectory getFileSystemRepresentation:root maxLength:sizeof(root)];
		loaded = TRUE;
	}
	
	return root;
}

void copyFromMainBundle( NSFileManager* fileManager,  NSString * source, NSString * target, BOOL remove )
{
	BOOL dir;
	if(![fileManager fileExistsAtPath:source isDirectory:&dir]) {
		//NSAssert1(0, @"Source item '%@' does not exists in bundle", source);
		return;
	}
	
	if (!remove && dir) {
		if (![fileManager fileExistsAtPath:target])
			[fileManager createDirectoryAtPath:target attributes:nil];
		
		NSDirectoryEnumerator *enumerator = [fileManager enumeratorAtPath:source];
		NSString *child;
		while (child = [enumerator nextObject]) {
			copyFromMainBundle( fileManager, [source stringByAppendingPathComponent:child],
							   [target stringByAppendingPathComponent:child], NO );
		}
	}
	else {
		NSError *error;
		if ([fileManager fileExistsAtPath:target] && ![fileManager removeItemAtPath:target error:&error]) {
			//NSAssert2(0, @"Failed to remove '%@': %@", target, [error localizedDescription]);
			return;
		}
		if (![fileManager copyItemAtPath:source toPath:target error:&error]) {
			//NSAssert3(0, @"Failed to copy '%@' to '%@': %@", source, target, [error localizedDescription]);
			return;
		}
	}
}
