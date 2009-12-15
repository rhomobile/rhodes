//
//  LogController.m
//
//

#import "LogController.h"

@implementation LogController
@synthesize delegate;
@synthesize outputTextView;
@synthesize task;
@synthesize pipe;
@synthesize inpipe;
@synthesize stdoutFileHandle;
@synthesize stdinFileHandle;
@synthesize defaultStringAttributes;
@synthesize isWaiting;
@synthesize finished;
@synthesize waitString;
@synthesize cmdQueue;

- (id) init
{
	self = [super init];
	if (self != nil) {
		[self setTask:nil];
		[self setPipe:nil];
		[self setStdoutFileHandle:nil];
		[self setDefaultStringAttributes:nil];
		[self setWaitString:@""];
		[self setCmdQueue:[NSMutableArray arrayWithCapacity:1]];
	}
	return self;
}

- (void) dealloc
{
	[self setTask:nil];
	[self setPipe:nil];
	[self setStdoutFileHandle:nil];
	[self setDefaultStringAttributes:nil];
	[super dealloc];
}

-(void) launchTask
{
	[self setPipe:[NSPipe pipe]];
	[self setInpipe:[NSPipe pipe]];
	
	// Catch the stdout in our pipe
	[self setStdoutFileHandle:[pipe fileHandleForReading]];
	[self setStdinFileHandle:[inpipe fileHandleForWriting]];
	[task setStandardOutput:pipe];
	[task setStandardError:pipe];
	[task setStandardInput:inpipe];
	
	[stdoutFileHandle readInBackgroundAndNotify];
	
	// Get a notification when the task ends
	[[NSNotificationCenter defaultCenter]
	 addObserver: self
	 selector: @selector(taskFinished:)
	 name: NSTaskDidTerminateNotification
	 object: task];
	
	// Listen for notifications from readInBackgroundAndNotify
	[[NSNotificationCenter defaultCenter]
	 addObserver: self
	 selector: @selector(gotData:)
	 name: NSFileHandleReadCompletionNotification
	 object: stdoutFileHandle];
	
	// Clear the output text view
	if(outputTextView) {
		[[[outputTextView textStorage] mutableString] setString:@""];
	
		[outputTextView setEditable:false];
	}
	
	[self appendString:@"Execution Started\n\n"];
	
	finished = false;
	// Run!
    [task launch];
}

- (void)taskFinished:(NSNotification *)notification
{
	// We're done - stop listening for notifications
	
	[[NSNotificationCenter defaultCenter] removeObserver: self
													name: NSFileHandleReadCompletionNotification
												  object: stdoutFileHandle];
    [[NSNotificationCenter defaultCenter] removeObserver: self
													name: NSTaskDidTerminateNotification
												  object: task];
	if ( delegate && [delegate respondsToSelector:@selector(logControllerTaskDidFinish:)] ) {
		[delegate logControllerTaskDidFinish:self];
	}
	[self appendString:@"\nExecution has terminated\n"];
	[self setFinished:true];
	[self setIsWaiting:false];

}

- (void)gotData: (NSNotification *)inNotification
{
	NSDictionary	*userInfoDictionary=NULL;
	NSData			*data=NULL;
	NSString		*stringData=NULL;
	
	userInfoDictionary = [inNotification userInfo];
	require(userInfoDictionary != NULL, exit);
	
	data = [userInfoDictionary objectForKey: NSFileHandleNotificationDataItem];
	require(data != NULL, exit);
	
	stringData = [[NSString alloc] initWithData: data encoding: NSUTF8StringEncoding];
	require(stringData != NULL, exit);
	
	// We got some data - append it to the end of the master script output
	//[self appendString:@"###"];
	[self appendString:stringData];
	
	if ([[self waitString] length] > 0) {
		if ([stringData rangeOfString:[self waitString]].location != NSNotFound) {
			if([cmdQueue count] > 0) {
				NSString *cmd = [cmdQueue lastObject];
				[self appendString:cmd];
				[[self stdinFileHandle] writeData:[cmd dataUsingEncoding:NSASCIIStringEncoding]];
				[cmdQueue removeLastObject];
			} else {
				isWaiting = true;
				if ( delegate && [delegate respondsToSelector:@selector(logControllerTaskIsWaiting:)] ) {
					[delegate logControllerTaskIsWaiting:self];
				}
				
			}
		}
	}	
exit:
	
	// Continue to listen for more data
	[stdoutFileHandle readInBackgroundAndNotify];
	
	if (stringData != NULL)
	{
		[stringData release];
	}
}

-(void)appendString:(NSString *)stringData
{
	NSAttributedString *attribString;
	if ( delegate && [delegate respondsToSelector:@selector(logController:willAppendNewString:)] ) {
		attribString = [delegate logController:self willAppendNewString:stringData];
	} else if (defaultStringAttributes) {
		attribString=[[[NSAttributedString alloc] initWithString:stringData attributes:defaultStringAttributes] autorelease];
	} else {
		attribString=[[[NSAttributedString alloc] initWithString:stringData] autorelease];
	}
	
	[self appendAttributedString:attribString];
}

-(void)appendAttributedString:(NSAttributedString *)stringData
{
	if( !stringData || [[stringData string] isEqualToString:@""])
		return;
	if(outputTextView) {
		[[outputTextView textStorage] beginEditing];
		[[outputTextView textStorage] appendAttributedString:stringData];
		[[outputTextView textStorage] endEditing];
		[outputTextView scrollRangeToVisible:NSMakeRange([[outputTextView textStorage] length],0)];
	}
}

-(void)clearTextView
{
	if(outputTextView) {
		NSAttributedString *attribString=[[NSAttributedString alloc] initWithString:@""];
		[[outputTextView textStorage] setAttributedString:attribString];
		[attribString release];
	}
}

-(void)sendcmd:(NSString *)cmd {
	[self sendCmd:cmd async:true];
}

-(void)sendCmd:(NSString *)cmd async:(BOOL)async {
	cmd = [cmd stringByAppendingString:@"\n"];
	if(async) {
		if([self isWaiting]) {
			[self setIsWaiting:false];
			[self appendString:cmd];
			[[self stdinFileHandle] writeData:[cmd dataUsingEncoding:NSASCIIStringEncoding]];
		} else {
			[cmdQueue insertObject:cmd atIndex:0];
		}
	} else {
		while (! [self isWaiting] ) { [[NSRunLoop currentRunLoop] runUntilDate:[NSDate dateWithTimeIntervalSinceNow:0.1]]; }
		[self setIsWaiting:false];
		[self appendString:cmd];
		[[self stdinFileHandle] writeData:[cmd dataUsingEncoding:NSASCIIStringEncoding]];
	}
}
@end
