//
//  RhoConnectClient.m
//  RhoCoonectClientTest
//
//  Created by evgeny vovchenko on 8/23/10.
//  Copyright 2010 RhoMobile. All rights reserved.
//

#import "RhoConnectClient.h"

#include "sync/SyncThread.h"
#include "common/RhoConf.h"
#import "common/RhodesAppBase.h"
#include "logging/RhoLogConf.h"
#import "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoConnectClient"

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
    RHO_CONNECT_NOTIFY oNotify = {0};
    rho_connectclient_parsenotify(szNotify, &oNotify);
	
	RhoConnectNotify* notify =  [[RhoConnectNotify alloc] init: &oNotify];
	
	CCallbackData* callbackObj = data;
	
    if ( callbackObj.targetThread != [NSThread currentThread] )
        [ callbackObj.targetObject performSelector:callbackObj.targetMethod onThread:callbackObj.targetThread withObject:notify waitUntilDone:TRUE];
    else
        [ callbackObj.targetObject performSelector:callbackObj.targetMethod withObject:notify];


	[callbackObj autorelease];
    [notify autorelease];
	return 0;
}

int callback_object_impl(const char* szNotify, void* data)
{
    RHO_CONNECT_OBJECT_NOTIFY oNotify = {0};
    rho_connectclient_parse_objectnotify(szNotify, &oNotify);
	
	RhoConnectObjectNotify* notify =  [[RhoConnectObjectNotify alloc] init: &oNotify];
	
	CCallbackData* callbackObj = data;
	
    if ( callbackObj.targetThread != [NSThread currentThread] )
        [ callbackObj.targetObject performSelector:callbackObj.targetMethod onThread:callbackObj.targetThread withObject:notify waitUntilDone:TRUE];
    else
        [ callbackObj.targetObject performSelector:callbackObj.targetMethod withObject:notify];
    
	[callbackObj autorelease];
    [notify autorelease];
	return 0;
}

void rho_free_callbackdata(void* pData)
{
	//CCallbackData* callbackObj = pData;
	//[callbackObj release];
}

@implementation RhoConnectClient

@synthesize threaded_mode;
@synthesize poll_interval;
@synthesize log_severity;
@synthesize sync_server;
@synthesize bulksync_state;

- (id) init
{
	self = [super init];
	return self;
}

