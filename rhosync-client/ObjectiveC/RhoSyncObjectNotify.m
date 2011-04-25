//
//  RhoSyncObjectNotify.m
//  SyncClient
//
//  Created by evgeny vovchenko on 4/22/11.
//  Copyright 2011 RhoMobile. All rights reserved.
//

#import "RhoSyncObjectNotify.h"


@implementation RhoSyncObjectNotify

@synthesize  deleted_objects;
@synthesize  updated_objects;
@synthesize  created_objects;
 
@synthesize  deleted_source_ids;
@synthesize  updated_source_ids;
@synthesize  created_source_ids;


- (id) init: (RHO_SYNC_OBJECT_NOTIFY*) data
{
	self = [super init];

	if ( data->deleted_count )
    {
        deleted_objects = [[NSMutableArray alloc] initWithCapacity:data->deleted_count];
        deleted_source_ids = [[NSMutableArray alloc] initWithCapacity:data->deleted_count];
        
        for ( int i = 0; i < data->deleted_count; i++ )
        {
            [deleted_objects addObject:[NSString stringWithUTF8String:data->deleted_objects[i]]];
            [deleted_source_ids addObject: [[NSNumber alloc] initWithInt:data->deleted_source_ids[i]]];
        }
    }

	if ( data->updated_count )
    {
        updated_objects = [[NSMutableArray alloc] initWithCapacity:data->updated_count];
        updated_source_ids = [[NSMutableArray alloc] initWithCapacity:data->updated_count];
        
        for ( int i = 0; i < data->updated_count; i++ )
        {
            [updated_objects addObject:[NSString stringWithUTF8String:data->updated_objects[i]]];
            [updated_source_ids addObject: [[NSNumber alloc] initWithInt:data->updated_source_ids[i]]];
        }
    }

	if ( data->created_count )
    {
        created_objects = [[NSMutableArray alloc] initWithCapacity:data->created_count];
        created_source_ids = [[NSMutableArray alloc] initWithCapacity:data->created_count];
        
        for ( int i = 0; i < data->created_count; i++ )
        {
            [created_objects addObject:[NSString stringWithUTF8String:data->created_objects[i]]];
            [created_source_ids addObject: [[NSNumber alloc] initWithInt:data->created_source_ids[i]]];
        }
    }
	
	rho_syncclient_free_sync_objectnotify(data);
	return self;
}

- (void)dealloc 
{
	
    [super dealloc];
}

@end
