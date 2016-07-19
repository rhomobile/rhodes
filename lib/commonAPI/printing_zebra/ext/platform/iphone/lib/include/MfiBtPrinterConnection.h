/**********************************************
 * CONFIDENTIAL AND PROPRIETARY
 *  
 * The information contained herein is the confidential and the exclusive property of
 * ZIH Corp. This document, and the information contained herein, shall not be copied, reproduced, published,
 * displayed or distributed, in whole or in part, in any medium, by any means, for any purpose without the express
 * written consent of ZIH Corp. 
 * 
 * Copyright ZIH Corp. 2014 
 * 
 * ALL RIGHTS RESERVED
 ***********************************************/

#import <Foundation/Foundation.h>
#import <ExternalAccessory/ExternalAccessory.h>
#import "ZebraPrinterConnection.h"


/**
 * Establishes a Bluetooth connection to a printer.
 * \remarks This class will only work with Zebra printers which have the Made For iPod/iPhone certification.
 * \remarks You need to include the External Accessory framework in your project to be able to use this class
 * \remarks You need to include the Zebra printer protocol string "com.zebra.rawport" in your info.plist file under "Supported external accessory protocols"
 * \remarks You need to Set the key "Required Background modes" to "App Communicates with an accessory" in your app's plist file
 *
 * \code
 #import <ExternalAccessory/ExternalAccessory.h>
 #import "MfiBtPrinterConnection.h"
 -(void)sendZplOverBluetooth{
 
     NSString *serialNumber = @"";
     //Find the Zebra Bluetooth Accessory
     EAAccessoryManager *sam = [EAAccessoryManager sharedAccessoryManager];
     NSArray * connectedAccessories = [sam connectedAccessories];
     for (EAAccessory *accessory in connectedAccessories) {
         if([accessory.protocolStrings indexOfObject:@"com.zebra.rawport"] != NSNotFound){
             serialNumber = accessory.serialNumber;
             break;
             //Note: This will find the first printer connected! If you have multiple Zebra printers connected, you should display a list to the user and have him select the one they wish to use
         }
     }
     // Instantiate connection to Zebra Bluetooth accessory
     id<ZebraPrinterConnection, NSObject> thePrinterConn = [[MfiBtPrinterConnection alloc] initWithSerialNumber:serialNumber];
     
     // Open the connection - physical connection is established here.
     BOOL success = [thePrinterConn open];
     
     // This example prints "This is a ZPL test." near the top of the label.
     NSString *zplData = @"^XA^FO20,20^A0N,25,25^FDThis is a ZPL test.^FS^XZ";
     
     NSError *error = nil;
     // Send the data to printer as a byte array.
     success = success && [thePrinterConn write:[zplData dataUsingEncoding:NSUTF8StringEncoding] error:&error];
     
     if (success != YES || error != nil) {
         UIAlertView *errorAlert = [[UIAlertView alloc] initWithTitle:@"Error" message:[error localizedDescription] delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
         [errorAlert show];
         [errorAlert release];
     }
     // Close the connection to release resources.
     [thePrinterConn close];
     
     [thePrinterConn release];
 }
 
 -(void)sendCpclOverBluetooth {
     NSString *serialNumber = @"";
     //Find the Zebra Bluetooth Accessory
     EAAccessoryManager *sam = [EAAccessoryManager sharedAccessoryManager];
     NSArray * connectedAccessories = [sam connectedAccessories];
     for (EAAccessory *accessory in connectedAccessories) {
         if([accessory.protocolStrings indexOfObject:@"com.zebra.rawport"] != NSNotFound){
             serialNumber = accessory.serialNumber;
             break;
             //Note: This will find the first printer connected! If you have multiple Zebra printers connected, you should display a list to the user and have him select the one they wish to use
         }
     }
     // Instantiate connection to Zebra Bluetooth accessory
     id<ZebraPrinterConnection, NSObject> thePrinterConn = [[MfiBtPrinterConnection alloc] initWithSerialNumber:serialNumber];
     
     // Open the connection - physical connection is established here.
     BOOL success = [thePrinterConn open];
     
     // This example prints "This is a CPCL test." near the top of the label.
     NSString *cpclData = @"! 0 200 200 210 1\r\nTEXT 4 0 30 40 This is a CPCL test.\r\nFORM\r\nPRINT\r\n";
     
     NSError *error = nil;
     // Send the data to printer as a byte array.
     success = success && [thePrinterConn write:[cpclData dataUsingEncoding:NSUTF8StringEncoding] error:&error];
     
     if (success != YES || error != nil) {
         UIAlertView *errorAlert = [[UIAlertView alloc] initWithTitle:@"Error" message:[error localizedDescription] delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
         [errorAlert show];
         [errorAlert release];
     }
     
     // Close the connection to release resources.
     [thePrinterConn close];
     
     [thePrinterConn release];
 }
 
-(void)sampleWithGCD {
    //Dispatch this task to the default queue
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^ {
        // Instantiate connection to Zebra Bluetooth accessory
        id<ZebraPrinterConnection, NSObject> thePrinterConn = [[MfiBtPrinterConnection alloc] initWithSerialNumber:@"SomeSerialNumer..."];

        // Open the connection - physical connection is established here.
        BOOL success = [thePrinterConn open];

        // This example prints "This is a ZPL test." near the top of the label.
        NSString *zplData = @"^XA^FO20,20^A0N,25,25^FDThis is a ZPL test.^FS^XZ";

        NSError *error = nil;
        // Send the data to printer as a byte array.
        success = success && [thePrinterConn write:[zplData dataUsingEncoding:NSUTF8StringEncoding] error:&error];

        //Dispath GUI work back on to the main queue!
        dispatch_async(dispatch_get_main_queue(), ^{
            if (success != YES || error != nil) {
            UIAlertView *errorAlert = [[UIAlertView alloc] initWithTitle:@"Error" message:[error localizedDescription] delegate:nil cancelButtonTitle:@"Ok" otherButtonTitles:nil];
            [errorAlert show];
            [errorAlert release];
            }
        });

        // Close the connection to release resources.
        [thePrinterConn close];

        [thePrinterConn release];

    });
}
 * \endcode
 */
