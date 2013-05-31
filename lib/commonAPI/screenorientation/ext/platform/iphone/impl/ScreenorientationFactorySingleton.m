
#import "IScreenOrientation.h"
#import "ScreenOrientationFactoryBase.h"

static ScreenOrientationFactoryBase* ourScreenOrientationFactory = nil;

@implementation ScreenOrientationFactorySingleton

+(id<IScreenOrientationFactory>) getScreenOrientationFactoryInstance {
    if (ourScreenOrientationFactory == nil) {
        ourScreenOrientationFactory = [[ScreenOrientationFactoryBase alloc] init];
    }
    return ourScreenOrientationFactory;
}

@end