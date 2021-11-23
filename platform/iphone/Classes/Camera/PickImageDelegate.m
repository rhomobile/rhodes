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
#include <sys/sysctl.h>

#import "Rhodes.h"
#import "PickImageDelegate.h"
#import "AppManager.h"
#import "common/RhodesApp.h"

#import "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "PickImageDelegate"

//#include "ruby/ext/rho/rhoruby.h"




@implementation RhoCameraSettings

@synthesize callback, camera_type, color_model, format, width, height, enable_editing, enable_editing_setted, save_to_shared_gallery, is_data_uri, fileName, flash_mode;

- (id)init:(NSDictionary*)data callback_api:(id<IMethodResult>)callback_api {
    self.callback = callback_api;
    self.camera_type = CAMERA_SETTINGS_TYPE_UNKNOWN;
    self.color_model = CAMERA_SETTINGS_COLOR_MODEL_RGB;
    self.format = CAMERA_SETTINGS_FORMAT_NOT_SETTED;
    self.width = 0;
    self.height = 0;
    self.enable_editing = 1;
    self.enable_editing_setted = 0;
    self.save_to_shared_gallery = false;
    self.is_data_uri = false;
    self.fileName = nil;
    self.flash_mode = CAMERA_SETTINGS_FLASH_AUTO;
    
    if ([data objectForKey:@"fileName"] != nil) {
        NSString* value = (NSString*)[data objectForKey:@"fileName"];
        if ([value length] > 0) {
            self.fileName = value;
        }
    }
    if ([data objectForKey:@"outputFormat"] != nil) {
        NSString* value = (NSString*)[data objectForKey:@"outputFormat"];
        if(strcasecmp([value UTF8String], "image") == 0 ) {
        }
        if(strcasecmp([value UTF8String], "dataUri") == 0 ) {
            self.is_data_uri = true;
        }
    }
    
    if ([data objectForKey:@"cameraType"] != nil) {
        NSString* value = (NSString*)[data objectForKey:@"cameraType"];
        if(strcasecmp([value UTF8String], "front") == 0 ) {
            self.camera_type = CAMERA_SETTINGS_TYPE_FRONT;
        }
        if((strcasecmp([value UTF8String], "back") == 0) || (strcasecmp([value UTF8String], "main") == 0 ) ) {
            self.camera_type = CAMERA_SETTINGS_TYPE_MAIN;
        }
    }
    if ([data objectForKey:@"colorModel"] != nil) {
        NSString* value = (NSString*)[data objectForKey:@"colorModel"];
        if((strcasecmp([value UTF8String], "Grayscale") == 0) || (strcasecmp([value UTF8String], "grayscale") == 0 ) ) {
            self.color_model = CAMERA_SETTINGS_COLOR_MODEL_GRAY;
        }
    }
    if ([data objectForKey:@"compressionFormat"] != nil) {
        NSString* value = (NSString*)[data objectForKey:@"compressionFormat"];
        if(strcasecmp([value UTF8String], "png") == 0 ) {
            self.format = CAMERA_SETTINGS_FORMAT_PNG;
        }
        else {
            self.format = CAMERA_SETTINGS_FORMAT_JPG;
        }
    }
    if ([data objectForKey:@"desiredWidth"] != nil) {
        NSString* value = (NSString*)[data objectForKey:@"desiredWidth"];
        self.width = [value intValue];
    }
    if ([data objectForKey:@"desiredHeight"] != nil) {
        NSString* value = (NSString*)[data objectForKey:@"desiredHeight"];
        self.height = [value intValue];
    }
    if ([data objectForKey:@"saveToDeviceGallery"] != nil) {
        NSString* value = (NSString*)[data objectForKey:@"saveToDeviceGallery"];
        if( (strcasecmp([value UTF8String], [[NSString stringWithFormat:@"%@", [NSNumber numberWithBool:YES]] UTF8String]) == 0) ||
           (strcasecmp([value UTF8String], "true") == 0) ) {
            self.save_to_shared_gallery = true;
        }
    }
    if ([data objectForKey:@"enableEditing"] != nil) {
        NSString* value = (NSString*)[data objectForKey:@"enableEditing"];
        self.enable_editing = 1;
        self.enable_editing_setted = 1;
        if( (strcasecmp([value UTF8String], [[NSString stringWithFormat:@"%@", [NSNumber numberWithBool:NO]] UTF8String]) == 0) ||
           (strcasecmp([value UTF8String], "false") == 0) ) {
            self.enable_editing = 0;
        }
    }
    
    if ([data objectForKey:@"flashMode"] != nil) {
        NSString* value = (NSString*)[data objectForKey:@"flashMode"];
        if(strcasecmp([value UTF8String], "auto") == 0 ) {
            self.flash_mode = CAMERA_SETTINGS_FLASH_AUTO;
        }
        if(strcasecmp([value UTF8String], "on") == 0 ) {
            self.flash_mode = CAMERA_SETTINGS_FLASH_ON;
        }
        if(strcasecmp([value UTF8String], "off") == 0 ) {
            self.flash_mode = CAMERA_SETTINGS_FLASH_OFF;
        }
    }
    
    return self;
}


