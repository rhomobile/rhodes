
#import "ICamera.h"
#import "CameraBase.h"

@interface Camera : CameraBase<ICamera> {
}

-(void) takePicture:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult;

+(NSDictionary*)convertValuesToString:(NSDictionary*)dict;

/* Shows the preview in user configurable viewer window. */
-(void) showPreview:(NSDictionary*)propertyMap methodResult:(id<IMethodResult>)methodResult;

/* Hides the preview opened by using showPreview. */
-(void) hidePreview:(id<IMethodResult>)methodResult;

/* Capture the image and save it to a file. */
-(void) capture:(id<IMethodResult>)methodResult;

@end