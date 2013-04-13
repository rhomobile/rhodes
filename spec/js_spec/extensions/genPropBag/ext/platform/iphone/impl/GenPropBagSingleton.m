
#import "GenPropBagSingleton.h"


@implementation GenPropBagSingleton


-(NSString*)getInitialDefaultID {
    return @"ABC1";
}


-(void) enumerate:(id<IMethodResult>)methodResult {
    NSMutableArray* ar = [NSMutableArray arrayWithCapacity:2];
    [ar addObject:@"ABC1"];
    [ar addObject:@"ABC2"];
    [methodResult setResult:ar];
}




@end