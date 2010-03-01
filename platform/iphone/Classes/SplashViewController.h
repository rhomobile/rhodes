//
//  SplashViewController.h
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 01.03.10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface SplashViewController : UIViewController {
    IBOutlet UIImageView *splashView;
    
@public
    UIView *superView;
}

- (void)showSplash:(NSString *)imagePath;
- (void)hideSplash;

@end
