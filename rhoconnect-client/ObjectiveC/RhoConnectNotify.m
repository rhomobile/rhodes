//
//  RhoConnectNotify.m
//  SyncClientTest
//
//  Created by evgeny vovchenko on 8/23/10.
//  Copyright 2010 RhoMobile. All rights reserved.
//

#import "RhoConnectNotify.h"

@implementation RhoConnectNotify

@synthesize  total_count;
@synthesize  processed_count;
@synthesize  cumulative_count;
@synthesize  source_id;
@synthesize  error_code;
@synthesize  source_name;
@synthesize  status;
@synthesize  sync_type;
@synthesize  bulk_status;
@synthesize  partition;
@synthesize  error_message;
@synthesize  callback_params;
@synthesize  notify_data;
@synthesize  create_errors;
@synthesize  update_errors;
@synthesize  delete_errors;


int enum_errors_messages_func(const char* szKey, const char* szValue, void* pThis)
{
	NSMutableDictionary* data = (NSMutableDictionary*)pThis;
	[data setValue : [NSString stringWithUTF8String:szValue] forKey : [NSString stringWithUTF8String:szKey]];							 
	return 1;
}

- (id) init: (RHO_CONNECT_NOTIFY*) data
{
	self = [super init];
	
    notify_data = *data;
	total_count = data->total_count;
	processed_count = data->processed_count;	
	cumulative_count = data->cumulative_count;	
	source_id = data->source_id;	
	error_code = data->error_code;	

	if ( data->source_name )
		source_name = [[NSString alloc] initWithUTF8String: data->source_name];
	if ( data->status )
		status = [[NSString alloc] initWithUTF8String: data->status];
	if ( data->sync_type )
		sync_type = [[NSString alloc] initWithUTF8String: data->sync_type];
	if ( data->bulk_status )
		bulk_status = [[NSString alloc] initWithUTF8String: data->bulk_status];
	if ( data->partition )
		partition = [[NSString alloc] initWithUTF8String: data->partition];
		
	if ( data->error_message )	
		error_message = [[NSString alloc] initWithUTF8String: data->error_message];
	if ( data->callback_params )	
		callback_params = [[NSString alloc] initWithUTF8String: data->callback_params];
    if ( data->create_errors_messages )
    {
        create_errors = [[NSMutableDictionary alloc] init];
        rho_connectclient_hash_enumerate(data->create_errors_messages, &enum_errors_messages_func, create_errors);
    }
    if ( data->update_errors_messages )
    {
        update_errors = [[NSMutableDictionary alloc] init];
        rho_connectclient_hash_enumerate(data->update_errors_messages, &enum_errors_messages_func, update_errors);
    }
    if ( data->delete_errors_messages )
    {
        delete_errors = [[NSMutableDictionary alloc] init];
        rho_connectclient_hash_enumerate(data->delete_errors_messages, &enum_errors_messages_func, delete_errors);
    }
	
	return self;
}

- (void)dealloc 
{
    rho_connectclient_free_syncnotify(&notify_data);
    
	if ( source_name )
		[source_name release];
	
	if ( status )
		[status release];
	
	if ( sync_type )
		[sync_type release];

	if ( bulk_status )
		[bulk_status release];

	if ( partition )
		[partition release];
	
	if (error_message)
		[error_message release];
	
	if (callback_params)
		[callback_params release];
    
    if (create_errors)
        [create_errors release];
    
    if (update_errors)
        [update_errors release];
    
    if (delete_errors)
        [delete_errors release];
	
    [super dealloc];
}

- (RHO_CONNECT_NOTIFY*)getNotifyPtr
{
    return &notify_data;
}

- (Boolean) hasCreateErrors
{
    return notify_data.create_errors_messages != 0;
}

- (Boolean) hasUpdateErrors
{
    return (notify_data.update_errors_obj != 0) || (notify_data.update_errors_messages != 0);
    
}

- (Boolean) hasDeleteErrors
{
    return (notify_data.delete_errors_obj != 0) || (notify_data.delete_errors_obj != 0);
}

- (Boolean) isUnknownClientError
{
    return [error_message caseInsensitiveCompare:@"unknown client"] == 0;
}

@end
