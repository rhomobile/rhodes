
#import "IScreenOrientation.h"
#import "ScreenOrientationSingletonBase.h"
#import "api_generator/iphone/IMethodResult.h"

@interface ScreenOrientationSingleton : ScreenOrientationSingletonBase<IScreenOrientationSingleton> {
}


-(id)init;


-(void) getAutoRotate:(id<IMethodResult>)methodResult;
-(void) setAutoRotate:(BOOL)autoRotate methodResult:(id<IMethodResult>)methodResult;
-(void) normal methodResult:(id<IMethodResult>)methodResult;
-(void) rightHanded methodResult:(id<IMethodResult>)methodResult;
-(void) leftHanded methodResult:(id<IMethodResult>)methodResult;
-(void) upsideDown methodResult:(id<IMethodResult>)methodResult;
-(void) setScreenOrientationEvent:(id<IMethodResult>)methodResult;

-(void)dealloc;


@property (nonatomic,retain) CMethodResult_SimpleHolder* mOrientationCallback;
-(void)handleOrientationChangeNotification:(NSNotification*)notification;


@end