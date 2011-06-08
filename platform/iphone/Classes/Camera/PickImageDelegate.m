//
//  PickImageDelegate.m
//  rhorunner
//
//  Created by Vlad on 2/12/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import "Rhodes.h"
#import "PickImageDelegate.h"
#import "AppManager.h"
#import "common/RhodesApp.h"


#include "ruby/ext/rho/rhoruby.h"




@implementation RhoCameraSettings

@synthesize callback_url, camera_type, color_model, format, width, height, enable_editing;

- (id)init:(void*)data url:(NSString*)url{
    self.callback_url = url;
    self.camera_type = CAMERA_SETTINGS_TYPE_MAIN;
    self.color_model = CAMERA_SETTINGS_COLOR_MODEL_RGB;
    self.format = CAMERA_SETTINGS_FORMAT_JPG;
    self.width = 0;
    self.height = 0;
    self.enable_editing = 1;
        
    if (data != NULL) {
        rho_param* p =(rho_param*)data; 
        
        if (p->type == RHO_PARAM_HASH) {
            for (int i = 0, lim = p->v.hash->size; i < lim; ++i) {
                const char *name = p->v.hash->name[i];
                rho_param *value = p->v.hash->value[i];
                
                if (strcasecmp(name, "camera_type") == 0) {
                    if (strcasecmp(value->v.string, "front") == 0) {
                        self.camera_type = CAMERA_SETTINGS_TYPE_FRONT;
                    }
                }
                if (strcasecmp(name, "color_model") == 0) {
                    if (strcasecmp(value->v.string, "Grayscale") == 0) {
                        self.color_model = CAMERA_SETTINGS_COLOR_MODEL_GRAY;
                    }
                }
                if (strcasecmp(name, "format") == 0) {
                    if (strcasecmp(value->v.string, "png") == 0) {
                        self.format = CAMERA_SETTINGS_FORMAT_PNG;
                    }
                }
                if (strcasecmp(name, "desired_width") == 0) {
                    NSString* s = [NSString stringWithUTF8String:value->v.string];
                    self.width = [s intValue];
                }
                if (strcasecmp(name, "desired_height") == 0) {
                    NSString* s = [NSString stringWithUTF8String:value->v.string];
                    self.height = [s intValue];
                }
                if (strcasecmp(name, "enable_editing") == 0) {
                    self.enable_editing = 1;
                    if (strcasecmp(value->v.string, "false") == 0) {
                        self.enable_editing = 0;
                    }
                }
                
            }
            
        }
    
    }
    
    return self;
}


- (void)dealloc {
    [callback_url release];
    [super dealloc];
}

@end




@implementation PickImageDelegate

@synthesize settings;

