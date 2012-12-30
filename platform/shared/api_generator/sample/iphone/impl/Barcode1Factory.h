
#import "../api/IBarcode1.h"
#import "Barcode1BaseFactory.h"

@interface Barcode1Factory : Barcode1BaseFactory<IBarcode1Factory> {
}

-(NSString*)getInitialDefaultID;

@end