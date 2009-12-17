#import <Cocoa/Cocoa.h>
#import "LogController.h"
#import "SourceViewController.h"
#import "GdbConnection.h"
#import "TcpConnection.h"

@interface DebuggerController : NSObject {
//    IBOutlet LogController * gdbController;
	IBOutlet GdbConnection * gdbConnection;
    IBOutlet LogController * rubyController;
    IBOutlet SourceViewController * sourceController;

	TcpConnection * tcpConnection;
	
    IBOutlet id statusLabel;
    IBOutlet id gdbStatusLabel;

	IBOutlet id bpFile;
	IBOutlet id bpLine;

	IBOutlet id webView;

	IBOutlet id outlineView;
	
	bool gdbAttached,gdbScriptLoaded, building;
	NSString *pid, *gdbPid;
	NSString *currentFile;
}
- (IBAction)rubyInput:(id)sender;
- (IBAction)pause:(id)sender;
- (IBAction)resume:(id)sender;
- (IBAction)step:(id)sender;
- (IBAction)stepOut:(id)sender;

- (IBAction)selectFile:(id)sender;


- (IBAction)setRhodesApp:(id)sender;
- (IBAction)launchApp:(id)sender;

- (IBAction)saveFile:(id)sender;

- (IBAction)useGdbCheck:(id)sender;
- (IBAction)disconnect:(id)sender;

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification;

- (void)attachTail;
- (void)removeBreakpoint;
- (void)setBreakPoint:(NSString *)file atLine:(int) line;
@end
