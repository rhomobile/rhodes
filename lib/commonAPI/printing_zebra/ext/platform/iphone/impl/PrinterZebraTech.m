//
//  PrinterZebraWorker.m
//  PrintingZebra

#import "TcpPrinterConnection.h"
#import "ZebraPrinterFactory.h"
#import "ZebraPrinter.h"
#import "ZebraErrorCode.h"
#import "PrinterStatus.h"
#import "NetworkDiscoverer.h"
#import "DiscoveredPrinterNetwork.h"
#import "FieldDescriptionData.h"
#import "SGD.h"

#import "PrinterZebraTech.h"

#import "IPrinterZebra.h"

#import <QuartzCore/CAAnimation.h>
#import "Reachability.h"

#include <arpa/inet.h>

/////////////////////////////////////////////////////////////////////////

bool getIntKey(NSDictionary* options, NSString* key, int* value) {
    bool isInitialized = false;
    NSObject* val = [options objectForKey:key];
    int retval = 0;
    if (val != nil)
    {
        if ([val isKindOfClass:[NSNumber class]]) {
            NSNumber* num = ((NSNumber*)val);
            retval = (int)([num integerValue]);
            isInitialized = true;
        } else if ([val isKindOfClass:[NSString class]]) {
            NSString* str = ((NSString*)val);
            NSScanner *scanner = [NSScanner scannerWithString:str];
            NSInteger numi;
            if([scanner scanInteger:&numi] && [scanner isAtEnd]) {
                retval = (int)(numi);
                isInitialized = true;
            }
        }
    }
    if (isInitialized) {
        *value = retval;
    }
    return isInitialized;
}

NSString* getPrintMode(ZplPrintMode mode) {
    NSString* result = @"none";
    switch (mode) {
        case ZPL_PRINT_MODE_REWIND:
            result = PRINT_MODE_REWIND;
            break;

        case ZPL_PRINT_MODE_PEEL_OFF:
            result = PRINT_MODE_PEEL_OFF;
            break;

        case ZPL_PRINT_MODE_TEAR_OFF:
            result = PRINT_MODE_TEAR_OFF;
            break;

        case ZPL_PRINT_MODE_CUTTER:
            result = PRINT_MODE_CUTTER;
            break;

        case ZPL_PRINT_MODE_APPLICATOR:
            result = PRINT_MODE_APPLICATOR;
            break;

        case ZPL_PRINT_MODE_DELAYED_CUT:
            result = PRINT_MODE_DELAYED_CUT;
            break;

        case ZPL_PRINT_MODE_LINERLESS_PEEL:
            result = PRINT_MODE_LINERLESS_PEEL;
            break;

        case ZPL_PRINT_MODE_LINERLESS_REWIND:
            result = PRINT_MODE_LINERLESS_REWIND;
            break;

        case ZPL_PRINT_MODE_PARTIAL_CUTTER:
            result = PRINT_MODE_PARTIAL_CUTTER;
            break;

        case ZPL_PRINT_MODE_RFID:
            result = PRINT_MODE_RFID;
            break;

        case ZPL_PRINT_MODE_KIOSK:
            result = PRINT_MODE_KIOSK;
            break;

        case ZPL_PRINT_MODE_UNKNOWN:
            result = PRINT_MODE_UNKNOWN;
            break;

        default:
            break;
    }
    return result;
}

NSUInteger errorCodeToStatus(NSError* error) {
    NSUInteger errorCode = 0;
    if (error != nil) {
        switch (error.code) {
            case ZEBRA_ERROR_NO_CONNECTION:
                errorCode = PRINTING_RESULT_ERR_NOT_CONNECTED;
                break;
            case ZEBRA_ERROR_WRITE_FAILURE:
                errorCode = PRINTING_RESULT_ERR_IO;
                break;
            case ZEBRA_ERROR_READ_FAILURE:
                errorCode = PRINTING_RESULT_ERR_IO;
                break;
            case ZEBRA_UNKNOWN_PRINTER_LANGUAGE:
                errorCode = PRINTING_RESULT_ERR_UNSUPPORTED;
                break;
            case ZEBRA_MALFORMED_NETWORK_DISCOVERY_ADDRESS:
                errorCode = PRINTING_RESULT_ERR_PARAM;
                break;
            case ZEBRA_NETWORK_ERROR_DURING_DISCOVERY:
                errorCode = PRINTING_RESULT_ERR_NETWORK;
                break;
            case ZEBRA_INVALID_DISCOVERY_HOP_COUNT:
                errorCode = PRINTING_RESULT_ERR_PARAM;
                break;
            case ZEBRA_MALFORMED_PRINTER_STATUS_RESPONSE:
                errorCode = PRINTING_RESULT_ERR_RESPONSE;
                break;
            case ZEBRA_INVALID_FORMAT_NAME:
                errorCode = PRINTING_RESULT_ERR_PARAM;
                break;
            case ZEBRA_BAD_FILE_DIRECTORY_ENTRY:
                errorCode = PRINTING_RESULT_ERR_PARAM;
                break;
            case ZEBRA_MALFORMED_FORMAT_FIELD_NUMBER:
                errorCode = PRINTING_RESULT_ERR_PARAM;
                break;
            case ZEBRA_INVALID_PRINTER_LANGUAGE:
                errorCode = PRINTING_RESULT_ERR_PARAM;
                break;
            case ZEBRA_INVALID_FILE_NAME:
                errorCode = PRINTING_RESULT_ERR_PARAM;
                break;
            case ZEBRA_INVALID_PRINTER_DRIVE_LETTER:
                errorCode = PRINTING_RESULT_ERR_PARAM;
                break;
        }
    } else {
        errorCode = PRINTING_RESULT_SUCCESS;
    }
    return errorCode;
}

NSString* errorCodeToString(NSError* error) {
    NSString* errorMessage = @"";
    if (error != nil) {
        switch (error.code) {
            case ZEBRA_ERROR_NO_CONNECTION:
                errorMessage = @"Unable to create a connection to a printer.";
                break;
            case ZEBRA_ERROR_WRITE_FAILURE:
                errorMessage = @"Write to a connection failed.";
                break;
            case ZEBRA_ERROR_READ_FAILURE:
                errorMessage = @"Read from a connection failed.";
                break;
            case ZEBRA_UNKNOWN_PRINTER_LANGUAGE:
                errorMessage = @"Unable to determine the control language of a printer.";
                break;
            case ZEBRA_MALFORMED_NETWORK_DISCOVERY_ADDRESS:
                errorMessage = @"Malformed discovery address";
                break;
            case ZEBRA_NETWORK_ERROR_DURING_DISCOVERY:
                errorMessage = @"Network error during discovery";
                break;
            case ZEBRA_INVALID_DISCOVERY_HOP_COUNT:
                errorMessage = @"Invalid multicast hop count";
                break;
            case ZEBRA_MALFORMED_PRINTER_STATUS_RESPONSE:
                errorMessage = @"Malformed status response - unable to determine printer status";
                break;
            case ZEBRA_INVALID_FORMAT_NAME:
                errorMessage = @"Invalid format name";
                break;
            case ZEBRA_BAD_FILE_DIRECTORY_ENTRY:
                errorMessage = @"Bad file directory entry";
                break;
            case ZEBRA_MALFORMED_FORMAT_FIELD_NUMBER:
                errorMessage = @"'^FN' integer must be between 1 and 9999";
                break;
            case ZEBRA_INVALID_PRINTER_LANGUAGE:
                errorMessage = @"Invalid printer language";
                break;
            case ZEBRA_INVALID_FILE_NAME:
                errorMessage = @"Invalid file name";
                break;
            case ZEBRA_INVALID_PRINTER_DRIVE_LETTER:
                errorMessage = @"Invalid drive specified";
                break;
        }
    } else {
        errorMessage = @"Success";
    }
    return errorMessage;
}


