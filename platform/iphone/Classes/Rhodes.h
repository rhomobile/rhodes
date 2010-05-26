/*
 *  Rhodes.h
 *  rhorunner
 *
 *  Created by Dmitry Moskalchuk on 07.03.10.
 *  Copyright 2010 Rhomobile Inc. All rights reserved.
 *
 */

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import "AppManager.h"
#import "RhoMainView.h"

#import "SplashViewController.h"
#import "LogOptionsController.h"
#import "LogViewController.h"
#import "DateTimePickerDelegate.h"
#import "PickImageDelegate.h"

@interface Rhodes : NSObject <UIApplicationDelegate,
    UITabBarControllerDelegate, AVAudioPlayerDelegate>
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
@public
    id<RhoMainView,NSObject> mainView;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) AVAudioPlayer *player;
@property (nonatomic, retain) id<RhoMainView,NSObject> mainView;
@property (nonatomic, retain) NSMutableDictionary *cookies;

+ (Rhodes*)sharedInstance;

+ (UIApplication*)application;

+ (CGRect)applicationFrame;
+ (void)setStatusBarHidden:(BOOL)v;

+ (UIView*)subviewWithTag:(NSInteger)tag ofView:(UIView*)view;

+ (void)performOnUiThread:(id)runnable wait:(BOOL)wait;
+ (void)performOnUiThread:(id)runnable arg:(id)arg wait:(BOOL)wait;
+ (void)performOnUiThread:(id)runnable arg:(id)arg1 arg:(id)arg2 wait:(BOOL)wait;

- (UIWindow*)rootWindow;

- (void)playStart:(NSString*)fileName mediaType:(NSString*)type;
- (void)playStop;

- (void)setCookie:(NSString*)cookie forUrl:(NSString*)url;
- (NSString*)cookie:(NSString*)url;

@end
