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
