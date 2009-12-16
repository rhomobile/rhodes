//
//  GdbConnection.h
//  Rhodes Debugger
//
//  Created by rhomobile on 12/14/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "DebugConnection.h"
#import "LogController.h"

@interface GdbConnection : DebugConnection {
    IBOutlet LogController * gdbController;
	LogController *tailController;

	NSString *pid, *gdbPid;
	NSTimer *timer;
	bool gdbAttached,gdbScriptLoaded;
}
@property(retain) NSString *pid;
@property(retain) NSString *gdbPid;
@property(retain) NSTimer *timer;


- (IBAction)gdbInput:(id)sender;
- (void)attachGdbTo:(NSString*)file withPid:(NSString*)inPid;
- (NSString *) getGdbPid;
- (void)attachTail;
@end
