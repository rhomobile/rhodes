#import <Cocoa/Cocoa.h>

@interface DebuggerController : NSObject {
    IBOutlet id gdbController;
    IBOutlet id rubyController;
    IBOutlet id sourceController;
	
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
- (IBAction)setBp:(id)sender;
- (IBAction)step:(id)sender;
- (IBAction)stepOut:(id)sender;

- (IBAction)selectFile:(id)sender;


- (IBAction)setRhodesApp:(id)sender;
- (IBAction)launchApp:(id)sender;

- (IBAction)saveFile:(id)sender;

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification;
- (void)statusScanner:(id)userData;

@end
