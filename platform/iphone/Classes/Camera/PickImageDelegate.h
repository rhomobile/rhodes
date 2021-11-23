/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "../RhoDelegate.h"
#import "../../../shared/api_generator/iphone/IMethodResult.h"


//#import "ruby/rho/rhoruby.h"


#define CAMERA_SETTINGS_TYPE_UNKNOWN 0
#define CAMERA_SETTINGS_TYPE_MAIN 1
#define CAMERA_SETTINGS_TYPE_FRONT 2
#define CAMERA_SETTINGS_TYPE_CHOOSE_IMAGE 3

#define CAMERA_SETTINGS_FORMAT_NOT_SETTED 0
#define CAMERA_SETTINGS_FORMAT_JPG 1
#define CAMERA_SETTINGS_FORMAT_PNG 2

#define CAMERA_SETTINGS_COLOR_MODEL_RGB 1
#define CAMERA_SETTINGS_COLOR_MODEL_GRAY 2

#define CAMERA_SETTINGS_EDITING_NOT_SETTED 0
#define CAMERA_SETTINGS_EDITING_ENABLE 1
#define CAMERA_SETTINGS_EDITING_DISABLE 2

#define CAMERA_SETTINGS_FLASH_AUTO 1
#define CAMERA_SETTINGS_FLASH_OFF 2
#define CAMERA_SETTINGS_FLASH_ON 3


@interface RhoCameraSettings : NSObject {
@public
    id<IMethodResult> callback;
    int camera_type;
    int color_model;
    int format;
    int width;
    int height;
    int enable_editing;
    int enable_editing_setted;
    bool save_to_shared_gallery;
    bool is_data_uri;
    NSString* fileName;
    int flash_mode;
}

@property (retain) id<IMethodResult> callback;
@property (assign) int camera_type;
@property (assign) int color_model;
@property (assign) int format;
@property (assign) int width;
@property (assign) int height;
@property (assign) int enable_editing;
@property (assign) int enable_editing_setted;
@property (assign) bool save_to_shared_gallery;
@property (assign) bool is_data_uri;
@property (retain) NSString* fileName;
@property (assign) int flash_mode;

- (id)init:(NSDictionary*)data callback_api:(id<IMethodResult>)callback_api;

@end



@interface PickImageDelegate : RhoDelegate <UINavigationControllerDelegate,
    UIImagePickerControllerDelegate
#ifdef __IPHONE_3_2
    , UIPopoverControllerDelegate
#endif
> {
@public
#ifdef __IPHONE_3_2
    UIPopoverController *popover;
#endif
    RhoCameraSettings* settings;
    BOOL isCaptureProcessRunning;

}

@property (retain) RhoCameraSettings* settings;
@property (atomic) BOOL isCaptureProcessRunning;


- (void)imagePickerController:(UIImagePickerController *)picker
        didFinishPickingImage:(UIImage *)image
                  editingInfo:(NSDictionary *)editingInfo;
- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker;
#ifdef __IPHONE_3_2
- (void)popoverControllerDidDismissPopover:(UIPopoverController *)popoverController;
#endif

@end
