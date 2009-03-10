//
//  HttpCallback.m
//  rhorunner
//
//  Created by Vlad on 3/9/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#include <pthread.h>
#include <unistd.h>

#import "Notifications.h"
#import "HttpCallback.h"


static void* notification_main_routine(void* data) {
	if (data) {
		notification_t* pn = (notification_t*)data;
		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

		// Get message body and its length
		NSData *postBody = [NSData dataWithBytes:pn->params length:strlen(pn->params)];	
		NSString *postLength = [NSString stringWithFormat:@"%d", [postBody length]];
		NSString* postUrl = [NSString stringWithCString:pn->url encoding:[NSString defaultCStringEncoding]];
	
		// Create post request
		NSMutableURLRequest *request = [[NSMutableURLRequest alloc] init];
		[request setURL:[NSURL URLWithString:postUrl]];
		[request setHTTPMethod:@"POST"];
		[request setValue:postLength forHTTPHeaderField:@"Content-Length"];
		[request setHTTPBody:postBody];
	
		// Send request
		[NSURLConnection sendSynchronousRequest:request returningResponse:nil error:nil];	
	
		[pool drain];
		[pool release];
		free_notification_t(pn);
	}
	return NULL;
}


static void start_notification_thread(char* callback, char* message) {
    pthread_attr_t  attr;
    pthread_t       p_thread_id;
    int             return_val;
    int				thread_error;
	
	if (callback && message) {
		notification_t* pn = (notification_t*) malloc(sizeof(notification_t));
		pn->url = strdup(callback);
		pn->params = strdup(message);
	
		// Initialize thread
		return_val = pthread_attr_init(&attr);
		assert(!return_val);
		return_val = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		assert(!return_val);
	
		thread_error = pthread_create(&p_thread_id, &attr, &notification_main_routine, pn);
	
		return_val = pthread_attr_destroy(&attr);
		assert(!return_val);
	
		if (thread_error != 0) {
			free_notification_t(pn);
		}
	}
}

void perform_notification(char* callback, char* message) {
	start_notification_thread(callback, message); 
}


