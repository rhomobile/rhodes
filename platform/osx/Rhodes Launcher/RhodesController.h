//
//  RhodesController.h
//  Runner
//
//  Created by rhomobile on 9/10/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface RhodesController : NSObject {
	IBOutlet NSWindow *window;
	IBOutlet NSTextField *downloadSize;
	
    NSMutableData *dataAccumulator;
	NSURLConnection *webConnection;
}

- (IBAction)reload:(id)sender;
- (void) applicationDidFinishLaunching:(NSNotification *)notification;

- (void) relaunchEmulator;


//The following are delegate methods for NSURLConnection
- (void) connection:(NSURLConnection *)connection didReceiveData:(NSData *)data;
- (void) connectionDidFinishLoading:(NSURLConnection *)connection;
- (NSCachedURLResponse *)connection:(NSURLConnection *)connection willCacheResponse:(NSCachedURLResponse *)cachedResponse;
- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error;

@end