@implementation ZebraConnectParams

-(void) setFromDictionary:(NSDictionary*) options {
    if (options == nil || [options isKindOfClass:[NSNull class]]) {
        return;
    }
    if (getIntKey(options, HK_TIMEOUT, &mMaxTimeoutForOpen)) {
         mInitialized[PARAM_MAX_TIMEOUT_FOR_OPEN] = true;
    } /*else if (getIntKey(options, PROPERTY_MAX_TIMEOUT_FOR_OPEN, &mMaxTimeoutForOpen)) {
        mInitialized[PARAM_MAX_TIMEOUT_FOR_OPEN] = true;
    }
    if (getIntKey(options, PROPERTY_MAX_TIMEOUT_FOR_READ, &mMaxTimeoutForRead)) {
        mInitialized[PARAM_MAX_TIMEOUT_FOR_READ] = true;
    }
    if (getIntKey(options, PROPERTY_TIME_TO_WAIT_FOR_MORE_DATA, &mTimeToWaitForMoreData)) {
        mInitialized[PARAM_TIME_TO_WAIT_FOR_MORE_DATA] = true;
    }
    if (getIntKey(options, PROPERTY_TIME_TO_WAIT_AFTER_READ_IN_MILLISECONDS, &mTimeToWaitAfterReadInMilliseconds)) {
        mInitialized[PARAM_TIME_TO_WAIT_AFTER_READ_IN_MILLISECONDS] = true;
    }
    if (getIntKey(options, PROPERTY_TIME_TO_WAIT_AFTER_WRITE_IN_MILLISECONDS, &mTimeToWaitAfterWriteInMilliseconds)) {
        mInitialized[PARAM_TIME_TO_WAIT_AFTER_WRITE_IN_MILLISECONDS] = true;
    }*/
}

-(NSDictionary*) getAsDictionary {
    NSMutableDictionary* dict = [[NSMutableDictionary alloc] initWithCapacity:PARAM_MAX];

    if (mInitialized[PARAM_MAX_TIMEOUT_FOR_OPEN]) {
        //[dict setObject:[NSNumber numberWithInt:mMaxTimeoutForOpen] forKey:PROPERTY_MAX_TIMEOUT_FOR_OPEN];
        [dict setObject:[NSNumber numberWithInt:mMaxTimeoutForOpen] forKey:HK_TIMEOUT];
    }/*
    if (mInitialized[PARAM_MAX_TIMEOUT_FOR_READ]) {
        [dict setObject:[NSNumber numberWithInt:mMaxTimeoutForRead] forKey:PROPERTY_MAX_TIMEOUT_FOR_READ];
    }
    if (mInitialized[PARAM_TIME_TO_WAIT_FOR_MORE_DATA]) {
        [dict setObject:[NSNumber numberWithInt:mTimeToWaitForMoreData] forKey:PROPERTY_TIME_TO_WAIT_FOR_MORE_DATA];
    }
    if (mInitialized[PARAM_TIME_TO_WAIT_AFTER_READ_IN_MILLISECONDS]) {
        [dict setObject:[NSNumber numberWithInt:mTimeToWaitAfterReadInMilliseconds] forKey:PROPERTY_TIME_TO_WAIT_AFTER_READ_IN_MILLISECONDS];
    }
    if (mInitialized[PARAM_TIME_TO_WAIT_AFTER_WRITE_IN_MILLISECONDS]) {
        [dict setObject:[NSNumber numberWithInt:mTimeToWaitAfterWriteInMilliseconds] forKey:PROPERTY_TIME_TO_WAIT_AFTER_WRITE_IN_MILLISECONDS];
    }*/
    return dict;
}

-(void) resetToDefaultValues {
    mMaxTimeoutForOpen = -1;
    mInitialized[PARAM_MAX_TIMEOUT_FOR_OPEN] = true;

    /*mMaxTimeoutForRead = [TcpPrinterConnection DEFAULT_MAX_TIMEOUT_FOR_READ];
    mInitialized[PARAM_MAX_TIMEOUT_FOR_READ] = true;

    mTimeToWaitForMoreData = [TcpPrinterConnection DEFAULT_TIME_TO_WAIT_FOR_MORE_DATA];
    mInitialized[PARAM_TIME_TO_WAIT_FOR_MORE_DATA] = true;

    mTimeToWaitAfterReadInMilliseconds = 10;
    mInitialized[PARAM_TIME_TO_WAIT_AFTER_READ_IN_MILLISECONDS] = true;

    mTimeToWaitAfterWriteInMilliseconds = 200;
    mInitialized[PARAM_TIME_TO_WAIT_AFTER_WRITE_IN_MILLISECONDS] = true;*/
}
@end

/////////////////////////////////////////////////////////////////////////

@implementation PrinterZebraInstace

-(id)init {
    self = [super init];
    if( self )
    {
        mConnection = nil;
        mParams = [[ZebraConnectParams alloc] init];
        [mParams resetToDefaultValues];
        [self setSupportedLanguages:[NSArray arrayWithObject:PRINTER_LANGUAGE_CPCL_CONST]];
    }
    return self;
}

-(void)dealloc {
    @synchronized(self) {
        if (mConnection) {
            [mConnection close];
            [mConnection release];
            mConnection = nil;
        }
    }
    if (mParams) {
        [mParams release];
        mParams = nil;
    }

    [super dealloc];
}

+(NSString*) getPrinterType {
    return PRINTER_TYPE_ZEBRA;
}

+(NSString*) getVendor {
    return @"ZEBRA";
}

-(BOOL) isConnected {
    BOOL connection = NO;
    @synchronized(self) {
        connection = mConnection != nil;
    }
    return connection;
}

