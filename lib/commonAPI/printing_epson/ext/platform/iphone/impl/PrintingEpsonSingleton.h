
#import "IPrintingEpson.h"
#import "PrintingEpsonSingletonBase.h"

@interface PrintingEpsonSingleton : PrintingEpsonSingletonBase<IPrintingEpsonSingleton> {
}


-(NSString*)getInitialDefaultID;


-(void) enumerate:(id<IMethodResult>)methodResult;




@end