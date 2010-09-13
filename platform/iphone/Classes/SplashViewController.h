//
//  SplashViewController.h
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 01.03.10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RhoViewController.h"

@interface SplashViewController : RhoViewController {
    IBOutlet UIImageView *splashView;
    
@public
    UIView *parentView;
}

- (id)initWithParentView:(UIView*)v;
- (void)showSplash:(NSString *)imagePath;
- (void)hideSplash;
- (void)bringToFront;

@end