-(NSDictionary*) updatePrinterState:(PrinterStatus*)status {
    if (mPrinterStatus) {
        [mPrinterStatus release];
        mPrinterStatus = nil;
    }
    NSDictionary* result = [NSDictionary dictionaryWithObjectsAndKeys:

                            [NSNumber numberWithBool:[status isReadyToPrint]], PRINTER_STATE_IS_READY_TO_PRINT,
                            [NSNumber numberWithBool:[status isHeadOpen]], PRINTER_STATE_IS_COVER_OPENED,
                            [NSNumber numberWithBool:[status isPaperOut]], PRINTER_STATE_IS_PAPER_OUT,
                            [NSNumber numberWithBool:[status isHeadCold]], PRINTER_STATE_IS_HEAD_COLD,
                            [NSNumber numberWithBool:[status isHeadOpen]], PRINTER_STATE_IS_HEAD_OPEN,
                            [NSNumber numberWithBool:[status isHeadTooHot]], PRINTER_STATE_IS_HEAD_TOO_HOT,
                            [NSNumber numberWithBool:[status isPartialFormatInProgress]], PRINTER_STATE_IS_PARTIAL_FORMAT_IN_PROGRESS,
                            [NSNumber numberWithBool:[status isPaused]], PRINTER_STATE_IS_PAUSED,
                            [NSNumber numberWithBool:[status isReceiveBufferFull]], PRINTER_STATE_IS_RECEIVE_BUFFER_FULL,
                            [NSNumber numberWithBool:[status isRibbonOut]], PRINTER_STATE_IS_RIBBON_OUT,
                            [NSNumber numberWithInt:[status labelLengthInDots]], PRINTER_STATE_LABEL_LENGTH_IN_DOTS,
                            [NSNumber numberWithInt:[status labelsRemainingInBatch]], PRINTER_STATE_LABELS_REMAINING_IN_BATCH,
                            [NSNumber numberWithInt:[status numberOfFormatsInReceiveBuffer]], PRINTER_STATE_NUMBER_OF_FORMATS_IN_RECEIVE_BUFFER,
                            getPrintMode([status printMode]), PRINTER_STATE_PRINT_MODE,
                            nil];
    mPrinterStatus = [result retain];
    
    return result;
}

-(NSString*) sendAndReceive:(NSString*)cmd {
    NSString* newStr = @"";
    if (mConnection) {
        @synchronized(mConnection) {
            NSError* error = nil;
            NSMutableData* data = [[cmd dataUsingEncoding:NSUTF8StringEncoding] mutableCopy];
            /* */
            int blockSize = 1024;
            int totalSize = [data length];
            int bytesRemaining = totalSize;

            while (bytesRemaining > 0 && error == nil) {
                int bytesToSend = MIN(blockSize, bytesRemaining);
                NSRange range = NSMakeRange(0, bytesToSend);

                NSData *partialLabel = [data subdataWithRange:range];
                [mConnection write:partialLabel error:&error];

                bytesRemaining -= bytesToSend;

                [data replaceBytesInRange:range withBytes:NULL length:0];
            }

            if (error == nil) {
                [mConnection waitForData:5000];
                if ([mConnection hasBytesAvailable]) {
                    NSData* received = [mConnection read:&error];
                    newStr = [[NSString alloc] initWithData:received encoding:NSUTF8StringEncoding];
                }
            } else {
                NSLog(@"ConnErr: %@", [error localizedDescription]);
            }
        }
    }
    return newStr;
}

-(void) connect:(id<ICallbackStruct>)callback {
    [self connect:callback withParams:nil];
}

-(void) connect:(id<ICallbackStruct>)callback withParams:(id<IConnectParams>)params; {

    [callback retain];
    [callback setCommand:PRINTING_COMMAND_CONNECT];

    NSUInteger taskID = -1;


    if (mInitialized) {
        taskID = [mSheduler sheduleTask:^{
             NSError *error = nil;

            BOOL connected = NO;

            if ([self isConnected]) {
                id<ZebraPrinter, NSObject> printer = [ZebraPrinterFactory getInstance:mConnection error:&error];

                if (error == nil) {
                    PrinterStatus* status = [printer getCurrentStatus:&error];
                }

                if (error == nil) {
                    connected = YES;
                    [callback invokeWithEvent:PRINTING_EVENT_FINISHED result:PRINTING_RESULT_SUCCESS];
                } else {
                    @synchronized(self) {
                        mConnection = nil;
                    }
                }
            }

            if (!connected) {
                [callback invokeWithEvent:PRINTING_EVENT_STARTING result:PRINTING_RESULT_NONE];

                id<ZebraPrinterConnection, NSObject> connection = nil;

                BOOL connectionParamsAreSet = params && [params maxTimeoutForRead] >= 0 && [params timeToWaitForMoreData] >=0;
                BOOL btParamsAreSet = params && ([params timeToWaitAfterReadInMilliseconds] >= 0 || [params timeToWaitAfterWriteInMilliseconds] >= 0);

                switch (mConnectionType) {
                    case CONNECTION_TCP: {
                        TcpPrinterConnection *conn = nil;
                        if (connectionParamsAreSet) {
                            conn = [[TcpPrinterConnection alloc] initWithAddress:mConnectionAddress withPort:mPort withMaxTimeoutForRead:[params maxTimeoutForRead] andWithTimeToWaitForMoreData:[params timeToWaitForMoreData]];
                        } else {
                            conn = [[TcpPrinterConnection alloc] initWithAddress:mConnectionAddress andWithPort:mPort];
                        }
                        if (params && [params maxTimeoutForOpen] >= 0) {
                            [conn setMaxTimeoutForOpen:[params maxTimeoutForOpen]];
                        } else {
                            [conn setMaxTimeoutForOpen:5000];
                        }
                        connection = conn;

                    } break;
                    case CONNECTION_BLUETOOTH: {
                        MfiBtPrinterConnection *conn = nil;
                        if (connectionParamsAreSet) {
                             conn = [[MfiBtPrinterConnection alloc] initWithSerialNumber:mConnectionAddress withMaxTimeoutForRead:[params maxTimeoutForRead] andWithTimeToWaitForMoreData:[params timeToWaitForMoreData]];

                        } else {
                            conn = [[MfiBtPrinterConnection alloc] initWithSerialNumber:mConnectionAddress];
                        }
                        if (btParamsAreSet) {
                            if ([params isInitialized:PARAM_TIME_TO_WAIT_AFTER_READ_IN_MILLISECONDS]) {
                                [conn setTimeToWaitAfterReadInMilliseconds:[params timeToWaitAfterReadInMilliseconds]];
                            }
                            if ([params isInitialized:PARAM_TIME_TO_WAIT_AFTER_WRITE_IN_MILLISECONDS]) {
                                [conn setTimeToWaitAfterWriteInMilliseconds:[params timeToWaitAfterWriteInMilliseconds]];
                            }
                        }
                        connection = conn;
                    } break;
                    default:
                        connection = nil;
                        break;
                }

                if (connection) {

                    BOOL connOk = NO;

                    @try {
                        connOk = [connection open];
                    }
                    @catch (NSException *exception) {
                        connOk = NO;
                        NSLog(@"%@", exception.reason);
                    }

                    if (connOk) {
                        @synchronized(self) {
                            mConnection = [connection retain];
                        }

                        [SGD SET:@"device.languages" withValue:@"zpl" andWithPrinterConnection:mConnection error:&error];


                        /*#ifdef RHO_DEBUG
                         // TODO: remove, debugging only commands
                         [SGD SET:@"ezpl.media_type" withValue:@"continuous" andWithPrinterConnection:mConnection error:&error];
                         [SGD SET:@"zpl.label_length" withValue:@"500" andWithPrinterConnection:mConnection error:&error];
                         #endif*/
                        /*{
                            NSString* devcap = [self sendAndReceive:@"{}{\"file.dir\":null}\r\n"];
                            NSLog(@"DevCap:%@",devcap);
                        }*/

                        id<ZebraPrinter, NSObject> printer = [ZebraPrinterFactory getInstance:mConnection error:&error];

                        PrinterStatus* status = [printer getCurrentStatus:&error];

                        PrinterLanguage lang = [printer getPrinterControlLanguage];

                        if (lang == 0) {
                            [self setSupportedLanguages:[NSArray arrayWithObjects:PRINTER_LANGUAGE_ZPL_CONST, PRINTER_LANGUAGE_CPCL_CONST, nil]];
                        } else {
                            [self setSupportedLanguages:[NSArray arrayWithObjects:PRINTER_LANGUAGE_CPCL_CONST, nil]];
                        }

                        if (error == nil) {
                            NSDictionary* dictStat = [self updatePrinterState:status];
                            // get device name
                            NSString* deviceName = [SGD GET:@"device.friendly_name" withPrinterConnection:mConnection error:&error];
                            if (deviceName == nil || [deviceName length] < 1) {
                                deviceName = @"ZebraPrinter";
                            }
                            NSMutableDictionary* withPName = [NSMutableDictionary dictionaryWithDictionary:dictStat];
                            [withPName setObject:deviceName forKey:PROPERTY_DEVICE_NAME];
                            [callback invokeWithEvent:PRINTING_EVENT_FINISHED result:PRINTING_RESULT_SUCCESS info:withPName];
                        } else {
                            @synchronized(self) {
                                mConnection = nil;
                            }
                            [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:errorCodeToStatus(error)];
                        }
                    } else {
                        @try {
                            [connection close];
                        }
                        @catch (NSException *exception) {
                            connOk = NO;
                            NSLog(@"%@", exception.reason);
                        }

                        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_NOT_CONNECTED ];

                    }
                } else {
                    [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNSUPPORTED];
                }
                [connection release];
            }
            [callback release];
        }];
    } else {
        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNINITIALIZED];
        [callback release];
    }
}

