
#import "IKeyboard.h"
#import "KeyboardSingletonBase.h"

@interface KeyboardSingleton : KeyboardSingletonBase<IKeyboardSingleton> {
}


-(NSString*)getInitialDefaultID;


-(void) enumerate:(id<IMethodResult>)methodResult;




@end