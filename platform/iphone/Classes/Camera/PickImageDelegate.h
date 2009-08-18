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

@interface PickImageDelegate : RhoDelegate <UINavigationControllerDelegate, UIImagePickerControllerDelegate> {
}

- (void)imagePickerController:(UIImagePickerController *)picker
		didFinishPickingImage:(UIImage *)image
		editingInfo:(NSDictionary *)editingInfo;
- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker;


@end