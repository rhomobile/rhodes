//
//  SplashViewController.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 01.03.10.
//  Copyright 2010 Rhomobile Inc. All rights reserved.
//

#import "SplashViewController.h"

#import "rho/common/SplashScreenImpl.h"

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
	
	
	{
		CGRect scrnFrame = [[UIScreen mainScreen] bounds];
		float scales = 1;//[[UIScreen mainScreen] scale];
#ifdef __IPHONE_4_0
		scales = [[UIScreen mainScreen] scale];
#endif
		

		int image_width = (int)[img size].width; 
		int image_height = (int)[img size].height;
		
		CGRect appFrame = [[UIScreen mainScreen] applicationFrame];
		
		int scrnWidth = (int)(scrnFrame.size.width*scales+0.5);
		int scrnHeight = (int)(scrnFrame.size.height*scales+0.5);
		
		if ((image_width != scrnWidth) || (image_height != scrnHeight)) {
			// scale to app frame
			splashView.frame = appFrame;
		}
	}
	
    [parentView bringSubviewToFront:splashView];
    //rho_splash_screen_start();
}

- (void)hideSplash {
    rho_splash_screen_hide();
    [splashView removeFromSuperview];
}

- (void)bringToFront {
    [parentView bringSubviewToFront:splashView];
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

+ (NSString*)detectLoadingImage {
    NSFileManager *fileManager = [NSFileManager defaultManager];
    
	
	NSString * result = nil;
	
    NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
    
	NSString *pngLoadingPath = [NSString stringWithFormat:@"%@/apps/app/loading.png", resourcePath];
	NSString *pngDefaultPath = [NSString stringWithFormat:@"%@/Default.png", resourcePath];
	NSString *pngDefault2xPath = [NSString stringWithFormat:@"%@/Default@2x.png", resourcePath];
	NSString *pngDefaultPortraitPath = [NSString stringWithFormat:@"%@/Default-Portrait.png", resourcePath];
	NSString *pngDefaultPortraitUpsideDownPath = [NSString stringWithFormat:@"%@/Default-PortraitUpsideDown.png", resourcePath];
	NSString *pngDefaultLandscapePath = [NSString stringWithFormat:@"%@/Default-Landscape.png", resourcePath];
	NSString *pngDefaultLandscapeLeftPath = [NSString stringWithFormat:@"%@/Default-LandscapeLeft.png", resourcePath];
	NSString *pngDefaultLandscapeRightPath = [NSString stringWithFormat:@"%@/Default-LandscapeRight.png", resourcePath];

	CGRect frame = [[UIScreen mainScreen] bounds];
	float scales = 1;//[[UIScreen mainScreen] scale];
#ifdef __IPHONE_4_0
	scales = [[UIScreen mainScreen] scale];
#endif
	
	BOOL is_HiResolution = ((frame.size.width*scales) > 500);
	BOOL is_iPad = NO;
	
	NSString *model = [[UIDevice currentDevice] model]; // "iPad ..."
	if ([model hasPrefix:@"iPad"]) {
		is_iPad = YES;
	}
	
	UIInterfaceOrientation orientation = [[UIApplication sharedApplication] statusBarOrientation];
	
	
	if (is_iPad) {
		if ([fileManager fileExistsAtPath:pngDefaultPath]) {
			result = pngDefaultPath;
		}
		if ([fileManager fileExistsAtPath:pngDefault2xPath]) {
			result = pngDefault2xPath;
		}
		if ((orientation == UIInterfaceOrientationPortrait) || (orientation == UIInterfaceOrientationPortraitUpsideDown) ) {
			if ([fileManager fileExistsAtPath:pngDefaultPortraitPath]) {
				result = pngDefaultPortraitPath;
			}
			if (orientation == UIInterfaceOrientationPortraitUpsideDown) {
				if ([fileManager fileExistsAtPath:pngDefaultPortraitUpsideDownPath]) {
					result = pngDefaultPortraitUpsideDownPath;
				}
			}
		}
		else {
			if ([fileManager fileExistsAtPath:pngDefaultLandscapePath]) {
				result = pngDefaultLandscapePath;
			}
			if (orientation == UIInterfaceOrientationLandscapeLeft) {
				if ([fileManager fileExistsAtPath:pngDefaultLandscapeLeftPath]) {
					result = pngDefaultLandscapeLeftPath;
				}
			}
			else {
				if ([fileManager fileExistsAtPath:pngDefaultLandscapeRightPath]) {
					result = pngDefaultLandscapeRightPath;
				}
			}
		}
	}
	else {
		if ([fileManager fileExistsAtPath:pngDefaultPath]) {
			result = pngDefaultPath;
		}
		if (is_HiResolution) {
			if ([fileManager fileExistsAtPath:pngDefault2xPath]) {
				result = pngDefault2xPath;
			}
		}
	}

	return result;
}



@end
