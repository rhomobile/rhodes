//
//  BrowserAppDelegate.m
//  Browser
//
//  Created by adam blum on 9/4/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//
#import <AudioToolbox/AudioToolbox.h>

#import "defs.h"
#import "RhoRunnerAppDelegate.h"
#import "WebViewController.h"
//#import "AppManager.h"
//#import "common/RhoConf.h"
#import "logging/RhoLog.h"
#include "sync/ClientRegister.h"
#include "sync/syncthread.h"
#include "common/RhodesApp.h"
#import "ParamsWrapper.h"
#import "DateTime.h"
#import "NativeBar.h"
#import "BarItem.h"
#import "WebViewUrl.h"
#import "RhoDelegate.h"
#ifdef __IPHONE_3_0
#import "MapViewController.h"
#endif

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoRunnerAppDelegate"

@implementation RhoRunnerAppDelegate

@synthesize window;
@synthesize webViewController;
@synthesize player; 
@synthesize nativeBar;
/*
- (NSString*)normalizeUrl:(NSString*)url 
{
//	if([url hasPrefix:@"http://"]) {
//		return url;
//	}
//	NSString* location = [@"http://localhost:8080" stringByAppendingString:[@"/" stringByAppendingPathComponent:url]];
//	return location;
	char* szNormUrl = rho_http_normalizeurl([url cStringUsingEncoding:[NSString defaultCStringEncoding]]);
	NSString* strRes = [NSString stringWithCString:szNormUrl encoding:[NSString defaultCStringEncoding]];
	rho_http_free(szNormUrl);
	
	return strRes;
}*/

- (void)loadStartPath:(NSString*)location {
	if (nativeBar.barType == TOOLBAR_TYPE || nativeBar.barType == NOBAR_TYPE) {
		[webViewController navigateRedirect:location];
	} else {
		// Load tab #0 on app load
        BarItem *item = (BarItem*)[tabBarDelegate.barItems objectAtIndex:0];
		[tabBarDelegate loadTabBarItemFirstPage:item];
	}	
}

- (void)onServerStarted:(NSString*)data {
	RAWLOG_INFO("Server Started notification is recived");
	NSString* location = [NSString stringWithCString:rho_rhodesapp_getfirststarturl() encoding:[NSString defaultCStringEncoding]];
	/*
	//try to restore previous location
	if ( rho_conf_getBool("KeepTrackOfLastVisitedPage") ) {
		char* lastVisitedPage = rho_conf_getString("LastVisitedPage");
		if (lastVisitedPage && strlen(lastVisitedPage)>0) {
			location = [NSString stringWithCString:lastVisitedPage
										  encoding:[NSString defaultCStringEncoding]];
		}
		rho_conf_freeString(lastVisitedPage);
	} 
	
	//if there is no previous location navigate to the default start page 
	if (!location) {
		location = [self normalizeUrl:(NSString*)data];
	}*/
	
	appStarted = true;
	[self loadStartPath:location];
}

- (void)onRefreshView:(int)index {
    if (self.nativeBar.barType == TABBAR_TYPE) {
        BarItem *item = (BarItem*)[tabBarDelegate.barItems objectAtIndex:index];
        [tabBarDelegate refresh:item];
    }
    else {
        [webViewController refresh];
    }
}

- (void)onNavigateTo:(WebViewUrl*) wvUrl {
	if (self.nativeBar.barType == TABBAR_TYPE) {
        int size = [tabBarDelegate.barItems count];
        if (wvUrl.webViewIndex >= size) {
            RAWLOG_ERROR2("Web view index out of bound: %d (%d tabs)", wvUrl.webViewIndex, size);
            return;
        }
		BarItem* bItem = (BarItem*)[tabBarDelegate.barItems objectAtIndex:wvUrl.webViewIndex];
        [tabBarDelegate loadTabBarItemLocation:bItem url:wvUrl.url];
	} else {
		[webViewController navigateRedirect:wvUrl.url];
	}
}

- (void)onExecuteJs:(JSString *)js {
    if (self.nativeBar.barType == TABBAR_TYPE) {
        BarItem *item = (BarItem *)[tabBarDelegate.barItems objectAtIndex:js->index];
        [tabBarDelegate executeJs:item js:js];
    }
    else {
        [webViewController executeJs:js];
    }
}
/*
- (void)onSetViewHomeUrl:(NSString *)url {
	[webViewController setViewHomeUrl:url];
}*/

