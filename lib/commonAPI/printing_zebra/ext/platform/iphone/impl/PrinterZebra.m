
#import "ZebraPrinter.h"

#import "PrinterZebra.h"
#import "PrinterZebraTech.h"

#import "../../../../../printing/ext/platform/iphone/impl/PrinterTechBase.h"

/////////////////////////////////////////////////////////////////////////

@implementation PrinterZebra

+(NSString*) errorStringFromCode:(PrinterResult)code {
    NSString* result = nil;
    switch (code) {
        case PRINTING_RESULT_NONE:
            result = @"";
            break;
        case PRINTING_RESULT_SUCCESS:
            result = PRINTER_STATUS_SUCCESS;
            break;
        case PRINTING_RESULT_ERROR:
            result = PRINTER_STATUS_ERROR;
            break;
        case PRINTING_RESULT_ERR_IO:
            result = PRINTER_STATUS_ERR_IO;
            break;
        case PRINTING_RESULT_ERR_MEMORY:
            result = PRINTER_STATUS_ERR_MEMORY;
            break;
        case PRINTING_RESULT_ERR_NETWORK:
            result = PRINTER_STATUS_ERR_NETWORK;
            break;
        case PRINTING_RESULT_ERR_NOT_CONNECTED:
            result = PRINTER_STATUS_ERR_NOT_CONNECTED;
            break;
        case PRINTING_RESULT_ERR_NOT_FOUND:
            result = PRINTER_STATUS_ERR_NOT_FOUND;
            break;
        case PRINTING_RESULT_ERR_PARAM:
            result = PRINTER_STATUS_ERR_PARAM;
            break;
        case PRINTING_RESULT_ERR_PROCESSING:
            result = PRINTER_STATUS_ERR_PROCESSING;
            break;
        case PRINTING_RESULT_ERR_RESPONSE:
            result = PRINTER_STATUS_ERR_RESPONSE;
            break;
        case PRINTING_RESULT_ERR_TIMEOUT:
            result = PRINTER_STATUS_ERR_TIMEOUT;
            break;
        case PRINTING_RESULT_ERR_UNSUPPORTED:
            result = PRINTER_STATUS_ERR_UNSUPPORTED;
            break;

        default:
            result = @"Invalid return code";
            break;
    }
    return result;
}

-(id) init {
    self = [super init];
    if( self )
    {
        mPrinterEventCallback = nil;
        mPrinterInstance = nil;
        mPrinterStatus = [[NSMutableDictionary alloc] init];
        [mPrinterStatus setValue:[mProperties objectForKey:@"ID"] forKey:PROPERTY_DEVICE_NAME];
        ZebraConnectParams* params = [[ZebraConnectParams alloc] init];
        [params resetToDefaultValues];
        NSDictionary* paramVals = [params getAsDictionary];
        //for (NSString* key in paramVals) {
        //    [self setProperty:key propertyValue:[paramVals objectForKey:key] methodResult:nil];
        //}
        [self setProperty:PROPERTY_DEVICE_NAME propertyValue:[mProperties objectForKey:@"ID"] methodResult:nil];
        [self setProperty:PROPERTY_PRINTER_TYPE propertyValue:PRINTER_TYPE_ZEBRA methodResult:nil];
        [self setDefaultPrinterStatus];
        [self setDefaultPrinterProperties];
    }
    return self;
}

-(void) applyPrinterTech:(id<IPrinterZebraInstance>)printerInstance {
    mPrinterInstance = [printerInstance retain];

    NSString* conStr = CONNECTION_TYPE_ON_BOARD;

    switch ([mPrinterInstance getConnectionType]) {
        case CONNECTION_TCP:
            conStr = CONNECTION_TYPE_TCP;
            break;
        case CONNECTION_BLUETOOTH:
            conStr = CONNECTION_TYPE_BLUETOOTH;
            break;
        case CONNECTION_USB:
            conStr = CONNECTION_TYPE_ON_BOARD;
        default:
            break;
    }

    [self setProperty:PROPERTY_CONNECTION_TYPE propertyValue:conStr methodResult:nil];
    [self setProperty:PROPERTY_DEVICE_ADDRESS propertyValue:[mPrinterInstance getAddress] methodResult:nil];
    [self setProperty:PROPERTY_DEVICE_PORT propertyValue:[NSString stringWithFormat:@"%d", [mPrinterInstance getPort]] methodResult:nil];
    [self setProperty:PROPERTY_IS_CONNECTED propertyValue:[NSNumber numberWithBool:NO] methodResult:nil];
}

-(void)dealloc {
    [mPrinterStatus release];
    [mPrinterInstance release];

    [super dealloc];
}

-(BOOL) isInitialized {
    return mPrinterInstance != nil;
}

