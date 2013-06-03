
#import "IScreenOrientation.h"
#import "ScreenOrientationSingletonBase.h"
#import "api_generator/iphone/IMethodResult.h"

@interface ScreenOrientationSingleton : ScreenOrientationSingletonBase<IScreenOrientationSingleton> {
}


-(id)init;


-(void) getAutoRotate:(id<IMethodResult>)methodResult;
-(void) setAutoRotate:(BOOL)autoRotate;
-(void) normal;
-(void) rightHanded;
-(void) leftHanded;
-(void) upsideDown;
-(void) setScreenOrientationEvent:(id<IMethodResult>)methodResult;

-(void)dealloc;


@property (nonatomic,retain) CMethodResult_SimpleHolder* mOrientationCallback;
-(void)handleOrientationChangeNotification:(NSNotification*)notification;


@end