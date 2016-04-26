
#import "IBarcode.h"
#import "BarcodeBase.h"

@interface Barcode : BarcodeBase<IBarcode> {
}

-(void) enable:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult;
-(void) registerBluetoothStatus:(id<IMethodResult>)methodResult;
-(void) start:(id<IMethodResult>)methodResult;
-(void) stop:(id<IMethodResult>)methodResult;
-(void) disable:(id<IMethodResult>)methodResult;
-(void) barcode_recognize:(NSString*)imageFilePath methodResult:(id<IMethodResult>)methodResult;
-(void) getSupportedProperties:(id<IMethodResult>)methodResult;
-(void) take:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult;
-(void) take_barcode:(NSString*)rubyCallbackURL propertyMap:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult;
-(void) commandRemoteScanner:(NSString*)command methodResult:(id<IMethodResult>)methodResult;



@end