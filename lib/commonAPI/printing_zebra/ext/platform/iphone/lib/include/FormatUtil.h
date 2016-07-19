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
 * Defines functions used for interacting with printer formats.
 * 
 * Print a stored format with the given variables.
 * This ZPL will store a format on a printer, for use with the example code.
 * \code
 * ^XA
 * ^DFE:FORMAT.ZPL
 * ^FS
 * ^FT26,243^A0N,56,55^FH\^FN12"First Name"^FS
 * ^FT26,296^A0N,56,55^FH\^FN11"Last Name"^FS
 * ^FT258,73^A0N,39,38^FH\^FDVisitor^FS
 * ^BY2,4^FT403,376^B7N,4,0,2,2,N^FH^FDSerial Number^FS
 * ^FO5,17^GB601,379,8^FS
 * ^XZ    
 * \endcode
 * 
 *  \code
#import "TcpPrinterConnection.h"
#import <UIKit/UIKit.h>
#import "ZebraPrinter.h"
#import "ZebraPrinterFactory.h"

TcpPrinterConnection *zebraPrinterConnection = [[TcpPrinterConnection alloc] initWithAddress:@"192.168.1.100" andWithPort:9100];

BOOL success = [zebraPrinterConnection open];
NSError *error = nil;
id<ZebraPrinter,NSObject> printer = [ZebraPrinterFactory getInstance:zebraPrinterConnection error:&error];
// FORMAT.ZPL has two fields - the first is number 12, the second is number 11
NSMutableDictionary *vars = [[NSMutableDictionary alloc] init]; 
[vars setObject:@"John" forKey:[NSNumber numberWithInt:12]];
[vars setObject:@"Smith" forKey:[NSNumber numberWithInt:11]];
success = success && [[printer getFormatUtil] printStoredFormat:@"E:FORMAT.ZPL" withDictionary:vars error:&error];

if (error != nil || printer == nil || success == NO) {
	UIAlertView *errorAlert = [[UIAlertView alloc] initWithTitle:@"Error" message:[error localizedDescription] delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
	[errorAlert show];
	[errorAlert release];
} 

[zebraPrinterConnection close];
[zebraPrinterConnection release];
[vars release];
 *	\endcode
 */
@protocol FormatUtil

/**
 * Retrieves a format from the printer. On a ZPL printer, only <code>.ZPL</code> files are supported. On a CPCL
 * printer, only <code>.FMT</code> and <code>.LBL</code> files are supported.
 * 
 * @param formatPathOnPrinter The location of the file on the printer (e.g. "E:FORMAT.ZPL").
 * @param error Will be set to the error that occured.
 * @return The contents of the format file.
 */
-(NSData*) retrieveFormatFromPrinterWithPath:(NSString*)formatPathOnPrinter error:(NSError**) error;

/**
 * Returns a list of descriptors of the variable fields in this format.
 * 
 * @param formatContents The contents of the recalled format.
 * @param error Will be set to the error that occured.
 * @return A list of field data descriptors. In CPCL the nth element of the list will contain the integer n and no
 * name. In ZPL, each element will contain an ^FN number and a variable name if present. If the format contains
 * multiple ^FNs with the same number, only the last one will be in the result.<br>
 * See FieldDescriptionData for an example of how variable fields look.
 * @see FieldDescriptionData
 */
-(NSArray*) getVariableFieldsWithFormatContents:(NSString*)formatContents error:(NSError**)error;

/**
 * Prints a stored format on the printer, filling in the fields specified by the array. The values of any format
 * variables will be encoded using the default <c>encoding</c> type.
 * See NSStringEncoding for more information about encoding types.
 * @param formatPathOnPrinter The name of the format on the printer, including the extension (e.g. "E:FORMAT.ZPL").
 * @param vars An array of strings representing the data to fill into the format. For ZPL formats, index 0 of the
 * array corresponds to field number 2 (^FN2). For CPCL, the variables are passed in the order that they are found
 * in the format.
 * @param error Will be set to the error that occured.
 * @return <code>NO</code> if there was an error printing the format.
 */
-(BOOL) printStoredFormat:(NSString*)formatPathOnPrinter withFormatData:(NSArray*)vars error:(NSError**)error;
 
/**
 * Prints a stored format on the printer, filling in the fields specified by the array. The values of any format
 * variables will be encoded using the provided <c>encoding</c> type. e.g. NSUTF8StringEncoding.
 * See NSStringEncoding for more information about encoding types.
 * @param formatPathOnPrinter The location of the file on the printer (e.g. "E:FORMAT.ZPL").
 * @param vars An array of strings representing the data to fill into the format. For ZPL formats, index 0 of the
 * array corresponds to field number 2 (^FN2). For CPCL, the variables are passed in the order that they are found
 * in the format.
 * @param encoding A character-encoding name (e.g. NSUTF8StringEncoding).
 * @param error Will be set to the error that occured.
 * @return <code>NO</code> if there was an error printing the format.
 */
-(BOOL) printStoredFormat:(NSString*)formatPathOnPrinter withFormatData:(NSArray*)vars andWithEncoding:(NSStringEncoding)encoding error:(NSError**)error;
 
/**
 * Prints a stored format on the printer, filling in the fields specified by the NSDictionary. The values of any format
 * variables will be encoded using the default <c>encoding</c> type.
 * See NSStringEncoding for more information about encoding types.
 * @param formatPathOnPrinter The location of the file on the printer (e.g. "E:FORMAT.ZPL").
 * @param vars An NSDictionary which contains the key/value pairs for the stored format. For ZPL formats,
 * the key number should correspond directly to the number of the field in the format. For CPCL formats, the values
 * will be passed in ascending numerical order.
 * @param error Will be set to the error that occured.
 * @return <code>NO</code> if there was an error printing the format.
 */
-(BOOL) printStoredFormat:(NSString*)formatPathOnPrinter withDictionary:(NSMutableDictionary*)vars error:(NSError**)error;
 
/**
 * Prints a stored format on the printer, filling in the fields specified by the NSDictionary. The values of any format
 * variables will be encoded using the provided <c>encoding</c> type. e.g. NSUTF8StringEncoding.
 * See NSStringEncoding for more information about encoding types.
 * @param formatPathOnPrinter The location of the file on the printer (e.g. "E:FORMAT.ZPL").
 * @param vars An NSDictionary which contains the key/value pairs for the stored format. For ZPL formats,
 * the key number should correspond directly to the number of the field in the format. For CPCL formats, the values
 * will be passed in ascending numerical order.
 * @param encoding A character-encoding name (e.g. NSUTF8StringEncoding).
 * @param error Will be set to the error that occured.
 * @return <code>NO</code> if there was an error printing the format.
 */
-(BOOL) printStoredFormat:(NSString*)formatPathOnPrinter withDictionary:(NSMutableDictionary*)vars andWithEncoding:(NSStringEncoding)encoding error:(NSError**)error;

@end
