
#import "IKeyCapture.h"
#import "KeyCaptureSingletonBase.h"
#import <UIKit/UIView.h>

typedef void (^SomeFunction)();

@interface KeyCaptureSingleton : KeyCaptureSingletonBase<IKeyCaptureSingleton> {
    float launchVolume;
    float savedLaunchVolume;
    BOOL hadToLowerVolume;
    BOOL hadToRaiseVolume;

    BOOL _isCapturingVolumeButtons;
    BOOL _suspended;
    UIView *_volumeView;

    id<IMethodResult> callbackHolder;
    NSMutableDictionary *keyCaptureCallbacks;
}

@property (nonatomic, copy) SomeFunction upBlock;
@property (nonatomic, copy) SomeFunction downBlock;
@property (nonatomic, retain) NSMutableDictionary* keyCaptureCallbacks;
@property (readonly) float launchVolume;

-(id) init;
-(void) getHomeKeyValue:(id<IMethodResult>)methodResult;
-(void) setHomeKeyValue:(NSString*)homeKeyValue methodResult:(id<IMethodResult>)methodResult;
-(void) captureKey:(BOOL)dispatch keyValue:(NSString*)keyValue methodResult:(id<IMethodResult>)methodResult;
-(void) remapKey:(NSString*)keyValue remapTo:(NSString*)remapTo methodResult:(id<IMethodResult>)methodResult;
-(void) captureTrigger:(id<IMethodResult>)methodResult;


@end
