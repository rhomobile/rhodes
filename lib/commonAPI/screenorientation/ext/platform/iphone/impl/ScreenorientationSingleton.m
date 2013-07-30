
#import "ScreenOrientationSingleton.h"
#import "Rhodes.h"
#import "UIKit/UIKit.h"


@implementation ScreenOrientationSingleton

@synthesize mOrientationCallback;

-(id)init {
    self = [super init];
    
    [[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(handleOrientationChangeNotification:) name: UIDeviceOrientationDidChangeNotification object: nil];

    
    return self;
}

-(void)handleOrientationChangeNotification:(NSNotification*)notification {
    if ( (mOrientationCallback != nil) && (![[Rhodes sharedInstance] isRotationLocked])) {
        UIDeviceOrientation dorient = [UIDevice currentDevice].orientation;

        if (dorient == UIDeviceOrientationPortrait) {
            [mOrientationCallback setResult: @"normal"];
        } else if (dorient == UIDeviceOrientationPortraitUpsideDown) {
            [mOrientationCallback setResult: @"upsidedown"];
        } else if (dorient == UIDeviceOrientationLandscapeLeft) {
            [mOrientationCallback setResult: @"lefthanded"];
        } else if (dorient == UIDeviceOrientationLandscapeRight) {
            [mOrientationCallback setResult: @"righthanded"];
        } else {
            /*
            UIInterfaceOrientation orientation = [[UIApplication sharedApplication] statusBarOrientation];
            
            if ( mOrientationCallback != nil ) {
                switch (orientation) {
                    case UIInterfaceOrientationPortrait:
                        [mOrientationCallback setResult: @"normal"];
                        break;
                    case UIInterfaceOrientationPortraitUpsideDown:
                        [mOrientationCallback setResult: @"upsidedown"];
                        break;
                    case UIInterfaceOrientationLandscapeLeft:
                        [mOrientationCallback setResult: @"lefthanded"];
                        break;
                    case UIInterfaceOrientationLandscapeRight:
                        [mOrientationCallback setResult: @"righthanded"];
                        break;
                }
            }
             */
        }
    }
}


-(void) getAutoRotate:(id<IMethodResult>)methodResult {
    BOOL auto_enable = ![[Rhodes sharedInstance] isRotationLocked];
    [methodResult setResult:[NSNumber numberWithBool:auto_enable]];
}

-(void) setAutoRotate:(BOOL)autoRotate methodResult:(id<IMethodResult>)methodResult {
    [[Rhodes sharedInstance] setRotationLocked: !autoRotate];
}

-(void) normal:(id<IMethodResult>)methodResult {

}

-(void) rightHanded:(id<IMethodResult>)methodResult {

}

-(void) leftHanded:(id<IMethodResult>)methodResult {

}

-(void) upsideDown:(id<IMethodResult>)methodResult {

}

-(void) setScreenOrientationEvent:(id<IMethodResult>)methodResult {
    mOrientationCallback = methodResult;
}

-(void)dealloc {
    self.mOrientationCallback = nil;
    [super dealloc];
}




@end