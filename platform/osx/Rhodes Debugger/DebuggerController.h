#import <Cocoa/Cocoa.h>

@interface DebuggerController : NSObject {
    IBOutlet id gdbController;
    IBOutlet id rubyController;
    IBOutlet id statusLabel;
    IBOutlet id gdbStatusLabel;
    IBOutlet id preserveBreakpoint;

	IBOutlet id bpFile;
	IBOutlet id bpLine;

	
	bool gdbAttached,gdbScriptLoaded;
	NSString *pid, *gdbPid;
}
- (IBAction)gdbInput:(id)sender;
- (IBAction)rubyInput:(id)sender;
- (IBAction)pause:(id)sender;
- (IBAction)resume:(id)sender;
- (IBAction)setBp:(id)sender;
- (IBAction)step:(id)sender;
- (IBAction)stepOut:(id)sender;


- (void)applicationDidFinishLaunching:(NSNotification*)aNotification;
- (void)statusScanner:(id)userData;

@end
