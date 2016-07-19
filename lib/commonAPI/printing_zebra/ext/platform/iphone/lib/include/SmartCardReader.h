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
 * Provides access to the smartcard reader, for printers equipped with one.
 *
 * @note Only mobile Zebra printers are available with built-in readers.
 */
@protocol SmartCardReader
    
/**
 * Sends a CT_ATR command to the printer's smartcard reader, if present.
 * 
 * @param error Will be set to the error that occured.
 * @return A byte array containing the response from the smartcard reader.
 */
-(NSData*) getATR:(NSError**)error;
    
/**
 * Sends a CT_DATA command to the printer's smartcard reader, if present.
 *
 * @param asciiHexData Data to be sent to the smartcard using the CT_DATA card command.
 * @param error Will be set to the error that occured.
 * @return A byte array containing the response from the smartcard reader.
 */
-(NSData*) doCommand:(NSString*)asciiHexData error:(NSError**)error;
    
/**
 * Turns the printer's smartcard reader off, if present. This call should be made after
 * communicating with the smartcard via the SmartCardReader::doCommand:error: method.
 *
 * @param error Will be set to the error that occured.
 */
-(BOOL) close:(NSError**)error;

@end