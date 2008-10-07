/*
 *  RhoSyncClientAppDelegate.m
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

#import "rhosynctestappdelegate.h"
#import "RootViewController.h"
#import <SystemConfiguration/SystemConfiguration.h>
#import <Foundation/Foundation.h>

@implementation rhosynctestappdelegate

@synthesize window;
@synthesize navigationController;
@synthesize list;

- (sqlite3 *)getDatabase {
	return database;
}

- (BOOL)isDataSourceAvailable
{
    static BOOL checkNetwork = YES;
    if (checkNetwork) { // Since checking the reachability of a host can be expensive, cache the result and perform the reachability check once.
        checkNetwork = NO;
        
        Boolean success;    
        const char *host_name = SYNC_SOURCE;
		
        SCNetworkReachabilityRef reachability = SCNetworkReachabilityCreateWithName(NULL, host_name);
        SCNetworkReachabilityFlags flags;
        success = SCNetworkReachabilityGetFlags(reachability, &flags);
        _isDataSourceAvailable = success && (flags & kSCNetworkFlagsReachable) && !(flags & kSCNetworkFlagsConnectionRequired);
        CFRelease(reachability);
    }
    return _isDataSourceAvailable;
}

- (void)runSync {
	wake_up_sync_engine();
}

- (int)runRefresh {
	
	int status = 0;
	char url_string[4096];
	
	strcpy(url_string, SYNC_SOURCE);
	strcat(url_string, "/refresh");
	
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	[UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
	
	NSMutableURLRequest *request = [[[NSMutableURLRequest alloc] init] autorelease];
	[request setURL:[NSURL URLWithString:[[NSString alloc] initWithUTF8String:url_string]]];
	[request setHTTPMethod:@"GET"];
	NSHTTPURLResponse *response = [[NSURLResponse alloc] init];
	NSURLConnection *conn = [[NSURLConnection alloc] initWithRequest:request delegate:self];
	if (conn) {
		NSData *data = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:nil];
		if (data == nil || ([response statusCode] == 500)) {
			printf("Error doing refresh...\n");
			status = 1;
		}
	}
	[pool release];
	free(url_string);
	[UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
	return status;
}

- (int)initializeDatabaseConn {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString *path = [documentsDirectory stringByAppendingPathComponent:@"syncdb.sqlite"];

	return sqlite3_open([path UTF8String], &database);
}
	
- (void)fetchRecordsFromDatabase {
	[list release];
	self.list = [[NSMutableArray alloc] init];
	// Get the primary key for all objects.
	const char *sql = "SELECT id FROM object_values where update_type = 'query'";
	sqlite3_stmt *statement;
	pSyncObject *db_list;
	SyncObjectWrapper *newWrapper;
	int size = MAX_SYNC_OBJECTS;
	db_list = malloc(size*sizeof(pSyncObject));
	if (sqlite3_prepare_v2(database, sql, -1, &statement, NULL) == SQLITE_OK) {
		for (int i = 0; sqlite3_step(statement) == SQLITE_ROW; i++) {
			newWrapper = [[SyncObjectWrapper alloc] init];
			int primary_key = sqlite3_column_int(statement, 0);
			newWrapper.wrappedObject->_primary_key = primary_key;
			newWrapper.wrappedObject->_hydrated = 0;
			newWrapper.wrappedObject->_database = database;
			hydrate(newWrapper.wrappedObject);
			[list addObject:newWrapper];
				
		}
		sqlite3_finalize(statement);
	}
}


- (void)reloadTable
{
    [[(RootViewController *)[self.navigationController topViewController] tableView] reloadData];
}

// Creates a writable copy of the bundled default database in the application Documents directory.
- (void)createEditableCopyOfDatabaseIfNeeded {
    // First, test for existence.
    BOOL success;
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSError *error;
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString *writableDBPath = [documentsDirectory stringByAppendingPathComponent:@"syncdb.sqlite"];
    success = [fileManager fileExistsAtPath:writableDBPath];
    if (success) return;
    // The writable database does not exist, so copy the default to the appropriate location.
    NSString *defaultDBPath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"syncdb.sqlite"];
    success = [fileManager copyItemAtPath:defaultDBPath toPath:writableDBPath error:&error];
    if (!success) {
        NSAssert1(0, @"Failed to create writable database file with message '%@'.", [error localizedDescription]);
    }
}

- (NSUInteger)countOfList {
	return [list count];
}

- (id)objectInListAtIndex:(NSUInteger)theIndex {
	return [list objectAtIndex:theIndex];
}

- (void)getList:(id *)objsPtr range:(NSRange)range {
	[list getObjects:objsPtr range:range];
}

- (void)applicationDidFinishLaunching:(UIApplication *)application {
	
	// Initialize database connection and list of records
	[self createEditableCopyOfDatabaseIfNeeded];
	[self initializeDatabaseConn];
	[self fetchRecordsFromDatabase];
	[self reloadTable];
	
	// Startup the sync engine thread
	start_sync_engine(database);
	
	// Create the navigation and view controllers
	RootViewController *rootViewController = [[RootViewController alloc] initWithStyle:UITableViewStylePlain];
	UINavigationController *aNavigationController = [[UINavigationController alloc] initWithRootViewController:rootViewController];
	self.navigationController = aNavigationController;
	[aNavigationController release];
	[rootViewController release];
	
	// Configure and show the window
	[window addSubview:[navigationController view]];
	
	[window makeKeyAndVisible];
}

- (void)removeSyncObject:(SyncObjectWrapper *)oldSyncObject {
	add_delete_type_to_database(oldSyncObject.wrappedObject);
    [list removeObject:oldSyncObject];
	[self reloadTable];
}

- (void)applicationWillTerminate:(UIApplication *)application {
	
    finalize_statements();
	finalize_op_statements();
	
	// Stop the sync engine
	stop_sync_engine();
	
    // Close the database.
    if (sqlite3_close(database) != SQLITE_OK) {
        NSAssert1(0, @"Error: failed to close database with message '%s'.", sqlite3_errmsg(database));
    }
}

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    //[self performDehydrateOperations];
}

- (void)dealloc {
	[navigationController release];
	[window release];
	[list release];
	[super dealloc];
}

@end
