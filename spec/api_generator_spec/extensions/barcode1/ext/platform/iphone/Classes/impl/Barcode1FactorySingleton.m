
#import "../api/IBarcode1.h"
#import "Barcode1FactoryBase.h"

static Barcode1FactoryBase* ourBarcode1Factory = nil;

@implementation Barcode1FactorySingleton

+(id<IBarcode1Factory>) getBarcode1FactoryInstance {
    if (ourBarcode1Factory == nil) {
        ourBarcode1Factory = [[Barcode1FactoryBase alloc] init];
    }
    return ourBarcode1Factory;
}

@end