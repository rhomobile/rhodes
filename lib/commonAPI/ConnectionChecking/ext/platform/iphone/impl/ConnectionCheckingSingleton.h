
#import "IConnectionChecking.h"
#import "ConnectionCheckingSingletonBase.h"

@interface ConnectionCheckingSingleton : ConnectionCheckingSingletonBase<IConnectionCheckingSingleton> {
}


-(NSString*)getInitialDefaultID;


-(void) enumerate:(id<IMethodResult>)methodResult;




@end