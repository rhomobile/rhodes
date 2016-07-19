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

#import "ZplPrintMode.h"

/**
 *  A class used to obtain the status of a Zebra printer.
 * 
 *  Get the current status of a printer.
 *
 *  \code
#import "TcpPrinterConnection.h"
#import "ZebraPrinterFactory.h"
#import "ZebraPrinter.h"
#import "PrinterStatus.h"
#import <UIKit/UIKit.h>

TcpPrinterConnection *zebraPrinterConnection = [[TcpPrinterConnection alloc] initWithAddress:@"192.168.1.100" andWithPort:9100];
BOOL success = [zebraPrinterConnection open];
NSError *error = nil;
id<ZebraPrinter, NSObject> printer = [ZebraPrinterFactory getInstance:zebraPrinterConnection error:&error];

PrinterStatus *printerStatus = [printer getCurrentStatus:&error];
if (printerStatus.isReadyToPrint) {
	NSLog(@"Ready To Print");
} else if (printerStatus.isPaused) {
	NSLog(@"Cannot Print because the printer is paused.");
} else if (printerStatus.isHeadOpen) {
	NSLog(@"Cannot Print because the printer head is open.");
} else if (printerStatus.isPaperOut) {
	NSLog(@"Cannot Print because the paper is out.");
} else {
	NSLog(@"Cannot Print.");
}

if (error != nil || printer == nil || success == NO) {
	UIAlertView *errorAlert = [[UIAlertView alloc] initWithTitle:@"Error" message:[error localizedDescription] delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
	[errorAlert show];
	[errorAlert release];
}
 *  \endcode
 */
@interface PrinterStatus : NSObject {

	/**
	 * <c>YES</c> if the printer reports back that it is ready to print.
	 */
	@private BOOL isReadyToPrint;

	/**
	 * <c>YES</c> if the head is open.
	 */
	@private BOOL isHeadOpen;

	/**
	 * <c>YES</c> if the head is cold. For CPCL printers this is always <c>NO</c>.
	 */
	@private BOOL isHeadCold;

	/**
	 * <c>YES</c> if the head is too hot. For CPCL printers this is always <c>NO</c>.
	 */
	@private BOOL isHeadTooHot;

	/**
	 * <c>YES</c> if the paper is out.
	 */
	@private BOOL isPaperOut;

	/**
	 * <c>YES</c> if the ribbon is out.
	 */
	@private BOOL isRibbonOut;

	/**
	 * <c>YES</c> if the receive buffer is full. For CPCL printers this is always <c>NO</c>.
	 */
	@private BOOL isReceiveBufferFull;

	/**
	 * <c>YES</c> if the printer is paused. For CPCL printers this is always <c>NO</c>.
	 */
	@private BOOL isPaused;

	/**
	 * The length of the label in dots. For CPCL printers this is always 0.
	 */
	@private NSInteger labelLengthInDots;

	/**
	 * The number of formats currently in the receive buffer of the printer. For CPCL printers this is always 0.
	 */
	@private NSInteger numberOfFormatsInReceiveBuffer;

	/**
	 * The number of labels remaining in the batch. For CPCL printers this is always 0.
	 */
	@private NSInteger labelsRemainingInBatch;

	/**
	 * <c>YES</c> if there is a partial format in progress. For CPCL printers this is always <c>NO</c>.
	 */
	@private BOOL isPartialFormatInProgress;

	/**
	 * The print mode. For CPCL printers this is always \link _ZplPrintMode::ZPL_PRINT_MODE_UNKNOWN ZPL_PRINT_MODE_UNKNOWN\endlink
	 */
    @private ZplPrintMode printMode;
}


@property (assign) BOOL isReadyToPrint;
@property (assign) BOOL isHeadOpen;
@property (assign) BOOL isHeadCold;
@property (assign) BOOL isHeadTooHot;
@property (assign) BOOL isPaperOut;
@property (assign) BOOL isRibbonOut;
@property (assign) BOOL isReceiveBufferFull;
@property (assign) BOOL isPaused;
@property (assign) NSInteger labelLengthInDots;
@property (assign) NSInteger numberOfFormatsInReceiveBuffer;
@property (assign) NSInteger labelsRemainingInBatch;
@property (assign) BOOL isPartialFormatInProgress; 
@property (assign) ZplPrintMode printMode;

/**
 *  Initializes a new instance of the PrinterStatus class that can be used to determine the status of a printer.<br/>
 *  <b>Note:</b> this will only query the printer's status upon creation. If the status needs to be updated see:<br/>
 *  ZebraPrinter::getCurrentStatus:
 * 
 */
-(id)init;

@end