-(void) updateConnectedState {
    if (mPrinterInstance) {
        BOOL connected = [mPrinterInstance isConnected];
        NSNumber* data = [NSNumber numberWithBool:connected];
        [self setProperty:PROPERTY_IS_CONNECTED propertyValue:data methodResult:nil];
        [mPrinterStatus setObject:data forKey:PROPERTY_IS_CONNECTED];
    }
}

-(BOOL) handleEvent:(PrinterCommand)command event:(PrinterEvent)event result:(PrinterResult)resultCode info:(NSDictionary*)info tag:(NSObject*)tag {


    if (PRINTING_EVENT_FAILURE == event || PRINTING_EVENT_FINISHED == event) {
        if (command == PRINTING_COMMAND_CONNECT) {
            for (NSString* key in mDeclaredProperties) {
                NSObject* val = [info objectForKey:key];
                if (val != nil) {
                    [self setProperty:key propertyValue:val methodResult:nil];
                }
            }
        }
        if (command == PRINTING_COMMAND_CONNECT || command == PRINTING_COMMAND_DISCONNECT) {
            [self updateConnectedState];
        }
    }

    // no callback
    if (!tag) {
        return NO;
    }
    // not a IMethodResult
    if (![tag conformsToProtocol:@protocol(IMethodResult)]) {
        return NO;
    }
    id<IMethodResult> callback = (id<IMethodResult>)tag;


    if (![callback hasCallback])
    {
        return NO;
    }

    if (PRINTING_EVENT_FAILURE == event || PRINTING_EVENT_FINISHED == event) {
        BOOL success = event != PRINTING_EVENT_FAILURE;

        NSString* resultStr = [[self class] errorStringFromCode:resultCode];
/*
 PRINTING_COMMAND_RECEIVE_CONTENT,
 PRINTING_COMMAND_RECEIVE_STORED_FORMAT,
 PRINTING_COMMAND_UPDATE_STATUS,
 */
        switch (command) {
            case PRINTING_COMMAND_CONNECT:
            case PRINTING_COMMAND_DISCONNECT:
            case PRINTING_COMMAND_STORE_FILE:
            case PRINTING_COMMAND_SEND_CONTENT:
            case PRINTING_COMMAND_PRINT:
            {
                //dispatch_async(dispatch_get_main_queue(), ^{
                    [callback setResult:resultStr];
                //});
            }
            break;
            case PRINTING_COMMAND_RAW_COMMAND:
            {
                NSDictionary* res = [NSDictionary dictionaryWithObjectsAndKeys:resultStr, HK_STATUS, nil];

                [callback setResult:res];
            }
            break;
            case PRINTING_COMMAND_RECEIVE_FILE_LIST:
            {
                NSDictionary* cbResult = nil;

                if (success && info != nil) {
                    cbResult = [NSDictionary dictionaryWithObjectsAndKeys:info, HK_FILE_NAMES, PRINTER_STATUS_SUCCESS, HK_STATUS, nil];
                } else {
                    cbResult = [NSDictionary dictionaryWithObjectsAndKeys:resultStr, HK_STATUS, nil];
                }

                //dispatch_async(dispatch_get_main_queue(), ^{
                    [callback setResult:cbResult];
                //});

            }
                break;
            case PRINTING_COMMAND_UPDATE_STATUS:
            {
                NSMutableDictionary* cbResult = nil;

                if (success) {
                    NSDictionary* props = [mPrinterInstance getCurrentStatus];
                    if (props) {
                        for (NSString* key in props) {
                            NSObject* value = [props objectForKey:key];
                            [mProperties setObject:value forKey:key];
                            [self setProperty:key propertyValue:value methodResult:nil];
                        }
                    }
                }

                if (success && [info isKindOfClass:[NSDictionary class]]) {
                    cbResult = [NSMutableDictionary dictionaryWithDictionary:info];
                } else {
                    cbResult = [NSMutableDictionary dictionaryWithCapacity:5];
                }

                [cbResult setObject:resultStr forKey:HK_STATUS];

                //dispatch_async(dispatch_get_main_queue(), ^{
                    [callback setResult:cbResult];
                //});
            }
                break;
            default:
                break;
        }
    }
    return YES;
}

