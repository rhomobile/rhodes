
#import "IBarcode.h"
#import "BarcodeFactoryBase.h"

static BarcodeFactoryBase* ourBarcodeFactory = nil;

@implementation BarcodeFactorySingleton

+(id<IBarcodeFactory>) getBarcodeFactoryInstance {
    if (ourBarcodeFactory == nil) {
        ourBarcodeFactory = [[BarcodeFactoryBase alloc] init];
    }
    return ourBarcodeFactory;
}

@end