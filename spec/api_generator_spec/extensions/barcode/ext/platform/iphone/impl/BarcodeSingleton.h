
#import "IBarcode.h"
#import "BarcodeSingletonBase.h"

@interface BarcodeSingleton : BarcodeSingletonBase<IBarcodeSingleton> {
}


-(NSString*)getInitialDefaultID;


-(void) enumerate:(id<IMethodResult>)methodResult;




@end