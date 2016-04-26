
#import "IKeyState.h"
#import "KeyStateSingletonBase.h"

@interface KeyStateSingleton : KeyStateSingletonBase<IKeyStateSingleton> {
}



-(void) showStates:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult ;
-(void) hideStates:(id<IMethodResult>)methodResult ;




@end