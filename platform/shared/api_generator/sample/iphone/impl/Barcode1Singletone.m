
#import "../api/IBarcode1.h"
#import "Barcode1BaseFactory.h"
#import "Barcode1Factory.h"

static Barcode1Factory* ourBarcode1BaseFactory = nil;

@implementation Barcode1Singletone

+(id<IBarcode1Factory>) getBarcode1Factory {
    if (ourBarcode1BaseFactory == nil) {
        ourBarcode1BaseFactory = [[Barcode1Factory alloc] init];
    }
    return ourBarcode1BaseFactory;
}

@end