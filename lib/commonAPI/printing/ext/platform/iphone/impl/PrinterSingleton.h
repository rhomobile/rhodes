
#import "IPrinter.h"
#import "PrinterSingletonBase.h"

#import "IPrinterTech.h"

@interface PrinterSingleton : PrinterSingletonBase<IPrinterSingleton, IPrinterCallbackHandler> {
    // Dictionary with callback-to-method-result mapping
    NSMutableDictionary* mCallbackToMethodResult;
    // Callback to tech mapping
    NSMutableDictionary* mCallbackToTechMapping;
    // Map method result to tech set, on each success remove item from set
    // on empty set return success
    NSMutableDictionary* mMethodResultTechSets;
}

-(NSString*)getInitialDefaultID;

-(void) enumerateSupportedTypes:(id<IMethodResult>)methodResult;
-(void) searchPrinters:(NSDictionary*)options methodResult:(id<IMethodResult>)methodResult;
-(void) stopSearch:(id<IMethodResult>)methodResult;
-(void) getPrinterByID:(NSString*)printerId methodResult:(id<IMethodResult>)methodResult;
-(void) enumerate:(id<IMethodResult>)methodResult;


@end