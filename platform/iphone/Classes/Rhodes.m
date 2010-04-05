#import "Rhodes.h"
#import "SimpleMainView.h"
#import "RhoAlert.h"
#import "ParamsWrapper.h"

#include "sync/ClientRegister.h"
#include "sync/syncthread.h"
#include "logging/RhoLogConf.h"
#include "logging/RhoLog.h"
#include "common/RhoConf.h"
#include "common/RhodesApp.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Rhodes"

void rho_geoimpl_init();

@implementation Rhodes

@synthesize window, player, mainView;

static Rhodes *instance = NULL;

+ (Rhodes*)sharedInstance {
    return instance;
}

+ (UIApplication*)application {
    return [Rhodes sharedInstance]->application;
}

- (void)fixFrameFor:(UIView*)view delta:(int)delta {
    CGRect frame = view.frame;
    frame.origin.y -= delta;
    view.frame = frame;
    
    NSArray *subviews = view.subviews;
    for (int i = 0, lim = [subviews count]; i < lim; ++i) {
        UIView *subview = [subviews objectAtIndex:i];
        [self fixFrameFor:subview delta:delta];
    }
}

- (void)fixFrame {
    UIApplication *app = [UIApplication sharedApplication];
    if (!app.statusBarHidden) {
        CGRect frame = [[UIScreen mainScreen] applicationFrame];
        [window setFrame:frame];
        [self fixFrameFor:window delta:10];
    }
}

- (void)setStatusBarHidden:(BOOL)hidden {
    UIApplication *app = [Rhodes application];
    [app setStatusBarHidden:hidden];
    CGRect frame = [[UIScreen mainScreen] applicationFrame];
    if (!hidden) {
        frame.origin.y -= 10;
        frame.size.height += 10;
    }
    [window setFrame:frame];
}

- (void)runRunnable:(NSArray*)args {
    id runnable = [args objectAtIndex:0];
    if ([runnable respondsToSelector:@selector(run)])
        [runnable performSelector:@selector(run)];
    else if ([runnable respondsToSelector:@selector(run:)]) {
        id arg = nil;
        if ([args count] == 2)
            arg = [args objectAtIndex:1];
        [runnable performSelector:@selector(run:) withObject:arg];
    }
    else if ([runnable respondsToSelector:@selector(run::)]) {
        id arg1 = nil;
        if ([args count] > 1)
            arg1 = [args objectAtIndex:1];
        id arg2 = nil;
        if ([args count] > 2)
            arg2 = [args objectAtIndex:2];
        [runnable performSelector:@selector(run::) withObject:arg1 withObject:arg2];
    }
    else {
        RAWLOG_ERROR("runRunnable: unrecognized parameters");
    }
    [args release];
}

+ (void)performOnUiThread:(id)runnable wait:(BOOL)wait {
    NSMutableArray *args = [NSMutableArray arrayWithCapacity:1];
    [args addObject:runnable];
    [[Rhodes sharedInstance] performSelectorOnMainThread:@selector(runRunnable:) withObject:args waitUntilDone:wait];
}

+ (void)performOnUiThread:(id)runnable arg:(id)arg wait:(BOOL)wait {
    NSMutableArray *args = [NSMutableArray arrayWithCapacity:2];
    [args addObject:runnable];
    if (arg)
        [args addObject:arg];
    [[Rhodes sharedInstance] performSelectorOnMainThread:@selector(runRunnable:) withObject:args waitUntilDone:wait];
}

+ (void)performOnUiThread:(id)runnable arg:(id)arg1 arg:(id)arg2 wait:(BOOL)wait {
    NSMutableArray *args = [NSMutableArray arrayWithCapacity:3];
    [args addObject:runnable];
    if (arg1)
        [args addObject:arg1];
    if (arg2)
        [args addObject:arg2];
    [[Rhodes sharedInstance] performSelectorOnMainThread:@selector(runRunnable:) withObject:args waitUntilDone:wait];
}

- (void)openMapLocation:(NSString*)query {
    NSURL* url = [NSURL URLWithString:[@"http://maps.google.com/?" stringByAppendingString:query]];
	[[UIApplication sharedApplication] openURL:url];
}

- (void)mapLocation:(NSString *)query {
    [self performSelectorOnMainThread:@selector(openMapLocation:) withObject:query waitUntilDone:NO];
}

- (void)doSysCall:(ParamsWrapper*)params {
    PARAMS_WRAPPER pw;
	do_syscall([params unwrap:&pw]);
	[params release];
}

- (void)sysCall:(PARAMS_WRAPPER*)params {
    ParamsWrapper* pw = [ParamsWrapper wrap:params]; 	
    [self performSelectorOnMainThread:@selector(doSysCall:) withObject:pw waitUntilDone:NO];
}

- (void)doShowLogOptions {
    if (logOptionsController!=NULL) {
		[window addSubview:logOptionsController.view];
		logOptionsController.view.hidden = NO;
	}
}

