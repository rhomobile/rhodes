//
//  RhoSyncNotify.m
//  SyncClientTest
//
//  Created by evgeny vovchenko on 8/23/10.
//  Copyright 2010 RhoMobile. All rights reserved.
//

#import "RhoSyncNotify.h"

@implementation RhoSyncNotify

@synthesize  total_count;
@synthesize  processed_count;
@synthesize  cumulative_count;
@synthesize  source_id;
@synthesize  error_code;
@synthesize  source_name;
@synthesize  status;
@synthesize  sync_type;
@synthesize  error_message;
@synthesize  callback_params;

- (id) init: (RHO_SYNC_NOTIFY*) data
{
	self = [super init];
	
	total_count = data->total_count;
	processed_count = data->processed_count;	
	cumulative_count = data->cumulative_count;	
	source_id = data->source_id;	
	error_code = data->error_code;	

	if ( data->source_name )
		source_name = [NSString stringWithUTF8String: data->source_name];
	if ( data->status )
		status = [NSString stringWithUTF8String: data->status];
	if ( data->sync_type )
		sync_type = [NSString stringWithUTF8String: data->sync_type];
	if ( data->error_message )	
		error_message = [NSString stringWithUTF8String: data->error_message];
	if ( data->callback_params )	
		callback_params = [NSString stringWithUTF8String: data->callback_params];
	
	return self;
}

@end
