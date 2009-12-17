//
//  GdbConnection.m
//  Rhodes Debugger
//
//  Created by rhomobile on 12/14/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "GdbConnection.h"


@implementation GdbConnection
@synthesize pid;
@synthesize gdbPid;
@synthesize timer;

- (id) init
{
	self = [super init];
	if (self != nil) {
		[self setPid:@""];
		[self setGdbPid:@""];
		[self setTimer:nil];
		gdbAttached = FALSE;
		gdbScriptLoaded = FALSE;
		tailController = [[LogController alloc] init];
		[tailController setDelegate:self];
	}
	return self;
}

- (void)statusScanner:(id)userData {
	NSTask *task = [[NSTask alloc] init];
	[task setLaunchPath:@"/bin/sh"];
	NSArray *args = [NSArray arrayWithObjects:@"-c",@"ps xo pid,command | grep rhorunner.app | grep iPhone | grep -v grep | grep -v gdb",nil];
	[task setArguments: args];
	
	NSPipe *pipe = [NSPipe pipe];
	[task setStandardOutput:pipe];
	
	NSFileHandle *file = [pipe fileHandleForReading];
	[task launch];
	
	NSString *output = [[NSString alloc] initWithData:[file readDataToEndOfFile] encoding:NSUTF8StringEncoding];
    output = [output stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
	
	if ( [output length] < 1 && ![gdbController finished]) {
		[[gdbController task] terminate];
		[[tailController task] terminate];
        return;
	} 
	
	if ( [output length] > 0 && !gdbAttached) {
		NSArray *components = [output componentsSeparatedByString:@" "];
		pid = [components objectAtIndex:0];
		NSString *command = [output stringByReplacingOccurrencesOfString:pid withString:@""];
		[self attachGdbTo:[command stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] withPid:pid];
		if ( delegate && [delegate respondsToSelector:@selector(connecting:)] ) {
			[delegate connecting:self];
		}
		gdbAttached = TRUE;
		return;
	}

	if( gdbAttached && !gdbScriptLoaded && [gdbController isWaiting] ) {
		NSString * cmd = @"source \"";
		cmd = [cmd stringByAppendingString:[[[NSBundle mainBundle] bundlePath] stringByAppendingString:@"/Contents/Resources/ruby\""]];
        
		[gdbController sendCmd:cmd async:false];
		
		system("rm /tmp/ruby-debug*");
		
		[gdbController sendCmd:@"set objc-non-blocking-mode off" async:false];
		[gdbController sendCmd:@"redirect_stdout" async:false];
		[gdbController sendCmd:@"rb_trace" async:false];
		gdbScriptLoaded = true;
		
		[self attachTail];

		[self setIsConnected:true];

		if ( delegate && [delegate respondsToSelector:@selector(resumed:)] ) {
			[delegate resumed:self];
		}
		
		if ( delegate && [delegate respondsToSelector:@selector(connected:)] ) {
			[delegate connected:self];
		}
		
	}
	
	
}




- (void)attachGdbTo:(NSString*)file withPid:(NSString*)inPid {
	
	NSTask *ps = [[NSTask alloc] init];
   	[ps setLaunchPath:@"/usr/bin/gdb"];
	
	NSMutableArray *args = [[NSMutableArray alloc] init];
	[args addObject:file];
	[args addObject:inPid];
	
	[ps setArguments:args];
	
	[gdbController setTask:ps];
	[gdbController setWaitString:@"(gdb)"];
	[gdbController launchTask];
	
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
	
	return [components objectAtIndex:0];
}

- (void)attachTail {
	NSTask *ps = [[NSTask alloc] init];
   	[ps setLaunchPath:@"/usr/bin/tail"];
	
	NSMutableArray *args = [[NSMutableArray alloc] init];
	[args addObject:@"-f"];
	[args addObject:[NSString stringWithFormat:@"/tmp/ruby-debug.%@",pid]];
	
	[ps setArguments:args];
	
	
	[tailController setTask:ps];
	[tailController launchTask];
}



#pragma mark -- Debug Connection Implementation --

- (void) terminate {
	[[gdbController task] terminate];
}
-(void) setBreakPointInFile:(NSString*)file atLine:(int)line {
    NSString *lineString = [NSString stringWithFormat:@"%d",line];
	NSString *cmd = [NSString stringWithFormat:@"call (unsigned long)rb_eval_string(\"$_file = '%@'; $_line = %@;\")",file,lineString];
	if(![gdbController finished] && gdbScriptLoaded) {
		[self pause];
		[gdbController sendCmd:cmd async:true];
		[gdbController sendCmd:@"call (unsigned long)rb_eval_string(\"puts '-- Breakpoint set'\")" async:true];
		[gdbController sendCmd:@"rb_break" async:true];
		if ( delegate && [delegate respondsToSelector:@selector(resumed:)] ) {
			[delegate resumed:self];
		}
		
	}	
	
}



-(void) startWaiting {
	timer = [NSTimer scheduledTimerWithTimeInterval:0.5 target:self selector:@selector(statusScanner:) userInfo: nil repeats:true];
	[self setWaitForConnection:TRUE];
}

-(void) stopWaiting {
	if([self timer]){
		[[self timer] invalidate];
	}
	[self setWaitForConnection:FALSE];
}


- (void) pause {
	if(![gdbController isWaiting] && ! [gdbController finished] ) {
		NSString * myGdbPid = [self getGdbPid];
		NSString *cmd = @"/bin/kill -INT ";
		system([[cmd stringByAppendingString:myGdbPid] cStringUsingEncoding:NSASCIIStringEncoding]);
		
	}
}

- (void)step {
	if(![gdbController finished]) {
		[self pause];
		[gdbController sendCmd:@"rb_step" async:true];
		if ( delegate && [delegate respondsToSelector:@selector(resumed:)] ) {
			[delegate resumed:self];
		}
    }
}

- (void)stepOut {
	if(![gdbController finished]) {
		[self pause];
		[gdbController sendCmd:@"rb_finish" async:true];
		if ( delegate && [delegate respondsToSelector:@selector(resumed:)] ) {
			[delegate resumed:self];
		}
    }
}

-(void) resume {
	if(![gdbController finished]) {
		[self pause];
		[gdbController sendCmd:@"rb_break" async:true];
		if ( delegate && [delegate respondsToSelector:@selector(resumed:)] ) {
			[delegate resumed:self];
		}
	}
}


- (void) sendRubyCmd:(NSString *)rubyCmd {
	NSString *sanitized = [rubyCmd stringByReplacingOccurrencesOfString:@"\"" withString:@"\\\""];
    NSString *cmd = [NSString stringWithFormat:@"eval \"%@\"",sanitized];
	[self pause];
	
	[gdbController sendCmd:cmd async:true];
	
	
}

- (void) clearBreakPoints {
	if(![gdbController finished]) {
		[self pause];
		[gdbController sendCmd:@"rb_cont" async:true]; 
	}
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
		} else if ([sender isEqual:tailController]) {
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
					if ( delegate && [delegate respondsToSelector:@selector(stopInFile:atLine:sender:)] ) {
						[delegate stopInFile:file atLine:[stopLine intValue] sender:self];
					}
					
					
				}
			}
			
		}
	}
	if([sender isEqual: tailController]) {
		if ( delegate && [delegate respondsToSelector:@selector(rubyStdout:sender:)] ) {
			[delegate rubyStdout:[colorizedString mutableString] sender:self];
		}
		
	}
	// Finally, return our colorized version.
	return colorizedString;
}

- (void) logControllerTaskDidFinish:(id)sender {
	if([sender isEqual:gdbController]) {
		gdbAttached = FALSE;
		gdbScriptLoaded = FALSE;
		if ( delegate && [delegate respondsToSelector:@selector(disconnected:)] ) {
			[delegate disconnected:self];
		}
	}
}

- (void) logControllerTaskIsWaiting:(id)sender {
	if ( delegate && [delegate respondsToSelector:@selector(paused:)] ) {
		[delegate paused:self];
	}
}

#pragma mark -- Actions --

- (IBAction)gdbInput:(id)sender {
	if ([[sender stringValue] length] < 1) {return;}
	[self pause];
	if(![gdbController finished]) {
		[gdbController sendCmd:[sender stringValue] async:true];
	}
    [sender setStringValue:@""];
	
}

@end
