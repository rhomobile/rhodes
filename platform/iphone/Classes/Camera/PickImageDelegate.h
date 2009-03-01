//
//  PickImageDelegate.h
//  rhorunner
//
//  Created by Vlad on 2/12/09.
//  Copyright 2009 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@interface PickImageDelegate : NSObject <UINavigationControllerDelegate, UIImagePickerControllerDelegate>
{
	NSString* postUrl; 
}

@property (readwrite, copy) NSString *postUrl;

- (void)imagePickerController:(UIImagePickerController *)picker
		didFinishPickingImage:(UIImage *)image
		editingInfo:(NSDictionary *)editingInfo;
- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker;


@end