#ifdef __IPHONE_3_0
-(void) onCreateMap:(NSMutableArray*)items {
	MapViewController* map = [[MapViewController alloc] init];
	[map setParams:[items objectAtIndex:0]];
	[map setAnnotations:[items objectAtIndex:1]];
	map.actionTarget = self;
	map.onNavigate = @selector(onNavigateTo:);
	[window addSubview:map.view];
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
		UIImagePickerController* picker = [[UIImagePickerController alloc] init]; 
		picker.sourceType = type;
		picker.delegate = delegateObject; 
		picker.allowsImageEditing = YES; 
		if ( type == UIImagePickerControllerSourceTypePhotoLibrary ) {
			[window addSubview:picker.view];
		} else {
			[controller presentModalViewController:picker animated:YES]; 
		}
	} @catch(NSException* theException) {
		RAWLOG_ERROR2("startCameraPickerFromViewController failed(%s): %s", [[theException name] UTF8String], [[theException reason] UTF8String] );
		//NSLog(@"%@", theException);
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

-(BOOL)startNativeBarFromViewController:(UIViewController*)controller 
							   usingDelegate:(TabBarDelegate*)delegateObject
{
	@try {
		[delegateObject createTabBar:window];
	} @catch (NSException* theException) {
		RAWLOG_ERROR2("startNativeBarFromViewController failed(%s): %s", [[theException name] UTF8String], [[theException reason] UTF8String] );
		return NO;
	}
	return YES;
} 

- (void)onTakePicture:(NSString*) url {
	[pickImageDelegate setPostUrl:url];//[self normalizeUrl:url]];
	[self startCameraPickerFromViewController:webViewController 
								usingDelegate:pickImageDelegate 
								sourceType:UIImagePickerControllerSourceTypeCamera];
}

- (void)onChoosePicture:(NSString*) url {
	[pickImageDelegate setPostUrl:url];//[self normalizeUrl:url]];
	[self startCameraPickerFromViewController:webViewController 
								usingDelegate:pickImageDelegate 
								sourceType:UIImagePickerControllerSourceTypePhotoLibrary];
}

- (void)onChooseDateTime:(DateTime*)dateTime {
	dateTimePickerDelegate.dateTime = dateTime;
	[dateTimePickerDelegate setPostUrl:dateTime.url];//[self normalizeUrl:dateTime.url]];
	[self startDateTimePickerFromViewController:webViewController
								  usingDelegate:dateTimePickerDelegate];
}

- (void)onRemoveNativeBar {
    if (self.nativeBar == nil)
        return;
    
    if (self.nativeBar.barType == TABBAR_TYPE) {
        [tabBarDelegate deleteTabBar];
    }
    else {
        [webViewController showToolbar:NO];
    }
    self.nativeBar = nil;
}

- (void)onCreateNativeBar:(NativeBar*)bar {
    if (self.nativeBar != nil) {
        RAWLOG_INFO("Native bar already exists, remove it");
        [self onRemoveNativeBar];
    }
	
    // retain the nativebar so it doesn't get deleted
	[bar retain];
    int type = bar.barType;
    if (type == TABBAR_TYPE) {
        tabBarDelegate.tabBar = bar;
        [self startNativeBarFromViewController:webViewController usingDelegate:tabBarDelegate];
	}
    else if (type == TOOLBAR_TYPE) {
        [webViewController showToolbar:YES];
	}
    else if (type == NOBAR_TYPE) {
        [webViewController showToolbar:NO];
	}
    self.nativeBar = bar;
}

- (void)onSwitchTab:(NSValue*)value {
    int* pIndex = value.pointerValue;
    if (self.nativeBar == nil)
        return;
    
    if (self.nativeBar.barType == TABBAR_TYPE) {
        [tabBarDelegate switchTab:*pIndex];
    }
}

/*
- (void)onSetViewOptionsUrl:(NSString *)url {
	[webViewController setViewOptionsUrl:url];
}*/

- (void)onShowLog {
	if (logViewController!=NULL) {
		[window addSubview:logViewController.view];
		logViewController.view.hidden = NO;
	}
}

- (void)onShowLogOptions {
	if (logOptionsController!=NULL) {
		[window addSubview:logOptionsController.view];
		logOptionsController.view.hidden = NO;
	}
}

- (void)onShowPopup:(NSString *)message {
	UIAlertView *alert = [[UIAlertView alloc]
							   initWithTitle: @"Alert"
							   message: message
							   delegate:nil
							   cancelButtonTitle:@"OK"
							   otherButtonTitles:nil];
    [alert show];
    [alert release];
}

- (void)onVibrate:(int)duration {
	AudioServicesPlaySystemSound (kSystemSoundID_Vibrate);
}

- (void)onPlayFile:(NSString *)fileName {
	NSString *soundFilePath;
	//hack to work around iphone limitation when it will play push alerts only from the main bundle root
	if ([fileName hasPrefix:@"/public/alerts/"] || [fileName hasPrefix:@"/apps/public/alerts/"]) {
		NSString *file = [fileName lastPathComponent];
		soundFilePath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:file];		
	} else {
		soundFilePath = [[AppManager getApplicationsRootPath] stringByAppendingPathComponent:fileName];
	}
	NSLog(@"Playing %@: ", soundFilePath);
	
	NSURL *fileURL = [[NSURL alloc] initFileURLWithPath: soundFilePath];
	NSError* err = nil;
	AVAudioPlayer *newPlayer =
		[[AVAudioPlayer alloc] initWithContentsOfURL: fileURL error: &err];
	NSLog(@"Init media player returns: %@", err);
	
	[fileURL release];
	self.player = newPlayer;
	[newPlayer release];
	
	[player prepareToPlay];	
	[player setDelegate: self];
	[self.player play];
}

