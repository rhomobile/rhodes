//
//  DebugConnection.h
//  Rhodes Debugger
//

#import <Cocoa/Cocoa.h>

@protocol DebugConnectionDelegate 
@optional
- (void) stopInFile:(NSString *)file atLine:(int)line sender:(id)sender;
- (void) connecting:(id)sender;
- (void) connected:(id)sender;
- (void) disconnected:(id)sender;
- (void) paused:(id)sender;
- (void) resumed:(id)sender;
- (void) rubyStdout:(NSString *)output sender:(id)sender;
@end 

@interface DebugConnection : NSObject {
	IBOutlet NSObject <DebugConnectionDelegate> *delegate;
	bool waitForConnection;
	bool isConnected;
}
@property(assign) bool waitForConnection;
@property(assign) bool isConnected;
@property(assign) NSObject <DebugConnectionDelegate> *delegate;

- (void) step;
- (void) stepOut;
- (void) pause;
- (void) resume;
- (void) setBreakPointInFile:(NSString*)file atLine:(int)line;
- (void) sendRubyCmd:(NSString *)rubyCmd;
- (void) clearBreakPoints;
- (void) terminate;
- (void) startWaiting;
- (void) stopWaiting;

@end
