//
//  RhoSyncNotify.h
//  SyncClientTest
//
//  Created by evgeny vovchenko on 8/23/10.
//  Copyright 2010 RhoMobile. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "SyncClient/SyncClient.h"

@interface RhoSyncNotify : NSObject {
    int total_count;
    int processed_count;
    int cumulative_count;
    int source_id;
    NSString* source_name;
    NSString* status;
    NSString* sync_type;
    int error_code;
    NSString* error_message;
    NSString* callback_params;
}

@property int       total_count;
@property int       processed_count;
@property int       cumulative_count;
@property int       source_id;
@property int       error_code;
@property(assign) NSString* source_name;
@property(assign) NSString* status;
@property(assign) NSString* sync_type;
@property(assign) NSString* error_message;
@property(assign) NSString* callback_params;

- (id) init: (RHO_SYNC_NOTIFY*) data;


@end
