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
#include "SyncUtil.h"
#include "SyncEngine.h"

extern char *get_session(const char *url_string);
extern int logged_in();
extern void logout();

/* 
 * Pulls the latest object_values list 
 * for a given source and populates a list
 * of sync objects in memory and the database.
 */
char *fetch_remote_data(char *url_string) {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	char *session = NULL;
	session = str_assign(get_session(url_string));
	if (session || strstr(url_string, "clientcreate")) {
		printf("Fetching data from %s\n", url_string);
		char *ret_data;
		[UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
		NSURL *url = [NSURL URLWithString:[[[NSString alloc] initWithUTF8String:url_string] autorelease]];
		
		NSMutableURLRequest *request = [[[NSMutableURLRequest alloc] init] autorelease];
		NSError *error = nil;
		NSHTTPURLResponse* response;
		[request setURL:url];
		[request setHTTPMethod:@"GET"];
		if (session) {
			[request setValue:[[[NSString alloc] initWithUTF8String:session] autorelease] forHTTPHeaderField:@"Cookie"];
		}
		
		NSString *logData;

  NSData *data = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
  NSInteger code = [response statusCode];
  NSInteger errorCode = [error code];
  code = [response statusCode];
  [UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
  if (code != 200) {
    NSLog(@"An error occured connecting to the sync source: %i returned", code);
    if (errorCode == NSURLErrorUserCancelledAuthentication || 
      errorCode == NSURLErrorUserAuthenticationRequired) {
      logout();
    }
    if (session) free(session);
    [pool release];
    return NULL;
  }  else {
    logData = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
  }
		ret_data = str_assign((char *)[logData UTF8String]);
		[UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
		if (session) free(session);
		[pool release];
		return ret_data;
	} else {
		if (session) free(session);
		[pool release];
		return NULL;
	}
}

/*
 * Login with given login/password
 * If return success if cookie is stored into shared
 * cookie storage NSHTTPCookieStorage
 */
int login(const char *login, const char *password) {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	pSource *source_list;
	char *session = NULL;
	int i, source_length, cookie_size = 0;
	source_list = malloc(MAX_SOURCES*sizeof(pSource));
	source_length = get_sources_from_database(source_list, get_database(), MAX_SOURCES);
	//pthread_mutex_lock(&sync_mutex);
	for(i = 0; i < source_length; i++) {
		char login_string[4096] = "";
		sprintf(login_string, 
				"%s/client_login", 
				source_list[i]->_source_url);
		session = str_assign(get_session(source_list[i]->_source_url));
		if (!session) {
			[UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
			NSMutableURLRequest *request = [[[NSMutableURLRequest alloc] init] autorelease];
			NSError *error = [[[NSError alloc] init] autorelease];
			NSHTTPURLResponse *response = [[[NSHTTPURLResponse alloc] init] autorelease];
			NSString *linkString = [[NSString alloc] initWithUTF8String:login_string];
			NSMutableString *postBody = [[NSMutableString alloc] initWithString:@"login="];
			[postBody appendString:[[NSString alloc] initWithUTF8String:login]];
			[postBody appendString:@"&password="];
			[postBody appendString:[[NSString alloc] initWithUTF8String:password]];
			[postBody appendString:@"&remember_me=1"];
			[request setURL:[NSURL URLWithString:linkString]];
			[request setHTTPMethod:@"POST"];
			[request setHTTPBody:[postBody dataUsingEncoding:NSUTF8StringEncoding]];
			NSURLConnection *conn=[[NSURLConnection alloc] initWithRequest:request delegate:nil];
			if (conn) {
				[NSURLConnection sendSynchronousRequest: request returningResponse:&response error: &error];
				NSInteger errorCode = [error code];
				NSInteger code = [response statusCode];
				[UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
				if (code != 200) {
					NSLog(@"An error occured connecting to the sync source: %i returned", code);
					if (errorCode == NSURLErrorUserCancelledAuthentication || 
						errorCode == NSURLErrorUserAuthenticationRequired) {
						logout();
					}
				} else {
					NSHTTPCookieStorage *store = [NSHTTPCookieStorage sharedHTTPCookieStorage];
					NSArray *cookies = [NSHTTPCookie cookiesWithResponseHeaderFields:[(NSHTTPURLResponse*)response allHeaderFields] 
																			  forURL:[NSURL URLWithString:linkString]];
					NSEnumerator *c = [cookies objectEnumerator];
					id cookie;
					while (cookie = [c nextObject]) {
						NSLog(@"Storing Cookie: Name: %@, Value: %@",
							  [cookie name],
							  [cookie value]);				
						[store setCookie:cookie];
						cookie_size++;
					}
				}
			}
		} else {
			printf("Found existing session for url...\n");
		}
		if (session) free(session);
	}
	//pthread_mutex_unlock(&sync_mutex);
	if (cookie_size == 0) {
		[pool release];
		return 0;
	} else {
		[pool release];
		return 1;
	}
}

/*
 * Pushes changes from list to rhosync server
 */
int push_remote_data(char* url, char* data, size_t data_size) {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	if (data_size == 0 || data == NULL) {
		[pool release];
		return SYNC_PUSH_CHANGES_OK;
	}
	char *session = str_assign(get_session(url));
	if (session) {
		[UIApplication sharedApplication].networkActivityIndicatorVisible = YES;
		NSMutableURLRequest *request = [[[NSMutableURLRequest alloc] init] autorelease];
		NSError *error = nil;
		NSHTTPURLResponse *response;
		NSString *linkString = [[NSString alloc] initWithUTF8String:url];
		NSString *postBody = [[NSString alloc] initWithUTF8String:data];
		[request setURL:[NSURL URLWithString:linkString]];
		[request setHTTPMethod:@"POST"];
		if (session) {
			[request setValue:[[NSString alloc] initWithUTF8String:session] forHTTPHeaderField:@"Cookie"];
		}
		[request setHTTPBody:[postBody dataUsingEncoding:NSUTF8StringEncoding]];
		NSURLConnection *conn=[[NSURLConnection alloc] initWithRequest:request delegate:nil];
		if (conn) {
			NSData *returnData = [ NSURLConnection sendSynchronousRequest: request returningResponse:&response error: &error ];
			NSInteger errorCode = [error code];
			NSInteger code = [response statusCode];
			[UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
			if (code != 200) {
				NSLog(@"An error occured connecting to the sync source: %i returned", code);
				if (errorCode == NSURLErrorUserCancelledAuthentication || 
					errorCode == NSURLErrorUserAuthenticationRequired) {
					logout();
				}
				if (session) free(session);
				[pool release];
				return SYNC_PUSH_CHANGES_ERROR;
			} else {
				NSString *output = [NSString stringWithCString:[returnData bytes]];
				NSLog(@"RESPONSE: %@", output);
			}
		}
		[UIApplication sharedApplication].networkActivityIndicatorVisible = NO;
		if (session) free(session);
		[pool release];
		return SYNC_PUSH_CHANGES_OK;
	} else {
		if (session) free(session);
		[pool release];
		return SYNC_PUSH_CHANGES_ERROR;
	}
}

NSArray *get_all_cookies() {
	NSHTTPCookieStorage *cookieStore = [NSHTTPCookieStorage sharedHTTPCookieStorage];
	NSArray *cookies = [cookieStore cookies];
	return cookies;
}

int logged_in() {
	int i,retval = 0;
	NSArray *cookies;
	cookies = get_all_cookies();
	int count = [cookies count];
	// Iterate over all cookies and see if we have a rhosync_session
	for (i = 0; i < count; i++) {
		if ([[[cookies objectAtIndex:i] name] isEqualToString:@"rhosync_session"]) retval = 1;
	}
	return retval;
}

void logout() {
	int i;
	NSArray *cookies;
	cookies = get_all_cookies();
	int count = [cookies count];
	for (i = 0; i < count; i++) {
		NSHTTPCookie *cookie = [cookies objectAtIndex:i];
		NSString *name = [cookie name];
		if ([name isEqualToString:@"rhosync_session"] || [name isEqualToString:@"auth_token"]) {
			[[NSHTTPCookieStorage sharedHTTPCookieStorage] deleteCookie:cookie];
		}
	}
}

/*
 * Retrieve cookie from shared cookie storage
 */
char *get_session(const char *url_string) {
	NSURL *url = [[NSURL alloc] initWithString:[[NSString alloc] initWithUTF8String:url_string]];
	NSHTTPCookieStorage *cookieStore = [NSHTTPCookieStorage sharedHTTPCookieStorage];
	NSArray *cookies = [cookieStore cookiesForURL:url];
	char *session =  (char *)[[[NSHTTPCookie requestHeaderFieldsWithCookies:cookies] objectForKey:@"Cookie"] UTF8String];
	return session == NULL || strcmp(session,"") == 0 ? NULL : session;
}
