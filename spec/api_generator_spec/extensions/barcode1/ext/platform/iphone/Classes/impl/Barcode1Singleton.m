
#import "Barcode1Singleton.h"


@implementation Barcode1Singleton



-(void) enumerate:(id<IMethodResult>)methodResult {
    NSMutableArray* array = [NSMutableArray arrayWithCapacity:2];
    [array addObject:@"SC1"];
    [array addObject:@"SC2"];
    [methodResult setResult:array];
}

-(NSString*)getInitialDefaultID {
    return @"SC1";
}


@end