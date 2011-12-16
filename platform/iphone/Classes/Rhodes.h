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

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#ifdef __IPHONE_4_0
#import <EventKit/EventKit.h>
#endif
#import "AppManager.h"
#import "RhoMainView.h"

#import "SplashViewController.h"
#import "LogOptionsController.h"
#import "LogViewController.h"
#import "DateTimePickerDelegate.h"
#import "PickImageDelegate.h"
#import "SignatureDelegate.h"
#import "NVDelegate.h"

@interface Rhodes : NSObject <UIApplicationDelegate,
    UITabBarControllerDelegate, AVAudioPlayerDelegate, UIAlertViewDelegate>
{
    AppManager *appManager;
    UIApplication *application;
    UIWindow *window;
    AVAudioPlayer *player;
    NSMutableDictionary *cookies;
    // Controllers
    SplashViewController *splashViewController;
    LogOptionsController* logOptionsController;
    LogViewController* logViewController;
    DateTimePickerDelegate* dateTimePickerDelegate;
    PickImageDelegate* pickImageDelegate;
	SignatureDelegate* signatureDelegate;
	NVDelegate* nvDelegate;
#ifdef __IPHONE_4_0
    EKEventStore *eventStore;
#endif
	
    id<RhoMainView,NSObject> mainView;
    BOOL rotationLocked;
    BOOL mBlockExit;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) AVAudioPlayer *player;
@property (nonatomic, retain) NSMutableDictionary *cookies;
@property (nonatomic, copy) SignatureDelegate* signatureDelegate;
@property (nonatomic, copy) NVDelegate* nvDelegate;
@property (nonatomic, assign) BOOL mBlockExit;

#ifdef __IPHONE_4_0
@property (nonatomic, readonly) EKEventStore *eventStore;
#endif

+ (Rhodes*)sharedInstance;

+ (UIApplication*)application;

+ (CGRect)applicationFrame;
+ (void)setStatusBarHidden:(BOOL)v;

- (BOOL)isRotationLocked;

//+ (UIView*)subviewWithTag:(NSInteger)tag ofView:(UIView*)view;

+ (void)performOnUiThread:(id)runnable wait:(BOOL)wait;
+ (void)performOnUiThread:(id)runnable arg:(id)arg wait:(BOOL)wait;
+ (void)performOnUiThread:(id)runnable arg:(id)arg1 arg:(id)arg2 wait:(BOOL)wait;

+(void)restart_app;

- (UIWindow*)rootWindow;

- (void)playStart:(NSString*)fileName mediaType:(NSString*)type;
- (void)playStop;

- (void)hideSplash;

- (void)setCookie:(NSString*)cookie forUrl:(NSString*)url;
- (NSString*)cookie:(NSString*)url;

- (id<RhoMainView,NSObject>)mainView;
- (void)setMainView:(id<RhoMainView,NSObject>)view;

- (void)openFullScreenNativeView:(UIView*)view;
- (void)closeFullScreenNativeView;
	
// UIAlertViewDelegate
- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex;

- (void) exit_with_errormessage:(NSString*)title message:(NSString*)message;

@end