-(void) disconnect:(id<ICallbackStruct>)callback {

    [callback retain];
    [callback setCommand:PRINTING_COMMAND_DISCONNECT];

    NSUInteger taskID = -1;

    if (mInitialized) {
        taskID = [mSheduler sheduleTask:^{
            if (![self isConnected]) {
                [callback invokeWithEvent:PRINTING_EVENT_FINISHED result:PRINTING_RESULT_SUCCESS];
            } else {
                [callback invokeWithEvent:PRINTING_EVENT_STARTING result:PRINTING_RESULT_NONE];

                @synchronized(self) {
                    [mConnection close];
                    [mConnection release];
                    mConnection = nil;
                }

                [callback invokeWithEvent:PRINTING_EVENT_FINISHED result:PRINTING_RESULT_SUCCESS];
            }
            [callback release];
        }];
    } else {
        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNINITIALIZED];
        [callback release];
    }
}

-(void) requestState:(NSArray*)listOfParameters callback:(id<ICallbackStruct>)callback; {

    [callback setCommand:PRINTING_COMMAND_UPDATE_STATUS];
    
    NSUInteger taskID = -1;

    if (mInitialized) {
        taskID = [mSheduler sheduleOnInstance:self callback:callback printerTask:^PrinterResult {

            PrinterResult prr = PRINTING_RESULT_NONE;

            NSError *error = nil;

            id<ZebraPrinter, NSObject> printer = [ZebraPrinterFactory getInstance:mConnection withPrinterLanguage:PRINTER_LANGUAGE_ZPL];

            PrinterStatus* printerStatus = nil;

            if (error == nil) {
                printerStatus = [printer getCurrentStatus:&error];
            }

            if (error == nil) {
                NSDictionary* status = [self updatePrinterState:printerStatus];

                if (listOfParameters != nil && [listOfParameters count] > 0) {
                    NSMutableDictionary* dict = [NSMutableDictionary dictionaryWithCapacity:[listOfParameters count]];

                    for (NSString* str in listOfParameters) {
                        NSObject* val = [status objectForKey:str];
                        if (val) {
                            [dict setObject:val forKey:str];
                        }
                    }

                    status = dict;
                }

                [callback invokeWithEvent:PRINTING_EVENT_FINISHED result:PRINTING_RESULT_SUCCESS info:status];
            } else {
                prr = errorCodeToStatus(error);
            }

            return prr;
        }];
    } else {
        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNINITIALIZED];
    }
}

-(void) printFile:(NSString*)fileURI options:(NSDictionary*)options callback:(id<ICallbackStruct>)callback {

    [callback setCommand:PRINTING_COMMAND_PRINT];

    NSUInteger taskID = -1;

    NSString* lowercase = [fileURI lowercaseString];

    if (!([lowercase hasSuffix:@".jpg"] || [lowercase hasSuffix:@".png"])) {
        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNSUPPORTED];
        return;
    }

    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:fileURI]){
        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_NOT_FOUND];
        return;
    }

    if (mInitialized) {
        taskID = [mSheduler sheduleOnInstance:self callback:callback printerTask:^PrinterResult {

            PrinterResult prr = PRINTING_RESULT_NONE;

            NSError *error = nil;

            id<ZebraPrinter, NSObject> printer = [ZebraPrinterFactory getInstance:mConnection withPrinterLanguage:PRINTER_LANGUAGE_ZPL];

            if (error == nil) {
                [[printer getGraphicsUtil] printImageFromFile:fileURI atX:0 atY:0 error:&error];
            }

            if (error == nil) {
                [callback invokeWithEvent:PRINTING_EVENT_FINISHED result:PRINTING_RESULT_SUCCESS];

            } else {
                prr = errorCodeToStatus(error);
            }

            return prr;
        }];
    } else {
        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNINITIALIZED];
    }
}