@interface MfiBtPrinterConnection : NSObject<ZebraPrinterConnection>
{
    @private NSInteger maxTimeoutForRead;
    @private NSInteger timeToWaitForMoreData;
    @private NSInteger timeToWaitAfterWrite;
    @private NSInteger timeToWaitAfterRead;

}


/**
 * Initializes a new instance of the MfiBtPrinterConnection class. This constructor will use the default timeouts for
 * ZebraPrinterConnection::read:. The default timeout is a maximum of 10 seconds for any data to be
 * received. If no more data is available after 500 milliseconds the read operation is assumed to be complete.<br/>
 * To specify timeouts other than the defaults, use:<br/>
 * MfiBtPrinterConnection::initWithSerialNumber:withMaxTimeoutForRead:andWithTimeToWaitForMoreData:
 *
 * @param aSerialNumber The device's serial number. serialNumber is a property of EAAccessory.
 */
-(id)initWithSerialNumber : (NSString*)aSerialNumber;

/**
 * Initializes a new instance of the MfiBtPrinterConnection class. This constructor will use the specified timeouts
 * for ZebraPrinterConnection::read:. The timeout is a maximum of <c>maxTimeoutForRead</c> milliseconds for any data to be received.
 * If no more data is available after <c>timeToWaitForMoreData</c> milliseconds the read operation is assumed to be complete.
 *
 * @param aSerialNumber The device's serial number. serialNumber is a property of EAAccessory.
 * @param aMaxTimeoutForRead The maximum time, in milliseconds, to wait for any data to be received.
 * @param aTimeToWaitForMoreData The maximum time, in milliseconds, to wait in-between reads after the initial read.
 */
-(id)initWithSerialNumber:(NSString*)aSerialNumber withMaxTimeoutForRead:(NSInteger)aMaxTimeoutForRead andWithTimeToWaitForMoreData:(NSInteger)aTimeToWaitForMoreData;

/**
 * Overrides the time the write: method will wait after writing data to the stream. The default time is 10ms. This method is used to adapt to different
 * Bluetooth radio performance requirements. If you notice an issue writing bytes, try increasing this time.
 *
 * @param aTimeInMs Time in milliseconds to wait between writes.
 */
-(void)setTimeToWaitAfterWriteInMilliseconds:(NSInteger)aTimeInMs;


/**
 * Overrides the time the read: method will wait after writing data to the stream. The default time is 10ms. This method is used to adapt to different
 * Bluetooth radio performance requirements. If you notice an issue writing bytes, try increasing this time.
 *
 * @param aTimeInMs Time in milliseconds to wait between reads.
 */
-(void)setTimeToWaitAfterReadInMilliseconds:(NSInteger)aTimeInMs;

@end
