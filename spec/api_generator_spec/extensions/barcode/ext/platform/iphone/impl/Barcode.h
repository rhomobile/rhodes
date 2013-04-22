
#import "IBarcode.h"
#import "BarcodeBase.h"

@interface Barcode : BarcodeBase<IBarcode> {
}

-(void) enable:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult;
-(void) start;
-(void) stop;
-(void) disable;
-(void) take:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult;



@end