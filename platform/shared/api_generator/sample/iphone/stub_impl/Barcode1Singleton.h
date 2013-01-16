
#import "../api/IBarcode1.h"
#import "../base_impl/Barcode1SingletonBase.h"

@interface Barcode1Singleton : Barcode1SingletonBase<IBarcode1Singleton> {
}

-(void) enumerate:(id<IMethodResult>)methodResult;
-(NSString*)getInitialDefaultID;

@end