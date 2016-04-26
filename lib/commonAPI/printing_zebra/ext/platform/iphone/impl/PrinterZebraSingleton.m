
#import "ZebraPrinter.h"

#import "PrinterZebraSingleton.h"
#import "PrinterZebra.h"
#include <arpa/inet.h>

#import "../../../../../printing/ext/platform/iphone/impl/PrinterTechBase.h"
#import "PrinterZebraTech.h"

@implementation PrinterZebraSingleton

-(id) init {
    self = [super init];
    if( self )
    {
        mPrinterTech = [PrinterZebraTechSingleton getPrinterZebraWorkerInstance];
    }
    return self;
}

-(void) checkPrinterInstance:(NSString*)tag {
    id<IPrinterZebraInstance> inst = [mPrinterTech getPrinterZebraByTag:tag];
    if (inst) {
        NSString* ptag = [inst getTag];
        id<IPrinterZebra> pinst = [[PrinterZebraFactorySingleton getPrinterZebraFactoryInstance] getPrinterZebraByID:ptag];
        PrinterZebra* prz = ((PrinterZebra*)pinst);
        if (![prz isInitialized]) {
            [prz applyPrinterTech:inst];
        }
    }
}

-(BOOL) handleEvent:(PrinterCommand)command event:(PrinterEvent)event result:(PrinterResult)resultCode info:(NSObject*)info tag:(NSObject*)tag {

    if (PRINTING_COMMAND_DISCOVER == command) {
        if (PRINTING_EVENT_PROCESSING == event || PRINTING_RESULT_SUCCESS == resultCode ) {
            if ([info isKindOfClass:[NSArray class]]) {
                NSArray* arr = (NSArray*)info;
                for (NSString* pId in arr) {
                    [self checkPrinterInstance:pId];
                }
            } else if ([info isKindOfClass:[NSString class]]) {
                NSString* str = (NSString*)info;
                [self checkPrinterInstance:str];
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

        NSDictionary* result = nil;

        switch (event) {
            case PRINTING_EVENT_PROCESSING: {
                NSDictionary* dict = nil;
                if ([info isKindOfClass:[NSArray class]]) {
                    NSArray* arr = (NSArray*)info;
                    for (NSString* pId in arr) {
                        dict = [NSDictionary dictionaryWithObjectsAndKeys:pId, HK_PRINTER_ID, PRINTER_STATUS_SUCCESS, HK_STATUS, nil];
                        [callback setResult:dict];
                    }
                } else if ([info isKindOfClass:[NSString class]]) {
                    NSString* str = (NSString*)info;
                    dict = [NSDictionary dictionaryWithObjectsAndKeys:str, HK_PRINTER_ID, PRINTER_STATUS_SUCCESS, HK_STATUS, nil];
                    [callback setResult:dict];
                }
            } break;

            case PRINTING_EVENT_FINISHED: {
                result = [NSDictionary dictionaryWithObjectsAndKeys:PRINTER_STATUS_SUCCESS, HK_STATUS, nil];
                [callback setResult:result];
            } break;

            case PRINTING_EVENT_FAILURE: {
                result = [NSDictionary dictionaryWithObjectsAndKeys:[PrinterZebra errorStringFromCode:resultCode], HK_STATUS, nil];
                [callback setResult:result];
            } break;

            default:
                break;
        }
    }
    return YES;
}

-(NSString*)getInitialDefaultID {
    return @"";
}

-(void) enumerateSupportedTypes:(id<IMethodResult>)methodResult {
    NSArray* array = [NSArray arrayWithObject:PRINTER_TYPE_ZEBRA];
    [methodResult setResult:array];
}


-(void) searchPrinters:(NSDictionary*)options methodResult:(id<IMethodResult>)methodResult {
    NSString* printer_type = [options valueForKey:HK_PRINTER_TYPE];
    if (printer_type != nil)
    {
        if ( !([printer_type caseInsensitiveCompare:PRINTER_TYPE_ZEBRA] == NSOrderedSame || [printer_type caseInsensitiveCompare:PRINTER_TYPE_ANY] == NSOrderedSame) ) {
            [methodResult setResult:[CMethodResultError errorWithCode:-1 description:PRINTER_STATUS_ERR_UNSUPPORTED]];
            return;
        }
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

    if (cType == CONNECTION_NONE) {
        [methodResult setResult:[CMethodResultError errorWithCode:-1 description:PRINTER_STATUS_ERR_UNSUPPORTED]];
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

    [mPrinterTech searchPrinters:cType address:address port:port timeout:timeout callback:[PrinterCallback callbackWithHandler:self andTag:methodResult]];
}

-(void) stopSearch:(id<IMethodResult>)methodResult {
    [mPrinterTech stopSearch:[PrinterCallback callbackWithHandler:self andTag:methodResult]];
}


-(void) getPrinterByID:(NSString*)printerId methodResult:(id<IMethodResult>)methodResult {
    id<IPrinterInstance> inst = [mPrinterTech getPrinterZebraByTag:printerId];

    if (inst != nil)
    {
        [methodResult setResult:[CRhoAPIClassInstance rubyClassByName:@"Rho.PrinterZebra" instanceID:[inst getTag]]];
    } else {
        [methodResult setResult:[NSNull null]];
    }
    
}

-(void) enumerate:(id<IMethodResult>)methodResult {
    NSArray* printerIds = [[PrinterZebraFactorySingleton getPrinterZebraFactoryInstance] enumeratePrinterZebraInstances];
    NSMutableArray* instances = [[NSMutableArray alloc] initWithCapacity:[printerIds count]];
    
    for(id key in printerIds) {
        [instances addObject:[CRhoAPIClassInstance rubyClassByName:@"Rho.PrinterZebra" instanceID:key]];
    }

    [methodResult setResult:instances];

}


@end
