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
- (void)loadView {
    CGRect frame = [[UIScreen mainScreen] applicationFrame];
    splashView = [[UIImageView alloc] initWithFrame:frame];
    [superView addSubview:splashView];
}

- (void)showSplash:(NSString *)imagePath {
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:imagePath])
        return;
    
    UIImage *img = [[UIImage alloc] initWithContentsOfFile:imagePath];
    splashView.image = img;
}

- (void)hideSplash {
    //[[self modalViewController] dismissModalViewControllerAnimated:YES];
    [splashView removeFromSuperview];
}

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
*/

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}


- (void)dealloc {
    [super dealloc];
}


@end
