
#import "BarcodeFactory.h"
#import "Barcode.h"
#import "BarcodeSingleton.h"

@implementation BarcodeFactory

-(id<IBarcode>)createBarcodeByID:(NSString*)ID {
    Barcode* obj = [[Barcode alloc] init];

    [obj setProperty:@"ID" propertyValue:ID methodResult:nil];
    [obj setProperty:@"friendlyName" propertyValue:@"ZBar S/W Scanner" methodResult:nil];
    [obj setProperty:@"scannerType" propertyValue:@"Camera" methodResult:nil];

    return obj;
}
@end