- (void)showLogOptions {
    [self performSelectorOnMainThread:@selector(doShowLogOptions) withObject:nil waitUntilDone:NO];
}

- (void)doShowLog {
    if (logViewController!=NULL) {
		[window addSubview:logViewController.view];
		logViewController.view.hidden = NO;
	}
}

- (void)showLog {
    [self performSelectorOnMainThread:@selector(doShowLog) withObject:nil waitUntilDone:NO];
}

- (void)chooseDateTime:(DateTime*)dateTime {
    dateTimePickerDelegate.dateTime = dateTime;
	[dateTimePickerDelegate setPostUrl:dateTime.url];
    //[self normalizeUrl:dateTime.url]];
    @try {
		[dateTimePickerDelegate createPicker:window];
	} @catch (NSException* theException) {
		RAWLOG_ERROR2("startDateTimePickerFromViewController failed(%s): %s", [[theException name] UTF8String], [[theException reason] UTF8String] );
	}
}

-(BOOL)startCameraPicker:(id<UINavigationControllerDelegate, UIImagePickerControllerDelegate>)delegateObject 
                                sourceType:(UIImagePickerControllerSourceType)type
{ 
#if !defined __IPHONE_3_0
	if ( (![UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypePhotoLibrary]) || 
        (![UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera]) ||
        (delegateObject == nil)) {
		return NO; 
	}
#endif
	
	@try {
        [self setStatusBarHidden:YES];
		UIImagePickerController* picker = [[UIImagePickerController alloc] init]; 
		picker.sourceType = type;
		picker.delegate = delegateObject; 
		picker.allowsImageEditing = YES;
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

- (void)takePicture:(NSString*) url {
    [pickImageDelegate setPostUrl:url];
    //[self normalizeUrl:url]];
    [self startCameraPicker:pickImageDelegate 
                 sourceType:UIImagePickerControllerSourceTypeCamera];
}

- (void)choosePicture:(NSString*) url {
    [pickImageDelegate setPostUrl:url];
    //[self normalizeUrl:url]];
    [self startCameraPicker:pickImageDelegate 
                 sourceType:UIImagePickerControllerSourceTypePhotoLibrary];
}

- (UIWindow*)rootWindow {
    return window;
}

- (void)setMainView:(id<RhoMainView,NSObject>)view {
    if (mainView == view)
        return;
    [mainView.view removeFromSuperview];
    [mainView release];
    mainView = [view retain];
    [window addSubview:mainView.view];
}

- (void) showLoadingPage 
{
    NSFileManager *fileManager = [NSFileManager defaultManager];
    
    NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
    NSString *htmPath = [NSString stringWithFormat:@"%@/apps/app/loading.html", resourcePath];
    
    if ([fileManager fileExistsAtPath:htmPath]) {
        NSError *err;
        NSString *data = [NSString stringWithContentsOfFile:htmPath encoding:NSUTF8StringEncoding error:&err];
        [mainView loadHTMLString:data];
    }
}

- (void)doStartUp {
    instance = self;
    application = [UIApplication sharedApplication];
    
    appManager = [AppManager instance]; 
	//Configure AppManager
	[appManager configure];
    
    const char *szRootPath = rho_native_rhopath();
    rho_logconf_Init(szRootPath);
    rho_rhodesapp_create(szRootPath);
    
    //[[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleBlackTranslucent animated:YES];
    //[[UIApplication sharedApplication] setStatusBarHidden:YES];
    
    window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] applicationFrame]];
    window.contentMode = UIViewContentModeScaleToFill;
    window.autoresizesSubviews = YES;
    [window makeKeyAndVisible];
    
    [self fixFrame];
    
    mainView = nil;
    self.mainView = [[SimpleMainView alloc] initWithParentView:window];
    [self showLoadingPage];
    
    // Init controllers
    logOptionsController = [[LogOptionsController alloc] init];
    logViewController = [[LogViewController alloc] init];
    
    dateTimePickerDelegate = [[DateTimePickerDelegate alloc] init];
    pickImageDelegate = [[PickImageDelegate alloc] init];
    
    rho_geoimpl_init();
    rho_rhodesapp_start();
    
#ifdef __IPHONE_3_0
    [[UIApplication sharedApplication] registerForRemoteNotificationTypes:
            (UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeAlert | UIRemoteNotificationTypeSound)];
