/*
 *  SyncManager.m
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
#import <SystemConfiguration/SystemConfiguration.h>
#import <Foundation/Foundation.h>
#import "SyncManager.h"
#include "Constants.h"
#include "SyncJSONReader.h"
//#import "rhosynctestappdelegate.h"

/* 
 * Pulls the latest object_values list 
 * for a given source and populates a list
 * of sync objects in memory and the database.
 */
int fetch_remote_changes(pSyncObject *list, sqlite3 *database) {
	
	char url_string[4096];
	int max_size = 100;
	int *source_list;
	source_list = malloc(max_size*sizeof(int));
	int source_length = get_source_ids_from_database(source_list, database, max_size);
	int available = 0;
	int offset = 0;
	printf("Iterating over %i sources...\n", source_length);
	
	/* iterate over each source id and do a fetch */
	for(int i = 0; i < source_length; i++) {
		sprintf(url_string, "%s%d%s", SYNC_SOURCE, source_list[i], SYNC_SOURCE_FORMAT);
		printf("url_string: %s\n", url_string);
		
		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
		[UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
		
		NSMutableURLRequest *request = [[[NSMutableURLRequest alloc] init] autorelease];
		NSError *error = nil;
		NSHTTPURLResponse* response;
		[request setURL:[NSURL URLWithString:[[NSString alloc] initWithUTF8String:url_string]]];
		[request setHTTPMethod:@"GET"];
		
		NSURLConnection *conn = [[NSURLConnection alloc] initWithRequest:request delegate:nil];
		
		if (conn) {
			NSData *data = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
			NSInteger code = [response statusCode];
			if (error || code != 200) {
				NSLog(@"An error occured connecting to the sync source: %d returned", code);
			} else {
				NSString *logData = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
				char *json_string = (char *)[logData UTF8String];
				int size = MAX_SYNC_OBJECTS;
				
				// Initialize parsing list and call json parser
				available = parse_json_list(list, json_string, size);
				printf("Parsed %i records from sync source...\n", available);
				if(available > 0) {
					delete_from_database_by_source(database, source_list[i]);
					for(int i = offset; i < available; i++) {
						list[i]->_database = database;
						insert_into_database(list[i]);
						dehydrate(list[i]);
						offset++;
					}
				}
			}
			
		}
		[pool release];
		[UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
	}
	/* free the in-memory list after populating the database */
	free_ob_list(list, available);
	free(source_list);
	return available;
}

/*
 * Pushes changes from list to rhosync server
 */
int push_remote_changes(pSyncOperation *list, int size) {
	if (size == 0) {
		return SYNC_PUSH_CHANGES_OK;
	}
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	NSMutableURLRequest *request = [[[NSMutableURLRequest alloc] init] autorelease];
	NSError *error = nil;
	NSHTTPURLResponse *response;
	//Get the link from the first item in the list
	NSString *linkString = [[NSString alloc] initWithUTF8String:list[0]->_uri];
	NSMutableString *postBody = [[NSMutableString alloc] init];
	for (int i = 0; i < size; i++) {
		[postBody appendString:[[NSString alloc] initWithUTF8String:list[i]->_post_body]];
		if (i != (size - 1)) {
			[postBody appendString:@"&"];
		}
	}
	[request setURL:[NSURL URLWithString:linkString]];
	[request setHTTPMethod:@"POST"];
	[request setHTTPBody:[postBody dataUsingEncoding:NSUTF8StringEncoding]];
	NSURLConnection *conn=[[NSURLConnection alloc] initWithRequest:request delegate:nil];
	if (conn) {
		NSData *returnData = [ NSURLConnection sendSynchronousRequest: request returningResponse:&response error: &error ];
		NSInteger code = [response statusCode];
		if (error || code != 200) {
			NSLog(@"An error occured connecting to the sync source: %d returned", code);
			[pool release];
			return SYNC_PUSH_CHANGES_ERROR;
		} else {
			NSString *output = [NSString stringWithCString:[returnData bytes]];
			NSLog(@"RESPONSE: %@", output);
		}
	}
	[pool release];
	return SYNC_PUSH_CHANGES_OK;
}

void populate_list(sqlite3 *database) {
	/*rhosynctestappdelegate *appDelegate = (rhosynctestappdelegate *)[[UIApplication sharedApplication] delegate];
	[appDelegate.list release];
	appDelegate.list = [[NSMutableArray alloc] init];
	pSyncObject *db_list = malloc(MAX_SYNC_OBJECTS*sizeof(pSyncObject));
	int available = fetch_objects_from_database(database, db_list);
	for (int i = 0; i < available; i++) {
		SyncObjectWrapper *newWrapper = [[SyncObjectWrapper alloc] init];
		newWrapper.wrappedObject = db_list[i];
		[appDelegate.list addObject:newWrapper];
	}*/
}