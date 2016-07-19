/**********************************************
 * CONFIDENTIAL AND PROPRIETARY
 *  
 * The information contained herein is the confidential and the exclusive property of
 * ZIH Corp. This document, and the information contained herein, shall not be copied, reproduced, published,
 * displayed or distributed, in whole or in part, in any medium, by any means, for any purpose without the express
 * written consent of ZIH Corp. 
 * 
 * Copyright ZIH Corp. 2012 
 * 
 * ALL RIGHTS RESERVED
 ***********************************************/

#import "ZebraPrinterConnection.h"

/**
 * Establishes a TCP connection to a printer.
 * \remarks Each TcpPrinterConnection object should only be used on a single thread.
 *	The read and write streams are bound to the originating thread.
 *  Accessing the read and write streams on a different thread results in undefined behavior.
 *
 * \code
#import "TcpPrinterConnection.h"
#import "ZebraPrinterConnection.h"
#import <UIKit/UIKit.h>

-(void)sendZplOverTcp:(NSString*)theIpAddress {
	// Instantiate connection for ZPL TCP port at given address. 
	id<ZebraPrinterConnection, NSObject> thePrinterConn = [[TcpPrinterConnection alloc] initWithAddress:theIpAddress andWithPort:9100];

	// Open the connection - physical connection is established here.
	BOOL success = [thePrinterConn open];

	// This example prints "This is a ZPL test." near the top of the label.
	NSString *zplData = @"^XA^FO20,20^A0N,25,25^FDThis is a ZPL test.^FS^XZ";

	NSError *error = nil;
	// Send the data to printer as a byte array.
	success = success && [thePrinterConn write:[zplData dataUsingEncoding:NSUTF8StringEncoding] error:&error];

	if (success != YES || error != nil) {
		UIAlertView *errorAlert = [[UIAlertView alloc] initWithTitle:@"Error" message:[error localizedDescription] delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
		[errorAlert show];
		[errorAlert release];
	}
	// Close the connection to release resources.
	[thePrinterConn close];

	[thePrinterConn release];
}

-(void)sendCpclOverTcp:(NSString*) theIpAddress {
	// Instantiate connection for CPCL TCP port at given address.
	id<ZebraPrinterConnection,NSObject> thePrinterConn = [[TcpPrinterConnection alloc] initWithAddress:theIpAddress andWithPort:6101];

	// Open the connection - physical connection is established here.
	BOOL success = [thePrinterConn open];

	// This example prints "This is a CPCL test." near the top of the label.
	NSString *cpclData = @"! 0 200 200 210 1\r\nTEXT 4 0 30 40 This is a CPCL test.\r\nFORM\r\nPRINT\r\n";

	NSError *error = nil;
	// Send the data to printer as a byte array.
	success = success && [thePrinterConn write:[cpclData dataUsingEncoding:NSUTF8StringEncoding] error:&error];

	if (success != YES || error != nil) {
		UIAlertView *errorAlert = [[UIAlertView alloc] initWithTitle:@"Error" message:[error localizedDescription] delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
		[errorAlert show];
		[errorAlert release];
	}

	// Close the connection to release resources.
	[thePrinterConn close];

	[thePrinterConn release];
}
 
-(void)sampleWithGCD {
    //Dispatch this task to the default queue
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^ {
        // Instantiate connection to Tcp port at a given address
        id<ZebraPrinterConnection, NSObject> thePrinterConn = [[TcpPrinterConnection alloc] initWithAddress:@"192.168.1.5" andWithPort:9100];

        // Open the connection - physical connection is established here.
        BOOL success = [thePrinterConn open];

        // This example prints "This is a ZPL test." near the top of the label.
        NSString *zplData = @"^XA^FO20,20^A0N,25,25^FDThis is a ZPL test.^FS^XZ";

        NSError *error = nil;
        // Send the data to printer as a byte array.
        success = success && [thePrinterConn write:[zplData dataUsingEncoding:NSUTF8StringEncoding] error:&error];

        //Dispath GUI work back on to the main queue!
        dispatch_async(dispatch_get_main_queue(), ^{
            if (success != YES || error != nil) {
                UIAlertView *errorAlert = [[UIAlertView alloc] initWithTitle:@"Error" message:[error localizedDescription] delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
                [errorAlert show];
                [errorAlert release];
            }
        });

        // Close the connection to release resources.
        [thePrinterConn close];

        [thePrinterConn release];

    });
}
 * \endcode
 */
@interface TcpPrinterConnection : NSObject<ZebraPrinterConnection> {
	@private NSString *ipAddress;
	@private NSInteger port;
	@private NSInteger maxTimetouForRead;
    @private NSInteger timeToWaitForMoreData;
    @private NSInteger maxTimeoutForOpen;
	@private BOOL m_isConnected;
	@private CFReadStreamRef readStream;
	@private CFWriteStreamRef writeStream;
}

/**
 * Initializes a new instance of the TcpPrinterConnection class. This constructor will use the default timeouts for
 * ZebraPrinterConnection::read:. The default timeout is a maximum of 5 seconds for any data to be
 * received. If no more data is available after 500 milliseconds the read operation is assumed to be complete.<br/>
 * To specify timeouts other than the defaults, use:<br/>
 * TcpPrinterConnection::initWithAddress:withPort:withMaxTimeoutForRead:andWithTimeToWaitForMoreData:
 * 
 * @param anAddress The IP Address or DNS Hostname.
 * @param aPort The port number.
 */
- (id)initWithAddress:(NSString *)anAddress 
									andWithPort:(NSInteger)aPort;

/**
 * Initializes a new instance of the TcpPrinterConnection class. This constructor will use the specified timeouts
 * for ZebraPrinterConnection::read:. The timeout is a maximum of <c>maxTimeoutForRead</c> milliseconds for any data to be received.
 * If no more data is available after <c>timeToWaitForMoreData</c> milliseconds the read operation is assumed to be complete.
 * 
 * @param anAddress The IP Address or DNS Hostname.
 * @param aPort The port number.
 * @param aMaxTimeoutForRead The maximum time, in milliseconds, to wait for any data to be received.
 * @param aTimeToWaitForMoreData The maximum time, in milliseconds, to wait in-between reads after the initial read.
 */
- (id)initWithAddress:(NSString *)anAddress 
									withPort:(NSInteger)aPort 
					withMaxTimeoutForRead:(NSInteger)aMaxTimeoutForRead 
				   andWithTimeToWaitForMoreData:(NSInteger)aTimeToWaitForMoreData;

/**
 * The default time, in milliseconds, to wait for any data to be received.
 */
+(NSInteger)DEFAULT_MAX_TIMEOUT_FOR_READ;

/**
 * The default time, in milliseconds, to wait in-between reads after the initial read.
 */
+(NSInteger)DEFAULT_TIME_TO_WAIT_FOR_MORE_DATA;


/**
 * The default time, in milliseconds, to wait for a TCP connection to open. The default is set to the platform default. 
 * @param aTimeout Time to wait in milliseconds
 */
-(void)setMaxTimeoutForOpen:(int)aTimeout;

@end
