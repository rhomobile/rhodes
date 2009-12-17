#import "DebuggerController.h"
#import "FileSystemItem.h"
#import "MarkerLineNumberView.h"

@implementation DebuggerController

#pragma mark -- Timer Methods --




#pragma mark -- Helpers --

- (void) removeBreakpoint {
	[gdbConnection clearBreakPoints];
}


//called from MarkerLineNumberView when someone adds a marker
- (void) setBreakPoint:(NSString *)file atLine:(int) line {
	NSString *bFile = file;
	
	if(bFile == nil) {
		bFile = [[outlineView itemAtRow:[outlineView selectedRow]] fullPath];
		bFile = [[bFile componentsSeparatedByString:@"app/"] lastObject];
	}
	
	if([gdbConnection isConnected]) [gdbConnection setBreakPointInFile:bFile atLine:line];
	if([tcpConnection isConnected]) [tcpConnection setBreakPointInFile:bFile atLine:line];
	
}


- (void)buildAndRun:(NSString *)path {
	NSTask *ps = [[NSTask alloc] init];
   	[ps setLaunchPath:@"/bin/sh"];

	NSString *cmd = [NSString stringWithFormat:@"cd %@ && rake run:iphone 2>&1", path];
	
	NSMutableArray *args = [[NSMutableArray alloc] init];
	[args addObject:@"-c"];
	[args addObject:cmd];
	
	[ps setArguments:args];
	
	
	[rubyController setTask:ps];
	[rubyController launchTask];
	building = true;
}


- (void)attachTail {
	NSTask *ps = [[NSTask alloc] init];
   	[ps setLaunchPath:@"/usr/bin/tail"];
	
	NSMutableArray *args = [[NSMutableArray alloc] init];
	[args addObject:@"-f"];
	[args addObject:[NSString stringWithFormat:@"/tmp/ruby-debug.%@",[gdbConnection pid]]];
	
	[ps setArguments:args];
	
	
	[rubyController setTask:ps];
	[rubyController launchTask];
	[rubyController appendString:[NSString stringWithFormat:@"Attached to Rhodes ruby process %@\n\n",[gdbConnection pid]]];
}


-(void) clearHighLight {
	NSString *file = [sourceController stringValue];
	NSMutableAttributedString *colorizedString = [[NSMutableAttributedString alloc] initWithString:file];
	[sourceController setAttributedString:colorizedString];
}

-(void) highLight:(NSString*) file atLine:(int)lineNum {
	if([file rangeOfString:@"/app/"].location != NSNotFound) {
		NSString *fileContents = [sourceController stringValue];
		NSArray *lines = [fileContents componentsSeparatedByString:@"\n"];
		NSString *matchLine = [lines objectAtIndex:lineNum - 1];
		NSRange lineRange = [fileContents rangeOfString:matchLine];
		NSMutableAttributedString *colorizedString = [[NSMutableAttributedString alloc] initWithString:fileContents];
		
		NSMutableDictionary *attribDict = [NSMutableDictionary dictionary];
		[attribDict setObject:[NSColor cyanColor] forKey:NSBackgroundColorAttributeName];
		
		[colorizedString setAttributes:attribDict range:lineRange];
		
		[sourceController setAttributedString:colorizedString];
    }	
	
}


#pragma mark -- Actions --

- (IBAction)disconnect:(id)sender {
	[gdbConnection terminate];
	[tcpConnection terminate];
	
	[self disconnected:self];
}


- (IBAction)useGdbCheck:(id)sender {
	if([sender state]) {
		if(![gdbConnection waitForConnection]) {
			[gdbConnection startWaiting];
		}
	} else { 
		[gdbConnection stopWaiting];
	}
}

- (IBAction)step:(id)sender {
	if([gdbConnection isConnected]) [gdbConnection step];
	if([tcpConnection isConnected]) [tcpConnection step];
}

- (IBAction)stepOut:(id)sender {
	if([gdbConnection isConnected]) [gdbConnection stepOut];
	if([tcpConnection isConnected]) [tcpConnection stepOut];
}



- (IBAction)pause:(id)sender {
	if([gdbConnection isConnected]) [gdbConnection pause];
	if([tcpConnection isConnected]) [tcpConnection pause];
		
}
	
- (IBAction)resume:(id)sender {
	if([gdbConnection isConnected]) [gdbConnection resume];	
	if([tcpConnection isConnected]) [tcpConnection resume];	
	
	[self clearHighLight];
}

- (IBAction)selectFile:(id)sender {
	FileSystemItem *item = [sender itemAtRow:[sender selectedRow]];
	if([item numberOfChildren] == -1) {
		currentFile = [[item fullPath] copy];
		NSString *fileContents = [NSString stringWithContentsOfFile:currentFile encoding:NSASCIIStringEncoding error:nil];
		[sourceController setStringValue:fileContents];
	}
}

- (IBAction)saveFile:(id)sender {
	if(currentFile != nil) {
		[[sourceController stringValue] writeToFile:currentFile atomically:YES encoding:NSASCIIStringEncoding error:nil];
	}
	
}

- (IBAction)launchApp:(id)sender {
	[self buildAndRun:[[FileSystemItem rootItem] fullPath]];
	
}

