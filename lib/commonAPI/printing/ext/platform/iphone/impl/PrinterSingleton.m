
#import "PrinterSingleton.h"
#import "PrinterImpl.h"
#import "PrinterTechBase.h"

@implementation PrinterSingleton

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
        mCallbackToMethodResult = [[NSMutableDictionary alloc] initWithCapacity:5];
        mCallbackToTechMapping = [[NSMutableDictionary alloc] initWithCapacity:5];
        mMethodResultTechSets = [[NSMutableDictionary alloc] initWithCapacity:5];
    }
    return self;
}

-(id<ICallbackStruct>) makeCallbackForTech:(id<IPrinterTech>)tech andMethodResult:(id<IMethodResult>)result {
    // self-taggig callback
    PrinterCallback* cbk = [[PrinterCallback alloc] initWithSelfTagAndHandler:self];

    NSValue* key = [NSValue valueWithPointer:cbk];
    NSValue* techKey = [NSValue valueWithPointer:tech];
    NSValue* ptResult = [NSValue valueWithPointer:result];

    // create mapping cbk -> result, cbk -> tech
    [mCallbackToMethodResult setObject:result forKey:key];
    [mCallbackToTechMapping setObject:tech forKey:key];

    // add methodResult -> tech mapping
    NSMutableSet* techSet = [mMethodResultTechSets objectForKey:ptResult];

    if (techSet != nil) {
        [techSet addObject:tech];
    } else {
        [mMethodResultTechSets setObject:[NSMutableSet setWithObject:tech] forKey:ptResult];
    }
    return cbk;
}

-(void) checkPrinterInstance:(id<IPrinterTech>)tech andTag:(NSString*)tag {
    id<IPrinterInstance> inst = [tech getPrinterByTag:tag];
    if (inst) {
        NSString* ptag = [inst getTag];
        id<IPrinter> pinst = [[PrinterFactorySingleton getPrinterFactoryInstance] getPrinterByID:ptag];
        PrinterImpl* prn = ((PrinterImpl*)pinst);
        if (![prn isInitialized]) {
            [prn applyPrinterTech:inst];
        }
    }
}

-(BOOL) handleEvent:(PrinterCommand)command event:(PrinterEvent)event result:(PrinterResult)resultCode info:(NSObject*)info tag:(NSObject*)tag {

    id<IPrinterTech> tech = nil;
    id<IMethodResult> res = nil;
    NSMutableSet* techSet = nil;
    NSValue* ptRes = nil;
    NSValue* cbk = [NSValue valueWithPointer:tag];
    if (tag != nil && [tag isKindOfClass:[PrinterCallback class]]) {
        tech = [mCallbackToTechMapping objectForKey:cbk];
        res = [mCallbackToMethodResult objectForKey:cbk];
        if (res != nil) {
            ptRes = [NSValue valueWithPointer:res];
            techSet = [mMethodResultTechSets objectForKey:ptRes];
        }
    } else {
        cbk = nil;
    }

    if (PRINTING_COMMAND_DISCOVER == command && tech) {
        if (PRINTING_EVENT_PROCESSING == event || PRINTING_RESULT_SUCCESS == resultCode ) {
            if ([info isKindOfClass:[NSArray class]]) {
                NSArray* arr = (NSArray*)info;
                for (NSString* pId in arr) {
                    [self checkPrinterInstance:tech andTag:pId];
                }
            } else if ([info isKindOfClass:[NSString class]]) {
                NSString* str = (NSString*)info;
                [self checkPrinterInstance:tech andTag:str];
            }
        }
        BOOL lastTech = NO;

        if (((PRINTING_EVENT_FAILURE == event) || (PRINTING_EVENT_FINISHED == event)) && (techSet != nil)) {
            [techSet removeObject:tech];
            if ([techSet count] == 0) {
                [mMethodResultTechSets removeObjectForKey:ptRes];
                lastTech = YES;
            }
        }

        BOOL hasCallback = (res != nil) && [res conformsToProtocol:@protocol(IMethodResult)] && [res hasCallback];

        if (hasCallback) {
            NSDictionary* result = nil;

            switch (event) {
                case PRINTING_EVENT_PROCESSING: {
                    NSDictionary* dict = nil;
                    if ([info isKindOfClass:[NSArray class]]) {
                        NSArray* arr = (NSArray*)info;
                        for (NSString* pId in arr) {
                            dict = [NSDictionary dictionaryWithObjectsAndKeys:pId, HK_PRINTER_ID, PRINTER_STATUS_SUCCESS, HK_STATUS, nil];
                            [res setResult:dict];
                        }
                    } else if ([info isKindOfClass:[NSString class]]) {
                        NSString* str = (NSString*)info;
                        dict = [NSDictionary dictionaryWithObjectsAndKeys:str, HK_PRINTER_ID, PRINTER_STATUS_SUCCESS, HK_STATUS, nil];
                        [res setResult:dict];
                    }
                } break;

                case PRINTING_EVENT_FINISHED: {
                    if (lastTech) {
                        result = [NSDictionary dictionaryWithObjectsAndKeys:PRINTER_STATUS_SUCCESS, HK_STATUS, nil];
                        [res setResult:result];
                    }
                } break;

                case PRINTING_EVENT_FAILURE: {
                    if (lastTech) {
                        result = [NSDictionary dictionaryWithObjectsAndKeys:[PrinterSingleton errorStringFromCode:resultCode], HK_STATUS, nil];
                        [res setResult:result];
                    }
                } break;

                default:
                    break;
            }
        }
        if (PRINTING_EVENT_FAILURE == event || PRINTING_EVENT_FINISHED == event) {
            [mCallbackToTechMapping removeObjectForKey:cbk];
            [mCallbackToMethodResult removeObjectForKey:cbk];
        }
    }
    return YES;
}

