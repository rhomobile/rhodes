#import <Cocoa/Cocoa.h>
#import "LogController.h"
#import "SourceViewController.h"

@interface DebuggerController : NSObject {
    IBOutlet LogController * gdbController;
    IBOutlet LogController * rubyController;
    IBOutlet SourceViewController * sourceController;
	
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
- (IBAction)gdbInput:(id)sender;
- (IBAction)rubyInput:(id)sender;
- (IBAction)pause:(id)sender;
- (IBAction)resume:(id)sender;
- (IBAction)step:(id)sender;
- (IBAction)stepOut:(id)sender;

- (IBAction)selectFile:(id)sender;


- (IBAction)setRhodesApp:(id)sender;
- (IBAction)launchApp:(id)sender;

- (IBAction)saveFile:(id)sender;

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification;
- (void)statusScanner:(id)userData;

- (void)attachGdbTo:(NSString*)file withPid:(NSString*)pid;
- (void)attachTail;
- (void)removeBreakpoint;
- (void)setBreakPoint:(NSString *)file atLine:(int) line;
@end
