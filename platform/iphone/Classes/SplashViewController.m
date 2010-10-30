//
//  SplashViewController.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 01.03.10.
//  Copyright 2010 Rhomobile Inc. All rights reserved.
//

#import "SplashViewController.h"

#import "rho/common/SplashScreenImpl.h"


@implementation RhoSplashImageView

- (void)layoutSubviews
{
	[super layoutSubviews];
	
	CGRect frame = self.frame;
	CGRect rect = self.bounds;

	float scales = 1;
#ifdef __IPHONE_4_0
	if ( [[UIScreen mainScreen] respondsToSelector:@selector(scale)] ) {
		scales = [[UIScreen mainScreen] scale];
	}
#endif
	

	BOOL is_need_left = NO;
	
	NSString* imagePath = [SplashViewController detectLoadingImage:rect rotation_to_left:&is_need_left];

    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:imagePath])
        return;
    
    UIImage *img = [[UIImage alloc] initWithContentsOfFile:imagePath];
	
    self.image = img;
	
	{
		
		float img_scale = 1;
#ifdef __IPHONE_4_0
		if ( [img respondsToSelector:@selector(scale)] ) {
			img_scale = [img scale];
		}
#endif
		
		int image_width = (int)([img size].width*img_scale); 
		int image_height = (int)([img size].height*img_scale);
		
		CGRect appFrame = [[UIScreen mainScreen] applicationFrame];
		CGRect srcrnBounds = [[UIScreen mainScreen] bounds];
		CGRect sbFrame = [[UIApplication sharedApplication] statusBarFrame];
		
		UIInterfaceOrientation orientation = [[UIApplication sharedApplication] statusBarOrientation];
		
		int scrnWidth = 0;
		int scrnHeight = 0;

		if ((orientation == UIInterfaceOrientationPortrait) || (orientation == UIInterfaceOrientationPortraitUpsideDown)) {
			scrnWidth = (int)(srcrnBounds.size.width*scales+0.5);
			scrnHeight = (int)(srcrnBounds.size.height*scales+0.5);
		}
		else {
			scrnWidth = (int)(srcrnBounds.size.height*scales+0.5);
			scrnHeight = (int)(srcrnBounds.size.width*scales+0.5);
		}
		
	
		if ((image_width != scrnWidth) || (image_height != scrnHeight)) {
			// scale to app frame
			self.frame = appFrame;
		}
		else {
			self.frame = srcrnBounds;
		}
	}
}



@end




@implementation SplashViewController

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (id)initWithParentView:(UIView*)v {
	self = [super init];
    parentView = v;
    CGRect frame = [[UIScreen mainScreen] bounds];
    splashView = [[RhoSplashImageView alloc] initWithFrame:frame];
	
	splashView.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
	splashView.autoresizesSubviews = YES;
	self.view = splashView;
    [parentView addSubview:splashView];
    return self;
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

+ (NSString*)detectLoadingImage:(CGRect)myframe rotation_to_left:(BOOL*)rotation_to_left {
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

	CGRect win_frame = [[[UIApplication sharedApplication] keyWindow] bounds];
	
	CGRect frame = myframe;
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
			*rotation_to_left = YES;
			if ([fileManager fileExistsAtPath:pngDefaultLandscapePath]) {
				result = pngDefaultLandscapePath;
				*rotation_to_left = NO;
			}
			if (orientation == UIInterfaceOrientationLandscapeLeft) {
				if ([fileManager fileExistsAtPath:pngDefaultLandscapeLeftPath]) {
					result = pngDefaultLandscapeLeftPath;
					*rotation_to_left = NO;
				}
			}
			else {
				if ([fileManager fileExistsAtPath:pngDefaultLandscapeRightPath]) {
					result = pngDefaultLandscapeRightPath;
					*rotation_to_left = NO;
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
	if (result == nil) {
		if ([fileManager fileExistsAtPath:pngDefaultPath]) {
			result = pngDefaultPath;
		}
		else if ([fileManager fileExistsAtPath:pngDefault2xPath]) {
			result = pngDefault2xPath;
		}
		else if ([fileManager fileExistsAtPath:pngDefaultPortraitPath]) {
			result = pngDefaultPortraitPath;
		}
		else if ([fileManager fileExistsAtPath:pngDefaultLandscapePath]) {
			result = pngDefaultLandscapePath;
		}
		else if ([fileManager fileExistsAtPath:pngDefaultPortraitUpsideDownPath]) {
			result = pngDefaultPortraitUpsideDownPath;
		}
		else if ([fileManager fileExistsAtPath:pngDefaultLandscapeLeftPath]) {
			result = pngDefaultLandscapeLeftPath;
		}
		else if ([fileManager fileExistsAtPath:pngDefaultLandscapeRightPath]) {
			result = pngDefaultLandscapeRightPath;
		}
	}

	return result;
}

+ (BOOL)hasLoadingImage {
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSString *resourcePath = [[NSBundle mainBundle] resourcePath];

	NSString *pngLoadingPath = [NSString stringWithFormat:@"%@/apps/app/loading.png", resourcePath];
	NSString *pngDefaultPath = [NSString stringWithFormat:@"%@/Default.png", resourcePath];
	NSString *pngDefault2xPath = [NSString stringWithFormat:@"%@/Default@2x.png", resourcePath];
	NSString *pngDefaultPortraitPath = [NSString stringWithFormat:@"%@/Default-Portrait.png", resourcePath];
	NSString *pngDefaultPortraitUpsideDownPath = [NSString stringWithFormat:@"%@/Default-PortraitUpsideDown.png", resourcePath];
	NSString *pngDefaultLandscapePath = [NSString stringWithFormat:@"%@/Default-Landscape.png", resourcePath];
	NSString *pngDefaultLandscapeLeftPath = [NSString stringWithFormat:@"%@/Default-LandscapeLeft.png", resourcePath];
	NSString *pngDefaultLandscapeRightPath = [NSString stringWithFormat:@"%@/Default-LandscapeRight.png", resourcePath];
	
	return (
			([fileManager fileExistsAtPath:pngLoadingPath]) ||
			([fileManager fileExistsAtPath:pngDefaultPath]) ||
			([fileManager fileExistsAtPath:pngDefault2xPath]) ||
			([fileManager fileExistsAtPath:pngDefaultPortraitPath]) ||
			([fileManager fileExistsAtPath:pngDefaultPortraitUpsideDownPath]) ||
			([fileManager fileExistsAtPath:pngDefaultLandscapeLeftPath]) ||
			([fileManager fileExistsAtPath:pngDefaultLandscapeRightPath]) ||
			([fileManager fileExistsAtPath:pngDefaultLandscapePath])
	);


}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	BOOL is_iPad = NO;
	
	NSString *model = [[UIDevice currentDevice] model]; // "iPad ..."
	if ([model hasPrefix:@"iPad"]) {
		is_iPad = YES;
	}
	if (!is_iPad) {
		return NO;
	}
 	return YES;
}


@end
