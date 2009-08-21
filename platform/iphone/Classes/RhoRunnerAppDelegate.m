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
#import "AppManager.h"
#import "common/RhoConf.h"
#import "logging/RhoLog.h"
#include "sync/ClientRegister.h"
#import "ParamsWrapper.h"
#import "DateTime.h"
#import "RhoDelegate.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "RhoRunnerAppDelegate"

@implementation RhoRunnerAppDelegate

@synthesize window;
@synthesize webViewController;
@synthesize player; 

- (NSString*)normalizeUrl:(NSString*)url {
	if([url hasPrefix:@"http://"]) {
		return url;
	}
	NSString* location = [@"http://localhost:8080" stringByAppendingString:[@"/" stringByAppendingPathComponent:url]];
	return location;
}

- (void)onServerStarted:(NSString*)data {
	RAWLOG_INFO("Server Started notification is recived");
	NSString* location = NULL;
	
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
	}
	
	// Load the tab or nav bar
	
	UITabBarController *tabBarController = nil;
	tabBarController = [[UITabBarController alloc] initWithNibName:nil bundle:nil];
	tabBarController.moreNavigationController.navigationBar.barStyle = UIBarStyleBlackOpaque;
	NSMutableArray *tabs = [[[NSMutableArray alloc] init] autorelease];
	
	for(int i=0; i < 10; i++) {
		UIViewController *subController = [[[WebViewController alloc] initWithNibName:nil bundle:nil] autorelease];
		UIWebView *wView = [[[UIWebView alloc] init] autorelease];
		subController.title = [NSString stringWithFormat:@"tab-%i", i];
		subController.view = wView;
		[tabs addObject:subController];
	}
	
	tabBarController.viewControllers = tabs;
	tabBarController.customizableViewControllers = nil;
	[window addSubview:tabBarController.view];
	tabBarController.view.hidden = NO;

	NSString* escapedUrl = [location stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]; 
	escapedUrl = [escapedUrl stringByReplacingOccurrencesOfString: @"&" withString: @"%26"];
	NSString* redirector = [@"http://localhost:8080/system/redirect_to?url=" stringByAppendingString:escapedUrl];
	UIWebView* currentWebView = (UIWebView*)[[tabBarController.viewControllers objectAtIndex:0] view];

	[currentWebView loadRequest:[NSURLRequest requestWithURL: [NSURL URLWithString:redirector]]];
	appStarted = true;
}

- (void)onRefreshView {
	[webViewController refresh];
}

- (void)onNavigateTo:(NSString *)url {
	[webViewController navigateRedirect:url];
}

- (void)onExecuteJs:(NSString *)js {
	[webViewController executeJs:js];
}

- (void)onSetViewHomeUrl:(NSString *)url {
	//[webViewController setViewHomeUrl:[localhost stringByAppendingPathComponent:url]];
	[webViewController setViewHomeUrl:url];
}

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

- (void)onTakePicture:(NSString*) url {
	[pickImageDelegate setPostUrl:[self normalizeUrl:url]];
	[self startCameraPickerFromViewController:webViewController 
								usingDelegate:pickImageDelegate 
								sourceType:UIImagePickerControllerSourceTypeCamera];
}

- (void)onChoosePicture:(NSString*) url {
	[pickImageDelegate setPostUrl:[self normalizeUrl:url]];
	[self startCameraPickerFromViewController:webViewController 
								usingDelegate:pickImageDelegate 
								sourceType:UIImagePickerControllerSourceTypePhotoLibrary];
}

- (void)onChooseDateTime:(DateTime*)dateTime {
	dateTimePickerDelegate.dateTime = dateTime;
	[dateTimePickerDelegate setPostUrl:[self normalizeUrl:dateTime.url]];
	[self startDateTimePickerFromViewController:webViewController
								  usingDelegate:dateTimePickerDelegate];
}

- (void)onSetViewOptionsUrl:(NSString *)url {
	//[webViewController setViewOptionsUrl:[localhost stringByAppendingPathComponent:url]];
	[webViewController setViewOptionsUrl:url];
}

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
				[serverHost  doSyncFor:[url stringByTrimmingCharactersInSet:
										[NSCharacterSet characterSetWithCharactersInString:@" \t\r\n"]]];
			}
		}
		
		if (sync_all) {
			[serverHost doSync];
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

- (void) doStartUp {
	//
	appStarted = false;
	// Log View
	logViewController = [[LogViewController alloc] init];
	logViewController->actionTarget = self;
	logViewController->onShowLogOptions = @selector(onShowLogOptions);
	
	logOptionsController = [[LogOptionsController alloc] init];
	
	webViewController->actionTarget = self;
	webViewController->onShowLog = @selector(onShowLog);
	
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
	serverHost->onSetViewHomeUrl = @selector(onSetViewHomeUrl:);
	serverHost->onTakePicture = @selector(onTakePicture:);
	serverHost->onChoosePicture = @selector(onChoosePicture:);
	serverHost->onChooseDateTime = @selector(onChooseDateTime:);
	serverHost->onSetViewOptionsUrl = @selector(onSetViewOptionsUrl:);
	serverHost->onShowPopup = @selector(onShowPopup:);
	serverHost->onVibrate = @selector(onVibrate:);
	serverHost->onPlayFile = @selector(onPlayFile:);
	serverHost->onSysCall = @selector(onSysCall:);
    [serverHost start];
	
    //Create View
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
	// Hide the toolbar initially, we will re-draw it if there are no tabs
	[webViewController.toolbar removeFromSuperview];
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
		RhoDelegate* callback = [[RhoDelegate alloc] init];
		char* callbackUrl = rho_conf_getString("app_did_become_active_callback");
		if (callbackUrl && strlen(callbackUrl) > 0) {
			callback.postUrl = [self normalizeUrl:[NSString stringWithCString:callbackUrl
								  encoding:[NSString defaultCStringEncoding]]];
			[callback doCallback:@""];
		}
		[callback release];
	}
}


- (void) saveLastUsedTime {
	int now = [[NSDate date] timeIntervalSince1970];
	rho_conf_setInt("last_time_used",now);
	rho_conf_save();
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
	[super dealloc];
}

@end
