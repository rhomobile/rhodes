//
//  RhomModel.m
//  SyncClientTest
//
//  Created by evgeny vovchenko on 8/23/10.
//  Copyright 2010 RhoMobile. All rights reserved.
//

#import "RhomModel.h"
#import "SyncClient/SyncClient.h"
#include "sync/SyncThread.h"
#import "RhoSyncClient.h"

@implementation RhomModel

@synthesize name;
@synthesize sync_type;

- (id) init
{
	self = [super init];
	sync_type = RST_INCREMENTAL;
	
	return self;
}

- (RhoSyncNotify*) sync
{
    char* res = (char*)rho_sync_doSyncSourceByName([name cStringUsingEncoding:[NSString defaultCStringEncoding]]);
	
    RHO_SYNC_NOTIFY oNotify = {0};
    rho_syncclient_parsenotify(res, &oNotify);
	rho_sync_free_string(res);
	
	return [[RhoSyncNotify alloc] init: &oNotify];
}

- (void) sync: (SEL) callback target:(id)target
{
	[RhoSyncClient setNotification:callback target:target];

	rho_sync_doSyncSourceByName([name cStringUsingEncoding:[NSString defaultCStringEncoding]]);	 
}

- (void) create: (NSMutableDictionary *) data
{
    unsigned long item = rho_syncclient_hash_create();
	
	for (NSString* key in data) 
	{
		rho_syncclient_hash_put(item, 
			[key cStringUsingEncoding:[NSString defaultCStringEncoding]], 
			[[data objectForKey:key] cStringUsingEncoding:[NSString defaultCStringEncoding]]
		);
	}	
	
    rho_syncclient_create_object([name cStringUsingEncoding:[NSString defaultCStringEncoding]], 
								 item);
    const char* szValue = rho_syncclient_hash_get(item, "object");
	if ( szValue )
		[data setValue: [NSString stringWithUTF8String: szValue] forKey:@"object"];
	else
		[data setValue: @"" forKey:@"object"];
	
    szValue = rho_syncclient_hash_get(item, "source_id");
	if ( szValue )
		[data setValue: [NSString stringWithUTF8String: szValue] forKey:@"source_id"];
	else 
		[data setValue: @"" forKey:@"source_id"];
	
	rho_syncclient_hash_delete(item);
}

int enum_func(const char* szKey, const char* szValue, void* pThis)
{
	NSMutableDictionary* data = (NSMutableDictionary*)pThis;
	[data setValue : [NSString stringWithUTF8String:szValue] forKey : [NSString stringWithUTF8String:szKey]];							 
	return 1;
}

- (NSMutableDictionary *) find: (NSString*)object_id
{
    unsigned long item = rho_syncclient_find(
		[name cStringUsingEncoding:[NSString defaultCStringEncoding]], 
        [object_id cStringUsingEncoding:[NSString defaultCStringEncoding]] );
	
	if ( item == 0 )
		return NULL;
	NSMutableDictionary* data = [[NSMutableDictionary alloc] init];
	rho_syncclient_hash_enumerate(item, &enum_func, data );

	return data;
}

- (NSMutableDictionary *) find_first: (NSDictionary *)cond
{
    unsigned long condhash = rho_syncclient_hash_create();
	for (NSString* key in cond) 
	{
		rho_syncclient_hash_put(condhash, 
								[key cStringUsingEncoding:[NSString defaultCStringEncoding]], 
								[[cond objectForKey:key] cStringUsingEncoding:[NSString defaultCStringEncoding]]
								);
	}	
	
    unsigned long item = rho_syncclient_find_first([name cStringUsingEncoding:[NSString defaultCStringEncoding]], condhash );
    rho_syncclient_hash_delete(condhash);
	
	if ( item == 0 )
		return NULL;
	NSMutableDictionary* data = [[NSMutableDictionary alloc] init];
	rho_syncclient_hash_enumerate(item, &enum_func, data );
	
	return data;
}

- (NSMutableArray *) find_all: (NSDictionary *)cond
{
    unsigned long condhash = rho_syncclient_hash_create();
	if ( cond )
	{
		for (NSString* key in cond) 
		{
			rho_syncclient_hash_put(condhash, 
								[key cStringUsingEncoding:[NSString defaultCStringEncoding]], 
								[[cond objectForKey:key] cStringUsingEncoding:[NSString defaultCStringEncoding]]
								);
		}	
	}
	
    unsigned long items = rho_syncclient_find_all([name cStringUsingEncoding:[NSString defaultCStringEncoding]], condhash );
    rho_syncclient_hash_delete(condhash);
	
	if ( items == 0 )
		return NULL;
	
	int nSize = rho_syncclient_strhasharray_size(items);
	NSMutableArray* arRes = [[NSMutableArray alloc] initWithCapacity:nSize];
    for ( int i = 0; i < nSize; i++ )
    {
		NSMutableDictionary* data = [[NSMutableDictionary alloc] init];
		rho_syncclient_hash_enumerate(rho_syncclient_strhasharray_get(items, i), &enum_func, data );
			
		[arRes addObject:data];
    }
	
	return arRes;
}

- (void) save: (NSDictionary *)data
{
    unsigned long item = rho_syncclient_hash_create();
	
	for (NSString* key in data) 
	{
		rho_syncclient_hash_put(item, 
								[key cStringUsingEncoding:[NSString defaultCStringEncoding]], 
								[[data objectForKey:key] cStringUsingEncoding:[NSString defaultCStringEncoding]]
								);
	}	
	
    rho_syncclient_save( [name cStringUsingEncoding:[NSString defaultCStringEncoding]], item );	
	
    rho_syncclient_hash_delete(item);	
}

- (void) destroy: (NSDictionary *)data
{
    unsigned long item = rho_syncclient_hash_create();
	
	for (NSString* key in data) 
	{
		rho_syncclient_hash_put(item, 
								[key cStringUsingEncoding:[NSString defaultCStringEncoding]], 
								[[data objectForKey:key] cStringUsingEncoding:[NSString defaultCStringEncoding]]
								);
	}	
	
	rho_syncclient_itemdestroy( [name cStringUsingEncoding:[NSString defaultCStringEncoding]], 
		item );
	
    rho_syncclient_hash_delete(item);		
}

- (void) startBulkUpdate
{
	rho_syncclient_start_bulkupdate([name cStringUsingEncoding:[NSString defaultCStringEncoding]]);
}

- (void) stopBulkUpdate
{
	rho_syncclient_stop_bulkupdate([name cStringUsingEncoding:[NSString defaultCStringEncoding]]);
}

@end
