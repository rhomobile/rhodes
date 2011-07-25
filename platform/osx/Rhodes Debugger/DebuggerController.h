/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

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
