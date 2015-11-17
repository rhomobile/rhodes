/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

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
    
    self.contentMode = UIViewContentModeScaleToFill;
	
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
        
        int app_width = 0;
        int app_height = 0;


	scrnWidth = (int)(srcrnBounds.size.width*scales+0.5);
	scrnHeight = (int)(srcrnBounds.size.height*scales+0.5);
        app_width = (int)(appFrame.size.width+0.5);
        app_height = (int)(appFrame.size.height+0.5);

		
	
	if ((image_width != scrnWidth) || (image_height != scrnHeight) || (img_scale != scales)) 
        {
            if ((((int)[img size].width) != app_width) || (((int)[img size].height) != app_height)) {
                // scale to app frame
                self.frame = appFrame;
                CGRect rect;
                
                rect.origin.x = appFrame.origin.x;
                rect.origin.y = appFrame.origin.y;

                float res_w = 0;
                float res_h = 0;
                
                float width_k = (app_width * scales) / (float)image_width;
                float height_k = (app_height * scales) / (float)image_height;
                
                if (width_k >= height_k) {
                    res_w = app_width;
                    res_h = (((float)image_height) / scales) * width_k;
                }
                else {
                    res_h = app_height;
                    res_w = (((float)image_width) / scales) * height_k;
                }
                
                if ((orientation == UIInterfaceOrientationPortrait) || (orientation == UIInterfaceOrientationPortraitUpsideDown)) {
                    if (orientation == UIInterfaceOrientationPortraitUpsideDown) {
                        rect.origin.x -= (res_w - app_width);
                    }
                    rect.size.width = res_w;
                    rect.size.height = res_h;
                }
                else {
                    if (orientation == UIInterfaceOrientationLandscapeLeft) {
                        //rect.origin.x -= (res_h - app_height);
                    }
                    else {
                        rect.origin.x -= (res_h - app_height);
                    }
                    rect.size.width = res_h;
                    rect.size.height = res_w;
                }
                
                if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
                    self.frame = rect;
                }
            }
            
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
    
    //[parentView addSubview:splashView];
    ((UIWindow*)parentView).rootViewController = self;
    
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
	NSString *pngDefaultiPhone5 = [NSString stringWithFormat:@"%@/Default-568h@2x.png", resourcePath];
    NSString *pngDefaultiPhone6 = [NSString stringWithFormat:@"%@/Default-667h@2x.png", resourcePath];
    NSString *pngDefaultiPhone6plus = [NSString stringWithFormat:@"%@/Default-736h@3x.png", resourcePath];

	CGRect win_frame = [[[UIApplication sharedApplication] keyWindow] bounds];
	
	CGRect frame = myframe;
	float scales = 1;//[[UIScreen mainScreen] scale];
#ifdef __IPHONE_4_0
	if ( [[UIScreen mainScreen] respondsToSelector:@selector(scale)] ) {
		scales = [[UIScreen mainScreen] scale];
	}
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
            // check for iPhone5 screen
            if (frame.size.height*scales >= (1136-1)) {
                if ([fileManager fileExistsAtPath:pngDefaultiPhone5]) {
                    result = pngDefaultiPhone5;
                }
            }
            if (frame.size.height*scales >= (1334-1)) {
                if ([fileManager fileExistsAtPath:pngDefaultiPhone6]) {
                    result = pngDefaultiPhone6;
                }
            }
            if (frame.size.height*scales >= (2208-1)) {
                if ([fileManager fileExistsAtPath:pngDefaultiPhone6plus]) {
                    result = pngDefaultiPhone6plus;
                }
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
        else if ([fileManager fileExistsAtPath:pngDefaultiPhone5]) {
            result = pngDefaultiPhone5;
        }
        else if ([fileManager fileExistsAtPath:pngDefaultiPhone6]) {
            result = pngDefaultiPhone6;
        }
        else if ([fileManager fileExistsAtPath:pngDefaultiPhone6plus]) {
            result = pngDefaultiPhone6plus;
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

	//NSString *pngLoadingPath = [NSString stringWithFormat:@"%@/apps/app/loading.png", resourcePath];
	NSString *pngDefaultPath = [NSString stringWithFormat:@"%@/Default.png", resourcePath];
	NSString *pngDefault2xPath = [NSString stringWithFormat:@"%@/Default@2x.png", resourcePath];
	NSString *pngDefaultPortraitPath = [NSString stringWithFormat:@"%@/Default-Portrait.png", resourcePath];
	NSString *pngDefaultPortraitUpsideDownPath = [NSString stringWithFormat:@"%@/Default-PortraitUpsideDown.png", resourcePath];
	NSString *pngDefaultLandscapePath = [NSString stringWithFormat:@"%@/Default-Landscape.png", resourcePath];
	NSString *pngDefaultLandscapeLeftPath = [NSString stringWithFormat:@"%@/Default-LandscapeLeft.png", resourcePath];
	NSString *pngDefaultLandscapeRightPath = [NSString stringWithFormat:@"%@/Default-LandscapeRight.png", resourcePath];
	NSString *pngDefaultiPhone5 = [NSString stringWithFormat:@"%@/Default-568h@2x.png", resourcePath];
    NSString *pngDefaultiPhone6 = [NSString stringWithFormat:@"%@/Default-667@2x.png", resourcePath];
    NSString *pngDefaultiPhone6plus = [NSString stringWithFormat:@"%@/Default-736h@3x.png", resourcePath];
	
	return (
			//([fileManager fileExistsAtPath:pngLoadingPath]) ||
			([fileManager fileExistsAtPath:pngDefaultPath]) ||
			([fileManager fileExistsAtPath:pngDefault2xPath]) ||
			([fileManager fileExistsAtPath:pngDefaultPortraitPath]) ||
			([fileManager fileExistsAtPath:pngDefaultPortraitUpsideDownPath]) ||
			([fileManager fileExistsAtPath:pngDefaultLandscapeLeftPath]) ||
			([fileManager fileExistsAtPath:pngDefaultLandscapeRightPath]) ||
			([fileManager fileExistsAtPath:pngDefaultiPhone5]) ||
            ([fileManager fileExistsAtPath:pngDefaultiPhone6]) ||
            ([fileManager fileExistsAtPath:pngDefaultiPhone6plus]) ||
			([fileManager fileExistsAtPath:pngDefaultLandscapePath])
	);


}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
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

#ifdef __IPHONE_6_0
- (BOOL)shouldAutorotate
{
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


- (NSUInteger)supportedInterfaceOrientations
{
    return UIInterfaceOrientationMaskAll;
}
#endif


@end
