#import "DebuggerController.h"
#import "FileSystemItem.h"

@implementation DebuggerController

#pragma mark -- Timer Methods --

- (void)statusScanner:(id)userData {
	NSTask *task = [[NSTask alloc] init];
	[task setLaunchPath:@"/bin/sh"];
	NSArray *args = [NSArray arrayWithObjects:@"-c",@"ps xo pid,command | grep rhorunner.app | grep -v grep | grep -v gdb",nil];
	[task setArguments: args];
	
	NSPipe *pipe = [NSPipe pipe];
	[task setStandardOutput:pipe];
	
	NSFileHandle *file = [pipe fileHandleForReading];
	[task launch];
	
	NSString *output = [[NSString alloc] initWithData:[file readDataToEndOfFile] encoding:NSUTF8StringEncoding];
    output = [output stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
	
	if ( [gdbController isWaiting] || [gdbController finished] ) {
		[gdbStatusLabel setStringValue:@"Stopped"];
	} else {
		[gdbStatusLabel setStringValue:@"Running"];
	}
	
	
	
	if ( [output length] < 1 && ![gdbController finished]) {
		[[gdbController task] terminate];
		[[rubyController task] terminate];
        return;
	}
	
	if ( [gdbController finished] ) {
		gdbAttached = false;
		gdbScriptLoaded = false;
		
		[[sourceController lineNumberView] removeMarker:[[sourceController lineNumberView] markerAtLine:[[[sourceController lineNumberView] oldMarker] lineNumber]]];
		[[sourceController lineNumberView] setNeedsDisplay:YES];
		
		[statusLabel setStringValue:@"Waiting for Rhodes"];
		[statusLabel setTextColor:[NSColor redColor]];
	}
	
	if ( ! gdbAttached ) {
		
		if(building) {
			if([rubyController finished] ) {
				building = false;
			} else {
				return;
			}
		}
		
		if( [output length] > 0 ) {
			NSArray *components = [output componentsSeparatedByString:@" "];
			pid = [components objectAtIndex:0];
			NSString *command = [output stringByReplacingOccurrencesOfString:pid withString:@""];
			[self attachGdbTo:[command stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] withPid:pid];
			gdbAttached = true;
			[statusLabel setStringValue:@"Connecting"];
			[statusLabel setTextColor:[NSColor orangeColor]];
			return;
		}
		
	}
	if( gdbAttached && !gdbScriptLoaded && [gdbController isWaiting] ) {
		NSString * cmd = @"source \"";
		cmd = [cmd stringByAppendingString:[[[NSBundle mainBundle] bundlePath] stringByAppendingString:@"/Contents/Resources/ruby\""]];
        
		[gdbController sendCmd:cmd];
		
		system("rm /tmp/ruby-debug*");
		
		[gdbController sendCmd:@"set objc-non-blocking-mode off"];
		[gdbController sendCmd:@"redirect_stdout"];
		[gdbController sendCmd:@"rb_trace"];
		gdbScriptLoaded = true;
		
		
		
		[statusLabel setStringValue:@"Connected"];
		[statusLabel setTextColor:[NSColor blueColor]];
		
		[self attachTail];
		
	}
	
	
	
}

#pragma mark -- Helpers --

- (void) removeBreakpoint {
	if(![gdbController finished]) {
		[self pause:nil];
		[gdbController sendCmd:@"rb_cont"];
	}
}

- (void) setBreakPoint:(NSString *)file atLine:(int) line {
    NSString *lineString = [NSString stringWithFormat:@"%d",line];
	NSString *bFile = file;
	
	if(bFile == nil) {
		bFile = [[outlineView itemAtRow:[outlineView selectedRow]] fullPath];
		bFile = [[bFile componentsSeparatedByString:@"app/"] lastObject];
	}
	
	NSString *cmd = [NSString stringWithFormat:@"call (unsigned long)rb_eval_string(\"$_file = '%@'; $_line = %@;\")",bFile,lineString];
	if(![gdbController finished] && gdbScriptLoaded) {
		[self pause:nil];
		[gdbController sendCmd:cmd];
		[gdbController sendCmd:@"call (unsigned long)rb_eval_string(\"puts '-- Breakpoint set'\")"];
		[gdbController sendCmd:@"rb_break"];
	}	
}




- (NSString *) getGdbPid {
	NSTask *task = [[NSTask alloc] init];
	[task setLaunchPath:@"/bin/sh"];
	NSArray *args = [NSArray arrayWithObjects:@"-c",@"ps xo pid,command | grep rhorunner.app | grep gdb | grep -v grep ",nil];
	[task setArguments: args];
	
	NSPipe *pipe = [NSPipe pipe];
	[task setStandardOutput:pipe];
	
	NSFileHandle *file = [pipe fileHandleForReading];
	[task launch];
	
	NSString *output = [[NSString alloc] initWithData:[file readDataToEndOfFile] encoding:NSUTF8StringEncoding];
    output = [output stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];

	NSArray *components = [output componentsSeparatedByString:@" "];
	gdbPid = [components objectAtIndex:0];

	return [components objectAtIndex:0];;
}



- (void)attachTail {
	NSTask *ps = [[NSTask alloc] init];
   	[ps setLaunchPath:@"/usr/bin/tail"];
	
	NSMutableArray *args = [[NSMutableArray alloc] init];
	[args addObject:@"-f"];
	[args addObject:[NSString stringWithFormat:@"/tmp/ruby-debug.%@",pid]];
	
	[ps setArguments:args];
	
	
	[rubyController setTask:ps];
	[rubyController launchTask];
	[rubyController appendString:[NSString stringWithFormat:@"Attached to Rhodes ruby process %@\n\n",pid]];
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



- (void)attachGdbTo:(NSString*)file withPid:(NSString*)pid {

	NSTask *ps = [[NSTask alloc] init];
   	[ps setLaunchPath:@"/usr/bin/gdb"];

	NSMutableArray *args = [[NSMutableArray alloc] init];
	[args addObject:file];
	[args addObject:pid];
	
	[ps setArguments:args];

	[gdbController setTask:ps];
	[gdbController setWaitString:@"(gdb)"];
	[gdbController launchTask];

}



-(void) clearHighLight {
	NSString *file = [sourceController stringValue];
	NSMutableAttributedString *colorizedString = [[NSMutableAttributedString alloc] initWithString:file];
	[sourceController setAttributedString:colorizedString];
}

-(void) highLight:(NSString*) file atLine:(int)lineNum {
	if([file rangeOfString:@"app/"].location != NSNotFound) {
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

- (IBAction)step:(id)sender {
	if(![gdbController finished]) {
		[self pause:nil];
		[gdbController sendCmd:@"rb_step"];
    }
}

- (IBAction)stepOut:(id)sender {
	if(![gdbController finished]) {
		[self pause:nil];
		[gdbController sendCmd:@"rb_finish"];
    }
}



- (IBAction)pause:(id)sender {
	
	if(![gdbController isWaiting] && ! [gdbController finished] ) {
		NSString * gdbPid = [self getGdbPid];
		NSString *cmd = @"/bin/kill -INT ";
		system([[cmd stringByAppendingString:gdbPid] cString]);
		
	}
	
}
- (IBAction)resume:(id)sender {
	if(![gdbController finished]) {
		[self pause:nil];
		[gdbController sendCmd:@"rb_break"];
		[self clearHighLight];
	}
	
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


- (IBAction)gdbInput:(id)sender {
	if ([[sender stringValue] length] < 1) {return;}
	[self pause:nil];
	if(![gdbController finished]) {
		[gdbController sendCmd:[sender stringValue]];
	}
    [sender setStringValue:@""];
	
}

- (IBAction)rubyInput:(id)sender {
	if ([[sender stringValue] length] < 1) {return;}
	
	NSString *sanitized = [[sender stringValue] stringByReplacingOccurrencesOfString:@"\"" withString:@"\\\""];
    NSString *cmd = [NSString stringWithFormat:@"eval \"%@\"",sanitized];
	
	[self pause:nil];
	[rubyController appendString:@"\n"];
	[gdbController sendCmd:cmd];
	[sender setStringValue:@""];
	
	
}


#pragma mark -- Delegate Methods --

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
		
		
		if([sender isEqual:gdbController]) {
			NSRange gdbRange  = [thisLine rangeOfString:@"(gdb)"];
			if ( gdbRange.location != NSNotFound)
			{
				gdbRange.location += startOfLine;
				[attribDict setObject:[NSColor blueColor] forKey:NSForegroundColorAttributeName];
				[colorizedString addAttributes:attribDict range:gdbRange];
			}
		} else if ([sender isEqual:rubyController]) {
			NSRange gdbRange  = [thisLine rangeOfString:@"--"];
			if ( gdbRange.location == 0) {
				[attribDict setObject:[NSColor purpleColor] forKey:NSForegroundColorAttributeName];
				[colorizedString setAttributes:attribDict range:thisLineRange];
			}
			NSRange breakRange  = [thisLine rangeOfString:@"--Break"];
			NSRange stepRange  = [thisLine rangeOfString:@"--Step"];
			if ( breakRange.location != NSNotFound || stepRange.location != NSNotFound) {
				NSString *stopLine = [[thisLine componentsSeparatedByString:@":"] lastObject];
				NSRange evalRange = [thisLine rangeOfString:@"(eval)"];
				if(evalRange.location == NSNotFound) {
					NSString *file = [[[[thisLine componentsSeparatedByString:@" at "] lastObject] componentsSeparatedByString:@":"] objectAtIndex:0];
					[self clearHighLight];
					[self highLight:file atLine:[stopLine intValue]];
				}
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
	[gdbController setFinished:true];
	[[[[gdbController outputTextView] textStorage] mutableString] setString:@""];
	[[[[rubyController outputTextView] textStorage] mutableString] setString:@""];
	
    [[sourceController lineNumberView] setDebugger:self];
	[NSTimer scheduledTimerWithTimeInterval:0.5 target:self selector:@selector(statusScanner:) userInfo: nil repeats:true];
}

- (void)applicationWillTerminate:(NSNotification*)aNotification {
	
	if ( ![gdbController finished]) {
		[[gdbController task] terminate];
	}
	
}

- (void)awakeFromNib
{
	[sourceController awakeFromNib];
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *) sender { return YES; }



@end