- (void)dealloc {
    //[callback_url release];
    [super dealloc];
}

@end




@implementation PickImageDelegate

@synthesize settings, isCaptureProcessRunning;


-(void) sendCancelCallback {
    NSMutableDictionary* dict = [NSMutableDictionary dictionaryWithCapacity:10];
    [dict setObject:@"cancel" forKey:@"status"];
    [dict setObject:@"User canceled operation." forKey:@"message"];
    [ self.settings.callback setResult:dict];
}



- (void)useImage:(UIImage*)theImage {
    NSString *folder = [[AppManager getDbPath] stringByAppendingPathComponent:@"/db-files"];

    
    UIImage* img = theImage;
    
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:folder]) {
        NSError* error;
        [fileManager createDirectoryAtPath:folder withIntermediateDirectories:YES attributes:nil error:&error];
    }

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

    
    // normalize orientation
    if ([theImage imageOrientation] != UIImageOrientationUp) {
        
        CGImageRef cgImage = [img CGImage];
        CGColorSpaceRef rgb_space = CGColorSpaceCreateDeviceRGB();
        
        int curWidth = CGImageGetWidth(cgImage);
        int curHeight = CGImageGetHeight(cgImage);
        
        int newWidth = curWidth;
        int newHeight = curHeight;

        if (([theImage imageOrientation] == UIImageOrientationLeft) || ([theImage imageOrientation] == UIImageOrientationRight)) {
            newWidth = curHeight;
            newHeight = curWidth;
        }
        
        void * data = malloc(newWidth*newHeight*4);
        
        CGContextRef cgContext = CGBitmapContextCreate( data,
                                                       newWidth,
                                                       newHeight,
                                                       8,
                                                       newWidth*4,
                                                       rgb_space,
                                                       kCGImageAlphaNoneSkipLast);
        
        CGFloat rotation = 0.0;
        
        if ([theImage imageOrientation] == UIImageOrientationLeft) {
            rotation = 90.0 * M_PI / 180.0;
        }
        if ([theImage imageOrientation] == UIImageOrientationRight) {
            rotation = 270.0 * M_PI / 180.0;
        }
        if ([theImage imageOrientation] == UIImageOrientationDown) {
            rotation = 180.0 * M_PI / 180.0;
        }

        
        CGContextTranslateCTM(cgContext, ((double)newWidth) / 2.0f, ((double)newHeight) / 2.0f);
        CGContextRotateCTM(cgContext, rotation);
        
        
        CGContextDrawImage(cgContext, CGRectMake(-(((double)curWidth) / 2.0f), -(((double)curHeight) / 2.0f), curWidth, curHeight), cgImage);
        
        CGImageRef rgbImage = CGBitmapContextCreateImage(cgContext);
        //[img release];
        UIImage* newImg = [UIImage imageWithCGImage:rgbImage scale:1.0 orientation:UIImageOrientationUp];
        if (img != theImage) {
            //[img release];
        }
        img = newImg;
        
        CGColorSpaceRelease(rgb_space);
        CGImageRelease(rgbImage);
        CGContextRelease(cgContext);
        free(data);
        
    }
    
    
    int imageWidth = (int)img.size.width;
    int imageHeight = (int)img.size.height;
    
    if ( settings.save_to_shared_gallery ) {    
        UIImageWriteToSavedPhotosAlbum(img, nil, nil, nil);
    }    
    
    NSString *mimeType = nil;
    NSString *filename = nil;
    NSString *fullname = nil;
    if (settings.fileName != nil) {
        filename = settings.fileName;
    }
    else {
        filename = [NSString stringWithFormat:@"IMG_%@", now];
    }
    if (settings.format == CAMERA_SETTINGS_FORMAT_JPG) {
        filename = [filename stringByAppendingString:@".jpg"];
        mimeType = @"image/jpeg";
    }
    else {
        filename = [filename stringByAppendingString:@".png"];
        mimeType = @"image/png";
    }
    if (settings.fileName != nil) {
        
        fullname = filename;
        
        NSRange range = [fullname rangeOfString:@"/"];
        if (range.location != NSNotFound ) {
            //full path
            //nothing - use as is
        }
        else {
            range = [fullname rangeOfString:@"\\"];
            if (range.location != NSNotFound ) {
                //full path
                //nothing - use as is
            }
            else {
                // only name
                fullname = [folder stringByAppendingPathComponent:filename];
            }
        }

    }
    else {
        fullname = [folder stringByAppendingPathComponent:filename];
    }
    
    NSData *image = nil;
    if (settings.format == CAMERA_SETTINGS_FORMAT_JPG) {
        image = UIImageJPEGRepresentation(img, 0.9);
    }
    else {
        image = UIImagePNGRepresentation(img);
    }
    
    int isError = 0;
    NSString* str_result = @"";
    NSString* str_result_old = @"";

    
    // check data uri
    if (settings.is_data_uri) {
        str_result = [NSString stringWithFormat:@"data:%@;base64,%@", mimeType, [image base64EncodedStringWithOptions:0]];
        
    }
    else {
        @try {
            NSError* error = nil;
            isError = ![image writeToFile:fullname options:NSDataWritingAtomic error:&error];
        }
        @catch (NSException *exception) {
            isError = 1;
        }
        
        if (settings.fileName != nil) {
            str_result_old = fullname;
            str_result = fullname;
        }
        else {
            str_result_old = [str_result_old stringByAppendingString:@"db/db-files/"];
            str_result_old = [str_result_old stringByAppendingString:filename];
            str_result = fullname;
        }
    }

    if (img != theImage) {
        //[img release];
    }
    
    if (isError) {
        NSMutableDictionary* dict = [NSMutableDictionary dictionaryWithCapacity:10];
        [dict setObject:@"error" forKey:@"status"];
        [dict setObject:@"Can't write image to the storage" forKey:@"message"];
        [ self.settings.callback setResult:dict];
    }
    else {
        NSMutableDictionary* dict = [NSMutableDictionary dictionaryWithCapacity:10];
        [dict setObject:@"ok" forKey:@"status"];
        

        [dict setObject:str_result_old forKey:@"image_uri"];
        [dict setObject:str_result forKey:@"imageUri"];
        

        [dict setObject:[NSString stringWithFormat:@"%d", imageWidth] forKey:@"imageWidth"];
        [dict setObject:[NSString stringWithFormat:@"%d", imageHeight] forKey:@"imageHeight"];

        [dict setObject:[NSString stringWithFormat:@"%d", imageWidth] forKey:@"image_width"];
        [dict setObject:[NSString stringWithFormat:@"%d", imageHeight] forKey:@"image_height"];
        
        if (settings.format == CAMERA_SETTINGS_FORMAT_JPG) {
            [dict setObject:@"jpg" forKey:@"image_format"];
            [dict setObject:@"jpg" forKey:@"imageFormat"];
        }
        else {
            [dict setObject:@"png" forKey:@"image_format"];
            [dict setObject:@"png" forKey:@"imageFormat"];
        }
        
        [ self.settings.callback setResult:dict];

    }
    
	//const char* cb = [postUrl UTF8String];
	//const char* b = [strBody UTF8String];
    //char* norm_url = rho_http_normalizeurl(cb);
    //rho_net_request_with_data(norm_url, b);
    //rho_http_free(norm_url);
    
    //rho_rhodesapp_callCameraCallback([postUrl UTF8String], [filename UTF8String],
    //        isError ? "Can't write image to the storage." : "", 0 );
} 


- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{

    if (!self.isCaptureProcessRunning) {
        return;
    }
    //NSLog(@"$$$###$$$   didFinishPickingMediaWithInfo");
    self.isCaptureProcessRunning = NO;
    
    if (settings.format == CAMERA_SETTINGS_FORMAT_NOT_SETTED) {
        NSURL* url = (NSURL*)[info objectForKey:UIImagePickerControllerReferenceURL];
        //const char* cs_url = [[url path] UTF8String];
        if (url != nil) {
            NSString* ext = [url pathExtension];
            //const char* cs_ext = [ext UTF8String];
            if (ext != nil) {
                if (([ext compare:@"jpg" options:NSCaseInsensitiveSearch] == NSOrderedSame) || ([ext compare:@"jpeg" options:NSCaseInsensitiveSearch] == NSOrderedSame)) {
                    // jpg
                    settings.format = CAMERA_SETTINGS_FORMAT_JPG;
                }
                if (([ext compare:@"png" options:NSCaseInsensitiveSearch] == NSOrderedSame)) {
                    // png
                    settings.format = CAMERA_SETTINGS_FORMAT_PNG;
                }
            }
        }
        if (settings.format == CAMERA_SETTINGS_FORMAT_NOT_SETTED) {
            settings.format = CAMERA_SETTINGS_FORMAT_JPG;
        }
    }

    // Remove the picker interface and release the picker object.
    [picker dismissViewControllerAnimated:NO completion:nil];
    [picker.view removeFromSuperview];
    //picker.view.hidden = YES;
#ifdef __IPHONE_3_2
    [popover dismissPopoverAnimated:YES];
#endif
    [picker release];

    UIImage* image = (UIImage*)[info objectForKey:UIImagePickerControllerEditedImage];
    if (image == nil) {
        image = (UIImage*)[info objectForKey:UIImagePickerControllerOriginalImage];
    }
    if (image != nil) {
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            [self useImage:image];
        });
    }
    else {
        //rho_rhodesapp_callCameraCallback([postUrl UTF8String], "", "", 1);
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            [self sendCancelCallback];
        });
    }
}


