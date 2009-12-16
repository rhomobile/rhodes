//
//  TcpConnection.h
//  Rhodes Debugger
//

#import <Cocoa/Cocoa.h>
#import "DebugConnection.h"

@interface TcpConnection : DebugConnection {
	CFSocketRef socket;
	NSFileHandle *listeningHandle, *dataHandle;
	NSString *recvData;
}

@end