- (void)dealloc 
{
	rho_connectclient_destroy();
	
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

- (void) setLogSeverity:(int)severity
{
	log_severity = severity;
	rho_logconf_setSeverity(log_severity);
}

- (void) setBulkSyncState:(int)state
{
	rho_conf_setInt("bulksync_state", state);
}

- (int) getBulkSyncState
{
	return rho_conf_getInt("bulksync_state");
}

- (void) setConfigString: (NSString*)name param: (NSString*) param
{
	if( [name compare:@"MinSeverity"] == 0)
		rho_logconf_setSeverity([param intValue]);
	else	
		rho_conf_setString([name cStringUsingEncoding:[NSString defaultCStringEncoding]],
					   [param cStringUsingEncoding:[NSString defaultCStringEncoding]]);
}

- (NSString*) getConfigString: (NSString*)name
{
	NSString* ret;
	char* res = rho_conf_getString([name cStringUsingEncoding:[NSString defaultCStringEncoding]]);
	if ( res )
	{
		ret = [NSString stringWithUTF8String:res];
		rho_conf_freeString(res);
	}else {
		ret = [NSString stringWithUTF8String:""];
	}

	return ret;
}

- (void) addModels: (NSMutableArray*)models
{
	RHOM_MODEL rhom_models[models.count];
	int nModel = 0;
	for (RhomModel* model in models) 
	{
		rho_connectclient_initmodel(&rhom_models[nModel]);
		rhom_models[nModel].name = [model.name cStringUsingEncoding:[NSString defaultCStringEncoding]];

		rhom_models[nModel].sync_type = model.sync_type;
        rhom_models[nModel].type = model.model_type;
        
        if (model.associations != NULL) {
            for (NSString* key in model.associations) {
                rho_connectclient_hash_put(rhom_models[nModel].associations, 
                                           [key cStringUsingEncoding:[NSString defaultCStringEncoding]], 
                                           [[model.associations objectForKey:key] cStringUsingEncoding:[NSString defaultCStringEncoding]]
                                           );
            }	
        }
        
        if (0 < [model.blob_attribs length]) {
            rhom_models[nModel].blob_attribs = [model.blob_attribs cStringUsingEncoding:[NSString defaultCStringEncoding]];
        }
		nModel++;
	}
	
    rho_connectclient_init(rhom_models, models.count);	
	
    int i = 0;
    for (RhomModel* model in models) 
	{
        model.source_id = rhom_models[i].source_id; 
        rho_connectclient_destroymodel(&rhom_models[i]);
        i++;
    }
    
    [self setThreadedMode:FALSE];
	[self setPollInterval: 0];
}

- (void) setSourceProperty: (int) nSrcID szPropName:(NSString*) szPropName szPropValue:(NSString*) szPropValue
{
    rho_sync_set_source_property( nSrcID, [szPropName cStringUsingEncoding:[NSString defaultCStringEncoding]],
                                 [szPropValue cStringUsingEncoding:[NSString defaultCStringEncoding]] );
}

- (void) database_full_reset_and_logout
{
	rho_connectclient_database_full_reset_and_logout();	
}

- (void) database_client_reset
{
	rho_connectclient_database_client_reset();	
}

- (BOOL) is_logged_in
{
	return rho_sync_logged_in() == 1 ? TRUE : FALSE;
}

- (RhoConnectNotify*) loginWithUser: (NSString*) user pwd:(NSString*) pwd
{
	char* res = (char*)rho_sync_login( [user cStringUsingEncoding:[NSString defaultCStringEncoding]],
				   [pwd cStringUsingEncoding:[NSString defaultCStringEncoding]], "");
	
    RHO_CONNECT_NOTIFY oNotify = {0};
    rho_connectclient_parsenotify(res, &oNotify);
	rho_sync_free_string(res);
	
	return [[[RhoConnectNotify alloc] init: &oNotify] autorelease];
}

- (void) loginWithUser: (NSString*) user pwd:(NSString*) pwd callback:(SEL) callback target:(id)target
{
	rho_sync_login_c( [user cStringUsingEncoding:[NSString defaultCStringEncoding]],
					[pwd cStringUsingEncoding:[NSString defaultCStringEncoding]], 
					 callback_impl, [[CCallbackData alloc] init: callback target: target thread:[NSThread currentThread]] 
				   );
}

- (void) logout
{
	rho_sync_logout();
}

+ (void) setNotification: (SEL) callback target:(id)target
{
	rho_sync_set_notification_c(-1, callback_impl, 
      [[CCallbackData alloc] init: callback target: target thread:[NSThread currentThread]] );
}

- (void) setNotification: (SEL) callback target:(id)target
{
	rho_sync_set_notification_c(-1, callback_impl, 
	  [[CCallbackData alloc] init: callback target: target thread:[NSThread currentThread]] );
}

+ (void) setModelNotification: (int) nSrcID callback: (SEL) callback target:(id)target
{
    rho_sync_set_notification_c(nSrcID, callback_impl, 
                                [[CCallbackData alloc] init: callback target: target thread:[NSThread currentThread]] );    
}

- (void) clearNotification
{
	rho_sync_clear_notification(-1);
}

- (void) setObjectNotification: (SEL) callback target:(id)target
{
	rho_sync_setobjectnotify_url_c( callback_object_impl, 
                                [[ CCallbackData alloc] init: callback target: target thread:[NSThread currentThread]] );
}

- (void) clearObjectNotification
{
	rho_sync_clear_object_notification();
}

- (void) addObjectNotify: (int) nSrcID szObject:(NSString*) szObject
{
    rho_sync_addobjectnotify(nSrcID, [szObject cStringUsingEncoding:[NSString defaultCStringEncoding]]); 
}

- (RhoConnectNotify*) syncAll
{
    char* res = (char*)rho_sync_doSyncAllSources(0, "");
	
    RHO_CONNECT_NOTIFY oNotify = {0};
    rho_connectclient_parsenotify(res, &oNotify);
	rho_sync_free_string(res);
	
	return [[[RhoConnectNotify alloc] init: &oNotify] autorelease];
}

- (BOOL) is_syncing
{
    return rho_sync_issyncing() == 1 ? TRUE : FALSE;
}

- (void) stop_sync
{
    rho_sync_stop();
}

- (RhoConnectNotify*) search: (NSArray*)models from: (NSString*) from params: (NSString*)params sync_changes: (BOOL) sync_changes progress_step: (int) progress_step
{
    unsigned long ar_sources = rho_connectclient_strarray_create();
	for (RhomModel* model in models)
	{
		rho_connectclient_strarray_add(ar_sources, [model.name cStringUsingEncoding:[NSString defaultCStringEncoding]]);
	}
	
    char* res = (char*)rho_sync_doSearchByNames( ar_sources, 
		[from cStringUsingEncoding:[NSString defaultCStringEncoding]], 
		[params cStringUsingEncoding:[NSString defaultCStringEncoding]],
		sync_changes ? 1 : 0, 
		progress_step, "", "" );
	
    rho_connectclient_strarray_delete(ar_sources);

    RHO_CONNECT_NOTIFY oNotify = {0};
    rho_connectclient_parsenotify(res, &oNotify);
	rho_sync_free_string(res);
	
	return [[[RhoConnectNotify alloc] init: &oNotify] autorelease];
}

- (void) onCreateError: (RhoConnectNotify*)notify action: (NSString*)action
{
    rho_connectclient_on_sync_create_error(
        [notify.source_name cStringUsingEncoding:[NSString defaultCStringEncoding]],
        [notify getNotifyPtr],
        [action cStringUsingEncoding:[NSString defaultCStringEncoding]]                                  
    );
}

- (void) onUpdateError: (RhoConnectNotify*)notify action: (NSString*)action
{
    rho_connectclient_on_sync_update_error(
       [notify.source_name cStringUsingEncoding:[NSString defaultCStringEncoding]],
       [notify getNotifyPtr],
       [action cStringUsingEncoding:[NSString defaultCStringEncoding]]                                  
       );    
}

- (void) onDeleteError: (RhoConnectNotify*)notify action: (NSString*)action
{
    rho_connectclient_on_sync_delete_error(
       [notify.source_name cStringUsingEncoding:[NSString defaultCStringEncoding]],
       [notify getNotifyPtr],
       [action cStringUsingEncoding:[NSString defaultCStringEncoding]]                                  
       );    
}

void copyFromMainBundle( NSFileManager* fileManager,  NSString * source, NSString * target, BOOL remove );
void createFolder( NSFileManager* fileManager,  NSString * target, BOOL remove );
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
    createFolder( fileManager, [self blobPath], NO );
}

