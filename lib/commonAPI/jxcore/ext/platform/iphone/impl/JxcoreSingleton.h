
#import "IJxcore.h"
#import "JxcoreSingletonBase.h"

@interface JxcoreSingleton : JxcoreSingletonBase<IJxcoreSingleton> {
}


-(NSString*)getInitialDefaultID;


-(void) enumerate:(id<IMethodResult>)methodResult;




@end