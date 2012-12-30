
#import "../api/IBarcode1.h"
#import "Barcode1Base.h"

@interface Barcode1 : Barcode1Base<IBarcode1> {
    
}

- (id) init:(NSString*)_ID;

-(void) getProps:(id<IMethodResult>)methodResult;
-(void) getPropsWithString:(NSString*)param1 methodResult:(id<IMethodResult>)methodResult;
-(void) getPropsWithArray:(NSArray*)param1 methodResult:(id<IMethodResult>)methodResult;

+(void) enumerate:(id<IMethodResult>)methodResult;


@end