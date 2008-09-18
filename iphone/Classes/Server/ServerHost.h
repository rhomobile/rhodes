//
//  ServerHost.h
//  Browser
//
//  Created by adam blum on 9/10/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

@interface ServerHost : NSObject
{
    CFRunLoopRef runLoop;
}

- (void) start;
//TODO - implement pause and stop operations and graceful termination of the thread...
- (void) stop;

@end