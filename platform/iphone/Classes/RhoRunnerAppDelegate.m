//
//  BrowserAppDelegate.m
//  Browser
//
//  Created by adam blum on 9/4/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//
#import <AudioToolbox/AudioToolbox.h>

#include "common/RhoPort.h"
#import "RhoRunnerAppDelegate.h"
#import "SimpleMainView.h"

#import "logging/RhoLog.h"
#include "sync/ClientRegister.h"
#include "sync/syncthread.h"
#include "common/RhodesApp.h"
#import "ParamsWrapper.h"
#import "DateTime.h"
#import "NativeBar.h"
#import "RhoDelegate.h"
#ifdef __IPHONE_3_0
#import "MapViewController.h"
#endif

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoRunnerAppDelegate"

@implementation RhoRunnerAppDelegate

@synthesize window;
//@synthesize splashViewController;
//@synthesize mainView;
//@synthesize player; 
//@synthesize nativeBar;

/*
+ (RhoRunnerAppDelegate*)sharedDelegate {
    UIApplication *app = [UIApplication sharedApplication];
    return (RhoRunnerAppDelegate*)app.delegate;
}
*/

/*
- (void)fixFrame {
    UIApplication *app = [UIApplication sharedApplication];
    CGRect frame = [[UIScreen mainScreen] applicationFrame];
    if (!app.statusBarHidden) {
        int newY = 10;
        frame.size.height += frame.origin.y - newY;
        frame.origin.y = newY;
    }
    [window setFrame:frame];
}
*/

/*
- (void)hideSplash {
    [splashViewController hideSplash];
    splashDisplayed = false;
}
*/

- (void) doStartUp {
	appStarted = false;
    splashDisplayed = false;
    
    [[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleBlackTranslucent];
    //Create local server and start it
    //serverHost = [[ServerHost alloc] init];
    //serverHost = [ServerHost sharedInstance];
    //[serverHost create];
    
    //CGRect sbFrame = [[UIApplication sharedApplication] statusBarFrame];
    //UIApplication *app = [UIApplication sharedApplication];
    //[app setStatusBarStyle:UIStatusBarStyleBlackTranslucent];
    //[app setStatusBarHidden:YES];
    
    //[[UIApplication sharedApplication]
    //    setStatusBarStyle:UIStatusBarStyleBlackTranslucent animated:YES];
    
    window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]];
    window.contentMode = UIViewContentModeScaleToFill;
    window.autoresizesSubviews = YES;
    window.userInteractionEnabled = YES;
    [window makeKeyAndVisible];

    //[self fixFrame];
    
    //webViewController = [[WebViewController alloc] initWithParentWindow:window];
    //webViewController->actionTarget = self;
    //webViewController->onShowLog = @selector(onShowLog);
    
    //mainView = [[SimpleMainView alloc] initWithParentWindow:window];
    
    //splashViewController = [[SplashViewController alloc] initWithParentView:[mainView getView]];
    
    //[self showLoadingPage];
    
    // Log View
    /*
	logViewController = [[LogViewController alloc] init];
	logViewController->actionTarget = self;
	logViewController->onShowLogOptions = @selector(onShowLogOptions);
     */
	
	//logOptionsController = [[LogOptionsController alloc] init];    
	
	//TabBar delegate
	//tabBarDelegate = [[TabBarDelegate alloc] init];
	
	//Camera delegate
	//pickImageDelegate = [[PickImageDelegate alloc] init];
	
	//DateTime delegate
	//dateTimePickerDelegate = [[DateTimePickerDelegate alloc] init];
	
	serverHost->actionTarget = self;
	serverHost->onTakePicture = @selector(onTakePicture:);
	serverHost->onChoosePicture = @selector(onChoosePicture:);
	serverHost->onChooseDateTime = @selector(onChooseDateTime:);
	serverHost->onCreateMap = @selector(onCreateMap:);
    //[serverHost start];
		
#ifdef __IPHONE_3_0
	[[UIApplication sharedApplication] registerForRemoteNotificationTypes:(UIRemoteNotificationTypeBadge 
																		   | UIRemoteNotificationTypeAlert | UIRemoteNotificationTypeSound)];
#endif
}

- (void)dealloc {
    [serverHost release];
	[window release];
	//[pickImageDelegate release];
	//[dateTimePickerDelegate release];
	//[tabBarDelegate release];
	//[nativeBar release];
	[super dealloc];
}

@end
