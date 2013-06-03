
#import "ScreenOrientationSingleton.h"
#import "Rhodes.h"


@implementation ScreenOrientationSingleton

@synthesize mOrientationCallback;

-(id)init {
    self = [super init];
    
    [[NSNotificationCenter defaultCenter] addObserver: self selector: @selector(handleOrientationChangeNotification:) name: UIDeviceOrientationDidChangeNotification object: nil];

    
    return self;
}

-(void)handleOrientationChangeNotification:(NSNotification*)notification {
	UIInterfaceOrientation orientation = [[UIApplication sharedApplication] statusBarOrientation];
    
    if ( mOrientationCallback != nil ) {
        switch (orientation) {
            case UIInterfaceOrientationPortrait:
                [mOrientationCallback set: @"normal"];
                break;
            case UIInterfaceOrientationPortraitUpsideDown:
                [mOrientationCallback set: @"upsidedown"];
                break;
            case UIInterfaceOrientationLandscapeLeft:
                [mOrientationCallback set: @"lefthanded"];
                break;
            case UIInterfaceOrientationLandscapeRight:
                [mOrientationCallback set: @"righthanded"];
                break;
        }
    }
}


-(void) getAutoRotate:(id<IMethodResult>)methodResult {
    [methodResult setResult :
     [NSNumber numberWithBool:
      ![[Rhodes sharedInstance] isRotationLocked]
     ]
    ];
}

-(void) setAutoRotate:(BOOL)autoRotate {
    [[Rhodes sharedInstance] setRotationLocked: !autoRotate];
}

-(void) normal {

}

-(void) rightHanded {

}

-(void) leftHanded {

}

-(void) upsideDown {

}

-(void) setScreenOrientationEvent:(id<IMethodResult>)methodResult {
    mOrientationCallback = methodResult;
}

-(void)dealloc {
    self.mOrientationCallback = nil;
    [super dealloc];
}




@end