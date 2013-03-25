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
#include "common/RhodesApp.h"

@implementation RhoViewController

#if defined(__DEBUG_CONTROLLERS)
- (id)init
{
    NSLog(@"RVC::%@::init",[[self class] description]);
    return [super init];
}
#endif

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
    #if defined(__DEBUG_CONTROLLERS)
    NSLog(@"RVC::%@::shouldAutorotateToInterfaceOrientation",[[self class] description]);
    #endif
    if ([[Rhodes sharedInstance] isRotationLocked])
        return toInterfaceOrientation == UIInterfaceOrientationPortrait;
	return YES;
}

#if defined(__IPHONE_6_0)
- (BOOL)shouldAutorotate {
    #if defined(__DEBUG_CONTROLLERS)
    NSLog(@"RVC::%@::shouldAutorotate",[[self class] description]);
    #endif
    if ([[Rhodes sharedInstance] isRotationLocked])
        return NO;
	return YES;
}

- (NSUInteger)supportedInterfaceOrientations
{
    #if defined(__DEBUG_CONTROLLERS)
    NSLog(@"RVC::%@::supportedInterfaceOrientations",[[self class] description]);
    #endif
    if ([[Rhodes sharedInstance] isRotationLocked])
         return UIInterfaceOrientationMaskPortrait;
    return UIInterfaceOrientationMaskAll;
}
#endif

@end


int rho_sys_get_screen_width();
int rho_sys_get_screen_height();


@implementation RhoRootViewController

- (void) didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
    UIInterfaceOrientation current_orientation = [[UIApplication sharedApplication] statusBarOrientation];
	if (current_orientation == fromInterfaceOrientation) {
		return;
	}
	int width = rho_sys_get_screen_width();
	int height = rho_sys_get_screen_height();
	// send after rotate message
	//CGRect wFrame = [webView frame];
	int angle = 0;
	switch (fromInterfaceOrientation) {
		case UIInterfaceOrientationPortrait: {
			switch (current_orientation) {
				case UIInterfaceOrientationLandscapeLeft: {
					angle = 90;
				}
					break;
				case UIInterfaceOrientationPortraitUpsideDown: {
					angle = 180;
				}
					break;
				case UIInterfaceOrientationLandscapeRight: {
					angle = -90;
				}
					break;
                    
                default:
                    angle = 0;
			}
		}
            break;
		case UIInterfaceOrientationLandscapeLeft: {
			switch (current_orientation) {
				case UIInterfaceOrientationPortrait: {
					angle = -90;
				}
					break;
				case UIInterfaceOrientationPortraitUpsideDown: {
					angle = 90;
				}
					break;
				case UIInterfaceOrientationLandscapeRight: {
					angle = 180;
				}
					break;
                    
                default:
                    angle = 0;
			}
		}
            break;
		case UIInterfaceOrientationPortraitUpsideDown: {
			switch (current_orientation) {
				case UIInterfaceOrientationPortrait: {
					angle = 180;
				}
					break;
				case UIInterfaceOrientationLandscapeLeft: {
					angle = -90;
				}
					break;
				case UIInterfaceOrientationLandscapeRight: {
					angle = 90;
				}
					break;
                
                default:
                    angle = 0;
			}
		}
            break;
		case UIInterfaceOrientationLandscapeRight: {
			switch (current_orientation) {
				case UIInterfaceOrientationPortrait: {
					angle = 90;
				}
					break;
				case UIInterfaceOrientationLandscapeLeft: {
					angle = 180;
				}
					break;
				case UIInterfaceOrientationPortraitUpsideDown: {
					angle = -90;
				}
					break;
                    
                default:
                    angle = 0;
			}
		}
            break;
	}
	//if ((current_orientation == UIInterfaceOrientationLandscapeLeft) || (current_orientation == UIInterfaceOrientationLandscapeRight)) {
	//	int t = width;
	//	width = height;
	//	height = t;
	//}
	//rho_rhodesapp_callScreenRotationCallback((int)wFrame.size.width, (int)wFrame.size.height, angle);
	rho_rhodesapp_callScreenRotationCallback(width, height, angle);
}

@end
