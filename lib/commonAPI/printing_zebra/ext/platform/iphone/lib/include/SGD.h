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
 *  A utility class used to wrap and send SGD commands to a connection.
 * 
 *  Sets the IP Address of the connection.
 *  \code
#import "TcpPrinterConnection.h"
#import "ZebraPrinter.h"
#import "ZebraPrinterFactory.h"
#import "SGD.h"
#import <UIKit/UIKit.h>

TcpPrinterConnection *zebraPrinterConnection = [[TcpPrinterConnection alloc] initWithAddress:@"192.168.1.100" andWithPort:6101];
BOOL success = [zebraPrinterConnection open];
NSError *error = nil;
id<ZebraPrinter,NSObject> printer = [ZebraPrinterFactory getInstance:zebraPrinterConnection error:&error];
success = success && [SGD SET:@"ip.addr" withValue:@"192.168.1.100" andWithPrinterConnection:zebraPrinterConnection error:&error];

NSString *ipAddress = [SGD GET:@"ip.addr" withPrinterConnection:zebraPrinterConnection error:&error];
NSLog(@"The IP address is: %@", ipAddress);

[zebraPrinterConnection close];
if (error != nil || printer == nil || success == NO) {
	UIAlertView *errorAlert = [[UIAlertView alloc] initWithTitle:@"Error" message:[error localizedDescription] delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
	[errorAlert show];
	[errorAlert release];
}
[zebraPrinterConnection release];
 *  \endcode
 */
@interface SGD : NSObject {
}

/**
 * Constructs an SGD SET command and sends it to the printer. This method will not wait for a response from the
 * printer. If the SGD SET command returns a response, the caller is responsible for reading the response.
 * If a response is expected, use SGD::DO:withValue:andWithPrinterConnection:error: command.
 * 
 * @param setting The SGD setting.
 * @param value The setting's value.
 * @param printerConnection The connection to send the command to.
 * @param error Will be set to the error that occured.
 * @return <code>NO</code> if there was an error sending the SGD.
 */
+(BOOL) SET: (NSString*) setting 
	  withValue:(NSString*) value
andWithPrinterConnection: (id<ZebraPrinterConnection,NSObject>) printerConnection 
	  error:(NSError**)error;

/**
 * Constructs an SGD SET command and sends it to the printer. This method will not wait for a response from the
 * printer. If the SGD SET command returns a response, the caller is responsible for reading the response.
 * If a response is expected, use SGD::DO:withValue:andWithPrinterConnection:error: command.
 * 
 * @param setting The SGD setting.
 * @param value The setting's value.
 * @param printerConnection The connection to send the command to.
 * @param error Will be set to the error that occured.
 * @return <code>NO</code> if there was an error sending the SGD.
 */
+(BOOL) SET: (NSString*) setting 
	  withValueAsInt:(NSInteger) value
andWithPrinterConnection: (id<ZebraPrinterConnection,NSObject>) printerConnection 
	  error:(NSError**)error;

/**
 * Constructs an SGD GET command and sends it to the printer. This method waits for a maximum of
 * ZebraPrinterConnection::getMaxTimeoutForRead milliseconds for any data to be received. Once some data
 * has been received it waits until no more data is available within
 * ZebraPrinterConnection::getTimeToWaitForMoreData milliseconds. This method returns the SGD value
 * associated with <code>setting</code> without the surrounding quotes.
 * 
 * @param setting The SGD setting.
 * @param printerConnection The connection to send the command to.
 * @param error Will be set to the error that occured.
 * @return The setting's value or <code>nil</code> if an error occurred.
 */
+(NSString*) GET: (NSString*) setting 
withPrinterConnection: (id<ZebraPrinterConnection,NSObject>) printerConnection 
	   error:(NSError**)error;


/**
 * Constructs an SGD GET command and sends it to the printer. This method waits for a maximum of
 * maxTimeoutForRead milliseconds for any data to be received. Once some data
 * has been received it waits until no more data is available within
 * timeToWaitForMoreData milliseconds. This method returns the SGD value
 * associated with <code>setting</code> without the surrounding quotes.
 * 
 * @param setting The SGD setting.
 * @param printerConnection The connection to send the command to.
 * @param maxTimeoutForRead The maximum time, in milliseconds, to wait for a response from the printer.
 * @param timeToWaitForMoreData The maximum time, in milliseconds, to wait in between reads after the initial data
 * @param error Will be set to the error that occured.
 * @return The setting's value or <code>nil</code> if an error occurred.
 */
+(NSString*) GET: (NSString*) setting withPrinterConnection: (id<ZebraPrinterConnection,NSObject>) printerConnection withMaxTimeoutForRead:(NSInteger) 
		maxTimeoutForRead andWithTimeToWaitForMoreData:(NSInteger) timeToWaitForMoreData error:(NSError**)error;

/**
 * Constructs an SGD DO command and sends it to the printer. This method waits for a maximum of
 * ZebraPrinterConnection::getMaxTimeoutForRead milliseconds for any data to be received. Once some data
 * has been received it waits until no more data is available within
 * ZebraPrinterConnection::getTimeToWaitForMoreData milliseconds. This method returns
 * the SGD value associated with <code>setting</code> without the surrounding quotes.
 * 
 * @param setting The SGD setting.
 * @param value The setting's value.
 * @param printerConnection The connection to send the command to.
 * @param error Will be set to the error that occured.
 * @return The response from the SGD DO command.
 */
+(NSString*) DO: (NSString*) setting 
	  withValue:(NSString*) value
andWithPrinterConnection: (id<ZebraPrinterConnection,NSObject>) printerConnection 
	  error:(NSError**)error;

/**
 * Constructs an SGD DO command and sends it to the printer. This method waits for a maximum of
 * <code>maxTimeoutForRead</code> milliseconds for any data to be received. Once some data has been received it
 * waits until no more data is available within <code>timeToWaitForMoreData</code> milliseconds. This method returns
 * the SGD value associated with <code>setting</code> without the surrounding quotes.
 * 
 * @param setting The SGD setting.
 * @param value The setting's value.
 * @param printerConnection The connection to send the command to.
 * @param maxTimeoutForRead The maximum time, in milliseconds, to wait for a response from the printer.
 * @param timeToWaitForMoreData The maximum time, in milliseconds, to wait in between reads after the initial data
 * is received.
 * @param error Will be set to the error that occured.
 * @return The response from the SGD DO command.
 */
+(NSString*) DO: (NSString*) setting 
	  withValue:(NSString*) value 
withPrinterConnection: (id<ZebraPrinterConnection,NSObject>) printerConnection 
withMaxTimeoutForRead:(NSInteger) maxTimeoutForRead 
andWithTimeToWaitForMoreData:(NSInteger) timeToWaitForMoreData
	  error:(NSError**)error;


@end
