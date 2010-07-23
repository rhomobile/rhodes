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

@implementation PickImageDelegate

- (void)useImage:(UIImage*)theImage { 
    NSString *folder = [[AppManager getApplicationsRootPath] stringByAppendingPathComponent:@"/../db/db-files"];

    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:folder])
        [fileManager createDirectoryAtPath:folder attributes:nil];

    NSString *now = [[[NSDate date] descriptionWithLocale:nil]
             stringByReplacingOccurrencesOfString: @":" withString: @"."];
    now = [now stringByReplacingOccurrencesOfString: @" " withString: @"_"];
    now = [now stringByReplacingOccurrencesOfString: @"+" withString: @"_"];
    NSString *filename = [NSString stringWithFormat:@"Image_%@.png", now]; 	
    NSString *fullname = [folder stringByAppendingPathComponent:filename];
    NSData *pngImage = UIImagePNGRepresentation(theImage);

    int isError = ![pngImage writeToFile:fullname atomically:YES];
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

@end

void take_picture(char* callback_url) {
    NSString *url = [NSString stringWithUTF8String:callback_url];
    [[Rhodes sharedInstance] performSelectorOnMainThread:@selector(takePicture:)
                                              withObject:url waitUntilDone:NO];
}

void choose_picture(char* callback_url) {
    NSString *url = [NSString stringWithUTF8String:callback_url];
    [[Rhodes sharedInstance] performSelectorOnMainThread:@selector(choosePicture:)
                                              withObject:url waitUntilDone:NO];
}