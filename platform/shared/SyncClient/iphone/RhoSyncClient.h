//
//  RhoSyncClient.h
//  SyncClientTest
//
//  Created by evgeny vovchenko on 8/23/10.
//  Copyright 2010 RhoMobile. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RhomModel.h"
#import "RhoSyncNotify.h"
#include "SyncClient/SyncClient.h"

@interface RhoSyncClient : NSObject {
	BOOL threaded_mode;
	int  poll_interval;
	NSString* sync_server;
}

@property(setter=setThreadedMode) BOOL threaded_mode;
@property(setter=setPollInterval) int  poll_interval;
@property(assign, setter=setSyncServer) NSString* sync_server;

+ (void) initDatabase;
- (void) addModels:(NSArray*)models;
- (void) database_full_reset_and_logout;
- (BOOL) is_logged_in;

- (RhoSyncNotify*) loginWithUser: (NSString*) user pwd:(NSString*) pwd;
- (RhoSyncNotify*) syncAll;

- (RhoSyncNotify*) search: (NSArray*)models from: (NSString*) from params: (NSString*)params sync_changes: (BOOL) sync_changes progress_step: (int) progress_step;

@end
