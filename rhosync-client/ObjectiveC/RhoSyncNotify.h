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
}

@property int       total_count;
@property int       processed_count;
@property int       cumulative_count;
@property int       source_id;
@property int       error_code;
@property(readonly) NSString* source_name;
@property(readonly) NSString* status;
@property(readonly) NSString* sync_type;
@property(readonly) NSString* error_message;
@property(readonly) NSString* callback_params;

- (id) init: (RHO_SYNC_NOTIFY*) data;
- (void)dealloc;

@end
