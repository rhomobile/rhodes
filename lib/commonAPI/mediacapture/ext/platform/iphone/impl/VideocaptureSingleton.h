
#import "IVideocapture.h"
#import "VideocaptureSingletonBase.h"

@interface VideocaptureSingleton : VideocaptureSingletonBase<IVideocaptureSingleton> {
}


-(NSString*)getInitialDefaultID;


-(void) enumerate:(id<IMethodResult>)methodResult;




@end