- (void) audioPlayerDidFinishPlaying: (AVAudioPlayer *) player successfully: (BOOL) flag {
	if (flag == YES) {
		//[self.button setTitle: @"Play" forState: UIControlStateNormal];
		NSLog(@"Audio player finished playing...");
	}	
}

- (void)audioPlayerDecodeErrorDidOccur:(AVAudioPlayer *)player error:(NSError *)error {
	NSLog(@"Audio player decoding error %@", error);
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

- (void)onActiveTab:(NSValue*)val {
	//TODO: This is a bit weird, but saves us creating another wrapper class
	int* res = val.pointerValue;
	if (tabBarDelegate && tabBarDelegate.tabBar) {
		*res = tabBarDelegate.activeTab;
	} else {
		*res = 0;
	}
}

#ifdef __IPHONE_3_0
- (void)processDoSync:(NSDictionary *)userInfo
{
	NSArray *do_sync = [userInfo objectForKey:@"do_sync"];
	if (do_sync) {
		NSEnumerator *enumerator = [do_sync objectEnumerator];
		id url;
		
		NSLog(@"do_sync array: ");
		bool sync_all = false;
		while ( url = [enumerator nextObject] ) {
			NSLog( @"url = %@", url );
			if ([@"all" caseInsensitiveCompare:url] == NSOrderedSame) {
				sync_all = true;
			} else {
				//do sync of individual source
				NSString* srcUrl = [url stringByTrimmingCharactersInSet: [NSCharacterSet characterSetWithCharactersInString:@" \t\r\n"]];
				rho_sync_doSyncSourceByUrl([srcUrl cStringUsingEncoding:[NSString defaultCStringEncoding]]);
			}
		}
		
		if (sync_all) {
			rho_sync_doSyncAllSources(TRUE);
		}
	}
}
- (void)processPushMessage:(NSDictionary *)userInfo
{
	RAWLOG_INFO("Processing PUSH message...");
	NSDictionary *aps = [userInfo objectForKey:@"aps"];
	if (aps) {
		NSString *alert = [aps objectForKey:@"alert"];
		if (alert && [alert length] > 0) {
			NSLog(@"Push Alert: %@", alert);
			[self onShowPopup:alert];
		}
		NSString *sound = [aps objectForKey:@"sound"];
		if (sound && [sound length] > 0) {
			NSLog(@"Sound file name: %@", sound);
			[self onPlayFile:[@"/public/alerts/" stringByAppendingPathComponent:sound]];
		}
		NSString *vibrate = [aps objectForKey:@"vibrate"];
		if (vibrate && [vibrate length] > 0) {
			NSLog(@"Do vibrate...");
			[self onVibrate:1];
		}
	}
	[self processDoSync:userInfo];
}
#endif

- (void) showLoadingPage 
{
	NSString *filePath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:@"apps/app/loading.html"];
//	NSString *filePath = [NSString stringWithCString:rho_rhodesapp_getloadingpagepath() encoding:[NSString defaultCStringEncoding]];
	NSFileManager *fileManager = [NSFileManager defaultManager];
	if ([fileManager fileExistsAtPath:filePath]) {
		NSData *data = [fileManager contentsAtPath:filePath];
		NSString *loadingPage = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
		[webViewController loadHTMLString:loadingPage];
	}
}

