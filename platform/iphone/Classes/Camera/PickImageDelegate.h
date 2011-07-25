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
#import "RhoDelegate.h"


//#import "ruby/rho/rhoruby.h"


#define CAMERA_SETTINGS_TYPE_MAIN 1
#define CAMERA_SETTINGS_TYPE_FRONT 2

#define CAMERA_SETTINGS_FORMAT_JPG 1
#define CAMERA_SETTINGS_FORMAT_PNG 2

#define CAMERA_SETTINGS_COLOR_MODEL_RGB 1
#define CAMERA_SETTINGS_COLOR_MODEL_GRAY 2



@interface RhoCameraSettings : NSObject {
@public
    NSString* callback_url;
    int camera_type;
    int color_model;
    int format;
    int width;
    int height;
    int enable_editing;
}

@property (retain) NSString* callback_url;
@property (assign) int camera_type;
@property (assign) int color_model;
@property (assign) int format;
@property (assign) int width;
@property (assign) int height;
@property (assign) int enable_editing;

- (id)init:(void*)data url:(NSString*)url;

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

}

@property (retain) RhoCameraSettings* settings;


- (void)imagePickerController:(UIImagePickerController *)picker
        didFinishPickingImage:(UIImage *)image
                  editingInfo:(NSDictionary *)editingInfo;
- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker;
#ifdef __IPHONE_3_2
- (void)popoverControllerDidDismissPopover:(UIPopoverController *)popoverController;
#endif

@end
