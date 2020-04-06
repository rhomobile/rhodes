
#import "BarcodeFactory.h"
#import "Barcode.h"
#import "BarcodeSingleton.h"

#include "common/barcode_engine.h"

@implementation BarcodeFactory

-(id<IBarcode>)createBarcodeByID:(NSString*)ID {
    Barcode* obj = [[Barcode alloc] init];

    [obj setProperty:@"ID" propertyValue:ID methodResult:nil];
#ifdef APPLE_BARCODE_ENGINE
    // Apple scanner
    [obj setProperty:@"friendlyName" propertyValue:@"Apple S/W Scanner" methodResult:nil];
#else
#ifdef ZXING
    // ZXing engine
    [obj setProperty:@"friendlyName" propertyValue:@"ZXing S/W Scanner" methodResult:nil];
#else
    //ZBar
    [obj setProperty:@"friendlyName" propertyValue:@"ZBar S/W Scanner" methodResult:nil];
#endif
#endif
    [obj setProperty:@"scannerType" propertyValue:@"Camera" methodResult:nil];

    return obj;
}
@end
