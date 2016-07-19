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
 *  This is a utility class for performing file operations on a printer.
 *  
 *  Send file contents to a printer:
 *
 *	\code
#import "TcpPrinterConnection.h"
#import <UIKit/UIKit.h>
#import "ZebraPrinter.h"
#import "ZebraPrinterFactory.h"

TcpPrinterConnection *zebraPrinterConnection = [[TcpPrinterConnection alloc] initWithAddress:@"192.168.1.101" andWithPort:6101];
[zebraPrinterConnection open];
NSError *error = nil;
id<ZebraPrinter,NSObject> printer = [ZebraPrinterFactory getInstance:zebraPrinterConnection error:&error];
// If SAMPLE.FMT contains a CPCL format, it will be printed

BOOL success = [[printer getFileUtil] sendFileContents:@"/Documents/SAMPLE.FMT" error:&error];

if (error != nil || success == NO || printer == nil) {
	UIAlertView *errorAlert = [[UIAlertView alloc] initWithTitle:@"Error" message:[error localizedDescription] delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
	[errorAlert show];
	[errorAlert release];
}

[zebraPrinterConnection close];
[zebraPrinterConnection release];
 *	\endcode
 *
 *  Get directory listing from a printer:
 *
 *	\code
#import "TcpPrinterConnection.h"
#import <UIKit/UIKit.h>
#import "ZebraPrinter.h"
#import "ZebraPrinterFactory.h"

TcpPrinterConnection *zebraPrinterConnection = [[TcpPrinterConnection alloc] initWithAddress:@"192.168.1.101" andWithPort:6101];

BOOL success = [zebraPrinterConnection open];
NSError *error = nil;
id<ZebraPrinter,NSObject> printer = [ZebraPrinterFactory getInstance:zebraPrinterConnection error:&error];

// Get a list of the files on the printer
NSArray *fileNames = [[printer getFileUtil] retrieveFileNames:&error];

if (error != nil || printer == nil || success == NO) {
	UIAlertView *errorAlert = [[UIAlertView alloc] initWithTitle:@"Error" message:[error localizedDescription] delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
	[errorAlert show];
	[errorAlert release];
}
NSLog(@"Number of files - %d", [fileNames count]);

[zebraPrinterConnection close];
[zebraPrinterConnection release];
 *	\endcode
 */
@protocol FileUtil

/**
 * Sends the contents of a file to the printer.
 *
 * @param filePath The full file path (e.g. \@"/Documents/sample.txt").
 * @param error Will be set to the error that occured.
 * @return <code>NO</code> if there was an error sending the contents.
 */
-(BOOL)sendFileContents:(NSString*)filePath error:(NSError**)error;

/**
 * Retrieves the names of the files which are stored on the printer.
 * 
 * @param error Will be set to the error that occured.
 * @return A list of file names.
 */
-(NSArray*) retrieveFileNames:(NSError**)error;

/**
 * Retrieves the names of the files which are stored on the printer. This method only returns files which have the
 * extensions in <code>extensions</code>.
 * 
 * @param extensions The extensions to filter on.
 * @param error Will be set to the error that occured.
 * @return A list of file names.
 */
-(NSArray*) retrieveFileNamesWithExtensions:(NSArray*)extensions error:(NSError**)error;

@end