-(void) printRawString:(NSString*)command options:(NSDictionary*)options callback:(id<ICallbackStruct>)callback {

    [callback setCommand:PRINTING_COMMAND_RAW_COMMAND];

    NSUInteger taskID = -1;

    if (mInitialized) {
        taskID = [mSheduler sheduleOnInstance:self callback:callback printerTask:^PrinterResult {

            PrinterResult prr = PRINTING_RESULT_NONE;

            NSError *error = nil;


            id<ZebraPrinter, NSObject> printer = [ZebraPrinterFactory getInstance:mConnection withPrinterLanguage:PRINTER_LANGUAGE_ZPL];

            if (error == nil) {
                // TODO: Add row string send functionality
                //[mConnection write:<#(NSData *)#> error:<#(NSError **)#>]
                [[printer getToolsUtil] sendCommand:command error:&error];
            }

            if (error == nil) {
                [callback invokeWithEvent:PRINTING_EVENT_FINISHED result:PRINTING_RESULT_SUCCESS ];
                
            } else {
                prr = errorCodeToStatus(error);
            }

            return prr;
        }];
    } else {
        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNINITIALIZED];
    }
}


NSArray* parseFileNames(NSString* dirstat, NSArray* ext) {
    // make extensions uppercase
    NSMutableSet* extSet = nil;
    if (ext != nil) {
        extSet = [NSMutableSet setWithCapacity:[ext count]];
        for (NSString* str in ext) {
            [extSet addObject:[str uppercaseString]];
        }
    }
    // split result into lines
    NSArray* rawfiles = [dirstat componentsSeparatedByString:@"\r\n"];
    NSMutableArray* result = [NSMutableArray arrayWithCapacity:[rawfiles count]];
    // parse text lines
    for (NSString* str in rawfiles) {
        // each line should contain __name.ext___size
        NSRegularExpression *regex = [NSRegularExpression regularExpressionWithPattern:@"\\s+([^\\s]+)\\s+[\\d]+" options:NSRegularExpressionCaseInsensitive error:NULL];
        // capture first text group
        NSTextCheckingResult *checkRes = [regex firstMatchInString:str options:NSRegularExpressionCaseInsensitive range:NSMakeRange(0, str.length)];
        // if filename exists, exctract it
        if (checkRes != nil) {
            NSString *substringForFirstMatch = [str substringWithRange:[checkRes rangeAtIndex:1]];
            if (extSet != nil && substringForFirstMatch != nil) {
                NSString* fileExt = [[substringForFirstMatch pathExtension] uppercaseString];
                if (![extSet containsObject:fileExt]) {
                    substringForFirstMatch = nil;
                }
            }
            if (substringForFirstMatch != nil) {
                [result addObject:substringForFirstMatch];
            }
        }
    }
    return result;
}

-(void) retrieveFileNames:(id<ICallbackStruct>)callback {

    [callback setCommand:PRINTING_COMMAND_RECEIVE_FILE_LIST];

    NSUInteger taskID = -1;

    if (mInitialized) {
        taskID = [mSheduler sheduleOnInstance:self callback:callback printerTask:^PrinterResult {

            PrinterResult prr = PRINTING_RESULT_NONE;

            NSError *error = nil;


            //id<ZebraPrinter, NSObject> printer = [ZebraPrinterFactory getInstance:mConnection withPrinterLanguage:PRINTER_LANGUAGE_ZPL];

            NSArray *files = nil;

            if (error == nil) {
                NSString* dirstat = [SGD GET:@"file.dir" withPrinterConnection:mConnection error:&error];
                files = parseFileNames(dirstat, nil);
            }

            if (error == nil) {
                [callback invokeWithEvent:PRINTING_EVENT_FINISHED result:PRINTING_RESULT_SUCCESS info:files];
            } else {
                prr = errorCodeToStatus(error);
            }

            return prr;
        }];
    } else {
        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNINITIALIZED];
    }
}

-(void) retrieveFileNamesWithExtensions:(NSArray*)extensions callback:(id<ICallbackStruct>)callback {

    [callback setCommand:PRINTING_COMMAND_RECEIVE_FILE_LIST];

    NSUInteger taskID = -1;

    if (mInitialized) {
        taskID = [mSheduler sheduleOnInstance:self callback:callback printerTask:^PrinterResult {

            PrinterResult prr = PRINTING_RESULT_NONE;

            NSError *error = nil;

            //id<ZebraPrinter, NSObject> printer = [ZebraPrinterFactory getInstance:mConnection withPrinterLanguage:PRINTER_LANGUAGE_ZPL];

            NSArray *files = nil;

            if (error == nil) {
                NSString* dirstat = [SGD GET:@"file.dir" withPrinterConnection:mConnection error:&error];
                files = parseFileNames(dirstat, extensions);
            }

            if (error == nil) {
                [callback invokeWithEvent:PRINTING_EVENT_FINISHED result:PRINTING_RESULT_SUCCESS info:files];
            } else {
                prr = errorCodeToStatus(error);
            }

            return prr;
        }];
    } else {
        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNINITIALIZED];
    }
}

-(void) sendFileContents:(NSString*)filePath callback:(id<ICallbackStruct>)callback {

    [callback setCommand:PRINTING_COMMAND_STORE_FILE];

    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:filePath]){
        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_NOT_FOUND];
        return;
    }

    NSUInteger taskID = -1;
    if (mInitialized) {
        taskID = [mSheduler sheduleOnInstance:self callback:callback printerTask:^PrinterResult {

            PrinterResult prr = PRINTING_RESULT_NONE;

            NSError *error = nil;


            id<ZebraPrinter, NSObject> printer = [ZebraPrinterFactory getInstance:mConnection withPrinterLanguage:PRINTER_LANGUAGE_ZPL];

            if (error == nil) {
                [[printer getFileUtil] sendFileContents:filePath error:&error];
            }

            if (error == nil) {
                [callback invokeWithEvent:PRINTING_EVENT_FINISHED result:PRINTING_RESULT_SUCCESS];

            } else {
                prr = errorCodeToStatus(error);
            }

            return prr;
        }];
    } else {
        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNINITIALIZED];
    }
}

-(void) retrieveFormatFromPrinterWithPath:(NSString*)formatPathOnPrinter callback:(id<ICallbackStruct>)callback {

    [callback setCommand:PRINTING_COMMAND_RECEIVE_STORED_FORMAT];

    NSUInteger taskID = -1;
    if (mInitialized) {
        taskID = [mSheduler sheduleOnInstance:self callback:callback printerTask:^PrinterResult {

            PrinterResult prr = PRINTING_RESULT_NONE;

            NSError *error = nil;


            id<ZebraPrinter, NSObject> printer = [ZebraPrinterFactory getInstance:mConnection withPrinterLanguage:PRINTER_LANGUAGE_ZPL];

            NSString* contentsAsString = nil;

            if (error == nil) {
                NSData *formatContents = [[printer getFormatUtil] retrieveFormatFromPrinterWithPath:formatPathOnPrinter error:&error];
                if (formatContents != nil) {
                    contentsAsString = [[NSString alloc]initWithData:formatContents encoding:NSUTF8StringEncoding];
                }
            }

            if (error == nil) {
                [callback invokeWithEvent:PRINTING_EVENT_FINISHED result:PRINTING_RESULT_SUCCESS info:contentsAsString];
            } else {
                prr = errorCodeToStatus(error);
            }

            return prr;
        }];
    } else {
        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNINITIALIZED];
    }
}

