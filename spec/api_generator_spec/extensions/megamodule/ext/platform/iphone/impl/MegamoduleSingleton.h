
#import "IMegamodule.h"
#import "MegamoduleSingletonBase.h"

@interface MegamoduleSingleton : MegamoduleSingletonBase<IMegamoduleSingleton> {
}


-(NSString*)getInitialDefaultID;


-(void) getObjectsCount:(id<IMethodResult>)methodResult;
-(void) getObjectByIndex:(int)index methodResult:(id<IMethodResult>)methodResult;
-(void) enumerate:(id<IMethodResult>)methodResult;




@end