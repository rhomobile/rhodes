
#import "VideocaptureSingleton.h"


@implementation VideocaptureSingleton


-(NSString*)getInitialDefaultID {
    return @"UniversalVideoCapture";
}


-(void) enumerate:(id<IMethodResult>)methodResult {
    NSMutableArray* ar = [NSMutableArray arrayWithCapacity:1];
    [ar addObject:@"UniversalVideoCapture"];
    [methodResult setResult:ar];
}




@end