NSArray* getFormatFromFile(NSString* formatPath, id<ZebraPrinter> printer) {
    NSArray *result = nil;
    NSError *error = nil;

    id<FormatUtil, NSObject> formatUtil = [printer getFormatUtil];

    NSData *formatContents = [formatUtil retrieveFormatFromPrinterWithPath:formatPath error:&error];
    if (formatContents != nil) {
        NSString *contentsAsString = [[[NSString alloc]initWithData:formatContents encoding:NSUTF8StringEncoding]autorelease];
        result = [formatUtil getVariableFieldsWithFormatContents:contentsAsString error:&error];
    }

    return result;
}

-(void) printStoredFormat:(NSString*)formatPathOnPrinter withFormatData:(NSArray*)vars callback:(id<ICallbackStruct>)callback {

    [callback setCommand:PRINTING_COMMAND_PRINT];
    
    NSUInteger taskID = -1;
    if (mInitialized) {
        taskID = [mSheduler sheduleOnInstance:self callback:callback printerTask:^PrinterResult {

            PrinterResult prr = PRINTING_RESULT_NONE;

            NSError *error = nil;

            id<ZebraPrinter, NSObject> printer = [ZebraPrinterFactory getInstance:mConnection withPrinterLanguage:PRINTER_LANGUAGE_ZPL];

            NSArray* format = getFormatFromFile(formatPathOnPrinter, printer);

            if (format != nil) {

                if (error == nil) {
                    [[printer getFormatUtil] printStoredFormat:formatPathOnPrinter withFormatData:vars error:&error];
                }

                if (error == nil) {
                    [callback invokeWithEvent:PRINTING_EVENT_FINISHED result:PRINTING_RESULT_SUCCESS];

                } else {
                    prr = errorCodeToStatus(error);
                }
            } else {
                prr = PRINTING_RESULT_ERR_IO;
            }

            return prr;
        }];
    } else {
        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNINITIALIZED];
    }
}

-(void) printStoredFormat:(NSString*)formatPathOnPrinter withDictionary:(NSDictionary*)vars callback:(id<ICallbackStruct>)callback {

    [callback setCommand:PRINTING_COMMAND_PRINT];
    
    NSUInteger taskID = -1;
    if (mInitialized) {
        taskID = [mSheduler sheduleOnInstance:self callback:callback printerTask:^PrinterResult {

            PrinterResult prr = PRINTING_RESULT_NONE;

            NSError *error = nil;

            id<ZebraPrinter, NSObject> printer = [ZebraPrinterFactory getInstance:mConnection withPrinterLanguage:PRINTER_LANGUAGE_ZPL];

            NSArray* format = getFormatFromFile(formatPathOnPrinter, printer);

            if (format != nil) {

                NSMutableDictionary* dict =  [NSMutableDictionary dictionaryWithCapacity:[format count]];

                NSEnumerator *enumerator = [format objectEnumerator];
                FieldDescriptionData *descriptor = nil;
                BOOL wasFilled = YES;

                while (descriptor = [enumerator nextObject]) {
                    [dict setObject:@"" forKey:descriptor.fieldNumber];

                    // get value from hash by name
                    NSString* value = [vars objectForKey:descriptor.fieldName];
                    if (value != nil) {
                        [dict setObject:value forKey:descriptor.fieldNumber];
                    } else {
                        // get value from hash by field number
                        value = [vars objectForKey:descriptor.fieldNumber];
                        if (value != nil) {
                            [dict setObject:value forKey:descriptor.fieldNumber];
                        } else {
                            // get value from hash by field number converted to string
                            NSString* str = [descriptor.fieldNumber stringValue];
                            value = [vars objectForKey:str];
                            if (value != nil) {
                                [dict setObject:value forKey:descriptor.fieldNumber];
                            } else {
                                wasFilled = NO;
                            }
                        }
                    }
                }

                [[printer getFormatUtil] printStoredFormat:formatPathOnPrinter withDictionary:dict error:&error];

                if (error == nil) {
                    [callback invokeWithEvent:PRINTING_EVENT_FINISHED result:PRINTING_RESULT_SUCCESS];

                } else {
                    prr = errorCodeToStatus(error);
                }
            } else {
                prr = PRINTING_RESULT_ERR_IO;
            }

            return prr;
        }];
    } else {
        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNINITIALIZED];
    }
}


-(void) printImageFromFile:(NSString*)path atX:(NSInteger)x atY:(NSInteger)y withWidth:(NSInteger)width withHeight:(NSInteger)height andIsInsideFormat:(BOOL)isInsideFormat callback:(id<ICallbackStruct>)callback {

    [callback setCommand:PRINTING_COMMAND_PRINT];

    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:path]){
        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_NOT_FOUND];
        return;
    }

    NSUInteger taskID = -1;
    if (mInitialized) {
        taskID = [mSheduler sheduleOnInstance:self callback:callback printerTask:^PrinterResult {

            PrinterResult prr = PRINTING_RESULT_NONE;

            NSError *error = nil;


            id<ZebraPrinter, NSObject> printer = [ZebraPrinterFactory getInstance:mConnection withPrinterLanguage:PRINTER_LANGUAGE_ZPL];

            if (error == nil) {
                [[printer getGraphicsUtil] printImageFromFile:path atX:x atY:y withWidth:width withHeight:height andIsInsideFormat:isInsideFormat error:&error];
            }

            if (error == nil) {
                [callback invokeWithEvent:PRINTING_EVENT_FINISHED result:PRINTING_RESULT_SUCCESS];

            } else {
                prr = errorCodeToStatus(error);
            }

            return prr;
        }];
    } else {
        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNINITIALIZED];
    }
}


-(void) printImage: (CGImageRef)image atX:(NSInteger)x atY:(NSInteger)y withWidth:(NSInteger)width withHeight:(NSInteger)height andIsInsideFormat:(BOOL)isInsideFormat callback:(id<ICallbackStruct>)callback {

    [callback setCommand:PRINTING_COMMAND_PRINT];
    
    NSUInteger taskID = -1;
    if (mInitialized) {
        taskID = [mSheduler sheduleOnInstance:self callback:callback printerTask:^PrinterResult {

            PrinterResult prr = PRINTING_RESULT_NONE;
            

            NSError *error = nil;


            id<ZebraPrinter, NSObject> printer = [ZebraPrinterFactory getInstance:mConnection withPrinterLanguage:PRINTER_LANGUAGE_ZPL];

            if (error == nil) {
                    [[printer getGraphicsUtil] printImage:image atX:x atY:y withWidth:width withHeight:height andIsInsideFormat:isInsideFormat error:&error];
            }

            if (error == nil) {
                [callback invokeWithEvent:PRINTING_EVENT_FINISHED result:PRINTING_RESULT_SUCCESS];

            } else {
                prr = errorCodeToStatus(error);
            }

            return prr;
        }];
    } else {
        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNINITIALIZED];
    }
}