- (void) doStartUp {
	appStarted = false;
	// Log View
	logViewController = [[LogViewController alloc] init];
	logViewController->actionTarget = self;
	logViewController->onShowLogOptions = @selector(onShowLogOptions);
	
	logOptionsController = [[LogOptionsController alloc] init];
	
    webViewController->window = window;
	webViewController->actionTarget = self;
	webViewController->onShowLog = @selector(onShowLog);
	
	//TabBar delegate
	tabBarDelegate = [[TabBarDelegate alloc] init];
	
	[self showLoadingPage];
	
	//Camera delegate
	pickImageDelegate = [[PickImageDelegate alloc] init];
	
	//DateTime delegate
	dateTimePickerDelegate = [[DateTimePickerDelegate alloc] init];
	
    //Create local server and start it
    //serverHost = [[ServerHost alloc] init];
	serverHost = [ServerHost sharedInstance];
	serverHost->actionTarget = self;
	serverHost->onStartSuccess = @selector(onServerStarted:);
	serverHost->onRefreshView = @selector(onRefreshView);
	serverHost->onNavigateTo = @selector(onNavigateTo:);
	serverHost->onExecuteJs = @selector(onExecuteJs:);
	//serverHost->onSetViewHomeUrl = @selector(onSetViewHomeUrl:);
	serverHost->onTakePicture = @selector(onTakePicture:);
	serverHost->onChoosePicture = @selector(onChoosePicture:);
	serverHost->onChooseDateTime = @selector(onChooseDateTime:);
	serverHost->onCreateNativeBar = @selector(onCreateNativeBar:);
    serverHost->onRemoveNativeBar = @selector(onRemoveNativeBar);
    serverHost->onSwitchTab = @selector(onSwitchTab:);
	//serverHost->onSetViewOptionsUrl = @selector(onSetViewOptionsUrl:);
	serverHost->onShowPopup = @selector(onShowPopup:);
	serverHost->onVibrate = @selector(onVibrate:);
	serverHost->onPlayFile = @selector(onPlayFile:);
	serverHost->onSysCall = @selector(onSysCall:);
	serverHost->onMapLocation = @selector(onMapLocation:);
	serverHost->onCreateMap = @selector(onCreateMap:);
	serverHost->onActiveTab = @selector(onActiveTab:);
    [serverHost start];
	
	// Create View
	webViewController.toolbar.hidden = YES;
	[window addSubview:webViewController.view];
    [window makeKeyAndVisible];
	
#ifdef __IPHONE_3_0
	[[UIApplication sharedApplication] registerForRemoteNotificationTypes:(UIRemoteNotificationTypeBadge 
																		   | UIRemoteNotificationTypeAlert | UIRemoteNotificationTypeSound)];
#endif
}

#ifdef __IPHONE_3_0
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
	[self doStartUp];
	[self processDoSync:launchOptions];
	return NO;
}
#endif

- (void)applicationDidFinishLaunching:(UIApplication *)application {
	[self doStartUp];
}

#ifdef __IPHONE_3_0
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
	NSLog(@"Device token is %@", deviceToken);
	
	NSMutableString *stringBuffer = [NSMutableString stringWithCapacity:([deviceToken length] * 2)];
	const unsigned char *dataBuffer = [deviceToken bytes];
	for (int i = 0; i < [deviceToken length]; ++i)
		[stringBuffer appendFormat:@"%02x", (unsigned long)dataBuffer[ i ]];
	
	char* szpin = strdup([stringBuffer cStringUsingEncoding:[NSString defaultCStringEncoding]]);
	RAWLOG_INFO1("device pin: %s\n", szpin);

	rho_clientregister_create(szpin);
	free(szpin);
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
	NSLog(@"Push Notification Error: %@", [error localizedDescription]);
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
{
	[self processPushMessage:userInfo];
}
#endif

- (void)applicationDidBecomeActive:(UIApplication *)application {
	if (appStarted) {
	    rho_rhodesapp_callAppActiveCallback();
/*		RhoDelegate* callback = [[RhoDelegate alloc] init];
		char* callbackUrl = rho_conf_getString("app_did_become_active_callback");
		if (callbackUrl && strlen(callbackUrl) > 0) {
			callback.postUrl = [self normalizeUrl:[NSString stringWithCString:callbackUrl
								  encoding:[NSString defaultCStringEncoding]]];
			[callback doCallback:@""];
		}
		[callback release];*/
	}
}


- (void) saveLastUsedTime 
{
/*	int now = [[NSDate date] timeIntervalSince1970];
	rho_conf_setInt("last_time_used",now);
	rho_conf_save();*/
}

- (void)applicationWillResignActive:(UIApplication *)application {
	RAWLOG_INFO("Runner will resign active");
	[self saveLastUsedTime];
}

- (void)applicationWillTerminate:(UIApplication *)application {
    RAWLOG_INFO("Runner will terminate");
	[self saveLastUsedTime];
	//Stop HTTP server host 
    [serverHost stop];
}

- (void)dealloc {
    [serverHost release];
	[webViewController release];
	[window release];
	[pickImageDelegate release];
	[dateTimePickerDelegate release];
	[tabBarDelegate release];
	[nativeBar release];
	[super dealloc];
}

@end
