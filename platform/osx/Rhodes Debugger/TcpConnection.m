//
//  TcpConnection.m
//  Rhodes Debugger
//
//  Created by rhomobile on 12/15/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "TcpConnection.h"
#import <sys/socket.h>
#import <netinet/in.h>


@implementation TcpConnection
- (id) init
{
	self = [super init];
	if (self != nil) {
		socket = nil;
		recvData = @"";
	}
	return self;
}

-(void) step {
	[dataHandle writeData:[@"STEP" dataUsingEncoding:NSASCIIStringEncoding]];
}

-(void) stepOut {
}

-(void) pause {
}

-(void) resume {
	[dataHandle writeData:[@"CONT" dataUsingEncoding:NSASCIIStringEncoding]];
}

-(void) setBreakPointInFile:(NSString*)file atLine:(int)line {
	[dataHandle writeData:[[NSString stringWithFormat:@"BP:%@:%d\n",file,line] dataUsingEncoding:NSASCIIStringEncoding]];	
}

- (void) sendRubyCmd:(NSString *)rubyCmd {
	[dataHandle writeData:[[NSString stringWithFormat:@"EV:%@\n",rubyCmd] dataUsingEncoding:NSASCIIStringEncoding]];	

}

- (void) clearBreakPoints {
}

- (void) terminate {
	[dataHandle closeFile];
	[[NSNotificationCenter defaultCenter]
	 removeObserver:self
	 name:NSFileHandleDataAvailableNotification
	 object:dataHandle];
	
}

- (void) startWaiting {
	socket = CFSocketCreate(kCFAllocatorDefault, PF_INET, SOCK_STREAM,
							IPPROTO_TCP, 0, NULL, NULL);
	if (!socket)
	{
//		[self errorWithName:@"Unable to create socket."];
		return;
	}
	
	int reuse = true;
	int fileDescriptor = CFSocketGetNative(socket);
	if (setsockopt(fileDescriptor, SOL_SOCKET, SO_REUSEADDR,
				   (void *)&reuse, sizeof(int)) != 0)
	{
//		[self errorWithName:@"Unable to set socket options."];
		return;
	}
	
	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_len = sizeof(address);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(9000);
	CFDataRef addressData = CFDataCreate(NULL, (const UInt8 *)&address, sizeof(address));
	[(id)addressData autorelease];
	
	if (CFSocketSetAddress(socket, addressData) != kCFSocketSuccess)
	{
//		[self errorWithName:@"Unable to bind socket to address."];
		return;
	}
	
	listeningHandle = [[NSFileHandle alloc]
					   initWithFileDescriptor:fileDescriptor
					   closeOnDealloc:YES];
	
	[[NSNotificationCenter defaultCenter]
	 addObserver:self
	 selector:@selector(receiveConnection:)
	 name:NSFileHandleConnectionAcceptedNotification
	 object:nil];
	
	[listeningHandle acceptConnectionInBackgroundAndNotify];
	
	[self setWaitForConnection:TRUE];
}

- (void) stopWaiting {
	[self setWaitForConnection:FALSE];
}



#pragma mark -- Notification Handlers --

- (void)receiveConnection:(NSNotification *)notification
{
	NSDictionary *userInfo = [notification userInfo];
	NSFileHandle *incomingFileHandle = [userInfo objectForKey:NSFileHandleNotificationFileHandleItem];
	
    if(incomingFileHandle)
	{
		
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(receiveData:)
		 name:NSFileHandleDataAvailableNotification
		 object:incomingFileHandle];
				
		if ( delegate && [delegate respondsToSelector:@selector(connecting:)] ) {
			[delegate connecting:self];
		}
		dataHandle = [incomingFileHandle copy];
        [incomingFileHandle waitForDataInBackgroundAndNotify];
    }
	
	[listeningHandle acceptConnectionInBackgroundAndNotify];
}


- (void)receiveData:(NSNotification *)notification
{
	NSFileHandle *incomingFileHandle = [notification object];
	NSData *data = [incomingFileHandle availableData];
	
	
	NSString *stringData = [[NSString alloc] initWithData: data encoding: NSUTF8StringEncoding];

	recvData = [[recvData stringByAppendingString:stringData] stringByReplacingOccurrencesOfString:@"\r" withString:@""];
	
	
	while([recvData rangeOfString:@"\n"].location != NSNotFound) {
		NSMutableArray *components = [NSMutableArray arrayWithArray:[recvData componentsSeparatedByString:@"\n"]]; 
		NSString *cmd = [components objectAtIndex:0];
		
		if([cmd isEqual:@"QUIT"]) {
			[incomingFileHandle closeFile];
			[[NSNotificationCenter defaultCenter]
			 removeObserver:self
			 name:NSFileHandleDataAvailableNotification
			 object:incomingFileHandle];
			
		}
		
		[self handleCmd:cmd handle:incomingFileHandle];
		
		[components removeObjectAtIndex:0];
		
		recvData = [[components componentsJoinedByString:@"\n"] copy];
		
		
	}
	
	[incomingFileHandle waitForDataInBackgroundAndNotify];
}

- (void)handleCmd:(NSString *)cmd handle:(NSFileHandle *)handle {
	if([cmd isEqual:@"CONNECT"] ) {
		isConnected = TRUE;
		if ( delegate && [delegate respondsToSelector:@selector(connected:)] ) {
			[delegate connected:self];
		}
		[handle writeData:[@"CONNECTED" dataUsingEncoding:NSASCIIStringEncoding]];
	}
	
	if([cmd isEqual:@"QUIT"]) {
		isConnected = FALSE;
		if ( delegate && [delegate respondsToSelector:@selector(disconnected:)] ) {
			[delegate disconnected:self];
		}
	}
	
	if([cmd rangeOfString:@"BP:"].location != NSNotFound) {
		if ( delegate && [delegate respondsToSelector:@selector(stopInFile:atLine:sender:)] ) {
			NSArray *components = [cmd componentsSeparatedByString:@":"];
			[delegate stopInFile:[components objectAtIndex:1] atLine:[[components objectAtIndex:2] intValue] sender:self];
		}
		
	}

	if([cmd rangeOfString:@"EV:"].location != NSNotFound) {
		if ( delegate && [delegate respondsToSelector:@selector(rubyStdout:sender:)] ) {
			NSString *response = [[[cmd componentsSeparatedByString:@"EV:"] lastObject] stringByReplacingOccurrencesOfString:@"\\n" withString:@"\n"];
			[delegate rubyStdout:response sender:self];
		}
		
	}
	
	
	NSLog(cmd);
}


@end