#endif
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
	
	{
	    NSString* strData = [userInfo description];
	    const char* szData = [strData cStringUsingEncoding:[NSString defaultCStringEncoding]];
	    if ( rho_rhodesapp_callPushCallback(szData) )
	        return;
    }
	    
	NSDictionary *aps = [userInfo objectForKey:@"aps"];
	if (aps) {
		NSString *alert = [aps objectForKey:@"alert"];
		if (alert && [alert length] > 0) {
			NSLog(@"Push Alert: %@", alert);
            [RhoAlert showPopup:alert];
		}
		NSString *sound = [aps objectForKey:@"sound"];
		if (sound && [sound length] > 0) {
			NSLog(@"Sound file name: %@", sound);
            [RhoAlert playFile:[@"/public/alerts/" stringByAppendingPathComponent:sound] mediaType:NULL];
		}
		NSString *vibrate = [aps objectForKey:@"vibrate"];
		if (vibrate && [vibrate length] > 0) {
			NSLog(@"Do vibrate...");
            [RhoAlert vibrate:1];
		}
	}
	[self processDoSync:userInfo];
}
#endif

- (void) saveLastUsedTime {
    /*
    int now = [[NSDate date] timeIntervalSince1970];
    rho_conf_setInt("last_time_used",now);
    rho_conf_save();
    */
}

- (void)playStart:(NSString *)fileName mediaType:(NSString *)type {
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
	AVAudioPlayer *newPlayer = [AVAudioPlayer alloc];
	[newPlayer initWithContentsOfURL:fileURL error:&err];
	NSLog(@"Init media player returns: %@", err);
	
	[fileURL release];
	self.player = newPlayer;
	[newPlayer release];
	
	[player prepareToPlay];	
	[player setDelegate: self];
	[self.player play];
}

- (void)playStop {
    if (!self.player)
        return;
    
    [self.player stop];
    self.player = nil;
}

// AVAudioPlayerDelegate implementation

- (void) audioPlayerDidFinishPlaying: (AVAudioPlayer *) player successfully: (BOOL) flag {
	if (flag == YES) {
		NSLog(@"Audio player finished playing...");
	}	
}

- (void)audioPlayerDecodeErrorDidOccur:(AVAudioPlayer *)player error:(NSError *)error {
	NSLog(@"Audio player decoding error %@", error);
}

// UIApplicationDelegate implementation

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
#if 0
    if (appStarted) {
        //rho_rhodesapp_callAppActiveCallback();
        /*
        RhoDelegate* callback = [[RhoDelegate alloc] init];
        char* callbackUrl = rho_conf_getString("app_did_become_active_callback");
        if (callbackUrl && strlen(callbackUrl) > 0) {
            callback.postUrl = [self normalizeUrl:[NSString stringWithCString:callbackUrl
                            encoding:[NSString defaultCStringEncoding]]];
            [callback doCallback:@""];
        }
        [callback release];
        */
    }
#endif // #if 0
}

- (void)applicationWillResignActive:(UIApplication *)application {
	RAWLOG_INFO("Runner will resign active");
	[self saveLastUsedTime];
}

- (void)applicationWillTerminate:(UIApplication *)application {
    RAWLOG_INFO("Runner will terminate");
    [self saveLastUsedTime];
    rho_rhodesapp_destroy();
}

// UIWebViewDelegate imlementation

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request
 navigationType:(UIWebViewNavigationType)navigationType {
    return YES;
}

- (void)webViewDidStartLoad:(UIWebView *)webview {
	// TODO
    //[self active];
}

- (void)webViewDidFinishLoad:(UIWebView *)webview {
    [webview stringByEvaluatingJavaScriptFromString:@"document.documentElement.style.webkitTouchCallout = \"none\";"];
	// TODO
    /*
     [self inactive];
     
     if ([webView canGoBack]) {
     backBtn.enabled = YES;
     } else {
     backBtn.enabled = NO;
     }
     if ([webView canGoForward]) {
     forwardBtn.enabled = YES;
     } else {
     forwardBtn.enabled = NO;
     }
     
     //NSString* location = [webview stringByEvaluatingJavaScriptFromString:@"location.href"];
     //rho_rhodesapp_keeplastvisitedurl( [location cStringUsingEncoding:[NSString defaultCStringEncoding]] );									 
     
     if ([actionTarget respondsToSelector:@selector(hideSplash)])
     [actionTarget performSelectorOnMainThread:@selector(hideSplash) withObject:nil waitUntilDone:NO];
     */
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error {
    // TODO
}

// UITabBarControllerDelegate implementation

- (void)tabBarController:(UITabBarController *)tabBarController
 didSelectViewController:(UIViewController *)viewController {
    SEL sel = @selector(onSwitchTab);
    if ([mainView respondsToSelector:sel])
        [mainView performSelector:sel];
}

@end

void* rho_nativethread_start()
{
	return 0;//[[NSAutoreleasePool alloc] init];
}

void rho_nativethread_end(void* pData)
{
    //NSAutoreleasePool *pool = (NSAutoreleasePool *)pData;	
    //[pool release];	
}

void rho_map_location(char* query) {
    [[Rhodes sharedInstance] mapLocation:[NSString stringWithUTF8String:query]];
}

void _rho_ext_syscall(PARAMS_WRAPPER* params) {
	[[Rhodes sharedInstance] sysCall:params];
}

void rho_conf_show_log() {
    [[Rhodes sharedInstance] showLog];
}