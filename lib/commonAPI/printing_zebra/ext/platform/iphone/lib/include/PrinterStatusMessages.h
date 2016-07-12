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

#import "PrinterStatus.h"

/**
 *  This class is used to acquire a human readable string of the current errors/warnings stored in a
 *  <c>PrinterStatus</c> instance.
 * 
 *  Get a string array of the current errors and warnings.
 *  \code
#import "TcpPrinterConnection.h"
#import "ZebraPrinter.h"
#import "ZebraPrinterFactory.h"
#import "PrinterStatusMessages.h"
#import	<UIKit/UIKit.h>

TcpPrinterConnection *zebraPrinterConnection = [[TcpPrinterConnection alloc] initWithAddress:@"192.168.1.100" andWithPort:6101];
BOOL success = [zebraPrinterConnection open];
NSError *error = nil;
id<ZebraPrinter,NSObject> printer = [ZebraPrinterFactory getInstance:zebraPrinterConnection error:&error];

PrinterStatus *printerStatus = [printer getCurrentStatus:&error];
NSString* status = nil;

if (printerStatus.isReadyToPrint) {
	status = @"Ready To Print";
} else {
	PrinterStatusMessages *statusMessage = [[PrinterStatusMessages alloc]initWithPrinterStatus:printerStatus];
	NSArray *statusMessages = [statusMessage getStatusMessage];
	NSMutableString *joinedStatusMessage = [[NSMutableString alloc] initWithFormat:@""];
	for (NSUInteger i = 0; i < [statusMessages count]; i++) {
		[joinedStatusMessage appendFormat:@"%@;", [statusMessages objectAtIndex:i]];
	}
	status = [[NSString alloc] initWithFormat:@"Cannot Print: %@", joinedStatusMessage];
	[joinedStatusMessage release];
	[statusMessage release];
}

if (error != nil || printer == nil || success == NO) {
	UIAlertView *errorAlert = [[UIAlertView alloc] initWithTitle:@"Error" message:[error localizedDescription] delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
	[errorAlert show];
	[errorAlert release];
}

[zebraPrinterConnection release];
 *  \endcode
 */
@interface PrinterStatusMessages : NSObject {
@private PrinterStatus *printerStatus;
}

/**
 * Message to indicate the head is open.
 * @returns A message to indicate the head is open.
 */
+(NSString*) HEAD_OPEN_MSG;


/**
 * Message to indicate the head is too hot.
 * @returns A Message to indicate the head is too hot.
 */
+(NSString*) HEAD_TOO_HOT_MSG ;

/**
 * Message to indicate the paper is out.
 * @returns A message to indicate the paper is out.
 */
+(NSString*) PAPER_OUT_MSG;

/**
 * Message to indicate the ribbon is out.
 * @returns A message to indicate the ribbon is out.
 */
+(NSString*) RIBBON_OUT_MSG;

/**
 * Message to indicate the receive buffer is full.
 * @returns A message to indicate the receive buffer is full.
 */
+(NSString*) RECEIVE_BUFFER_FULL_MSG;

/**
 * Message to indicate printer is paused.
 * @returns A message to indicate printer is paused.
 */
+(NSString*) PAUSE_MSG;

/**
 * Used to acquire a human readable string of the current errors/warnings stored in <c>aPrinterStatus</c>
 * 
 * @param aPrinterStatus An instance of <c>PrinterStatus</c> that will be used to acquire the human readable string 
 * of warnings/errors stored in <c>aPrinterStatus</c>.
 */
-(id)initWithPrinterStatus:(PrinterStatus *)aPrinterStatus;

/**
 * Used to acquire a human readable string of the current errors/warnings passed to this instance.
 * 
 * @returns A human readable string array of the current errors/warnings passed to this instance.
 */
-(NSArray *)getStatusMessage;
@end
