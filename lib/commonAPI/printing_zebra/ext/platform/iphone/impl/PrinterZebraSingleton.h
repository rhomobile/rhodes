
#import "IPrinterZebra.h"
#import "PrinterZebraSingletonBase.h"
#import "IPrinterZebraTech.h"

#import "../../../../../printing/ext/platform/iphone/impl/IPrinterTech.h"

@interface PrinterZebraSingleton : PrinterZebraSingletonBase<IPrinterZebraSingleton, IPrinterCallbackHandler> {
    id<IPrinterZebraTech> mPrinterTech;
}

-(id) init;

-(BOOL) handleEvent:(PrinterCommand)command event:(PrinterEvent)event result:(PrinterResult)resultCode info:(NSObject*)info tag:(NSObject*)tag;

-(NSString*)getInitialDefaultID;

-(void) enumerateSupportedTypes:(id<IMethodResult>)methodResult;
-(void) searchPrinters:(NSDictionary*)options methodResult:(id<IMethodResult>)methodResult;
-(void) stopSearch:(id<IMethodResult>)methodResult;
-(void) getPrinterByID:(NSString*)printerId methodResult:(id<IMethodResult>)methodResult;
-(void) enumerate:(id<IMethodResult>)methodResult;

@end
