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

#import "FormatUtil.h"
#import "FileUtil.h"
#import "PrinterStatus.h"
#import "GraphicsUtil.h"
#import "MagCardReader.h"
#import "SmartCardReader.h"
#import "ToolsUtil.h"

/**
 * An interface used to obtain various properties of a Zebra printer.
 * 
 * How to create an instance of a ZebraPrinter.
 * \code
#import <UIKit/UIKit.h>
#import "TcpPrinterConnection.h"
#import "ZebraPrinter.h"
#import "ZebraPrinterFactory.h"

TcpPrinterConnection *zebraPrinterConnection = [[TcpPrinterConnection alloc] initWithAddress:@"192.168.1.100" andWithPort:6101];
BOOL success = [zebraPrinterConnection open];

NSError *error = nil;
id<ZebraPrinter,NSObject> printer = [ZebraPrinterFactory getInstance:zebraPrinterConnection error:&error];
PrinterLanguage pcLanguage = [printer getPrinterControlLanguage];
NSString *printerLanguageMessage = [[NSString alloc] initWithFormat:@"Printer Control Language is %@", pcLanguage];

[zebraPrinterConnection close];
if (error != nil || printer == nil || success == NO) {
	UIAlertView *errorAlert = [[UIAlertView alloc] initWithTitle:@"Error" message:[error localizedDescription] delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
	[errorAlert show];
	[errorAlert release];
}
[printerLanguageMessage release];
[zebraPrinterConnection release];
 * \endcode
 */
@protocol ZebraPrinter

/*! \public 
 * \brief Enumeration of the various printer control languages supported by Zebra printers.
 */
typedef enum {

	/**
	 * Printer control language ZPL
	 */
	PRINTER_LANGUAGE_ZPL,

	/**
	 * Printer control language CPCL
	 */
	PRINTER_LANGUAGE_CPCL

} PrinterLanguage;

/**
 * Returns the printer control language (ZPL or CPCL) of the printer.
 * 
 * @return The printer control language.
 * @see PrinterLanguage
 */
-(PrinterLanguage) getPrinterControlLanguage;

/**
 * Returns an instance of a <c>FormatUtil</c> object which contains convenience methods to interact with
 * formats on a Zebra printer.
 * 
 * @return Instance of a <c>FormatUtil</c> object that provides methods to interact with formats on a Zebra
 * printer.
 * @see FormatUtil
 */
-(id<FormatUtil, NSObject>) getFormatUtil;

/**
 * Returns an instance of a <code>FileUtil</code> object which contains convenience methods to interact with files
 * on a Zebra printer.
 * 
 * @return Instance of a <code>FileUtil</code> object that provides methods to interact with files on a Zebra
 * printer.
 * @see FileUtil
 */
-(id<FileUtil, NSObject>) getFileUtil;

/**
 * Returns an instance of a <code>GraphicsUtil</code> object which contains convenience methods to print
 * images to a Zebra printer.
 * @return Instance of a <code>GraphicsUtil</code> object that provides methods to print
 * images to a Zebra printer.
 * @see GraphicsUtil
 */
-(id<GraphicsUtil, NSObject>) getGraphicsUtil;

/**
 * Returns a new instance of <c>PrinterStatus</c> that can be used to determine the status of a printer. Each invocation of
 * this method will result in a query of the connected printer. If more than one status value is to be read, it is
 * recommended that a copy of <c>PrinterStatus</c> is stored locally.
 * 
 * Note: This method must be invoked again to retrieve the most up-to-date status of the printer. The
 * <c>PrinterStatus</c> object will only query the printer upon creation.<br/>
 * 
 * @param error Will be set to the error that occured.
 * @return A new instance of PrinterStatus.
 * @see PrinterStatus
 */
-(PrinterStatus *)getCurrentStatus:(NSError **)error;

/**
 * Returns an instance of a <c>MagCardReader</c> object which contains convenience methods for accessing the
 * printer's magnetic card reader.
 * 
 * @return A new instance of MagCardReader.
 * @see MagCardReader
 */
-(id<MagCardReader, NSObject>) getMagCardReader;

/**
 * Returns an instance of a <c>SmartCardReader</c> object which contains convenience methods for accessing the
 * printer's smart card reader.
 * 
 * @return A new instance of SmartCardReader.
 * @see SmartCardReader
 */
-(id<SmartCardReader, NSObject>) getSmartCardReader;

/**
 * Returns an instance of a <c>ToolsUtil</c> object which contains convenience methods to perform printer
 * actions. (Restore defaults, calibrate, etc.).
 * 
 * @return Instance of a <c>ToolsUtil</c> object that provides methods to perform printer actions. (Restore defaults, calibrate, etc).
 * @see ToolsUtil
 */
-(id<ToolsUtil, NSObject>) getToolsUtil;

@end

