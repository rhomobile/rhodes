
#import "ICamera.h"
#import "CameraFactory.h"

static CameraFactoryBase* ourCameraFactory = nil;

@implementation CameraFactorySingleton

+(id<ICameraFactory>) getCameraFactoryInstance {
    if (ourCameraFactory == nil) {
        ourCameraFactory = [[CameraFactory alloc] init];
    }
    return ourCameraFactory;
}

@end