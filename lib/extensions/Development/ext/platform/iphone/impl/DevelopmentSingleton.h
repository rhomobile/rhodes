
#import "IDevelopment.h"
#import "DevelopmentSingletonBase.h"

@interface DevelopmentSingleton : DevelopmentSingletonBase<IDevelopmentSingleton> {
}


-(NSString*)getInitialDefaultID;


-(void) enumerate:(id<IMethodResult>)methodResult;




@end