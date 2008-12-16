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
#include "Constants.h"
#include "SyncJSONReader.h"
#include "Utils.h"
//#import "rhosynctestappdelegate.h"

/* 
 * Pulls the latest object_values list 
 * for a given source and populates a list
 * of sync objects in memory and the database.
 */
char *fetch_remote_data(char *url_string) {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	[UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
	
	NSMutableURLRequest *request = [[[NSMutableURLRequest alloc] init] autorelease];
	NSError *error = nil;
	NSHTTPURLResponse* response;
	[request setURL:[NSURL URLWithString:[[NSString alloc] initWithUTF8String:url_string]]];
	[request setHTTPMethod:@"GET"];
	NSString *logData;
	
	NSURLConnection *conn = [[NSURLConnection alloc] initWithRequest:request delegate:nil];
	
	if (conn) {
		NSData *data = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
		NSInteger code = [response statusCode];
		if (error || code != 200) {
			NSLog(@"An error occured connecting to the sync source: %d returned", code);
			[UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
			[pool release];
			return NULL;
		} else {
			logData = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
		}
	}
	char *data = str_assign((char *)[logData UTF8String]);
	[UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
	[pool release];
	return data;
}

/*
 * Pushes changes from list to rhosync server
 */
int push_remote_data(char* url, char* data, size_t data_size) {
	if (data_size == 0 || data == NULL) {
		return SYNC_PUSH_CHANGES_OK;
	}
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	[UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
	NSMutableURLRequest *request = [[[NSMutableURLRequest alloc] init] autorelease];
	NSError *error = nil;
	NSHTTPURLResponse *response;
	NSString *linkString = [[NSString alloc] initWithUTF8String:url];
	NSString *postBody = [[[NSString alloc] initWithUTF8String:data] autorelease];
	[request setURL:[NSURL URLWithString:linkString]];
	[request setHTTPMethod:@"POST"];
	[request setHTTPBody:[postBody dataUsingEncoding:NSUTF8StringEncoding]];
	NSURLConnection *conn=[[NSURLConnection alloc] initWithRequest:request delegate:nil];
	if (conn) {
		NSData *returnData = [ NSURLConnection sendSynchronousRequest: request returningResponse:&response error: &error ];
		NSInteger code = [response statusCode];
		if (error || code != 200) {
			NSLog(@"An error occured connecting to the sync source: %d returned", code);
			[UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
			[pool release];
			return SYNC_PUSH_CHANGES_ERROR;
		} else {
			NSString *output = [NSString stringWithCString:[returnData bytes]];
			NSLog(@"RESPONSE: %@", output);
		}
	}
	[UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
	[pool release];
	return SYNC_PUSH_CHANGES_OK;
}