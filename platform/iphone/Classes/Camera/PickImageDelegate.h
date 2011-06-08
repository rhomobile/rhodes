//
//  PickImageDelegate.h
//  rhorunner
//
//  Created by Vlad on 2/12/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

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
