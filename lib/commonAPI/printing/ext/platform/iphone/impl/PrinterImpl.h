
#import "IPrinter.h"
#import "PrinterBase.h"
#import "IPrinterTech.h"

@interface PrinterImpl : PrinterBase<IPrinter, IPrinterCallbackHandler> {
    id<IMethodResult> mPrinterEventCallback;
    id<IPrinterInstance> mPrinterInstance;
    NSMutableDictionary* mPrinterStatus;
}

-(id) init;
-(void) applyPrinterTech:(id<IPrinterInstance>)printerTech;

-(BOOL) isInitialized;

-(void) dealloc;

-(BOOL) handleEvent:(PrinterCommand)command event:(PrinterEvent)event result:(PrinterResult)resultCode info:(NSDictionary*)info tag:(NSObject*)tag;

-(void) connect:(id<IMethodResult>)methodResult;

-(void) connectWithOptions:(NSDictionary*)options methodResult:(id<IMethodResult>)methodResult;

-(void) requestState:(NSArray*)listOfParameters methodResult:(id<IMethodResult>)methodResult;
-(void) disconnect:(id<IMethodResult>)methodResult;
-(void) printFile:(NSString*)fileURI options:(NSDictionary*)options methodResult:(id<IMethodResult>)methodResult;
-(void) printRawString:(NSString*)command options:(NSDictionary*)options methodResult:(id<IMethodResult>)methodResult;
-(void) enumerateSupportedControlLanguages:(id<IMethodResult>)methodResult;


@end