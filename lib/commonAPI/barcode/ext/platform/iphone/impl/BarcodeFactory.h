
#import "IBarcode.h"
#import "BarcodeFactoryBase.h"

@interface BarcodeFactory : BarcodeFactoryBase<IBarcodeFactory> {
}

-(id<IBarcode>)createBarcodeByID:(NSString*)ID;

@end