- (void)imagePickerController:(UIImagePickerController *)picker 
		didFinishPickingImage:(UIImage *)image 
		editingInfo:(NSDictionary *)editingInfo 
{
    //NSLog(@"$$$###$$$   didFinishPickingImage");
    if (settings.format == CAMERA_SETTINGS_FORMAT_NOT_SETTED) {
        settings.format = CAMERA_SETTINGS_FORMAT_JPG;
    }
    //If image editing is enabled and the user successfully picks an image, the image parameter of the 
    //imagePickerController:didFinishPicking Image:editingInfo:method contains the edited image. 
    //You should treat this image as the selected image, but if you want to store the original image, you can get 
    //it (along with the crop rectangle) from the dictionary in the editingInfo parameter. 
    [self useImage:image]; 
    // Remove the picker interface and release the picker object. 
    [picker dismissViewControllerAnimated:NO completion:nil];
    [picker.view removeFromSuperview];
	//picker.view.hidden = YES;
    [picker release];
#ifdef __IPHONE_3_2
    [popover dismissPopoverAnimated:YES];
#endif
} 

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker 
{ 
    if (!self.isCaptureProcessRunning) {
        return;
    }
    self.isCaptureProcessRunning = NO;
    // Remove the picker interface and release the picker object.
    [picker dismissViewControllerAnimated:YES completion:nil];
    [picker.view removeFromSuperview];
    //picker.view.hidden = YES;
#ifdef __IPHONE_3_2
    [popover dismissPopoverAnimated:YES];
#endif
    [picker release];


    // Notify view about cancel
    //rho_rhodesapp_callCameraCallback([postUrl UTF8String], "", "", 1);
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        [self sendCancelCallback];
    });
    
}

