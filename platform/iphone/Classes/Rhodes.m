#import "Rhodes.h"
#import "SimpleMainView.h"
#import "Alert.h"

#include "sync/ClientRegister.h"
#include "sync/syncthread.h"
#include "logging/RhoLogConf.h"
#include "logging/RhoLog.h"
#include "common/RhodesApp.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Rhodes"

@implementation Rhodes

@synthesize window, player, mainView;

static Rhodes *instance = NULL;

+ (Rhodes*)sharedInstance {
    return instance;
}

+ (UIApplication*)application {
    return [Rhodes sharedInstance]->application;
}

- (void)runRunnable:(id)runnable {
    if ([runnable conformsToProtocol:@protocol(RhoRunnable)])
        [runnable run];
    [runnable release];
}

+ (void)performOnUiThread:(id)runnable wait:(BOOL)wait {
    [runnable retain];
    [[Rhodes sharedInstance] performSelectorOnMainThread:@selector(runRunnable:) withObject:runnable waitUntilDone:wait];
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
    
    //[[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleBlackTranslucent animated:YES];
    //[[UIApplication sharedApplication] setStatusBarHidden:YES];
    
    //CGRect sbFrame = [[UIApplication sharedApplication] statusBarFrame];
    //int newY = sbFrame.size.height;
    int newY = 10;
    
    CGRect frame = [[UIScreen mainScreen] applicationFrame];
    frame.size.height += frame.origin.y - newY;
    frame.origin.y = newY;
    
    window = [[UIWindow alloc] initWithFrame:frame];
    window.contentMode = UIViewContentModeScaleToFill;
    window.autoresizesSubviews = YES;
    [window makeKeyAndVisible];
    
    const char *szRootPath = rho_native_rhopath();
    rho_logconf_Init(szRootPath);
    rho_rhodesapp_create(szRootPath);
    
    mainView = nil;
    self.mainView = [[SimpleMainView alloc] initWithParentView:window];
    [self showLoadingPage];
    
    appManager = [AppManager instance]; 
	//Configure AppManager
	[appManager configure];
    
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
	NSDictionary *aps = [userInfo objectForKey:@"aps"];
	if (aps) {
		NSString *alert = [aps objectForKey:@"alert"];
		if (alert && [alert length] > 0) {
			NSLog(@"Push Alert: %@", alert);
            [Alert showPopup:alert];
		}
		NSString *sound = [aps objectForKey:@"sound"];
		if (sound && [sound length] > 0) {
			NSLog(@"Sound file name: %@", sound);
            [Alert playFile:[@"/public/alerts/" stringByAppendingPathComponent:sound] mediaType:NULL];
		}
		NSString *vibrate = [aps objectForKey:@"vibrate"];
		if (vibrate && [vibrate length] > 0) {
			NSLog(@"Do vibrate...");
            [Alert vibrate:1];
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
