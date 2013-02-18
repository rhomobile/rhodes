
#import "ISimpleOnlyStaticModule.h"
#import "SimpleOnlyStaticModuleSingletonBase.h"

@interface SimpleOnlyStaticModuleSingleton : SimpleOnlyStaticModuleSingletonBase<ISimpleOnlyStaticModuleSingleton> {
}



-(void) calcSumm:(int)x y:(int)y methodResult:(id<IMethodResult>)methodResult;
-(void) joinStrings:(NSArray*)strings methodResult:(id<IMethodResult>)methodResult;
-(void) getPlatform:(id<IMethodResult>)methodResult;
-(void) showAlertFromUIThread;




@end