//
//  DebugConnection.m
//  Rhodes Debugger
//
//  Created by rhomobile on 12/14/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "DebugConnection.h"


@implementation DebugConnection
@synthesize waitForConnection;
@synthesize isConnected;
@synthesize delegate;

- (id) init
{
	self = [super init];
	if (self != nil) {
		[self setWaitForConnection:FALSE];
		[self setIsConnected:FALSE];
	}
	return self;
}

-(void) step {
}

-(void) stepOut {
}

-(void) pause {
}

-(void) resume {
}

-(void) setBreakPointInFile:(NSString*)file atLine:(int)line {
}

- (void) sendRubyCmd:(NSString *)rubyCmd {
}

- (void) clearBreakPoints {
}

- (void) terminate {
}

- (void) startWaiting {
}

- (void) stopWaiting {
}

@end
