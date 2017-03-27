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

#import "Rhodes.h"
#import "RhoViewController.h"

#import "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoViewController"

@implementation RhoViewController

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
    RAWTRACE1("RVC::%s::shouldAutorotateToInterfaceOrientation", [[[self class] description] UTF8String]);
    if ([[Rhodes sharedInstance] isRotationLocked])
        return toInterfaceOrientation == UIInterfaceOrientationPortrait;
	return YES;
}

#if defined(__IPHONE_7_0)
- (BOOL)prefersStatusBarHidden {
    return [[Rhodes sharedInstance] getFullScreen];
}
#endif

#if defined(__IPHONE_6_0)
- (BOOL)shouldAutorotate {
    RAWTRACE1("RVC::%s::shouldAutorotate", [[[self class] description] UTF8String]);
    if ([[Rhodes sharedInstance] isRotationLocked])
        return NO;
   
   //checking for device version
    NSString *version = [[UIDevice currentDevice] systemVersion];
    float versionFloat = [version floatValue];
    
    //if version is less than 7 no frame changes are made
    if(versionFloat < 7.0f)
        return YES;
    
    //Update frame on rotation
    CGRect screenBounds = [UIScreen mainScreen].bounds ;
    CGFloat width = CGRectGetWidth(screenBounds)  ;
    CGFloat height = CGRectGetHeight(screenBounds) ;
    UIInterfaceOrientation interfaceOrientation = [UIApplication sharedApplication].statusBarOrientation;
    
    //calculating frame
    screenBounds = self.view.frame;
    if(UIInterfaceOrientationIsPortrait(interfaceOrientation)){
        if (width<height) {
            screenBounds.size = CGSizeMake(width, height);
        } else {
            screenBounds.size = CGSizeMake(height, width);
        }
    }else if(UIInterfaceOrientationIsLandscape(interfaceOrientation)){
        if (width<height) {
            screenBounds.size = CGSizeMake(height, width);
        } else {
            screenBounds.size = CGSizeMake(width, height);
        }
    }
    //updating frame
    self.view.frame = screenBounds;

	return YES;
}

- (NSUInteger)supportedInterfaceOrientations
{
    RAWTRACE1("RVC::%s::supportedInterfaceOrientations", [[[self class] description] UTF8String]);
    if ([[Rhodes sharedInstance] isRotationLocked])
         return UIInterfaceOrientationMaskPortrait;
    return UIInterfaceOrientationMaskAll;
}
#endif

@end
