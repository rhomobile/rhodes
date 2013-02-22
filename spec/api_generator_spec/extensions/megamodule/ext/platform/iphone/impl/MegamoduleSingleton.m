
#import "MegamoduleSingleton.h"


@implementation MegamoduleSingleton


-(NSString*)getInitialDefaultID {
    return @"MM1";
}


-(void) getObjectsCount:(id<IMethodResult>)methodResult {
    [methodResult setResult:[NSNumber numberWithInt:3]];
}

-(void) getObjectByIndex:(int)index methodResult:(id<IMethodResult>)methodResult {
    NSString* sID = [NSString stringWithFormat:@"MM%@", [NSNumber numberWithInt:(index+1)]];
    [methodResult setResult:sID];
}

-(void) enumerate:(id<IMethodResult>)methodResult {
    NSMutableArray* array = [NSMutableArray arrayWithCapacity:2];
    [array addObject:@"MM1"];
    [array addObject:@"MM2"];
    [array addObject:@"MM3"];
    [methodResult setResult:array];
}




@end