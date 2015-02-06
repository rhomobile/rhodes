
#import "CameraFactory.h"
#import "Camera.h"
#import "CameraSingleton.h"

extern int get_camera_max_width(const char* camera_type);
extern int get_camera_max_height (const char* camera_type);



@implementation CameraFactory


-(id<ICamera>)createCameraByID:(NSString*)ID {
    Camera* obj = [[Camera alloc] init];
    [obj setProperty:@"ID" propertyValue:ID methodResult:nil];
    
    // set type
    [obj setProperty:@"cameraType" propertyValue:ID methodResult:nil];
    
    // set maxResolution
    [obj setProperty:@"maxWidth" propertyValue:[NSString stringWithFormat:@"%@", [NSNumber numberWithInt:get_camera_max_width([ID UTF8String])]] methodResult:nil];
    [obj setProperty:@"maxHeight" propertyValue:[NSString stringWithFormat:@"%@", [NSNumber numberWithInt:get_camera_max_height([ID UTF8String])]] methodResult:nil];
    
    return obj;
}

@end