- (void)setDefaultPrinterStatus {
    // default printer properties
    [mPrinterStatus setValue:CONNECTION_TYPE_ON_BOARD forKey:PROPERTY_CONNECTION_TYPE];
    [mPrinterStatus setValue:@"0.0.0.0" forKey:PROPERTY_DEVICE_ADDRESS];
    [mPrinterStatus setValue:[NSNumber numberWithInt:0] forKey:PROPERTY_DEVICE_PORT];
    [mPrinterStatus setValue:[NSNumber numberWithBool:NO] forKey:PROPERTY_IS_CONNECTED];
    [mPrinterStatus setValue:[NSNumber numberWithBool:NO] forKey:PRINTER_STATE_IS_READY_TO_PRINT];
    [mPrinterStatus setValue:[NSNumber numberWithBool:NO] forKey:PRINTER_STATE_IS_COVER_OPENED];
    [mPrinterStatus setValue:[NSNumber numberWithBool:NO] forKey:PRINTER_STATE_IS_DRAWER_OPENED];
    [mPrinterStatus setValue:[NSNumber numberWithBool:NO] forKey:PRINTER_STATE_IS_PAPER_OUT];
    [mPrinterStatus setValue:[NSNumber numberWithBool:NO] forKey:PRINTER_STATE_IS_BATTERY_LOW];
    // zebra properties
    [mPrinterStatus setValue:[NSNumber numberWithBool:NO] forKey:PRINTER_STATE_IS_HEAD_COLD];
    [mPrinterStatus setValue:[NSNumber numberWithBool:NO] forKey:PRINTER_STATE_IS_HEAD_OPEN];
    [mPrinterStatus setValue:[NSNumber numberWithBool:NO] forKey:PRINTER_STATE_IS_HEAD_TOO_HOT];
    [mPrinterStatus setValue:[NSNumber numberWithBool:NO] forKey:PRINTER_STATE_IS_PARTIAL_FORMAT_IN_PROGRESS];
    [mPrinterStatus setValue:[NSNumber numberWithBool:NO] forKey:PRINTER_STATE_IS_PAUSED];
    [mPrinterStatus setValue:[NSNumber numberWithBool:NO] forKey:PRINTER_STATE_IS_RECEIVE_BUFFER_FULL];
    [mPrinterStatus setValue:[NSNumber numberWithBool:NO] forKey:PRINTER_STATE_IS_RIBBON_OUT];
    [mPrinterStatus setValue:[NSNumber numberWithBool:NO] forKey:PRINTER_STATE_LABEL_LENGTH_IN_DOTS];
    [mPrinterStatus setValue:[NSNumber numberWithBool:NO] forKey:PRINTER_STATE_LABELS_REMAINING_IN_BATCH];
    [mPrinterStatus setValue:[NSNumber numberWithBool:NO] forKey:PRINTER_STATE_NUMBER_OF_FORMATS_IN_RECEIVE_BUFFER];
    [mPrinterStatus setValue:[NSNumber numberWithBool:NO] forKey:PRINTER_STATE_PRINT_MODE];
}

NSString* intToStr(int value) {
    return [NSString stringWithFormat:@"%d", value];
}

- (void)setDefaultPrinterProperties {
    // default printer properties
    [self setProperty:PROPERTY_CONNECTION_TYPE propertyValue:CONNECTION_TYPE_ON_BOARD methodResult:nil];
    [self setProperty:PROPERTY_DEVICE_ADDRESS propertyValue:@"0.0.0.0" methodResult:nil];
    [self setProperty:PROPERTY_DEVICE_PORT propertyValue:@"0" methodResult:nil];
    [self setProperty:PROPERTY_IS_CONNECTED propertyValue:@"false" methodResult:nil];
    /*
    [self setProperty:PROPERTY_MAX_TIMEOUT_FOR_OPEN propertyValue:intToStr(1000) methodResult:nil];
    // tcp
    [self setProperty:PROPERTY_MAX_TIMEOUT_FOR_READ propertyValue:intToStr([TcpPrinterConnection DEFAULT_MAX_TIMEOUT_FOR_READ]) methodResult:nil];
    [self setProperty:PROPERTY_TIME_TO_WAIT_FOR_MORE_DATA propertyValue:intToStr([TcpPrinterConnection DEFAULT_TIME_TO_WAIT_FOR_MORE_DATA]) methodResult:nil];
    // bluetooth
    [self setProperty:PROPERTY_TIME_TO_WAIT_AFTER_READ_IN_MILLISECONDS propertyValue:intToStr(10) methodResult:nil];
    [self setProperty:PROPERTY_TIME_TO_WAIT_AFTER_WRITE_IN_MILLISECONDS propertyValue:intToStr(200) methodResult:nil];
     */
 }

-(void) setPrinterEventCallback:(id<IMethodResult>)methodResult {
    [mPrinterEventCallback release];
    mPrinterEventCallback = [methodResult retain];
}

-(NSInteger) getIntKey:(NSDictionary*)options key:(NSString*)key {
    NSUInteger result = -1;
    NSObject* val = [options valueForKey:key];
    if (val != nil)
    {
        if ([val isKindOfClass:[NSNumber class]]) {
            result = [((NSNumber*)val) integerValue];
        }
    }
    return result;
}