- (void)useImage:(UIImage*)theImage { 
    NSString *folder = [[AppManager getApplicationsRootPath] stringByAppendingPathComponent:@"/../db/db-files"];

    
    UIImage* img = theImage;
    
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:folder])
        [fileManager createDirectoryAtPath:folder attributes:nil];

    NSString *now = [[[NSDate date] descriptionWithLocale:nil]
             stringByReplacingOccurrencesOfString: @":" withString: @"."];
    now = [now stringByReplacingOccurrencesOfString: @" " withString: @"_"];
    now = [now stringByReplacingOccurrencesOfString: @"+" withString: @"_"];

     
    CGImageRef cgImageToRelease = nil;
    
    if (settings.color_model == CAMERA_SETTINGS_COLOR_MODEL_GRAY) {
        CGImageRef cgImage = [theImage CGImage];
        CGColorSpaceRef mono_space = CGColorSpaceCreateDeviceGray();
        
        int curWidth = CGImageGetWidth(cgImage);//(int)img.size.width;
        int curHeight = CGImageGetHeight(cgImage);//(int)img.size.height;
        void * data = malloc(curWidth*curHeight);
        
        CGContextRef cgContext = CGBitmapContextCreate( data,
                                                       curWidth,
                                                       curHeight,
                                                       8,
                                                       curWidth,
                                                       mono_space,
                                                       kCGImageAlphaNone);
        
        
        CGContextDrawImage(cgContext, CGRectMake(0, 0, curWidth, curHeight), cgImage);
        
        CGImageRef monocgImage = CGBitmapContextCreateImage(cgContext);
        //[img release];
        img = [UIImage imageWithCGImage:monocgImage scale:1.0 orientation:[theImage imageOrientation]];
        
        CGColorSpaceRelease(mono_space);
        CGImageRelease(monocgImage);
        CGContextRelease(cgContext);
        free(data);
    }
    
    if ((settings.width > 0) && (settings.height > 0)) {
        float fcurWidth = img.size.width;
        float fcurHeight = img.size.height;
        
        float desiredWidth = settings.width;
        float desiredHeight = settings.height;
        
        float kw = desiredWidth / fcurWidth;
        float kh = desiredHeight / fcurHeight;
        
        if (kw > 1.0) {
            if (kh > kw) {
                kw = kw/kh;
                kh = 1.0;
            }
            else {
                kh = kh/kw;
                kw = 1.0;
            }
        }
        else {
            if (kh > 1.0) {
                kw = kw/kh;
                kh = 1.0;
            }
        }
        
        float k = kw;
        if (kw > kh) {
            k = kh;
        }

        //if (([theImage imageOrientation] == UIImageOrientationLeft) ||
        //    ([theImage imageOrientation] == UIImageOrientationRight) ||
        //    ([theImage imageOrientation] == UIImageOrientationLeftMirrored) ||
        //    ([theImage imageOrientation] == UIImageOrientationRightMirrored) ) 
        //{
        //    curWidth = img.size.height;
        //    curHeight = img.size.width;
        //    
        //}

        
        CGImageRef cgImage = [img CGImage];
        CGColorSpaceRef rgb_space = CGColorSpaceCreateDeviceRGB();
        
        int curWidth = CGImageGetWidth(cgImage);//(int)img.size.width;
        int curHeight = CGImageGetHeight(cgImage);//(int)img.size.height;
        
        int newWidth = curWidth*k;
        int newHeight = curHeight*k;
        
        void * data = malloc(newWidth*newHeight*4);
        
        CGContextRef cgContext = CGBitmapContextCreate( data,
                                                       newWidth,
                                                       newHeight,
                                                       8,
                                                       newWidth*4,
                                                       rgb_space,
                                                       kCGImageAlphaNoneSkipLast);
        
        
        CGContextDrawImage(cgContext, CGRectMake(0, 0, newWidth, newHeight), cgImage);
        
        CGImageRef rgbImage = CGBitmapContextCreateImage(cgContext);
        //[img release];
        UIImage* newImg = [UIImage imageWithCGImage:rgbImage scale:1.0 orientation:[theImage imageOrientation]];
        if (img != theImage) {
            //[img release];
        }
        img = newImg;
        
        CGColorSpaceRelease(rgb_space);
        CGImageRelease(rgbImage);
        CGContextRelease(cgContext);
        free(data);
        
        
        
        
        
        
        
        
        
        
        
        
        /*
        //float newWidth = (int)(curWidth*k);
        //float newHeight = (int)(curHeight*k);
        
        CGImageRef cgImage = [img CGImage];
        UIImage* newImg = [UIImage imageWithCGImage:cgImage scale:(k/10.0) orientation:[theImage imageOrientation]];
        //CGImageRelease(cgImage);
        if (img != theImage) {
            [img release];
        }
        img = newImg;
         */
    }
    
    
    NSString *filename = nil; 	
    if (settings.format == CAMERA_SETTINGS_FORMAT_JPG) {
        filename = [NSString stringWithFormat:@"Image_%@.jpg", now];
    }
    else {
        filename = [NSString stringWithFormat:@"Image_%@.png", now];
    }
    NSString *fullname = [folder stringByAppendingPathComponent:filename];

    
    NSData *image = nil;
    if (settings.format == CAMERA_SETTINGS_FORMAT_JPG) {
        image = UIImageJPEGRepresentation(img, 0.9);
    }
    else {
        image = UIImagePNGRepresentation(img);
    }
    
    int isError = ![image writeToFile:fullname atomically:YES];

    if (img != theImage) {
        //[img release];
    }
    
    
    rho_rhodesapp_callCameraCallback([postUrl UTF8String], [filename UTF8String],
            isError ? "Can't write image to the storage." : "", 0 );
} 

- (void)imagePickerController:(UIImagePickerController *)picker 
		didFinishPickingImage:(UIImage *)image 
		editingInfo:(NSDictionary *)editingInfo 
{ 
    //If image editing is enabled and the user successfully picks an image, the image parameter of the 
    //imagePickerController:didFinishPicking Image:editingInfo:method contains the edited image. 
    //You should treat this image as the selected image, but if you want to store the original image, you can get 
    //it (along with the crop rectangle) from the dictionary in the editingInfo parameter. 
    [self useImage:image]; 
    // Remove the picker interface and release the picker object. 
    [picker dismissModalViewControllerAnimated:YES];
    [picker.view removeFromSuperview];
	//picker.view.hidden = YES;
    [picker release];
#ifdef __IPHONE_3_2
    [popover dismissPopoverAnimated:YES];
#endif
} 

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker 
{ 
    // Notify view about cancel
    rho_rhodesapp_callCameraCallback([postUrl UTF8String], "", "", 1);
    
    // Remove the picker interface and release the picker object. 
    [picker dismissModalViewControllerAnimated:YES]; 
    [picker.view removeFromSuperview];
	//picker.view.hidden = YES;
    [picker release]; 
#ifdef __IPHONE_3_2
    [popover dismissPopoverAnimated:YES];
#endif
} 

#ifdef __IPHONE_3_2
// UIPopoverControllerDelegate implementation
- (void)popoverControllerDidDismissPopover:(UIPopoverController *)popoverController {
    rho_rhodesapp_callCameraCallback([postUrl UTF8String], "", "", 1);
}
#endif // __IPHONE_3_2


- (void)dealloc {
    [settings release];
    [super dealloc];
}

@end

void take_picture(char* callback_url, rho_param *options_hash) {
    NSString *url = [NSString stringWithUTF8String:callback_url];
    RhoCameraSettings* settings = [[RhoCameraSettings alloc] init:options_hash url:url];
    [[Rhodes sharedInstance] performSelectorOnMainThread:@selector(takePicture:)
                                              withObject:settings waitUntilDone:NO];
}

void choose_picture(char* callback_url) {
    NSString *url = [NSString stringWithUTF8String:callback_url];
    [[Rhodes sharedInstance] performSelectorOnMainThread:@selector(choosePicture:)
                                              withObject:url waitUntilDone:NO];
}


VALUE get_camera_info(const char* camera_type) {
    return rho_ruby_get_NIL();
}
