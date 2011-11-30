/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#import <Foundation/Foundation.h>
#import "RhomModel.h"
#import "RhoConnectNotify.h"
#import "RhoConnectObjectNotify.h"
#include "RhoConnectClient/RhoConnectClient.h"

@interface RhoConnectClient : NSObject {
}

@property(setter=setThreadedMode:) BOOL threaded_mode;
@property(setter=setPollInterval:) int  poll_interval;
@property(setter=setLogSeverity:) int  log_severity;
@property(assign, setter=setSyncServer:) NSString* sync_server;
@property(setter=setBulkSyncState:, getter=getBulkSyncState) int bulksync_state;

+ (void) initDatabase;
+ (void) setNotification: (SEL) callback target:(id)target;
+ (void) setModelNotification: (int) nSrcID callback: (SEL) callback target:(id)target;

- (void) setObjectNotification: (SEL) callback target:(id)target;
- (void) clearObjectNotification;
- (void) addObjectNotify: (int) nSrcID szObject:(NSString*) szObject;

- (id) init;
- (void)dealloc;

- (void) addModels:(NSMutableArray*)models;
- (void) database_full_reset_and_logout;
- (void) database_client_reset;
- (BOOL) is_logged_in;

- (RhoConnectNotify*) loginWithUser: (NSString*) user pwd:(NSString*) pwd;
- (void) loginWithUser: (NSString*) user pwd:(NSString*) pwd callback:(SEL) callback target:(id)target;
- (void) setNotification: (SEL) callback target:(id)target;
- (void) clearNotification;

- (RhoConnectNotify*) syncAll;

- (RhoConnectNotify*) search: (NSArray*)models from: (NSString*) from params: (NSString*)params sync_changes: (BOOL) sync_changes progress_step: (int) progress_step;

- (void) setConfigString: (NSString*)name param: (NSString*) param;
- (NSString*) getConfigString: (NSString*)name;

- (void) setSourceProperty: (int) nSrcID szPropName:(NSString*) szPropName szPropValue:(NSString*) szPropValue;

- (void) onCreateError: (RhoConnectNotify*)notify action: (NSString*)action;
- (void) onUpdateError: (RhoConnectNotify*)notify action: (NSString*)action;
- (void) onDeleteError: (RhoConnectNotify*)notify action: (NSString*)action;

+ (NSString*) storagePath;
+ (NSString*) pathForStorageFile: (NSString*) file;

+ (NSString*) blobFolder;
+ (NSString*) blobPath;
+ (NSString*) pathForBlob: (NSString*) uri;

+ (void) fromMainBundle: (NSFileManager*) fileManager copyFile: (NSString*) source toStorage: (NSString*) target forceRemove: (BOOL) remove;

@end