-(void) storeImage:(NSString *)printerDriveAndFileName  withImage:(CGImageRef)image withWidth:(NSInteger)width andWithHeight:(NSInteger)height callback:(id<ICallbackStruct>)callback {

    [callback setCommand:PRINTING_COMMAND_STORE_FILE];

    NSUInteger taskID = -1;
    if (mInitialized) {
        taskID = [mSheduler sheduleOnInstance:self callback:callback printerTask:^PrinterResult {

            PrinterResult prr = PRINTING_RESULT_NONE;


            NSError *error = nil;


            id<ZebraPrinter, NSObject> printer = [ZebraPrinterFactory getInstance:mConnection withPrinterLanguage:PRINTER_LANGUAGE_ZPL];

            if (error == nil) {
                [[printer getGraphicsUtil] storeImage:printerDriveAndFileName withImage:image withWidth:width andWithHeight:height error:&error];
            }

            if (error == nil) {
                [callback invokeWithEvent:PRINTING_EVENT_FINISHED result:PRINTING_RESULT_SUCCESS];

            } else {
                prr = errorCodeToStatus(error);
            }

            return prr;
        }];
    } else {
        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNINITIALIZED];
    }
}

-(void) storeImage:(NSString *)printerDriveAndFileName withPathOnDevice:(NSString *)imageFilePathOnDevice withWidth:(NSInteger)width andWithHeight:(NSInteger)height callback:(id<ICallbackStruct>)callback {

    [callback setCommand:PRINTING_COMMAND_STORE_FILE];

    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:imageFilePathOnDevice]){
        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_NOT_FOUND];
        return;
    }

    NSUInteger taskID = -1;
    if (mInitialized) {
        taskID = [mSheduler sheduleOnInstance:self callback:callback printerTask:^PrinterResult {

            PrinterResult prr = PRINTING_RESULT_NONE;

            NSError *error = nil;

            id<ZebraPrinter, NSObject> printer = [ZebraPrinterFactory getInstance:mConnection withPrinterLanguage:PRINTER_LANGUAGE_ZPL];

            if (error == nil) {
                [[printer getGraphicsUtil] storeImage:printerDriveAndFileName withPathOnDevice:imageFilePathOnDevice withWidth:width andWithHeight:height error:&error];
            }

            if (error == nil) {
                [callback invokeWithEvent:PRINTING_EVENT_FINISHED result:PRINTING_RESULT_SUCCESS];

            } else {
                prr = errorCodeToStatus(error);
            }

            return prr;
        }];
    } else {
        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNINITIALIZED];
    }
}
@end

@implementation PrinterZebraTech

// ITaskSheduler
-(NSUInteger) sheduleTask:(void (^)(void))task {
    NSUInteger result = 0;
    @synchronized(mOperationQueue) {
        if (mOperationQueue) {
            result = ++taskIdCounter;
            [mOperationQueue addOperationWithBlock:^{
                NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
                @try {
                    NSLog(@"sheduleTask block started");
                    task();
                }
                @finally {
                    NSLog(@"sheduleTask block finished");
                    [pool drain];
                }
            }];
        }
    }
    return result;
}

// IPrinterWorker
-(NSArray*) enumerateSupportedTypes {
    return [NSArray arrayWithObject:@"ZEBRA"];
}


-(PrintingDevice) getSupportedDeviceType {
    return PRINTING_DEVICE_ZEBRA;
}

BOOL hasValidAddress(NSString *address) {
    BOOL has_vaild_adress = NO;
    if ( address != nil )
    {
        const char *utf8 = [address UTF8String];
        int success;
        struct in_addr dst;
        success = inet_pton(AF_INET, utf8, &dst);
        
        if (success != 1) {
            struct in6_addr dst6;
            success = inet_pton(AF_INET6, utf8, &dst6);
        }
        
        has_vaild_adress = (success == 1 ? YES : NO);
    }
    return has_vaild_adress;
}

- (BOOL)connectToTCPPrinterInstance:(NSUInteger)port address:(NSString *)address callback:(id)callback {
    BOOL success = NO;

    // check for connection
    id<ZebraPrinterConnection, NSObject> connection = [[[TcpPrinterConnection alloc] initWithAddress:address andWithPort:port] autorelease];
    success = [connection open];
    [connection close];

    NSString* printerID = nil;

    // if connection was successful check for printer instance
    if (success) {

        NSString* key = [PrinterZebraInstace makeInstanceTag:CONNECTION_TCP withAdress:address];
        PrinterZebraInstace *instance = nil;
        
        @synchronized(mDiscoveredPrinters) {
            instance = [mDiscoveredPrinters objectForKey:key];
        }
        // if instance does not exists - create it
        if (instance == nil) {
            PrinterZebraInstace *instance = [[[PrinterZebraInstace alloc] initWithInstanceId:++printerIdCounter andSheduler:self withTCPAdress:address andPort:port] autorelease];
            
            printerID = [instance getTag];
            
            @synchronized(mDiscoveredPrinters) {
                [mDiscoveredPrinters setObject:instance forKey:printerID];
            }
        } else {
            printerID = [instance getTag];
        }
    }

    if (![callback isEnabled]) {
        return YES;
    }

    if (success) {
        // return printer ID
        [callback invokeWithEvent:PRINTING_EVENT_PROCESSING result:PRINTING_RESULT_SUCCESS info:printerID];
    } else {
        [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_NOT_CONNECTED ];
    }

    return success;
}

- (BOOL)networkDiscover:(NSInteger)timeout address:(NSString *)address callback:(id)callback {
    BOOL success = NO;

    NSError *error = nil;
    NSArray *printers = nil;
    
    if (address == nil || [address isEqualToString:LOCAL_CAST]) {
        if (timeout > 0) {
            double startTime = CACurrentMediaTime();
            printers = [NetworkDiscoverer localBroadcastWithTimeout:timeout error:&error];
            double stopTime = CACurrentMediaTime();
            if ((stopTime - startTime) > (timeout/1000)) {
                printers = nil;
            }
            [callback invokeWithEvent:PRINTING_EVENT_PROCESSING result:PRINTING_RESULT_ERR_TIMEOUT];

            return NO;
        } else {
            printers = [NetworkDiscoverer localBroadcast:&error];
        }
    }

    NSMutableArray* printerIds = nil;
    
    if (error == nil && printers != nil) {
        printerIds = [NSMutableArray arrayWithCapacity:[printers count]];
        
        for (id object in printers) {
            if ([object isKindOfClass:[DiscoveredPrinterNetwork class]]) {
                DiscoveredPrinterNetwork* padr = (DiscoveredPrinterNetwork*) object;
                
                NSString* addr = [padr address];
                
                NSString* key = [PrinterZebraInstace makeInstanceTag:CONNECTION_TCP withAdress:addr];
                
                PrinterZebraInstace *instance = nil;
                
                @synchronized(mDiscoveredPrinters) {
                    instance = [mDiscoveredPrinters objectForKey:key];
                }
                
                if (instance == nil) {
                    PrinterZebraInstace *instance = [[[PrinterZebraInstace alloc] initWithInstanceId:++printerIdCounter andSheduler:self withTCPAdress:addr andPort:[padr port]] autorelease];
                    
                    @synchronized(mDiscoveredPrinters) {
                        [mDiscoveredPrinters setObject:instance forKey:key];
                    }
                }
                
                [printerIds addObject:key];
            }
        }
        success = YES;
    }

    if (![callback isEnabled]) {
        return NO;
    }

    if (printerIds != nil) {
        [callback invokeWithEvent:PRINTING_EVENT_PROCESSING result:PRINTING_RESULT_SUCCESS info:printerIds];
    } else {
        if (error != nil) {
            [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:errorCodeToStatus(error)];
        }
    }
    return success;
}

