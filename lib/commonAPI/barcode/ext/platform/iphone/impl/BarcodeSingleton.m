
#import "BarcodeSingleton.h"


@implementation BarcodeSingleton


-(NSString*)getInitialDefaultID {
    return @"ZBarScanner";
}


-(void) enumerate:(id<IMethodResult>)methodResult {
    NSMutableArray* ar = [NSMutableArray arrayWithCapacity:10];
    [ar addObject:@"ZBarScanner"];
    [methodResult setResult:ar];
}




@end