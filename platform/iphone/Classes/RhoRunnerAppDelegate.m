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

#ifdef __IPHONE_3_0
-(void) onCreateMap:(NSValue*)value {
	MapViewController* map = [[MapViewController alloc] init];
    [map setParams:[value pointerValue]];
	map.actionTarget = self;
	map.onNavigate = @selector(onNavigateTo:);
	[window addSubview:map.view];
    [map.view setFrame:[window frame]];
}
#endif

-(BOOL)startCameraPickerFromViewController:(UIViewController*)controller 
							 usingDelegate:(id<UINavigationControllerDelegate, UIImagePickerControllerDelegate>)delegateObject 
							 sourceType:(UIImagePickerControllerSourceType)type
{ 
#if !defined __IPHONE_3_0
	if ( (![UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypePhotoLibrary]) || 
		 (![UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera]) ||
		 (delegateObject == nil) || (controller == nil)) {
		return NO; 
	}
#endif
	
	@try {
        [[UIApplication sharedApplication] setStatusBarHidden:YES];
        [self fixFrame];
		UIImagePickerController* picker = [[UIImagePickerController alloc] init]; 
		picker.sourceType = type;
		picker.delegate = delegateObject; 
		picker.allowsImageEditing = YES;
        
        //[picker.view setFrame:frame];
		[window addSubview:picker.view];
	} @catch(NSException* theException) {
		RAWLOG_ERROR2("startCameraPickerFromViewController failed(%s): %s", [[theException name] UTF8String], [[theException reason] UTF8String] );
		//NSLog(@"%@", theException);
        [[UIApplication sharedApplication] setStatusBarHidden:NO];
        [self fixFrame];
		return NO;
	}
	
	return YES;
} 

-(BOOL)startDateTimePickerFromViewController:(UIViewController*)controller 
							 usingDelegate:(DateTimePickerDelegate*)delegateObject
{
	@try {
		[delegateObject createPicker:window];
	} @catch (NSException* theException) {
		RAWLOG_ERROR2("startDateTimePickerFromViewController failed(%s): %s", [[theException name] UTF8String], [[theException reason] UTF8String] );
		return NO;
	}
	return YES;
} 

- (void)onTakePicture:(NSString*) url {
    // TODO:
    /*
	[pickImageDelegate setPostUrl:url];//[self normalizeUrl:url]];
	[self startCameraPickerFromViewController:webViewController 
								usingDelegate:pickImageDelegate 
								sourceType:UIImagePickerControllerSourceTypeCamera];
    */
}

- (void)onChoosePicture:(NSString*) url {
    // TODO:
    /*
	[pickImageDelegate setPostUrl:url];//[self normalizeUrl:url]];
	[self startCameraPickerFromViewController:webViewController 
								usingDelegate:pickImageDelegate 
								sourceType:UIImagePickerControllerSourceTypePhotoLibrary];
    */
}

- (void)onChooseDateTime:(DateTime*)dateTime {
    // TODO:
    /*
	dateTimePickerDelegate.dateTime = dateTime;
	[dateTimePickerDelegate setPostUrl:dateTime.url];//[self normalizeUrl:dateTime.url]];
	[self startDateTimePickerFromViewController:webViewController
								  usingDelegate:dateTimePickerDelegate];
    */
}

- (void)onShowLog {
    /*
	if (logViewController!=NULL) {
		[window addSubview:logViewController.view];
		logViewController.view.hidden = NO;
	}
     */
}

- (void)onShowLogOptions {
    /*
	if (logOptionsController!=NULL) {
		[window addSubview:logOptionsController.view];
		logOptionsController.view.hidden = NO;
	}
     */
}

- (void)onSysCall:(ParamsWrapper*)params {
	PARAMS_WRAPPER pw;
	do_syscall([params unwrap:&pw]);
	[params release];
}

- (void)onMapLocation:(NSString*)query {
	NSURL* url = [NSURL URLWithString:[@"http://maps.google.com/?" stringByAppendingString:query]];
	[[UIApplication sharedApplication] openURL:url];
}

- (void) doStartUp {
	appStarted = false;
    splashDisplayed = false;
    
    [[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleBlackTranslucent];
    //Create local server and start it
    //serverHost = [[ServerHost alloc] init];
    serverHost = [ServerHost sharedInstance];
    [serverHost create];
    
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

    [self fixFrame];
    
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
	serverHost->onRefreshView = @selector(onRefreshView);
	//serverHost->onSetViewHomeUrl = @selector(onSetViewHomeUrl:);
	serverHost->onTakePicture = @selector(onTakePicture:);
	serverHost->onChoosePicture = @selector(onChoosePicture:);
	serverHost->onChooseDateTime = @selector(onChooseDateTime:);
    serverHost->onRemoveNativeBar = @selector(onRemoveNativeBar);
	//serverHost->onSetViewOptionsUrl = @selector(onSetViewOptionsUrl:);
	serverHost->onShowPopup = @selector(onShowPopup:);
	serverHost->onVibrate = @selector(onVibrate:);
	serverHost->onPlayFile = @selector(onPlayFile:);
    serverHost->onStopPlaying = @selector(onStopPlaying);
	serverHost->onSysCall = @selector(onSysCall:);
	serverHost->onMapLocation = @selector(onMapLocation:);
	serverHost->onCreateMap = @selector(onCreateMap:);
    serverHost->onShowLog = @selector(onShowLog);
    [serverHost start];
		
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
