
#import "ICamera.h"
#import "CameraBase.h"

@interface Camera : CameraBase<ICamera> {
}

-(void) takePicture:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult;
-(void) choosePicture:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult;
-(void) saveImageToDeviceGallery:(NSString*)pathToImage;



@end