+ (NSString*) storagePath
{
    return [NSString stringWithUTF8String:rho_native_rhopath()];
}

+ (NSString*) pathForStorageFile: (NSString*) file
{
    return [[self storagePath] stringByAppendingString: file];
}

+ (NSString*) blobFolder
{
    return @"db/db-files/";
}

+ (NSString*) blobPath
{
    return [[self storagePath] stringByAppendingString: [self blobFolder]];
}

+ (NSString*) pathForBlob: (NSString*) uri
{
    return [[self storagePath] stringByAppendingString: uri];
}

+ (void) fromMainBundle: (NSFileManager*) fileManager copyFile: (NSString*) source toStorage: (NSString*) target forceRemove: (BOOL) remove
{
	NSString *bundleRoot = [[NSBundle mainBundle] resourcePath];
    copyFromMainBundle(fileManager, [bundleRoot stringByAppendingPathComponent: source], [self pathForStorageFile: target], remove);
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
		while (nil != (child = [enumerator nextObject])) {
			copyFromMainBundle( fileManager, [source stringByAppendingPathComponent: child],
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

void createFolder( NSFileManager* fileManager,  NSString * target, BOOL remove )
{
	BOOL dir;
    [fileManager fileExistsAtPath:target isDirectory:&dir];
	if (!remove && dir) {
        return;
    }
    
    NSError* error;
    if (remove) {
        if (![fileManager removeItemAtPath:target error:&error]) {
            return;
        }
    }
    
    if (![fileManager createDirectoryAtPath:target withIntermediateDirectories:YES attributes:nil error:&error]) {
        return;
    }
}

int rho_net_ping_network(const char* szHost)
{
	RAWLOG_INFO("PING network.");
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	NSMutableURLRequest *request = [[[NSMutableURLRequest alloc] init] autorelease];
	NSString *linkString = [NSString stringWithUTF8String: szHost];
	
	[request setURL:[NSURL URLWithString:linkString]];
	[request setTimeoutInterval:10];
	
	NSError *error = nil;
	NSHTTPURLResponse *response;
	NSData *returnData = NULL;
	returnData = [ NSURLConnection sendSynchronousRequest: request returningResponse:&response error: &error ];
	
	if (!returnData)
		RAWLOG_ERROR2("PING network FAILED. NSError: %d. NSErrorInfo : %s", [error code], [[error localizedDescription] UTF8String]);
	else
		RAWLOG_INFO("PING network SUCCEEDED.");	
	
	[pool release];
	
	return returnData == NULL ? 0 : 1;
}