-(NSString*)getInitialDefaultID {
    return @"";
}

NSString* stringForDevice(PrintingDevice dev) {
    NSString* result = nil;
    switch (dev) {
        case PRINTING_DEVICE_ANY:
            result = PRINTER_TYPE_ANY;
            break;
        case PRINTING_DEVICE_APD:
            result = PRINTER_TYPE_APD;
            break;
        case PRINTING_DEVICE_EPSON:
            result = PRINTER_TYPE_EPSON;
            break;
        case PRINTING_DEVICE_NATIVE:
            result = PRINTER_TYPE_NATIVE;
            break;
        case PRINTING_DEVICE_ZEBRA:
            result = PRINTER_TYPE_ZEBRA;
            break;
        default:
            break;
    }
    return result;
}

-(void) enumerateSupportedTypes:(id<IMethodResult>)methodResult {
    PrinterTechManagerSingleton* manager = [PrinterTechManagerSingleton sharedInstance];
    NSArray* techList = [manager getRegisteredTech];

    NSMutableSet* techNames = [[NSMutableSet alloc] initWithCapacity:[techList count]];

    for (id<IPrinterTech> tech in techList) {
        [techNames addObject:stringForDevice([tech getSupportedDeviceType])];
    }

    [methodResult setResult:[techNames allObjects]];
}