#ifdef __IPHONE_3_2
// UIPopoverControllerDelegate implementation
- (void)popoverControllerDidDismissPopover:(UIPopoverController *)popoverController {
    //rho_rhodesapp_callCameraCallback([postUrl UTF8String], "", "", 1);
    [self sendCancelCallback];
}
#endif // __IPHONE_3_2


- (void)dealloc {
    [settings release];
    [super dealloc];
}

@end

/*
void take_picture(char* callback_url, rho_param *options_hash) {
    NSString *url = [NSString stringWithUTF8String:callback_url];
    RhoCameraSettings* settings = [[RhoCameraSettings alloc] init:options_hash url:url];
    if (settings.format == CAMERA_SETTINGS_FORMAT_NOT_SETTED) {
        settings.format = CAMERA_SETTINGS_FORMAT_JPG;
    }
    [[Rhodes sharedInstance] performSelectorOnMainThread:@selector(takePicture:)
                                              withObject:settings waitUntilDone:NO];
}

void choose_picture(char* callback_url, rho_param *options_hash) {
    NSString *url = [NSString stringWithUTF8String:callback_url];
    RhoCameraSettings* settings = [[RhoCameraSettings alloc] init:options_hash url:url];
    if (!settings.enable_editing_setted) {
        settings.enable_editing = 1;
    }
    settings.camera_type = CAMERA_SETTINGS_TYPE_CHOOSE_IMAGE;
    [[Rhodes sharedInstance] performSelectorOnMainThread:@selector(choosePicture:)
                                              withObject:settings waitUntilDone:NO];
}
*/


void camera_take_picture(NSDictionary* options, id<IMethodResult> callback_api) {
    RhoCameraSettings* settings = [[RhoCameraSettings alloc] init:options callback_api:callback_api];
    if (settings.format == CAMERA_SETTINGS_FORMAT_NOT_SETTED) {
        settings.format = CAMERA_SETTINGS_FORMAT_JPG;
    }
    if (settings.camera_type == CAMERA_SETTINGS_TYPE_UNKNOWN) {
        NSMutableDictionary* dict = [NSMutableDictionary dictionaryWithCapacity:10];
        [dict setObject:@"error" forKey:@"status"];
        [dict setObject:@"unknown camera type !" forKey:@"message"];
        [settings.callback setResult:dict];
        return;
    }
    [[Rhodes sharedInstance] performSelectorOnMainThread:@selector(takePicture:)
                                              withObject:settings waitUntilDone:NO];
    
}

void camera_choose_picture(NSDictionary* options, id<IMethodResult> callback_api) {
    RhoCameraSettings* settings = [[RhoCameraSettings alloc] init:options callback_api:callback_api];
    if (!settings.enable_editing_setted) {
        settings.enable_editing = 1;
    }
    settings.camera_type = CAMERA_SETTINGS_TYPE_CHOOSE_IMAGE;
    [[Rhodes sharedInstance] performSelectorOnMainThread:@selector(choosePicture:)
                                              withObject:settings waitUntilDone:NO];
    
}