-(BOOL)searchTCP:(NSString *)address port:(NSUInteger)port timeout:(NSInteger)timeout  callback:(id<ICallbackStruct>)callback {
    BOOL has_vaild_adress = hasValidAddress(address);

    if (has_vaild_adress) {
        return [self connectToTCPPrinterInstance:port address:address callback:callback];
    } else {
        return [self networkDiscover:timeout address:address callback:callback];
    }
}

-(void) searchBluetooth:(NSString*)address timeout:(NSInteger)timeout callback:(id<ICallbackStruct>)callback {
    BOOL anyAddress = [address length] == 0 || ([address caseInsensitiveCompare:@"*"] == NSOrderedSame);

    // search for zebra printers
    EAAccessoryManager *sam = [EAAccessoryManager sharedAccessoryManager];
    NSArray * connectedAccessories = [sam connectedAccessories];
    NSMutableArray * zebraAccessories = [[NSMutableArray alloc] initWithCapacity:[connectedAccessories count]];
    for (EAAccessory *accessory in connectedAccessories) {
        if ([accessory.protocolStrings indexOfObject:@"com.zebra.rawport"] != NSNotFound) {
            if (anyAddress) {
                [zebraAccessories addObject:accessory.serialNumber];
            } else if ([address caseInsensitiveCompare:accessory.serialNumber] == NSOrderedSame) {
                [zebraAccessories addObject:accessory.serialNumber];
            }
        }
    }

    // check for connections
    NSMutableArray * activePrinters = [[NSMutableArray alloc] initWithCapacity:[zebraAccessories count]];
    for(NSString *devSN in zebraAccessories) {
        id<ZebraPrinterConnection, NSObject> thePrinterConn = [[MfiBtPrinterConnection alloc] initWithSerialNumber:devSN];
        BOOL success = [thePrinterConn open];
        if (success) {
            [activePrinters addObject:devSN];
        }
        [thePrinterConn close];
    }

    // create printer instance for each found printer
    NSMutableArray* printerIds = [NSMutableArray arrayWithCapacity:[activePrinters count]];
    for(NSString *devSN in activePrinters) {
        NSString* key = [PrinterZebraInstace makeInstanceTag:CONNECTION_BLUETOOTH withAdress:devSN];

        PrinterZebraInstace *instance = nil;

        @synchronized(mDiscoveredPrinters) {
            instance = [mDiscoveredPrinters objectForKey:key];
        }

        if (instance == nil) {
            PrinterZebraInstace *instance = [[[PrinterZebraInstace alloc] initWithInstanceId:++printerIdCounter andSheduler:self withBTAdress:devSN] autorelease];

            @synchronized(mDiscoveredPrinters) {
                [mDiscoveredPrinters setObject:instance forKey:key];
            }
        }

        [printerIds addObject:key];
    }

    if (![callback isEnabled]) {
        return;
    }

    [callback invokeWithEvent:PRINTING_EVENT_PROCESSING result:PRINTING_RESULT_SUCCESS info:printerIds];
}

-(void) searchPrinters:(ConnectionType)connectionType address:(NSString*)address port:(NSUInteger)port timeout:(NSInteger)timeout  callback:(id<ICallbackStruct>)callback {

    [mSearchCallbacks addObject:callback];

    [self sheduleTask:^{
        // set up discovery callback
        [callback setCommand:PRINTING_COMMAND_DISCOVER];
        [callback invokeWithEvent:PRINTING_EVENT_STARTING result:PRINTING_RESULT_NONE];

        BOOL valid_seach = NO;
        BOOL ctAny = connectionType == CONNECTION_ANY;
        // check callback state before each discovery round
        if (![callback isEnabled] || [callback isInvokedWithError]) {
            return;
        }
        if (connectionType == CONNECTION_TCP || ctAny) {
            Reachability *reachability = [Reachability reachabilityForLocalWiFi];
            [reachability startNotifier];

            NetworkStatus status = [reachability currentReachabilityStatus];

            if (connectionType == CONNECTION_TCP || status != NotReachable) {
                [self searchTCP:address port:port timeout:timeout callback:callback];
            }

            valid_seach = YES;
        }
        // check callback state before each discovery round
        if (![callback isEnabled] || [callback isInvokedWithError]) {
            return;
        }
        if (connectionType == CONNECTION_BLUETOOTH || ctAny) {
            [self searchBluetooth:address timeout:timeout callback:callback];
            valid_seach = YES;
        }
        // check callback state after discovery
        if (![callback isEnabled] || [callback isInvokedWithError]) {
            return;
        }
        if (valid_seach) {
            [callback invokeWithEvent:PRINTING_EVENT_FINISHED result:PRINTING_RESULT_SUCCESS];
        } else {
            [callback invokeWithEvent:PRINTING_EVENT_FAILURE result:PRINTING_RESULT_ERR_UNSUPPORTED];
        }

        [mSearchCallbacks removeObject:callback];
    }];
}

// IPrinterZebraTech
-(id<IPrinterZebraInstance>) getPrinterZebraByTag:(NSString*)instanceID {
    id<IPrinterZebraInstance> printer = nil;

    @synchronized(mDiscoveredPrinters) {
        printer = [mDiscoveredPrinters objectForKey:instanceID];
    }
    return printer;

}

-(void) stopCommand:(NSUInteger)command callback:(id<ICallbackStruct>)callback tag:(NSObject*)tag {
    
}

-(NSArray*) getDiscoveredPrinters {
    NSArray* printers;
    @synchronized(mDiscoveredPrinters) {
        printers = [mDiscoveredPrinters allKeys];
    }
    return printers;
}

@end

static PrinterZebraTech* ourPrinterZebraWorker = nil;

@implementation PrinterZebraTechSingleton

+(id<IPrinterZebraTech>) getPrinterZebraWorkerInstance {
    @synchronized(self) {
        if (ourPrinterZebraWorker == nil) {
            ourPrinterZebraWorker = [[PrinterZebraTech alloc] init];
            [[PrinterTechManagerSingleton sharedInstance] registerTech:ourPrinterZebraWorker];
        }
    }
    return ourPrinterZebraWorker;
}

@end
