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

#include "common/RhoConf.h"

static UIImage* splash_image = nil;
static UIViewController* launch_screen_view_controller = nil;

CGFloat DegreesToRadians(CGFloat degrees) {return degrees * M_PI / 180;};
CGFloat RadiansToDegrees(CGFloat radians) {return radians * 180/M_PI;};


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
	
    /*
	BOOL is_need_left = NO;
	
	NSString* imagePath = [SplashViewController detectLoadingImage:rect rotation_to_left:&is_need_left];

    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:imagePath])
        return;
    
    UIImage *img = [[UIImage alloc] initWithContentsOfFile:imagePath];
	*/
    UIImage *img = [SplashViewController getLoadingImage];
    
    
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
                self.frame = srcrnBounds;//appFrame;
                CGRect rect;
                
                rect.origin.x = 0;//appFrame.origin.x;
                rect.origin.y = 0;//appFrame.origin.y;

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
                
                //if ((orientation == UIInterfaceOrientationPortrait) || (orientation == UIInterfaceOrientationPortraitUpsideDown)) {
                    if (orientation == UIInterfaceOrientationPortraitUpsideDown) {
                        rect.origin.x -= (res_w - app_width);
                    }
                    rect.size.width = res_w;
                    rect.size.height = res_h;
                //}
                //else {
                //    if (orientation == UIInterfaceOrientationLandscapeLeft) {
                //        //rect.origin.x -= (res_h - app_height);
                //    }
                //    else {
                //        rect.origin.x -= (res_h - app_height);
                //    }
                //    rect.size.width = res_h;
                //    rect.size.height = res_w;
                //}
                
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


+(BOOL)isReplaceContentWhenSnapshot {
    BOOL save_for_splash = NO;
    if (rho_conf_is_property_exists("WebView.replaceContentBySplashWhenSnapshotBySystem")) {
        int v = rho_conf_getBool("WebView.replaceContentBySplashWhenSnapshotBySystem");
        if (v) {
            save_for_splash = YES;
        }
    }
    return save_for_splash;
}


// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (id)initWithParentView:(UIView*)v {
	self = [super init];
    parentView = v;
    CGRect frame = [[UIScreen mainScreen] bounds];
    
    
    if ([SplashViewController hasLoadingImage]) {
        splashView = [[RhoSplashImageView alloc] initWithFrame:frame];
        splashView.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
        splashView.autoresizesSubviews = YES;
        self.view = splashView;
        
        //UIView *view = [[UIView alloc] initWithFrame:frame];
        //view.backgroundColor = [UIColor yellowColor];

        //self.view = view;
    }
    else {
        UIView *view = [[UIView alloc] initWithFrame:frame];
        view.backgroundColor = [UIColor blackColor];
        
        @try {
            UIStoryboard *sb = [UIStoryboard storyboardWithName:@"LaunchScreen" bundle:nil];
            launch_screen_view_controller = [[sb instantiateInitialViewController] retain];
            
            UIView* vi = launch_screen_view_controller.view;
            CGRect rrr = vi.frame;
            
            //view = vc.view;
            
            ((UIWindow*)parentView).rootViewController = launch_screen_view_controller;
            [((UIWindow*)parentView) bringSubviewToFront:launch_screen_view_controller.view];
            //self.view = view;
            
            //((UIWindow*)parentView).rootViewController = self;
            //[self presentViewController:launch_screen_view_controller animated:NO completion:nil];
            
            
            return self;
        }
        @catch (NSException* e) {
            
        }
        self.view = view;


    }
    
    //[parentView addSubview:splashView];
    ((UIWindow*)parentView).rootViewController = self;
    
    return self;
}

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (id)initWithFrame:(CGRect)frame {
    self = [super init];
    //parentView = v;
    //CGRect frame = [[UIScreen mainScreen] bounds];
    
    if ([SplashViewController hasLoadingImage]) {
        splashView = [[RhoSplashImageView alloc] initWithFrame:frame];
        splashView.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
        splashView.autoresizesSubviews = YES;
        self.view = splashView;
        
        //UIView *view = [[UIView alloc] initWithFrame:frame];
        //view.backgroundColor = [UIColor yellowColor];

        //self.view = view;
        
    }
    else {
        UIView *view = [[UIView alloc] initWithFrame:frame];
        view.backgroundColor = [UIColor blackColor];
        
        @try {
            UIStoryboard *sb = [UIStoryboard storyboardWithName:@"LaunchScreen" bundle:nil];
            launch_screen_view_controller = [[sb instantiateInitialViewController] retain];
            
            UIView* vi = launch_screen_view_controller.view;
            CGRect rrr = vi.frame;

            //view = vc.view;
            
            ((UIWindow*)parentView).rootViewController = launch_screen_view_controller;
            [((UIWindow*)parentView) bringSubviewToFront:launch_screen_view_controller.view];
            //self.view = view;

            //((UIWindow*)parentView).rootViewController = self;
            //[self presentViewController:launch_screen_view_controller animated:NO completion:nil];

            
            return self;
        }
        @catch (NSException* e) {
            
        }
        self.view = view;
    }
    
    return self;
}