//void save_image_to_device_gallery(const char* image_path, rho_param* options_hash) {
int save_image_to_device_gallery(const char* image_path) {
	NSString* path = [NSString stringWithUTF8String:image_path];
	UIImage* img = [UIImage imageWithContentsOfFile:path];
	
	if ( img != nil ) {
		UIImageWriteToSavedPhotosAlbum(img, nil, nil, nil);
    }
    else {
        return 0;
    }
    return 1;
}


// cameraPosition: AVCaptureDevicePositionBack, AVCaptureDevicePositionFront
// return struct CMVideoDimensions {int width, int height}
CMVideoDimensions getCameraMaxStillImageResolution(AVCaptureDevicePosition cameraPosition) {
    
    CMVideoDimensions max_resolution;
    max_resolution.width = 0;
    max_resolution.height = 0;
    
    AVCaptureDevice *captureDevice = nil;
    
    NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    for (AVCaptureDevice *device in devices) {
        if ([device position] == cameraPosition) {
            captureDevice = device;
            break;
        }
    }
    if (captureDevice == nil) {
        return max_resolution;
    }
    
    NSArray* availFormats=captureDevice.formats;
    
    for (AVCaptureDeviceFormat* format in availFormats) {
        CMVideoDimensions resolution = format.highResolutionStillImageDimensions;
        int w = resolution.width;
        int h = resolution.height;
        if ((w * h) > (max_resolution.width * max_resolution.height)) {
            max_resolution.width = w;
            max_resolution.height = h;
        }
    }
    
    return max_resolution;
}

