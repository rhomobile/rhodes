
#import "ICamera.h"
#import "CameraFactoryBase.h"

@interface CameraFactory : CameraFactoryBase<ICameraFactory> {
}

-(id<ICamera>)createCameraByID:(NSString*)ID;

@end