
#import "IVideocapture.h"
#import "VideocaptureBase.h"

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

@interface Videocapture : VideocaptureBase<IVideocapture,UIImagePickerControllerDelegate> {
    id<IMethodResult> callback;
    BOOL isSaveFile;
    UIImagePickerController* moviePickerController;
    NSString* recorderFilePath;
}

-(id) init;
-(void) start:(id<IMethodResult>)methodResult;
-(void) stop;
-(void) cancel;

- (void) fireCallback:(NSString*)status fileName:(NSString*)fileName fileSize:(NSNumber*)fileSize;

// UIImagePickerControllerDelegate
- (void) imagePickerControllerDidCancel: (UIImagePickerController *) picker;
- (void) imagePickerController: (UIImagePickerController *) picker didFinishPickingMediaWithInfo: (NSDictionary *) info;

@end