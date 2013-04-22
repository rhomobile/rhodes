
#import "IGenPropBag.h"
#import "GenPropBagSingletonBase.h"

@interface GenPropBagSingleton : GenPropBagSingletonBase<IGenPropBagSingleton> {
}


-(NSString*)getInitialDefaultID;


-(void) enumerate:(id<IMethodResult>)methodResult;




@end