-(BOOL) getBoolKey:(NSDictionary*)options key:(NSString*)key {
    BOOL result = NO;
    NSObject* val = [options valueForKey:key];
    if (val != nil)
    {
        if ([val isKindOfClass:[NSNumber class]]) {
            result = [((NSNumber*)val) boolValue];
        } else if ([val isKindOfClass:[NSString class]]) {
            result = [((NSString*)val) caseInsensitiveCompare:@"true"] == NSOrderedSame;
        }
    }
    return result;
}

-(void) connect:(id<IMethodResult>)methodResult {
    [mPrinterInstance connect:[PrinterCallback callbackWithHandler:self andTag:methodResult]];
}

-(void) connectWithOptions:(NSDictionary*)options methodResult:(id<IMethodResult>)methodResult {
    ZebraConnectParams* params = [ZebraConnectParams new];
    [params setFromDictionary:mProperties];
    [params setFromDictionary:options];
    [mPrinterInstance connect:[PrinterCallback callbackWithHandler:self andTag:methodResult] withParams:params];
}

-(void) requestState:(NSArray*)listOfParameters methodResult:(id<IMethodResult>)methodResult {
    [self updateConnectedState];
    [mPrinterInstance requestState:listOfParameters callback:[PrinterCallback callbackWithHandler:self andTag:methodResult]];
}

-(void) disconnect:(id<IMethodResult>)methodResult {
    [mPrinterInstance disconnect:[PrinterCallback callbackWithHandler:self andTag:methodResult]];
}

-(void) printFile:(NSString*)fileURI options:(NSDictionary*)options methodResult:(id<IMethodResult>)methodResult {

    [mPrinterInstance printFile:fileURI options:options callback:[PrinterCallback callbackWithHandler:self andTag:methodResult]];
}

-(void) printRawString:(NSString*)command options:(NSDictionary*)options methodResult:(id<IMethodResult>)methodResult {

    [mPrinterInstance printRawString:command options:options callback:[PrinterCallback callbackWithHandler:self andTag:methodResult]];
}

-(void) enumerateSupportedControlLanguages:(id<IMethodResult>)methodResult {
    [methodResult setResult:[mPrinterInstance getSupportedPrinterLanguages]];
}

-(void) retrieveFileNames:(id<IMethodResult>)methodResult {
    [mPrinterInstance retrieveFileNames:[PrinterCallback callbackWithHandler:self andTag:methodResult]];
}

-(void) retrieveFileNamesWithExtensions:(NSArray*)extensions methodResult:(id<IMethodResult>)methodResult {
    [mPrinterInstance retrieveFileNamesWithExtensions:extensions callback:[PrinterCallback callbackWithHandler:self andTag:methodResult]];
}

-(void) sendFileContents:(NSString*)file methodResult:(id<IMethodResult>)methodResult {
    [mPrinterInstance sendFileContents:file callback:[PrinterCallback callbackWithHandler:self andTag:methodResult]];
}

-(void) printStoredFormatWithHash:(NSString*)formatPathOnPrinter vars:(NSDictionary*)vars methodResult:(id<IMethodResult>)methodResult {
    [mPrinterInstance printStoredFormat:formatPathOnPrinter withDictionary:vars callback:[PrinterCallback callbackWithHandler:self andTag:methodResult]];
}

-(void) printStoredFormatWithArray:(NSString*)formatPathOnPrinter vars:(NSArray*)vars methodResult:(id<IMethodResult>)methodResult {
    [mPrinterInstance printStoredFormat:formatPathOnPrinter withFormatData:vars callback:[PrinterCallback callbackWithHandler:self andTag:methodResult]];
}

-(void) printImageFromFile:(NSString*)path x:(int)x y:(int)y options:(NSDictionary*)options methodResult:(id<IMethodResult>)methodResult {
    // todo - handle options

    [mPrinterInstance printImageFromFile:path atX:x atY:y withWidth:[self getIntKey:options key:HK_WIDTH] withHeight:[self getIntKey:options key:HK_HEIGHT] andIsInsideFormat:[self getBoolKey:options key:HK_IS_INSIDE_FORMAT] callback:[PrinterCallback callbackWithHandler:self andTag:methodResult]];
}

-(void) storeImage:(NSString*)printerDriveAndFileName imageFilePathOnDevice:(NSString*)imageFilePathOnDevice width:(int)width height:(int)height methodResult:(id<IMethodResult>)methodResult {
    [mPrinterInstance storeImage:printerDriveAndFileName withPathOnDevice:imageFilePathOnDevice withWidth:width andWithHeight:height callback:[PrinterCallback callbackWithHandler:self andTag:methodResult]];
}

@end
