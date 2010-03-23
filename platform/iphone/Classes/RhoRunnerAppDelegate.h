//
//  BrowserAppDelegate.h
//  Browser
//
//  Created by adam blum on 9/4/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import "ServerHost.h"
#import "SplashViewController.h"
#import "PickImageDelegate.h"
#import "DateTimePickerDelegate.h"
#import "TabBarDelegate.h"
#import "LogViewController.h"
#import "LogOptionsController.h"
#import "NativeBar.h"

@class WebViewController;

@interface RhoRunnerAppDelegate : NSObject <UIApplicationDelegate, AVAudioPlayerDelegate> {
    UIWindow *window;
    SplashViewController *splashViewController;
    WebViewController *webViewController;
    LogViewController* logViewController;
    LogOptionsController* logOptionsController;
    ServerHost * serverHost;
    PickImageDelegate* pickImageDelegate;
    DateTimePickerDelegate* dateTimePickerDelegate;
    TabBarDelegate* tabBarDelegate;
    NativeBar* nativeBar;
    AVAudioPlayer *player;
    bool appStarted;
    bool splashDisplayed;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) SplashViewController *splashViewController;
@property (nonatomic, retain) WebViewController *webViewController;
@property (nonatomic, retain) AVAudioPlayer *player;
@property (nonatomic, retain) NativeBar* nativeBar;

+ (RhoRunnerAppDelegate*)sharedDelegate;
- (void)fixFrame;

@end

