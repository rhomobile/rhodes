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
#import "ZebraPrinter.h"
#import "ZebraPrinterConnection.h"

/**
 * A factory used to acquire an instance of a ZebraPrinter.
 */
@interface ZebraPrinterFactory : NSObject {

}

/**
 * Factory method to create the correct ZebraPrinter concrete class based on the printer's control language.
 * If the SGD value, <c>appl.name</c>, starts with one of the following, the printer is determined to be a
 * CPCL printer. Otherwise it is considered to be a ZPL printer<br>
 * <list type=bullet><item>SH</item> <item>H8</item> <item>C</item></list> <br>
 * If you wish to use prefixes other than the ones listed above, use
 * ZebraPrinterFactory::getInstance:withCpclPrefixes:error:
 * 
 * Note : iMZ printers are configured by default to be in "Line Print" mode. This method will put the printer ZPL mode so that the operations on a ZebraPrinter object will work. 
 * To put the printer back in Line Print mode, set the SGD "device.languages" to "line_print" (! U1 setvar "device.languages" "line_print")
 *
 * @param connection An open connection to a printer.
 * @param error Will be set to the error that occured.
 * <c>error.code</c> will be \link ZebraErrorCode::ZEBRA_ERROR_NO_CONNECTION ZEBRA_ERROR_NO_CONNECTION\endlink if the connection was not open.
 * <c>error.code</c> will be \link ZebraErrorCode::ZEBRA_UNKNOWN_PRINTER_LANGUAGE ZEBRA_UNKNOWN_PRINTER_LANGUAGE\endlink if the <c>appl.name</c> SGD value could not be obtained from the printer.
 * @return An instance of a ZebraPrinter.
 */
+(id<ZebraPrinter,NSObject>) getInstance:(id<ZebraPrinterConnection,NSObject>) connection 
								   error:(NSError**)error;

/**
 * Factory method to create the correct ZebraPrinter concrete class given a user supplied list of known CPCL prefixes.
 * If the SGD value, <c>appl.name</c>, starts with one of the \a cpclPrefixes, the printer
 * is determined to be a CPCL printer. Otherwise it is considered to be a ZPL printer<br>
 * 
 * Note : iMZ printers are configured by default to be in "Line Print" mode. This method will put the printer ZPL mode so that the operations on a ZebraPrinter object will work.
 * To put the printer back in Line Print mode, set the SGD "device.languages" to "line_print" (! U1 setvar "device.languages" "line_print")
 *
 * @param connection An open connection to a printer.
 * @param cpclPrefixes An array of possible CPCL version number prefixes.
 * @param error Will be set to the error that occured.
 * <c>error.code</c> will be \link ZebraErrorCode::ZEBRA_ERROR_NO_CONNECTION ZEBRA_ERROR_NO_CONNECTION\endlink if the connection was not open.
 * <c>error.code</c> will be \link ZebraErrorCode::ZEBRA_UNKNOWN_PRINTER_LANGUAGE ZEBRA_UNKNOWN_PRINTER_LANGUAGE\endlink if the <c>appl.name</c> SGD value could not be obtained from the printer.
 * @return An instance of a ZebraPrinter.
 */
+(id<ZebraPrinter,NSObject>) getInstance:(id<ZebraPrinterConnection,NSObject>) connection 
						withCpclPrefixes:(NSArray*) cpclPrefixes
								   error:(NSError**)error;

/**
 * Factory method to create the correct ZebraPrinter concrete class based on the provided printer control
 * language.
 * 
 * @param connection An open connection to a printer.
 * @param language The language of the printer instance to be created.
 * @return An instance of a ZebraPrinter.
 */
+(id<ZebraPrinter,NSObject>) getInstance:(id<ZebraPrinterConnection,NSObject>) connection 
					 withPrinterLanguage:(PrinterLanguage) language;

@end
