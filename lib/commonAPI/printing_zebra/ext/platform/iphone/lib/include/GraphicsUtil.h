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

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>

/**
 * This is an utility class for printing images on a printer.
 * 
 * Print an image from the device to a printer.
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

NSInteger x = 100;
NSInteger y = 200;
success = success && [[printer getGraphicsUtil] printImageFromFile:@"/Documents/sample.jpg" atX:x atY:y error:&error];
[zebraPrinterConnection close];

if (error != nil || printer == nil || success == NO) {
	UIAlertView *errorAlert = [[UIAlertView alloc] initWithTitle:@"Error" message:[error localizedDescription] delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
	[errorAlert show];
	[errorAlert release];
}
[zebraPrinterConnection release];
 * \endcode
 *
 * @see ZebraPrinter::getGraphicsUtil
 */
@protocol GraphicsUtil

/**
 * Prints an image from the Apple&reg; mobile digital device's file system to the connected printer as a monochrome image. <br />
 * <b>Note:</b> if the image resolution is large (e.g. 1024x768) this method may take a long time to execute. 
 * 
 * @param path Full path to image file. (The image must be either a PNG or JPG).
 * @param x Horizontal starting position in dots.
 * @param y Vertical starting position in dots.
 * @param error Will be set to the error that occured.
 * @return <code>NO</code> if there was an error printing the image.
 */
-(BOOL) printImageFromFile: (NSString*)path atX:(NSInteger)x atY:(NSInteger)y error:(NSError**)error;

/**
 * Prints an image from the Apple&reg; mobile digital device's file system to the connected printer as a monochrome image. <br />
 * <b>Note:</b> if the image resolution is large (e.g. 1024x768) this method may take a long time to execute.
 * 
 * @param path Full path to image file. (The image must be either a PNG or JPG).
 * @param x Horizontal starting position in dots.
 * @param y Vertical starting position in dots.
 * @param width Desired width of the printed image. Passing -1 will preserve original width.
 * @param height Desired height of the printed image. Passing -1 will preserve original height.
 * @param isInsideFormat Boolean value indicating whether this image should be printed by itself (<c>NO</c>), or is part
 * of a format being written to the connection (<c>YES</c>).
 * @param error Will be set to the error that occured.
 * @return <code>NO</code> if there was an error printing the image.
 */
-(BOOL) printImageFromFile:(NSString*)path atX:(NSInteger)x atY:(NSInteger)y withWidth:(NSInteger)width withHeight:(NSInteger)height andIsInsideFormat:(BOOL)isInsideFormat error:(NSError**)error;

/**
 * Prints the given <c>image</c> to the connected printer as a monochrome image. <br />
 * <b>Note:</b> if the image resolution is large (e.g. 1024x768) this method may take a long time to execute.
 * 
 * @param image The image to print.
 * @param x Horizontal starting position in dots.
 * @param y Vertical starting position in dots.
 * @param width Desired width of the printed image. Passing -1 will preserve original width.
 * @param height Desired height of the printed image. Passing -1 will preserve original height.
 * @param isInsideFormat Boolean value indicating whether this image should be printed by itself (<c>NO</c>), or is part
 * of a format being written to the connection (<c>YES</c>).
 * @param error Will be set to the error that occured.
 * @return <code>NO</code> if there was an error printing the image.
 */
-(BOOL) printImage: (CGImageRef)image atX:(NSInteger)x atY:(NSInteger)y withWidth:(NSInteger)width withHeight:(NSInteger)height andIsInsideFormat:(BOOL)isInsideFormat error:(NSError**)error;

/**
 * Stores the specified <c>image</c> to the connected printer as a monochrome image.
 * The image will be stored on the printer at <c>printerDriveAndFileName</c> with the extension GRF. 
 * If a drive letter is not supplied, E will be used as the default (e.g. FILE becomes E:FILE.GRF).
 * If an extension is supplied, it is ignored (E:FILE.BMP becomes E:FILE.GRF).
 * <b>Note:</b> if the image resolution is large (e.g. 1024x768) this method may take a long time to execute.
 *
 * @param printerDriveAndFileName Path on the printer where the image will be stored.
 * @param image Bitmap image to print.
 * @param width Desired width of the printed image, in dots. Passing -1 will preserve original width.
 * @param height Desired height of the printed image, in dots. Passing -1 will preserve original height.
 * @param error Will be set to the error that occured.
 * @return <code>NO</code> if there was an error storing the image.
 */
-(BOOL) storeImage:(NSString *)printerDriveAndFileName  withImage:(CGImageRef)image withWidth:(NSInteger)width andWithHeight:(NSInteger)height error:(NSError**)error;

/**
 * Stores an image from the Apple&reg; mobile digital device's file system to the connected printer as a monochrome image.
 * The image will be stored on the printer at <c>printerDriveAndFileName</c> with the extension GRF. 
 * If a drive letter is not supplied, E will be used as the default (e.g. FILE becomes E:FILE.GRF).
 * If an extension is supplied, it is ignored (E:FILE.BMP becomes E:FILE.GRF).
 * <b>Note:</b> if the image resolution is large (e.g. 1024x768) this method may take a long time to execute.
 *
 * @param printerDriveAndFileName Path on the printer where the image will be stored.
 * @param imageFilePathOnDevice Path to the file containing the bitmap image to send.
 * @param width Desired width of the printed image, in dots. Passing -1 will preserve original width.
 * @param height Desired height of the printed image, in dots. Passing -1 will preserve original height.
 * @param error Will be set to the error that occured.
 * @return <code>NO</code> if there was an error storing the image.
 */
-(BOOL) storeImage:(NSString *)printerDriveAndFileName withPathOnDevice:(NSString *)imageFilePathOnDevice withWidth:(NSInteger)width andWithHeight:(NSInteger)height error:(NSError**)error;

@end