-(void) searchPrinters:(NSDictionary*)options methodResult:(id<IMethodResult>)methodResult {
    PrintingDevice pd = PRINTING_DEVICE_NONE;
    NSString* printer_type = [options valueForKey:HK_PRINTER_TYPE];

    for (int i = PRINTING_DEVICE_NONE; i < PRINTING_DEVICE_COUNT; i++) {
        NSString* printerName = stringForDevice(i);
        if (printerName != nil) {
            if ([printer_type caseInsensitiveCompare:printerName] == NSOrderedSame) {
                pd = i;
            }
        }
    }

    if (pd == PRINTING_DEVICE_NONE) {
        [methodResult setResult:[CMethodResultError errorWithCode:-1 description:PRINTER_STATUS_ERR_UNSUPPORTED]];
        return;
    }

    ConnectionType cType = CONNECTION_ANY;
    NSString* connection_type = [options valueForKey:HK_CONNECTION_TYPE];
    if (connection_type != nil)
    {
        if ( [connection_type caseInsensitiveCompare:CONNECTION_TYPE_TCP] == NSOrderedSame ) {
            cType = CONNECTION_TCP;
        } else if ([connection_type caseInsensitiveCompare:CONNECTION_TYPE_BLUETOOTH] == NSOrderedSame) {
            cType = CONNECTION_BLUETOOTH;
        } else if ([connection_type caseInsensitiveCompare:CONNECTION_TYPE_ANY] == NSOrderedSame) {
            cType = CONNECTION_ANY;
        } else {
            cType = CONNECTION_NONE;
        }
    }

    NSDictionary* errUnsupported = [NSDictionary dictionaryWithObjectsAndKeys:[PrinterSingleton errorStringFromCode:PRINTING_RESULT_ERR_UNSUPPORTED], HK_STATUS, nil];

    if (cType == CONNECTION_NONE) {
        [methodResult setResult:errUnsupported];
    }

    NSString* address = [options valueForKey:HK_DEVICE_ADDRESS];

    int port = 6101; // default port id
    NSString* port_str = [options valueForKey:HK_DEVICE_PORT];
    if (port_str)
    {
        int port_val = [port_str intValue];
        if (port_val > 0) {
            port = port_val;
        }
    }

    NSUInteger timeout = -1;
    NSString* timeout_str = [options valueForKey:HK_TIMEOUT];
    if (timeout_str) {
        int timeout_val = [timeout_str intValue];
        if (timeout_val > 0) {
            timeout = timeout_val;
        }
    }

    PrinterTechManagerSingleton* manager = [PrinterTechManagerSingleton sharedInstance];
    NSArray* techList = [manager getRegisteredTech];

    NSMutableArray* usedTech = nil;

    if (pd == PRINTING_DEVICE_ANY) {
        usedTech = [[NSMutableArray alloc] initWithArray:techList];
    } else {
        usedTech = [[NSMutableArray alloc] initWithCapacity:[techList count]];

        for (id<IPrinterTech> tech in techList) {
            if ([tech getSupportedDeviceType] == pd) {
                [usedTech addObject:tech];
            }
        }
    }

    if ([usedTech count] < 1) {
        [methodResult setResult:errUnsupported];
        return;
    }

    for (id<IPrinterTech> tech in usedTech) {
        id<ICallbackStruct> cbk = [self makeCallbackForTech:tech andMethodResult:methodResult];

        [tech searchPrinters:cType address:address port:port timeout:timeout callback:cbk];
    }
}

-(void) stopSearch:(id<IMethodResult>)methodResult {
    PrinterTechManagerSingleton* manager = [PrinterTechManagerSingleton sharedInstance];
    NSArray* techList = [manager getRegisteredTech];

    for (id<IPrinterTech> tech in techList) {
        [tech stopSearch:[self makeCallbackForTech:tech andMethodResult:methodResult]];
    }
}


-(void) getPrinterByID:(NSString*)printerId methodResult:(id<IMethodResult>)methodResult {
    id<IPrinterInstance> instance = nil;

    PrinterTechManagerSingleton* manager = [PrinterTechManagerSingleton sharedInstance];
    NSArray* techList = [manager getRegisteredTech];

    for (id<IPrinterTech> tech in techList) {
        instance = [tech getPrinterByTag:printerId];
        if (instance != nil) {
            break;
        }
    }

    if (instance != nil) {
        [methodResult setResult:[CRhoAPIClassInstance rubyClassByName:@"Rho.Printer" instanceID:[instance getTag]]];
    } else {
        [methodResult setResult:[NSNull null]];
    }

}

-(void) enumerate:(id<IMethodResult>)methodResult {
    NSArray* printerIds = [[PrinterFactorySingleton getPrinterFactoryInstance] enumeratePrinterInstances];
    NSMutableArray* instances = [[NSMutableArray alloc] initWithCapacity:[printerIds count]];

    for(id key in printerIds) {
        [instances addObject:[CRhoAPIClassInstance rubyClassByName:@"Rho.Printer" instanceID:key]];
    }
    
    [methodResult setResult:instances];
    
}


@end