//
//  RhoDelegate.m
//  rhorunner
//
//  Created by lars on 8/11/09.
//  Copyright 2009 Home. All rights reserved.
//

#import "RhoDelegate.h"

@implementation RhoDelegate

@synthesize postUrl;
/*
- (void)doCallback:(NSString*)message {
	// Create post body
	NSData* postBody = [message dataUsingEncoding:NSUTF8StringEncoding];
	// Start notification thread	
	[NSThread detachNewThreadSelector:@selector(NotifyViewThreadRoutine:)
							 toTarget:self withObject:postBody];		
}

- (void)NotifyViewThreadRoutine:(id)object {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    
	// Get message body and its length
	NSData *postBody = (NSData*)object;	
	NSString *postLength = [NSString stringWithFormat:@"%d", [postBody length]];
	
	// Create post request
	NSMutableURLRequest *request = [[NSMutableURLRequest alloc] init];
	[request setURL:[NSURL URLWithString:postUrl]];
	[request setHTTPMethod:@"POST"];
	[request setValue:postLength forHTTPHeaderField:@"Content-Length"];
	[request setHTTPBody:postBody];
	
	// Send request
	[NSURLConnection sendSynchronousRequest:request returningResponse:nil error:nil];
	
    [pool release];
}*/

@end
