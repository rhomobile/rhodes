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

@end 

@interface DebugConnection : NSObject {
	IBOutlet NSObject <DebugConnectionDelegate> *delegate;
	bool waitForConnection;
	bool isConnected;
}
@property(assign) bool waitForConnection;
@property(assign) bool isConnected;

-(void) step;
-(void) stepOut;
-(void) pause;
-(void) resume;
-(void) setBreakPointInFile:(NSString*)file atLine:(int)line;



@end
