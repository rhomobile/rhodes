//
//  SplashViewController.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 01.03.10.
//  Copyright 2010 Rhomobile Inc. All rights reserved.
//

#import "SplashViewController.h"


@implementation SplashViewController

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (id)initWithParentView:(UIView*)v {
    parentView = v;
    CGRect frame = [[UIScreen mainScreen] bounds];
    splashView = [[UIImageView alloc] initWithFrame:frame];
    [parentView addSubview:splashView];
    return self;
}

- (void)showSplash:(NSString *)imagePath {
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:imagePath])
        return;
    
    UIImage *img = [[UIImage alloc] initWithContentsOfFile:imagePath];
    splashView.image = img;
}

- (void)hideSplash {
    [splashView removeFromSuperview];
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)dealloc {
    [super dealloc];
}


@end
