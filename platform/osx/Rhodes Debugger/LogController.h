//
//  LogController.h
//

//	Purpose: A LogController is used to execute a task and have the output displayed in an
//	NSTextView. The best way to do this is to instantiate a LogController object in Interface 
//	Builder and create the following connections:
//		LogController* IBOutlet in your app's controller -> Instantiated object
//		Instantiated object's outputTextView -> an NSTextView in your app's window
//		Instantiated object's delegate -> your app's controller
//

#import <Cocoa/Cocoa.h>

// Tell the compiler about what our delegate could possibly implement.
// If you don't do this, you get compiler warnings when you try to call the delegate methods.
@protocol LogControllerDelegate 
@optional
- (void) logControllerTaskDidFinish:(id)sender;
- (void) logControllerTaskIsWaiting:(id)sender;
- (NSAttributedString *) logController:(id)sender willAppendNewString:(NSString *)plainString;
@end 

@interface LogController : NSObject {
	
	IBOutlet NSTextView *outputTextView;
	// Delegate implements the LogControllerDelegate protocol
	IBOutlet NSObject <LogControllerDelegate> *delegate;
	
	NSTask *task;
	NSPipe *pipe;
	NSPipe *inpipe;
	bool isWaiting, finished;
	NSFileHandle *stdoutFileHandle;
	NSFileHandle *stdinFileHandle;
	NSDictionary *defaultStringAttributes;
	NSString *waitString;
	NSMutableArray *cmdQueue;
}
@property(retain) NSTextView *outputTextView;
@property(assign) NSObject *delegate;
@property(retain) NSTask *task;
@property(retain) NSPipe *pipe;
@property(retain) NSPipe *inpipe;
@property(assign) bool isWaiting;
@property(assign) bool finished;
@property(retain) NSFileHandle *stdoutFileHandle;
@property(retain) NSFileHandle *stdinFileHandle;
@property(retain) NSDictionary *defaultStringAttributes;
@property(retain) NSString *waitString;
@property(retain) NSMutableArray *cmdQueue;

-(void) launchTask;
-(void) taskFinished:(NSNotification *)notification;
-(void) gotData:(NSNotification *)notification;
-(void) appendString:(NSString *)stringData;
-(void) appendAttributedString:(NSAttributedString *)stringData;
-(void) clearTextView;
-(void) sendCmd:(NSString *)cmd async:(BOOL)async;
-(void) sendcmd:(NSString *)cmd;

@end