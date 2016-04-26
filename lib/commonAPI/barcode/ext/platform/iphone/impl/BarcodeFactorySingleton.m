
#import "IBarcode.h"
#import "BarcodeFactory.h"

static BarcodeFactoryBase* ourBarcodeFactory = nil;

@implementation BarcodeFactorySingleton

+(id<IBarcodeFactory>) getBarcodeFactoryInstance {
    if (ourBarcodeFactory == nil) {
        ourBarcodeFactory = [[BarcodeFactory alloc] init];
    }
    return ourBarcodeFactory;
}

@end