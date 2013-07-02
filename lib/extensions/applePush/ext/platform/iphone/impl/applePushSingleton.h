
#import "IapplePush.h"
#import "applePushSingletonBase.h"

@interface ApplePushSingleton : ApplePushSingletonBase<IApplePushSingleton> {
}


-(NSString*)getInitialDefaultID;


-(void) enumerate:(id<IMethodResult>)methodResult;




@end