- (void)hideSplash {
    rho_splash_screen_hide();
    if (splashView != nil) {
        [splashView removeFromSuperview];
    }
    if (launch_screen_view_controller != nil) {
        //[launch_screen_view_controller dismissViewControllerAnimated:NO completion:nil];
        //[launch_screen_view_controller.view removeFromSuperview];
        launch_screen_view_controller = nil;
    }
}

- (void)bringToFront {
    if (splashView != nil) {
        [parentView bringSubviewToFront:splashView];
    }
    if (launch_screen_view_controller != nil) {
        [parentView bringSubviewToFront:launch_screen_view_controller.view];
    }
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



+ (UIImage*)getLoadingImage {
    [SplashViewController hasLoadingImage];
    return splash_image;
}


+ (BOOL)hasLoadingImage {
    
    if (splash_image != nil) {
        return YES;
    }
    // try to detect
    NSString* pngLaunchImage = @"LaunchImage";
    NSString* pngLaunchImage2 = nil;
    NSString *pngLaunchImagePath = @"LaunchImage";
    NSString *pngLaunchImage568Path = @"LaunchImage-700-568h";
    NSString *pngLaunchImage667Path = @"LaunchImage-800-667h";
    NSString *pngLaunchImage736PortraitPath = @"LaunchImage-800-Portrait-736h";
    NSString *pngLaunchImage736LandscapePath = @"LaunchImage-800-Landscape-736h";
    NSString *pngLaunchImage2436PortraitPath = @"LaunchImage-1100-Portrait-2436h";
    NSString *pngLaunchImage2688PortraitPath = @"LaunchImage-1200-Portrait-2688h";
    NSString *pngLaunchImage1792PortraitPath = @"LaunchImage-1200-Portrait-1792h";
    NSString *pngLaunchImage2436LandscapePath = @"LaunchImage-1100-Landscape-2436h";
    NSString *pngLaunchImage2688LandscapePath = @"LaunchImage-1200-Landscape-2688h";
    NSString *pngLaunchImage1792LandscapePath = @"LaunchImage-1200-Landscape-1792h";

    NSString *pngLaunchImageIPADPortraitPath = @"LaunchImage-Portrait~ipad";
    NSString *pngLaunchImageIPADLandscapePath = @"LaunchImage-Landscape~ipad";

    UIScreen *screen = [UIScreen mainScreen];
    CGFloat screenHeight = screen.bounds.size.height;
    BOOL isPortrait = YES;
    if (screen.bounds.size.width > screenHeight) {
        screenHeight = screen.bounds.size.width;
        isPortrait = NO;
    }
    CGFloat scale = screen.scale;
    BOOL isiPhone = ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone);
    BOOL isiPad = ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad);
    
    
    BOOL canBeRotated = NO;
    
    if (isiPad) {
        if (isPortrait) {
            pngLaunchImage = pngLaunchImageIPADPortraitPath;
        }
        else {
            pngLaunchImage = pngLaunchImageIPADLandscapePath;
            pngLaunchImage2 = pngLaunchImageIPADPortraitPath;
        }
    }
    else {
        if ((screenHeight >= (896-1)) && (screenHeight <= (896+1))) {
            // iPhoneXS MAX
            if (isPortrait) {
                pngLaunchImage = pngLaunchImage2688PortraitPath;
            }
            else {
                pngLaunchImage = pngLaunchImage2688LandscapePath;
                pngLaunchImage2 = pngLaunchImage2688PortraitPath;
            }
        }
        else {
            if ((screenHeight >= (828-1)) && (screenHeight <= (828+1))) {
                // iPhoneXR
                if (isPortrait) {
                    pngLaunchImage = pngLaunchImage1792PortraitPath;
                }
                else {
                    pngLaunchImage = pngLaunchImage1792LandscapePath;
                    pngLaunchImage2 = pngLaunchImage1792PortraitPath;
                }
            }
            else {
                if ((screenHeight >= (812-1)) && (screenHeight <= (812+1))) {
                    // iPhoneX or iPhone XS
                    if (isPortrait) {
                        pngLaunchImage = pngLaunchImage2436PortraitPath;
                    }
                    else {
                        pngLaunchImage = pngLaunchImage2436LandscapePath;
                        pngLaunchImage2 = pngLaunchImage2436PortraitPath;
                    }
                }
                else {
                    if (screenHeight >= (736-1)) {
                        // iPhonePlus
                        if (isPortrait) {
                            pngLaunchImage = pngLaunchImage736PortraitPath;
                        }
                        else {
                            pngLaunchImage = pngLaunchImage736LandscapePath;
                            pngLaunchImage2 = pngLaunchImage736PortraitPath;
                        }
                    }
                    else {
                        if (screenHeight >= (667-1)) {
                            // iPhone6,7
                            pngLaunchImage = pngLaunchImage667Path;
                            canBeRotated = YES;
                        }
                        else {
                            if (screenHeight >= (568-1)) {
                                // iPhone5
                                pngLaunchImage = pngLaunchImage568Path;
                                canBeRotated = YES;
                            }
                            else {
                                // old iPhones
                                pngLaunchImage = pngLaunchImagePath;
                                canBeRotated = YES;
                            }
                        }
                    }
                }
            }
        }
    }
    
    
    float angle = 0.0;
    if (!isPortrait) {
        angle = -90.0;
        UIInterfaceOrientation statusBarOrientation = [[UIApplication sharedApplication] statusBarOrientation];
        if (statusBarOrientation == UIInterfaceOrientationLandscapeLeft) {
            angle = 90.0;
        }
    }

    splash_image = [UIImage imageNamed:pngLaunchImage inBundle:nil compatibleWithTraitCollection:nil];
    
    if ((splash_image == nil) && (pngLaunchImage2 != nil)) {
        splash_image = [UIImage imageNamed:pngLaunchImage2 inBundle:nil compatibleWithTraitCollection:nil];
        canBeRotated = YES;
    }
    
    
    if ((splash_image != nil) && (canBeRotated) && ((angle < -0.01) || (angle > 0.01)) ) {
        // calculate the size of the rotated view's containing box for our drawing space
        UIView *rotatedViewBox = [[UIView alloc] initWithFrame:CGRectMake(0,0,splash_image.size.width, splash_image.size.height)];
        CGAffineTransform t = CGAffineTransformMakeRotation(DegreesToRadians(angle));
        rotatedViewBox.transform = t;
        CGSize rotatedSize = rotatedViewBox.frame.size;
        [rotatedViewBox release];
        
        // Create the bitmap context
        UIGraphicsBeginImageContext(rotatedSize);
        CGContextRef bitmap = UIGraphicsGetCurrentContext();
        
        // Move the origin to the middle of the image so we will rotate and scale around the center.
        CGContextTranslateCTM(bitmap, rotatedSize.width/2, rotatedSize.height/2);
        
        //   // Rotate the image context
        CGContextRotateCTM(bitmap, DegreesToRadians(angle));
        
        // Now, draw the rotated/scaled image into the context
        CGContextScaleCTM(bitmap, 1.0, -1.0);
        CGContextDrawImage(bitmap, CGRectMake(-splash_image.size.width / 2, -splash_image.size.height / 2, splash_image.size.width, splash_image.size.height), [splash_image CGImage]);
        
        UIImage *newImage = UIGraphicsGetImageFromCurrentImageContext();
        UIGraphicsEndImageContext();
        splash_image = [newImage retain];
    }

    if (splash_image != nil) {
        splash_image = [splash_image retain];
    }
    return (splash_image != nil);
   
}

+ (BOOL)hasLoadingScreenStoryboard {
    @try {
        UIStoryboard *sb = [UIStoryboard storyboardWithName:@"LaunchScreen" bundle:nil];
        launch_screen_view_controller = [sb instantiateInitialViewController];
        if (launch_screen_view_controller != nil) {
            return YES;
        }
    }
    @catch (NSException *e) {
        
    }
    return NO;
    
    /*
     NSArray *objects = [[NSBundle mainBundle] loadNibNamed:@"LaunchScreen" owner:self options:nil];
    if (objects != nil) {
        if ([objects count] > 0) {
            return YES;
        }
    }
    return NO;
     */
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