void get_camera_info_by_device_name(const char* camera_type, int* res_w, int* res_h) {
    
    int w = 0;
    int h = 0;
    BOOL isFront = strcmp(camera_type, "front") == 0;
    BOOL isBack = strcmp(camera_type, "back") == 0;
    
    if ((!isFront) && (!isBack)) {
        // unknown camera type
        *res_w = w;
        *res_h = h;
        return;
    }
    
    size_t size;
    sysctlbyname("hw.machine", NULL, &size, NULL, 0);
    char *answer = malloc(size);
    sysctlbyname("hw.machine", answer, &size, NULL, 0);
    NSString *platform = [NSString stringWithCString:answer encoding: NSUTF8StringEncoding];
    free(answer);
    
    
    // iPhone 2G
    if ([platform isEqualToString:@"iPhone1,1"]) {
        if (!isFront) {
            w = 1600;
            h = 1200;
        }
    }
    
    // iPhone 3G
    if ([platform isEqualToString:@"iPhone1,2"]) {
        if (!isFront) {
            w = 1600;
            h = 1200;
        }
    }

    // iPhone 3GS
    if ([platform hasPrefix:@"iPhone2"]) {
        if (!isFront) {
            w = 2048;
            h = 1536;
        }
    }
    
    // iPhone 4
    if ([platform hasPrefix:@"iPhone3"]) {
        if (!isFront) {
            w = 2592;
            h = 1936;
        }
        else {
            w = 640;
            h = 480;
        }
    }
    
    // iPhone 4S
    if ([platform hasPrefix:@"iPhone4"]) {
        if (!isFront) {
            w = 3264;
            h = 2448;
        }
        else {
            w = 640;
            h = 480;
        }
    }
    
    
    // iPhone 5
    if ([platform isEqualToString:@"iPhone5,1"] || [platform isEqualToString:@"iPhone5,2"]) {
        if (!isFront) {
            w = 3264;
            h = 2448;
        }
        else {
            w = 1280;
            h = 960;
        }
    }

    
    // iPhone 5C
    if ([platform isEqualToString:@"iPhone5,3"] || [platform isEqualToString:@"iPhone5,4"]) {
        if (!isFront) {
            w = 3264;
            h = 2448;
        }
        else {
            w = 1280;
            h = 960;
        }
    }
    
    // iPhone 5S
    if ([platform isEqualToString:@"iPhone6,1"] || [platform isEqualToString:@"iPhone6,2"]) {
        if (!isFront) {
            w = 3264;
            h = 2448;
        }
        else {
            w = 1280;
            h = 960;
        }
    }

    // iPhone 6
    if ([platform isEqualToString:@"iPhone7,2"]) {
        if (!isFront) {
            w = 3264;
            h = 2448;
        }
        else {
            w = 1280;
            h = 960;
        }
    }
    
    // iPhone 6 Plus
    if ([platform isEqualToString:@"iPhone7,1"]) {
        if (!isFront) {
            w = 3264;
            h = 2448;
        }
        else {
            w = 1280;
            h = 960;
        }
    }
    
    // iPhone 6S
    if ([platform isEqualToString:@"iPhone8,1"]) {
        if (!isFront) {
            w = 4032;
            h = 3024;
        }
        else {
            w = 2556;
            h = 1932;
        }
    }
    
    // iPhone 6S Plus
    if ([platform isEqualToString:@"iPhone8,2"]) {
        if (!isFront) {
            w = 4032;
            h = 3024;
        }
        else {
            w = 2556;
            h = 1932;
        }
    }

    // iPhone 5SE
    if ([platform isEqualToString:@"iPhone8,4"]) {
        if (!isFront) {
            w = 4032;
            h = 3024;
        }
        else {
            w = 1280;
            h = 960;
        }
    }
    
    // iPad 1
    if ([platform hasPrefix:@"iPad1"])  {
    }
    
    // iPad 2
    if ([platform isEqualToString:@"iPad2,1"] || [platform isEqualToString:@"iPad2,2"] || [platform isEqualToString:@"iPad2,3"] || [platform isEqualToString:@"iPad2,4"])  {
        if (!isFront) {
            w = 960;
            h = 720;
        }
        else {
            w = 640;
            h = 480;
        }
    }

    // iPad 3
    if ([platform isEqualToString:@"iPad3,1"] || [platform isEqualToString:@"iPad3,2"] || [platform isEqualToString:@"iPad3,3"])  {
        if (!isFront) {
            w = 2592;
            h = 1936;
        }
        else {
            w = 640;
            h = 480;
        }
    }
    
    // iPad 4
    if ([platform isEqualToString:@"iPad3,4"] || [platform isEqualToString:@"iPad3,5"] || [platform isEqualToString:@"iPad3,6"])  {
        if (!isFront) {
            w = 2592;
            h = 1936;
        }
        else {
            w = 1280;
            h = 960;
        }
    }
    
    // iPad Air
    if ([platform isEqualToString:@"iPad4,1"] || [platform isEqualToString:@"iPad4,2"] || [platform isEqualToString:@"iPad4,3"])  {
        if (!isFront) {
            w = 2592;
            h = 1936;
        }
        else {
            w = 1280;
            h = 960;
        }
    }
    
    // iPad Air 2
    if ([platform isEqualToString:@"iPad5,3"] || [platform isEqualToString:@"iPad5,4"])  {
        if (!isFront) {
            w = 3264;
            h = 2448;
        }
        else {
            w = 1280;
            h = 960;
        }
    }
    
    // iPad Pro 12
    if ([platform isEqualToString:@"iPad6,7"] || [platform isEqualToString:@"iPad6,8"])  {
        if (!isFront) {
            w = 3264;
            h = 2448;
        }
        else {
            w = 1280;
            h = 960;
        }
    }
    
    // iPad Pro 9
    if ([platform isEqualToString:@"iPad6,3"] || [platform isEqualToString:@"iPad6,4"])  {
        if (!isFront) {
            w = 4032;
            h = 3024;
        }
        else {
            w = 2556;
            h = 1932;
        }
    }

    // iPad mini
    if ([platform isEqualToString:@"iPad2,5"] || [platform isEqualToString:@"iPad2,6"] || [platform isEqualToString:@"iPad2,7"])  {
        if (!isFront) {
            w = 2592;
            h = 1936;
        }
        else {
            w = 1280;
            h = 960;
        }
    }
    
    // iPad mini 2
    if ([platform isEqualToString:@"iPad4,4"] || [platform isEqualToString:@"iPad4,5"] || [platform isEqualToString:@"iPad4,6"])  {
        if (!isFront) {
            w = 2592;
            h = 1936;
        }
        else {
            w = 1280;
            h = 960;
        }
    }

    // iPad mini 3
    if ([platform isEqualToString:@"iPad4,7"] || [platform isEqualToString:@"iPad4,8"] || [platform isEqualToString:@"iPad4,9"])  {
        if (!isFront) {
            w = 2592;
            h = 1936;
        }
        else {
            w = 1280;
            h = 960;
        }
    }

    // iPad mini 4
    if ([platform isEqualToString:@"iPad5,1"] || [platform isEqualToString:@"iPad5,2"])  {
        if (!isFront) {
            w = 3264;
            h = 2448;
        }
        else {
            w = 1280;
            h = 960;
        }
    }
    
    // iPod Touch
    if ([platform isEqualToString:@"iPod1,1"]) {
    }
    
    // iPod Touch 2
    if ([platform isEqualToString:@"iPod2,1"]) {
    }
    
    // iPod Touch 3
    if ([platform isEqualToString:@"iPod3,1"]) {
    }

    // iPod Touch 4
    if ([platform isEqualToString:@"iPod4,1"]) {
        if (!isFront) {
            w = 960;
            h = 720;
        }
        else {
            w = 640;
            h = 480;
        }
    }

    // iPod Touch 5
    if ([platform isEqualToString:@"iPod5,1"]) {
        if (!isFront) {
            w = 2592;
            h = 1936;
        }
        else {
            w = 1280;
            h = 960;
        }
    }

    // iPod Touch 6
    if ([platform isEqualToString:@"iPod7,1"]) {
        if (!isFront) {
            w = 3264;
            h = 2448;
        }
        else {
            w = 1280;
            h = 960;
        }
    }
    
    *res_w = w;
    *res_h = h;
    
}


