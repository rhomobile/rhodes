
#import "JxcoreSingleton.h"


@implementation JxcoreSingleton


-(NSString*)getInitialDefaultID {
    return @"SCN1";
}


-(void) enumerate:(id<IMethodResult>)methodResult {
    NSMutableArray* ar = [NSMutableArray arrayWithCapacity:2];
    [ar addObject:@"SCN1"];
    [ar addObject:@"SCN2"];
    [methodResult setResult:ar];
}




@end