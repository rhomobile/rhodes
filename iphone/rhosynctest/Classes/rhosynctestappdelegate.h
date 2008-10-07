/*
 *  RhoSyncClientAppDelegate.h
 *  RhoSyncClient
 *
 *  Copyright (C) 2008 Lars Burgess. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#import <UIKit/UIKit.h>
#import "SyncObjectWrapper.h"
#include "Constants.h"
#include "SyncJSONReader.h"
#include "SyncOperation.h"
#include "SyncEngine.h"

@interface rhosynctestappdelegate : NSObject <UIApplicationDelegate> {
    
    UIWindow *window;
    UINavigationController *navigationController;
	NSMutableArray *list;
	BOOL _isDataSourceAvailable;
	sqlite3 *database;
	
	// Manages connection to sync server
	pSyncOperation syncOperation;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet UINavigationController *navigationController;
@property (nonatomic, retain) NSMutableArray *list;

- (sqlite3 *)getDatabase;
- (BOOL)isDataSourceAvailable;

- (NSUInteger)countOfList;
- (void)removeSyncObject:(SyncObjectWrapper *)oldSyncObject;
- (void)runSync;
- (int)runRefresh;
- (void)reloadTable;
- (id)objectInListAtIndex:(NSUInteger)theIndex;
- (void)getList:(id *)objsPtr range:(NSRange)range;
- (void)fetchRecordsFromDatabase;

@end

