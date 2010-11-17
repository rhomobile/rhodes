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
	SignatureDelegate* signatureDelegate;
	NSString* start_parameters;
#ifdef __IPHONE_4_0
    EKEventStore *eventStore;
#endif
	
    id<RhoMainView,NSObject> mainView;
    BOOL rotationLocked;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) AVAudioPlayer *player;
@property (nonatomic, retain) NSMutableDictionary *cookies;
@property (nonatomic, copy) SignatureDelegate* signatureDelegate;
#ifdef __IPHONE_4_0
@property (nonatomic, readonly) EKEventStore *eventStore;
#endif
@property (nonatomic, retain) NSString* start_parameters;

+ (Rhodes*)sharedInstance;

+ (UIApplication*)application;

+ (CGRect)applicationFrame;
+ (void)setStatusBarHidden:(BOOL)v;

- (BOOL)isRotationLocked;

//+ (UIView*)subviewWithTag:(NSInteger)tag ofView:(UIView*)view;

+ (void)performOnUiThread:(id)runnable wait:(BOOL)wait;
+ (void)performOnUiThread:(id)runnable arg:(id)arg wait:(BOOL)wait;
+ (void)performOnUiThread:(id)runnable arg:(id)arg1 arg:(id)arg2 wait:(BOOL)wait;

- (UIWindow*)rootWindow;

- (void)playStart:(NSString*)fileName mediaType:(NSString*)type;
- (void)playStop;

- (void)hideSplash;

- (void)setCookie:(NSString*)cookie forUrl:(NSString*)url;
- (NSString*)cookie:(NSString*)url;

- (id<RhoMainView,NSObject>)mainView;
- (void)setMainView:(id<RhoMainView,NSObject>)view;

@end
