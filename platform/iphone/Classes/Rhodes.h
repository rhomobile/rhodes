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
#import "AppManager/AppManager.h"
#import "RhoMainView.h"

#import "SplashViewController.h"
#import "LogOptionsController.h"
#import "LogViewController.h"
#import "DateTimePickerDelegate.h"
//#import "Camera.old/PickImageDelegate.h"
#import "Camera/PickImageDelegate.h"

//#import "Signature.old/SignatureDelegate.h"
#import "Signature/SignatureDelegate.h"

#import "NativeView/NVDelegate.h"
#import "NetworkStatusMonitor.h"
#import "IPushNotificationsReceiver.h"
#import "IAppMessageReceiver.h"

#if defined(__IPHONE_10_0) && __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_10_0
#import <UserNotifications/UserNotifications.h>;
#endif

// Copied from Apple's header in case it is missing in some cases (e.g. pre-Xcode 8 builds).
#ifndef NSFoundationVersionNumber_iOS_9_x_Max
#define NSFoundationVersionNumber_iOS_9_x_Max 1299
#endif

//NSString *const kGCMMessageIDKey = @"gcm.message_id";
//#define kGCMMessageIDKey @"gcm.message_id"

@interface Rhodes : NSObject <UIApplicationDelegate,
    UITabBarControllerDelegate, AVAudioPlayerDelegate, UIAlertViewDelegate, UNUserNotificationCenterDelegate>
{
    AppManager *appManager;
    UIApplication *application;
    UIWindow *window;
    AVAudioPlayer *player;
    NSMutableDictionary *cookies;
    // Controllers
    SplashViewController *splashViewController;
    SplashViewController *splashViewControllerSnapShot;
    LogOptionsController* logOptionsController;
    LogViewController* logViewController;
    DateTimePickerDelegate* dateTimePickerDelegate;
    PickImageDelegate* pickImageDelegate;
	SignatureDelegate* signatureDelegate;
	NVDelegate* nvDelegate;
    id<IPushNotificationsReceiver> pushReceiver;
    id<IAppMessageReceiver> appMessageReceiver;
    
#ifdef __IPHONE_4_0
    EKEventStore *eventStore;
	__block UIBackgroundTaskIdentifier syncBackgroundTask;
#endif
	
    id<RhoMainView,NSObject> mainView;
    BOOL rotationLocked;
    BOOL mBlockExit;
    BOOL mIsFullScreen;
    BOOL mScreenStateChanged;
    
    // push specific
    NSDictionary* mPushStoredData_UserInfo;
    NSError* mPushStoredData_RegisterError;
    NSData* mPushStoredData_DeviceToken;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) AVAudioPlayer *player;
@property (nonatomic, retain) NSMutableDictionary *cookies;
@property (nonatomic, copy) SignatureDelegate* signatureDelegate;
@property (nonatomic, copy) NVDelegate* nvDelegate;
@property (nonatomic, assign) BOOL mBlockExit;
@property (atomic,assign) BOOL mScreenStateChanged;
@property (nonatomic, retain) NSCondition* mNetworkPollCondition;

+ (Rhodes*)sharedInstance;

+(void) makeSharedInstance;

+ (UIApplication*)application;

+ (CGRect)applicationFrame;
+ (void)setStatusBarHidden:(BOOL)v;

- (BOOL)isRotationLocked;
- (void) setRotationLocked:(BOOL)locked;

- (BOOL)getFullScreen;
- (void)setFullScreen:(BOOL)enable;
+ (BOOL)getStatusBarHidden;

//+ (UIView*)subviewWithTag:(NSInteger)tag ofView:(UIView*)view;

+ (void)performOnUiThread:(id)runnable wait:(BOOL)wait;
+ (void)performOnUiThread:(id)runnable arg:(id)arg wait:(BOOL)wait;
+ (void)performOnUiThread:(id)runnable arg:(id)arg1 arg:(id)arg2 wait:(BOOL)wait;

+(void)restart_app;

- (BOOL)didFinishLaunchingWithOptionsInternal:(NSDictionary *)launchOptions application:(UIApplication *)application;

- (UIWindow*)rootWindow;

- (void)playStart:(NSString*)fileName mediaType:(NSString*)type;
- (void)playStop;

- (void)hideSplash;

- (void)setCookie:(NSString*)cookie forUrl:(NSString*)url;
- (NSString*)cookie:(NSString*)url;

- (NSDictionary*)getCookies:(NSString*)url;
- (BOOL)removeCookie:(NSString*)url name:(NSString*)cookieName;
- (BOOL)removeAllCookies;

- (id<RhoMainView,NSObject>)mainView;
- (void)setMainView:(id<RhoMainView,NSObject>)view;

- (void)openFullScreenNativeView:(UIView*)view;
- (void)closeFullScreenNativeView;
	
// UIAlertViewDelegate
- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex;

- (void) exit_with_errormessage:(NSString*)title message:(NSString*)message;

- (void) registerForPushNotifications:(id<IPushNotificationsReceiver>)receiver;

- (void) signalNetworkStatusPollIntervalChanged;

- (void) setAppMessageReceiver:(id<IAppMessageReceiver>)receiver;
#ifdef __IPHONE_4_0
- (EKEventStore*) getEventStore;
#endif

- (void)registerForRemoteNotification;
- (void)processPushMessage:(NSDictionary*)userInfo;

@end
