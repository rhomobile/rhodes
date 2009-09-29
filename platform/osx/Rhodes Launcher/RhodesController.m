//
//  RhodesController.m
//  Runner
//
//  Created by rhomobile on 9/10/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "RhodesController.h"


@implementation RhodesController

- (void) relaunchEmulator {
	NSString *command = @"ruby \"";
	char *cmd = (char *) [[command stringByAppendingString:[[[NSBundle mainBundle] bundlePath] stringByAppendingString:@"/Contents/Resources/launch.rb\""]] UTF8String];
	system(cmd);
	
}


- (void) applicationDidFinishLaunching:(NSNotification *)notification {
	//[window orderOut:nil];
	
	dataAccumulator = [[NSMutableData alloc] init];
	webConnection = NULL;
	
    if( ! [[NSFileManager defaultManager] fileExistsAtPath:@"/Developer/Platforms/iPhoneSimulator.platform/Developer/Applications/iPhone Simulator.app"] ) {
		
		NSRunAlertPanel(@"Error",@"The iPhone SDK must be installed to use this application.\n\nYou can find it at: http://developer.apple.com", @"OK",nil,nil);
		
		[[NSApplication sharedApplication] terminate:nil];

	}
	
	
    [self relaunchEmulator];	
	
	
}

- (IBAction)reload:(id)sender {
	
	NSString *configFile = [NSString stringWithContentsOfFile:[[[NSBundle mainBundle] bundlePath] stringByAppendingPathComponent:@"/Contents/Resources/rhorunner.app/apps/rhoconfig.txt"]];
	NSScanner *scanner = [NSScanner scannerWithString:configFile];
	NSString *url;
	NSString *key = @"rhobundle_zip_url = '";
	NSCharacterSet *singleQuote = [NSCharacterSet characterSetWithCharactersInString:@"'"];

	
	[scanner scanUpToString:key intoString:NULL];
	[scanner scanString:key intoString:NULL];
	[scanner scanUpToCharactersFromSet:singleQuote intoString:&url];
	
	[downloadSize setStringValue:@"Contacting Server"];

	NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:url] cachePolicy: NSURLRequestReloadIgnoringLocalAndRemoteCacheData timeoutInterval:200.0];

	webConnection = [NSURLConnection connectionWithRequest:request delegate: self];
	
}

- (void) connection:(NSURLConnection *)connection didReceiveData:(NSData *)data {
	
	[dataAccumulator appendData:data];
	[downloadSize setStringValue:[NSString stringWithFormat:@"Downloaded: %ik",[dataAccumulator length] / 1024]];

}
- (void) connectionDidFinishLoading:(NSURLConnection *)connection {
    NSString *path = [[[NSBundle mainBundle] bundlePath] stringByAppendingPathComponent:@"/Contents/Resources/rhorunner.app/rhobundle.zip"];
	[dataAccumulator writeToFile:path atomically:YES];

	[downloadSize setStringValue:@"Download Complete"];
	
	[dataAccumulator release];
	dataAccumulator = [[NSMutableData alloc] init];

	NSString *command = @"ruby \"";
	char *cmd = (char *) [[command stringByAppendingString:[[[NSBundle mainBundle] bundlePath] stringByAppendingString:@"/Contents/Resources/unzipbundle.rb\""]] UTF8String];
	system(cmd);
    [self relaunchEmulator];
	
}

- (NSCachedURLResponse *)connection:(NSURLConnection *)connection willCacheResponse:(NSCachedURLResponse *)cachedResponse {
	return nil;
}

- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error {
    NSString *message = @"There was an error downloading the bundle, please try again later. \n\nError: ";
	
	[downloadSize setStringValue:@""];

	NSRunAlertPanel(@"Error",[message stringByAppendingString:[error localizedDescription]], @"OK",nil,nil);

}

@end