void get_camera_info(const char* camera_type, int* res_w, int* res_h) {
    int w = 0;
    int h = 0;
    BOOL isFront = strcmp(camera_type, "front") == 0;
    BOOL isBack = strcmp(camera_type, "back") == 0;
    
    if ((!isFront) && (!isBack)) {
        // unknown camera type
        *res_w = w;
        *res_h = h;
        return;
    }

    CMVideoDimensions res_from_av;
    AVCaptureDevicePosition cameraPos = AVCaptureDevicePositionUnspecified;
    if (isFront) {
        cameraPos = AVCaptureDevicePositionFront;
    }
    if (isBack) {
        cameraPos = AVCaptureDevicePositionBack;
    }
    res_from_av = getCameraMaxStillImageResolution(cameraPos);

    get_camera_info_by_device_name(camera_type, &w, &h);
    
    if ((res_from_av.width != w) || (res_from_av.height != h)) {
        RAWLOG_INFO4("WARNING: unknown device ! predefined image size [ %d x %d ], but actual size is [ %d x %d ] !!!", w, h, res_from_av.width, res_from_av.height);
    }

    if ((res_from_av.width * res_from_av.height) > (w * h)) {
        w = res_from_av.width;
        h = res_from_av.height;
    }
    
    *res_w = w;
    *res_h = h;
}


int get_camera_max_width(const char* camera_type) {
    int w;
    int h;
    get_camera_info(camera_type, &w, &h);
    return w;
}

int get_camera_max_height (const char* camera_type) {
    int w;
    int h;
    get_camera_info(camera_type, &w, &h);
    return h;
}


