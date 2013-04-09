
#import "CameraSingleton.h"
#import "CameraBase.h"

extern int get_camera_max_width(const char* camera_type);
void camera_choose_picture(NSDictionary* options, id<IMethodResult> callback_api);

@implementation CameraSingleton


-(NSString*)getInitialDefaultID {
    return CAMERA_TYPE_MAIN;
}


-(void) enumerate:(id<IMethodResult>)methodResult {
    NSMutableArray* ar = [NSMutableArray arrayWithCapacity:2];
    if (get_camera_max_width([CAMERA_TYPE_MAIN UTF8String]) > 0) {
        [ar addObject:CAMERA_TYPE_MAIN];
    }
    if (get_camera_max_width([CAMERA_TYPE_FRONT UTF8String]) > 0) {
        [ar addObject:CAMERA_TYPE_FRONT];
    }
    [methodResult setResult:ar];
}

-(void) getCameraByType:(NSString*)cameraType methodResult:(id<IMethodResult>)methodResult {
    if (get_camera_max_width([cameraType UTF8String]) > 0) {
        [methodResult setResult:cameraType];
    }
}


-(void) choosePicture:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult {
    camera_choose_picture([CameraBase applyAliasesToDictionary:propertyMap], methodResult);    
}



@end