
#import "Camera.h"


void camera_take_picture(NSDictionary* options, id<IMethodResult> callback_api);


@implementation Camera


-(NSDictionary*) constructProperties:(NSDictionary*)options {
    CMethodResult_SimpleHolder* holder = [CMethodResult_SimpleHolder makeEmptyHolder];
    [self getAllProperties:holder];
    NSMutableDictionary* props = [NSMutableDictionary dictionaryWithCapacity:16];
    [props addEntriesFromDictionary:(NSDictionary*)[holder getResult]];
    [props addEntriesFromDictionary:[CameraBase applyAliasesToDictionary:options]];
    return props;
}

-(void) takePicture:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult {
    camera_take_picture([self constructProperties:propertyMap], methodResult);
}






@end