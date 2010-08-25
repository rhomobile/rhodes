//
//  RhoSyncClient.m
//  SyncClientTest
//
//  Created by evgeny vovchenko on 8/23/10.
//  Copyright 2010 RhoMobile. All rights reserved.
//

#import "RhoSyncClient.h"

#include "sync/SyncThread.h"

@implementation RhoSyncClient

@synthesize threaded_mode;
@synthesize poll_interval;
@synthesize sync_server;

- (void) setSyncServer:(NSString *)server
{
	rho_sync_set_syncserver([server cStringUsingEncoding:[NSString defaultCStringEncoding]]);
}

- (void) setThreadedMode:(BOOL)mode
{
	rho_sync_set_threaded_mode(mode ? 1 : 0);
}

- (void) setPollInterval:(int)interval
{
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
	
    rho_syncclient_init(rhom_models, 2);	
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

@end
