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
 * This is a utility class for performing printer actions. (Restore defaults, calibrate, etc.).
 */
@protocol ToolsUtil

/** 
 *  Sends the appropriate calibrate command to the printer.
 * @param error Will be set to the error that occured.
 */
-(BOOL) calibrate:(NSError**)error;

/** 
 *  Sends the appropriate restore defaults command to the printer.
 * @param error Will be set to the error that occured.
 */
-(BOOL) restoreDefaults:(NSError**)error;

/** 
 *  Sends the appropriate print configuration command to the printer.
 * @param error Will be set to the error that occured.
 */
-(BOOL) printConfigurationLabel:(NSError**)error;

/** 
 *  Sends the specified command to the printer.
 * @param command The command to send to the printer.</param>
 * @param error Will be set to the error that occured.
 */
-(BOOL) sendCommand:(NSString*)command error:(NSError**)error;

/** 
 *  Sends the appropriate reset command to the printer.
 *  Note: You should call ZebraPrinterConnection::close after this method, as resetting the printer
 *  will terminate the connection.
 * @param error Will be set to the error that occured.
 */
-(BOOL) reset:(NSError**)error;

@end
