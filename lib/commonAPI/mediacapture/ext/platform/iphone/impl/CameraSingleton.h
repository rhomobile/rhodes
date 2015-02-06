
#import "ICamera.h"
#import "CameraSingletonBase.h"

@interface CameraSingleton : CameraSingletonBase<ICameraSingleton> {
}


-(NSString*)getInitialDefaultID;


/* Returns the cameras present on your device, allowing you to access your device's front or back camera. */
-(void) enumerate:(id<IMethodResult>)methodResult;

/* Returns the camera of requested type if that camera exist - else return nil. */
-(void) getCameraByType:(NSString*)cameraType methodResult:(id<IMethodResult>)methodResult;

/* Choose a picture from the album. */
-(void) choosePicture:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult;

/* Save an image to the device gallery. */
-(void) copyImageToDeviceGallery:(NSString*)pathToImage methodResult:(id<IMethodResult>)methodResult;



@end