- (IBAction)setRhodesApp:(id)sender {
	
	//	NSArray *fileTypes = [NSArray arrayWithObject:@"td"];
	NSOpenPanel *oPanel = [NSOpenPanel openPanel];
    int result;
	
	[oPanel setCanChooseFiles:false];
	[oPanel setCanChooseDirectories:true];
	[oPanel setAllowsMultipleSelection:false];
	
	result = [oPanel runModalForDirectory:NSHomeDirectory() file:nil];
	
	if(result == NSOKButton) {
		NSString *root = [[oPanel filenames] objectAtIndex:0];
		[FileSystemItem setRootItem:root]; 
		[outlineView reloadItem:nil reloadChildren:true];
		
	}
	
	
	
}



- (IBAction)rubyInput:(id)sender {
	if ([[sender stringValue] length] < 1) {return;}
	
	if([gdbConnection isConnected]) [gdbConnection sendRubyCmd: [sender stringValue]];
	if([tcpConnection isConnected]) [tcpConnection sendRubyCmd: [sender stringValue]];
	[rubyController appendString:@"\n"];
	[sender setStringValue:@""];
	
}


#pragma mark -- Delegate Methods --
- (void) rubyStdout:(NSString *)output sender:(id)sender {
	[rubyController appendString:output];
}


- (void) paused:(id)sender {
	[gdbStatusLabel setStringValue:@"Stopped"];	
}

- (void) resumed:(id)sender {
	[gdbStatusLabel setStringValue:@"Running"];	
}


- (void) disconnected:(id)sender {	
	[[sourceController lineNumberView] removeMarker:[[sourceController lineNumberView] markerAtLine:[[[sourceController lineNumberView] oldMarker] lineNumber]]];
	[[sourceController lineNumberView] setNeedsDisplay:YES];
	
	[statusLabel setStringValue:@"Waiting for Rhodes"];
	[statusLabel setTextColor:[NSColor redColor]];
	//[[rubyController task] terminate];
}

- (void) stopInFile:(NSString *)file atLine:(int)line sender:(id)sender {
	[self clearHighLight];
	[self highLight:file atLine:line];

}

- (void) connecting:(id)sender {
	[statusLabel setStringValue:@"Connecting"];
	[statusLabel setTextColor:[NSColor orangeColor]];
	
}
- (void) connected:(id)sender {
	[statusLabel setStringValue:@"Connected"];
	[statusLabel setTextColor:[NSColor blueColor]];
//	[self attachTail];
}
	

- (NSAttributedString *)logController:(id)sender willAppendNewString:(NSString *)plainString
{
	// NSLog(@"Colorizing string: %@", plainString);
	
	if( !plainString ) {
		return [[NSAttributedString alloc] initWithString:@""];
	}
	
	// First, we need a mutable attributed copy of it.
	// It'd be nice to use something like [NSMutableAttributedString mutableAttributedStringWithString:]
	// but that method doesn't exist. Make due with what I can.
	NSMutableAttributedString *colorizedString = [[NSMutableAttributedString alloc] initWithString:plainString];
	
	// We'll do all the math on plainString, do all the modifications on colorizedString.
	NSUInteger startOfLine=0, endOfLine=0;
	NSUInteger endOfString = [plainString length];
	NSRange restOfString=NSMakeRange(0, endOfString);
	
	
	// Go through plainString one line at a time
	for( startOfLine=0; endOfLine < endOfString; startOfLine=endOfLine)
	{
		// Find the next \n
		NSRange nextNewLineRange = [plainString
									rangeOfCharacterFromSet:[NSCharacterSet newlineCharacterSet]
									options:NSLiteralSearch range:restOfString];
		// If we can't find one, just pretend it's at the end
		if( nextNewLineRange.location == NSNotFound )
			nextNewLineRange.location = restOfString.location+restOfString.length;
		
		// The end of the line is right after the \n
		endOfLine = nextNewLineRange.location + nextNewLineRange.length;
		
		NSRange thisLineRange = NSMakeRange(startOfLine, endOfLine-startOfLine);
		NSString *thisLine = [plainString substringWithRange:thisLineRange];
		
		restOfString = NSMakeRange(endOfLine,endOfString-endOfLine);
		
		// The entire line will be black Monaco 10pt font
		NSMutableDictionary *attribDict = [NSMutableDictionary dictionary];
		[attribDict setObject:[NSFont fontWithName:@"Monaco" size:10.0f]
					   forKey:NSFontAttributeName];
		[attribDict setObject:[NSColor blackColor]
					   forKey:NSForegroundColorAttributeName];
		
		[colorizedString setAttributes:attribDict range:thisLineRange];
		
		
		if ([sender isEqual:rubyController]) {
			NSRange gdbRange  = [thisLine rangeOfString:@"--"];
			if ( gdbRange.location == 0) {
				[attribDict setObject:[NSColor purpleColor] forKey:NSForegroundColorAttributeName];
				[colorizedString setAttributes:attribDict range:thisLineRange];
			}
			
		}
	}
	
	// Finally, return our colorized version.
	return colorizedString;
}

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification {
	gdbAttached = false;
	gdbScriptLoaded = false;
	building = false;
	pid = @"";
	gdbPid = @"";
	currentFile = nil;
	[[[[rubyController outputTextView] textStorage] mutableString] setString:@""];
	
    [[sourceController lineNumberView] setDebugger:self];
//	[gdbConnection startWaiting];

	tcpConnection = [[TcpConnection alloc] init];
	[tcpConnection setDelegate: self];
	[tcpConnection startWaiting];
}

- (void)applicationWillTerminate:(NSNotification*)aNotification {
	[gdbConnection terminate];
	
}

- (void)awakeFromNib
{
	[sourceController awakeFromNib];
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *) sender { return YES; }



@end
