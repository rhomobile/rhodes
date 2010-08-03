//
//  SignatureDelegate.m
//  rhorunner
//
//  Created by Soldatenkov Dmitry on 27/07/10.
//  Copyright 2010 Rhomobile. All rights reserved.
//

#import "Rhodes.h"
#import "SignatureDelegate.h"
#import "AppManager.h"
#import "common/RhodesApp.h"

@implementation SignatureDelegate


-(void)setSignatureViewControllerValue:(SignatureViewController *)svc {
	signatureViewController = svc;
}

-(void)setParentView:(UIView*)parent_view {
	parentView = parent_view;
}

-(void)setPrevView:(UIView*)prev_view {
	prevView = prev_view;
}

-(void)setImageFormat:(NSString*)format {
	imageFormat = format;
}


- (void)useImage:(UIImage*)theImage { 
    NSString *folder = [[AppManager getApplicationsRootPath] stringByAppendingPathComponent:@"/../db/db-files"];

    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:folder])
        [fileManager createDirectoryAtPath:folder attributes:nil];

    NSString *now = [[[NSDate date] descriptionWithLocale:nil]
             stringByReplacingOccurrencesOfString: @":" withString: @"."];
    now = [now stringByReplacingOccurrencesOfString: @" " withString: @"_"];
    now = [now stringByReplacingOccurrencesOfString: @"+" withString: @"_"];

    NSString *filename;
    NSString *fullname;
    NSData *pngImage;
    
    if (imageFormat == @"png") {
	
        filename = [NSString stringWithFormat:@"Image_%@.png", now]; 	
        fullname = [folder stringByAppendingPathComponent:filename];
        pngImage = UIImagePNGRepresentation(theImage);
    else {
        filename = [NSString stringWithFormat:@"Image_%@.jpg", now]; 	
        fullname = [folder stringByAppendingPathComponent:filename];
        pngImage = UIImageJPEGRepresentation(theImage, 1.0);
    }

    int isError = ![pngImage writeToFile:fullname atomically:YES];
    rho_rhodesapp_callSignatureCallback([postUrl UTF8String], [filename UTF8String],
            isError ? "Can't write image to the storage." : "", 0 );
} 

-(void)doDone:(UIImage*)image {
	
    [self useImage:image]; 
    [signatureViewController.view removeFromSuperview];
    [signatureViewController release];
	[parentView addSubview:prevView];
	[prevView release];
	[image release];
}

-(void)doCancel {
    rho_rhodesapp_callSignatureCallback([postUrl UTF8String], "", "", 1);
    [signatureViewController.view removeFromSuperview];
    [signatureViewController release];
	[parentView addSubview:prevView];
	[prevView release];
}



/*

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
*/
@end

void rho_signature_take_signature(char* callback_url, char* image_format) {
    NSString *url = [NSString stringWithUTF8String:callback_url];
    NSString *iformat = [NSString stringWithUTF8String:image_format];
    [[Rhodes sharedInstance] performSelectorOnMainThread:@selector(takeSignature:)
                                              withObject:url withObject:iformat waitUntilDone:NO];
}

