
#import "BarcodeSingleton.h"
#include "common/barcode_engine.h"

@implementation BarcodeSingleton


-(NSString*)getInitialDefaultID {
#ifdef APPLE_BARCODE_ENGINE
    // Apple scanner
    return @"AppleScanner";
#else
#ifdef ZXING
    // ZXing engine
    return @"ZXingScanner";
#else
    //ZBar
    return @"ZBarScanner";
#endif
#endif
}


-(void) enumerate:(id<IMethodResult>)methodResult {
    NSMutableArray* ar = [NSMutableArray arrayWithCapacity:10];
#ifdef APPLE_BARCODE_ENGINE
        // Apple scanner
        [ar addObject:@"AppleScanner"];
#else
#ifdef ZXING
        // ZXing engine
        [ar addObject:@"ZXingScanner"];
#else
        //ZBar
        [ar addObject:@"ZBarScanner"];
#endif
#endif
    [methodResult setResult:ar];
}




@end
