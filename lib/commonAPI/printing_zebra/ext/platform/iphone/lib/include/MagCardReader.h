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

/** 
 * Provides access to the magnetic card reader, for printers equipped with one.
 * @note Only mobile Zebra printers are available with built-in readers.
 *
 * \code
#import "TcpPrinterConnection.h"
#import <UIKit/UIKit.h>
#import "ZebraPrinter.h"
#import "ZebraPrinterFactory.h"
 
TcpPrinterConnection *zebraPrinterConnection = [[TcpPrinterConnection alloc] initWithAddress:@"192.168.1.100" andWithPort:6101];
BOOL success = [zebraPrinterConnection open];

NSError *error = nil;
id<ZebraPrinter,NSObject> printer = [ZebraPrinterFactory getInstance:zebraPrinterConnection error:&error];

NSArray *tracks = [[printer getMagCardReader] read:1000 error:&error];

[zebraPrinterConnection close];

if (error != nil || printer == nil || success == NO) {
	UIAlertView *errorAlert = [[UIAlertView alloc] initWithTitle:@"Error" message:[error localizedDescription] delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
	[errorAlert show];
	[errorAlert release];
} else {
	NSLog(@"Track 1: %@", [tracks objectAtIndex:0]);
	NSLog(@"Track 2: %@", [tracks objectAtIndex:1]);
	NSLog(@"Track 3: %@", [tracks objectAtIndex:2]);
}

[zebraPrinterConnection release];
 * \endcode
 */
@protocol MagCardReader

/** 
 * Activates the printer's magnetic card reader, if present, and waits for <c>timeoutMS</c> milliseconds for a card to be swiped.
 * If the printer does not have a reader the call will timeout.
 * @param timeoutMS The amount of time in milliseconds to enable the reader and wait for a card to be swiped.
 * @param error Will be set to the error that occured.
 * @return An array of three strings corresponding to the tracks of the card. If a track could not be read that
 * string will be empty.
 */
-(NSArray*)read:(NSInteger)timeoutMS error:(NSError**)error;

@end
