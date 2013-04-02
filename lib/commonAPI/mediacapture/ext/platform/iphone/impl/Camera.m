
#import "Camera.h"

extern void save_image_to_device_gallery(const char* image_path);
void camera_take_picture(NSDictionary* options, id<IMethodResult> callback_api);
void camera_choose_picture(NSDictionary* options, id<IMethodResult> callback_api);


@implementation Camera


-(NSDictionary*) constructProperties:(NSDictionary*)options {
    CMethodResult_SimpleHolder* holder = [CMethodResult_SimpleHolder makeEmptyHolder];
    [self setProperties:options];
    [self getAllProperties:holder];
    NSMutableDictionary* props = [NSMutableDictionary dictionaryWithCapacity:16];
    [props addEntriesFromDictionary:(NSDictionary*)[holder getResult]];
    //[props addEntriesFromDictionary:options];
    return props;
}

-(void) takePicture:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult {
    camera_take_picture([self constructProperties:propertyMap], methodResult);
}

-(void) choosePicture:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult {
    camera_choose_picture([self constructProperties:propertyMap], methodResult);
}

-(void) saveImageToDeviceGallery:(NSString*)pathToImage {
    save_image_to_device_gallery([pathToImage UTF8String]);
}




@end