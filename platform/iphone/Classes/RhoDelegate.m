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
