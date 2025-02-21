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

#import "Rhodes.h"
#import "SimpleMainView.h"
#import "RhoAlert.h"
#import "ParamsWrapper.h"

#include "sync/RhoconnectClientManager.h"
#include "logging/RhoLogConf.h"
#include "logging/RhoLog.h"
#include "common/RhoConf.h"
#include "common/RhodesApp.h"
//#include "common/app_build_configs.h"
#import "SplitView/SplittedMainView.h"

#include "common/app_build_capabilities.h"

#include "InitMemoryInfoCollector.h"
#include "Reachability.h"

#import "CRhoURLProtocol.h"
#import "RhoExtManager/RhoExtManagerSingletone.h"


#ifdef RHO_STANDALONE_LIB
#import "RhoMainViewStubImpl.h"
#endif


int rho_rhodesapp_check_mode();
void rho_splash_screen_start();


//NSString* kGCMMessageIDKey = @"gcm.message_id";

/*
use this non-public code for see level of memory warning 
 
typedef enum {
	OSMemoryNotificationLevelAny      = -1,
	OSMemoryNotificationLevelNormal   =  0,
	OSMemoryNotificationLevelWarning  =  1,
	OSMemoryNotificationLevelUrgent   =  2,
	OSMemoryNotificationLevelCritical =  3
} OSMemoryNotificationLevel;

extern OSMemoryNotificationLevel OSMemoryNotificationCurrentLevel(void);
*/


extern int rho_sys_check_rollback_bundle(const char* szRhoPath);
void rho_sys_impl_exit_with_errormessage(const char* szTitle, const char* szMsg);

extern int rho_extensions_are_loaded();

extern void rho_sysimpl_sethas_network(int value);
extern void rho_sysimpl_sethas_cellnetwork(int value);

extern void rho_rhodesapp_callScreenOnCallbackAsync();



#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "Rhodes"

void rho_geoimpl_init();

static BOOL app_created = NO;


#define BLUR_VIEW_TAG 527196


@interface RhoActivateTask : NSObject {}
+ (void)run;
@end

@implementation RhoActivateTask
+ (void)run {
    rho_rhodesapp_callAppActiveCallback(1);

    if ([[Rhodes sharedInstance] mScreenStateChanged])
    {
        rho_rhodesapp_callScreenOnCallbackAsync();
        [[Rhodes sharedInstance] setMScreenStateChanged:NO];
    }
}
@end


@interface RhoFullScreenEnableTask : NSObject {}
+ (void)run;
@end

@implementation RhoFullScreenEnableTask
+ (void)run {
#ifdef __IPHONE_3_2
    //[[Rhodes application] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationFade];
#else
    //[[Rhodes application] setStatusBarHidden:YES animated:YES];
#endif
    //[[[[Rhodes sharedInstance] mainView] view] setFrame:[Rhodes applicationFrame]];
}
@end

@interface RhoFullScreenDisableTask : NSObject {}
+ (void)run;
@end

@implementation RhoFullScreenDisableTask
+ (void)run {
#ifdef __IPHONE_3_2
    //[[Rhodes application] setStatusBarHidden:NO withAnimation:UIStatusBarAnimationFade];
#else
    //[[Rhodes application] setStatusBarHidden:NO animated:YES];
#endif
    //[[[[Rhodes sharedInstance] mainView] view] setFrame:[Rhodes applicationFrame]];
}
@end

@implementation Rhodes

@synthesize window, player, cookies, signatureDelegate, nvDelegate, mBlockExit,  mScreenStateChanged, mNetworkPollCondition;


static Rhodes *instance = NULL;

+ (Rhodes*)sharedInstance {
    return instance;
}

+(void) makeSharedInstance {
    instance = [[Rhodes alloc] init];
}


+ (UIApplication*)application {
    return [Rhodes sharedInstance]->application;
}


- (void)blurCurrentView
{
    UIView *captured = [[[[Rhodes sharedInstance] mainView] getMainViewController].view snapshotViewAfterScreenUpdates:NO];

    UIView *blurView = nil;
    if ([UIVisualEffectView class]){
        blurView        = [[UIVisualEffectView alloc]initWithEffect:[UIBlurEffect effectWithStyle:UIBlurEffectStyleRegular]];
        blurView.frame  = captured.bounds;
        [captured addSubview:blurView];
    }
    else {
        UIToolbar *toolBar  = [[UIToolbar alloc] initWithFrame:captured.bounds];
        toolBar.barStyle    = UIBarStyleBlackTranslucent;
        [captured addSubview:toolBar];
    }
    captured.tag = BLUR_VIEW_TAG;
    
    if ([[[[Rhodes sharedInstance] mainView] getMainViewController].view viewWithTag:BLUR_VIEW_TAG]){
        return;
    }
    [[[[Rhodes sharedInstance] mainView] getMainViewController].view addSubview:captured];
}

- (void)unblurCirrentView
{
    [[[[[Rhodes sharedInstance] mainView] getMainViewController].view viewWithTag:BLUR_VIEW_TAG] removeFromSuperview];
}



+ (CGRect)applicationFrame {
    CGRect frame = [[UIScreen mainScreen] applicationFrame];
    //CGRect sbFrame = [[UIApplication sharedApplication] statusBarFrame];
    //frame.origin.y += sbFrame.size.height;
    //frame.size.height -= sbFrame.size.height;
    
    //CGRect screen_bounds = [[UIScreen mainScreen] bounds];

    CGRect appFrame = [[UIScreen mainScreen] applicationFrame];
    UIDeviceOrientation orientation = [[UIApplication sharedApplication] statusBarOrientation];
    if (UIDeviceOrientationLandscapeLeft == orientation ||
        UIDeviceOrientationLandscapeRight == orientation)
    {
        if (appFrame.size.width < appFrame.size.height)
        {
            appFrame = CGRectMake(appFrame.origin.y, appFrame.origin.x, appFrame.size.height, appFrame.size.width);
        }
    }
    return appFrame;//frame;
}


#ifdef __IPHONE_4_0
- (EKEventStore*) getEventStore {
    if (eventStore == nil) {
        eventStore = [[EKEventStore alloc] init];
    }
    return eventStore;
}
#endif


+ (void)setStatusBarHidden:(BOOL)v {
	if (v) {
        [[Rhodes sharedInstance] setFullScreen:YES];
		[Rhodes performOnUiThread:[RhoFullScreenEnableTask class] wait:NO];
	}
	else {
        [[Rhodes sharedInstance] setFullScreen:NO];
		[Rhodes performOnUiThread:[RhoFullScreenDisableTask class] wait:NO];
	}
    [[[[Rhodes sharedInstance] mainView] getMainViewController] setNeedsStatusBarAppearanceUpdate];
}

- (BOOL)getFullScreen {
    return mIsFullScreen;
}
- (void)setFullScreen:(BOOL)enable {
    mIsFullScreen = enable;
}

+ (BOOL)getStatusBarHidden {
    return [[Rhodes sharedInstance] getFullScreen];
}


#if 0
+ (UIView*)subviewWithTag:(NSInteger)tag ofView:(UIView*)view {
    if (!view)
        return nil;
    
    for (int i = 0, lim = [view.subviews count]; i < lim; ++i) {
        UIView *subview = [view.subviews objectAtIndex:i];
        if (subview.tag == tag)
            return subview;
        subview = [self subviewWithTag:tag ofView:subview];
        if (subview)
            return subview;
    }
    
    return nil;
}
#endif

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

- (BOOL)isRotationLocked {
    return rotationLocked;
}

-(void) setRotationLocked:(BOOL)locked {
    rotationLocked = locked;
}

- (void)openMapLocation:(NSString*)query {
	[self hideSplash];
    NSURL* url = [NSURL URLWithString:[@"https://maps.google.com/?" stringByAppendingString:query]];
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

- (void)setCookie:(NSString*)cookie forUrl:(NSString*)url {
    NSURL *parsed = [NSURL URLWithString:url];
    
    
    NSRange range = [cookie rangeOfString:@"="];
    if (range.location > 0) {
        NSMutableDictionary* dict = [NSMutableDictionary dictionaryWithCapacity:5];
        [dict setObject:parsed.host forKey:NSHTTPCookieDomain];
        [dict setObject:parsed.path forKey:NSHTTPCookiePath];
        [dict setObject:[cookie substringToIndex:range.location] forKey:NSHTTPCookieName];
        [dict setObject:[cookie substringFromIndex:(range.location+1)] forKey:NSHTTPCookieValue];
        
        NSHTTPCookie* cook = [NSHTTPCookie cookieWithProperties:dict];
        
        [[NSHTTPCookieStorage sharedHTTPCookieStorage] setCookie:cook];
    }
    //NSString *basicUrl = [NSString stringWithFormat:@"%@://%@/%@", parsed.scheme, parsed.host, parsed.path];
    //const char* cu = [basicUrl UTF8String];
    //[cookies setObject:cookie forKey:basicUrl];
}

- (NSDictionary*)getCookies:(NSString*)url {
    NSHTTPCookieStorage* storage = [NSHTTPCookieStorage sharedHTTPCookieStorage];
    NSURL *urlObj = [NSURL URLWithString:url];
    
    NSArray<NSHTTPCookie*>* cookies = [storage cookiesForURL:urlObj];
    NSMutableDictionary* dict = [NSMutableDictionary dictionaryWithCapacity:[cookies count]];
    
    for ( NSHTTPCookie* cookie in cookies ) {
        NSString* name = [cookie name];
        NSString* val = [cookie value];
        
        NSString* sRFCCookie = [NSString stringWithFormat:@"%@=%@",name,val];
        
        [dict setObject:sRFCCookie forKey:name];
    }
    
    return dict;
}

- (BOOL)removeCookie:(NSString*)url name:(NSString*)cookieName {
    NSHTTPCookieStorage* storage = [NSHTTPCookieStorage sharedHTTPCookieStorage];
    NSURL *urlObj = [NSURL URLWithString:url];
    
    BOOL removed = false;

    NSArray<NSHTTPCookie*>* cookies = [storage cookiesForURL:urlObj];
    for ( NSHTTPCookie* cookie in cookies ) {
        if ( [[cookie name] isEqualToString:cookieName] ) {
            [storage deleteCookie:cookie];
            removed = true;
            break;
        }
    }
    
    return removed;
}

- (BOOL)removeAllCookies {
    NSHTTPCookieStorage *storage = [NSHTTPCookieStorage sharedHTTPCookieStorage];
    for (NSHTTPCookie* cookie in storage.cookies) {
        [storage deleteCookie:cookie];
    }
    
    return true;
}

//unused now
- (NSString*)cookie:(NSString*)url {
    NSURL *parsed = [NSURL URLWithString:url];
    NSString *basicUrl = [NSString stringWithFormat:@"%@://%@/%@", parsed.scheme, parsed.host, parsed.path];
    NSString *c = [cookies objectForKey:basicUrl];
    if (c == nil) {
        NSString *basicDomainUrl = [NSString stringWithFormat:@"%@://%@/%@", parsed.scheme, parsed.host, @""];
        c = [cookies objectForKey:basicDomainUrl];
    }
    return c;
}

- (void)chooseDateTime:(DateTime*)dateTime {
	[self hideSplash];
    dateTimePickerDelegate.dateTime = dateTime;
    [dateTimePickerDelegate setPostUrl:dateTime.url];
    //[self normalizeUrl:dateTime.url]];
    @try {
        [dateTimePickerDelegate createPicker:[mainView view]];
    } @catch (NSException* theException) {
        RAWLOG_ERROR2("startDateTimePickerFromViewController failed(%s): %s", [[theException name] UTF8String], [[theException reason] UTF8String] );
    }
}

-(BOOL)startCameraPicker:(PickImageDelegate*)delegateObject 
                                sourceType:(UIImagePickerControllerSourceType)type
{
	[self hideSplash];
#ifndef __IPHONE_3_0
    if ( (![UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypePhotoLibrary]) || 
        (![UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera]) ||
        (delegateObject == nil)) {
        return NO; 
    }
#endif
	
    @try {
        UIImagePickerController* picker = [[UIImagePickerController alloc] init]; 
        picker.sourceType = type;
        picker.delegate = delegateObject;
        
#ifndef __IPHONE_3_1
        if (delegateObject.settings.enable_editing != 0) {
            picker.allowsImageEditing = YES;
        }
        else {
            picker.allowsImageEditing = NO;
        }
#else
        if (delegateObject.settings.enable_editing != 0) {
            picker.allowsEditing = YES;
        }
        else {
            picker.allowsEditing = NO;
        }
#endif
      
        delegateObject.isCaptureProcessRunning = YES;
        
        // Show picker
#ifdef __IPHONE_3_2
        if ((UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) && (type != UIImagePickerControllerSourceTypeCamera)) {
            // WARNING!!! DO NOT USE UIPopoverController DIRECTLY HERE!!!
            // It will cause problem of starting application on iPhone OS < 3.2
            // All such stuff should be loaded in runtime using NSClassFromString
            id popover = [[NSClassFromString(@"UIPopoverController") alloc] initWithContentViewController:picker];
            [popover setDelegate:delegateObject];
            delegateObject->popover = popover;
            CGRect rect = [[[self mainView] view] frame];
            [popover setPopoverContentSize:CGSizeMake(320, 320)];
            [popover presentPopoverFromRect:CGRectMake(rect.origin.x + rect.size.width/2, rect.origin.y,0,-600) inView:[[self mainView] view] permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
        }
        else 
#endif
        {
            if (type == UIImagePickerControllerSourceTypeCamera) {
                if (delegateObject.settings.camera_type == CAMERA_SETTINGS_TYPE_FRONT) {
                    picker.cameraDevice = UIImagePickerControllerCameraDeviceFront;
                }
                if (delegateObject.settings.camera_type == CAMERA_SETTINGS_TYPE_MAIN) {
                    picker.cameraDevice = UIImagePickerControllerCameraDeviceRear;
                }
                if (delegateObject.settings.flash_mode == CAMERA_SETTINGS_FLASH_AUTO) {
                    picker.cameraFlashMode = UIImagePickerControllerCameraFlashModeAuto;
                }
                if (delegateObject.settings.flash_mode == CAMERA_SETTINGS_FLASH_OFF) {
                    picker.cameraFlashMode = UIImagePickerControllerCameraFlashModeOff;
                }
                if (delegateObject.settings.flash_mode == CAMERA_SETTINGS_FLASH_ON) {
                    picker.cameraFlashMode = UIImagePickerControllerCameraFlashModeOn;
                }
            }
            
            //[window addSubview:picker.view];
            [[mainView getMainViewController] presentModalViewController:picker animated:NO];
        }
    } @catch(NSException* theException) {
        RAWLOG_ERROR2("startCameraPickerFromViewController failed(%s): %s", [[theException name] UTF8String], [[theException reason] UTF8String] );
        return NO;
    }
	
	return YES;
}

- (void)takePicture:(RhoCameraSettings*) settings {
    if (!rho_rhodesapp_check_mode())
        return;
#ifndef RHO_DISABLE_OLD_CAMERA_SIGNATURE_API
    //[pickImageDelegate setPostUrl:settings.callback_url];
#endif
    pickImageDelegate.settings = settings;
    [self startCameraPicker:pickImageDelegate 
                 sourceType:UIImagePickerControllerSourceTypeCamera];
}

- (void)takeSignature:(NSString*)url {
    if (!rho_rhodesapp_check_mode())
        return;
	[self hideSplash];
	[signatureDelegate setPostUrl:url];
	//[signatureDelegate setImageFormat:format];
	[signatureDelegate setParentView:window];
	[signatureDelegate setPrevView:mainView.view];
	@try {
		CGRect rect = window.frame;
		rect.origin.x = 0;
		rect.origin.y = 0;
		SignatureViewController* svc = [[SignatureViewController alloc] initWithRect:rect delegate:signatureDelegate];
		[signatureDelegate setSignatureViewControllerValue:svc];
        
        [[mainView getMainViewController] presentModalViewController:svc animated:YES];
 
        
        
		//[mainView.view retain];
		//[mainView.view removeFromSuperview];
		//[window addSubview:svc.view];
    } @catch(NSException* theException) {
        RAWLOG_ERROR2("startSignatureViewController failed(%s): %s", [[theException name] UTF8String], [[theException reason] UTF8String] );
    }
    
}

- (void)openFullScreenNativeView:(UIView*)view {
    if (!rho_rhodesapp_check_mode())
        return;
	[self hideSplash];
	[nvDelegate setParentView:window];
	[nvDelegate setPrevView:mainView.view];
	@try {
		CGRect rect = window.frame;
		rect.origin.x = 0;
		rect.origin.y = 0;
		NVViewController* svc = [[NVViewController alloc] initWithRect:rect nvview:view delegate:nvDelegate];
		[nvDelegate setNVViewControllerValue:svc];

		[mainView.view retain];
		[mainView.view removeFromSuperview];
		[window addSubview:svc.view];
		
		[window layoutSubviews];
		[window setNeedsDisplay];
		
	} @catch(NSException* theException) {
        RAWLOG_ERROR2("startFullSreeenNativeViewViewController failed(%s): %s", [[theException name] UTF8String], [[theException reason] UTF8String] );
    }
}

- (void)closeFullScreenNativeView {
	[nvDelegate doClose];
}


- (void)choosePicture:(RhoCameraSettings*) settings {
    if (!rho_rhodesapp_check_mode())
        return;
#ifndef RHO_DISABLE_OLD_CAMERA_SIGNATURE_API
    //[pickImageDelegate setPostUrl:settings.callback_url];
#endif
    pickImageDelegate.settings = settings;
    [self startCameraPicker:pickImageDelegate 
                 sourceType:UIImagePickerControllerSourceTypePhotoLibrary];
}

- (UIWindow*)rootWindow {
    return window;
}

- (void)hideSplash {
    if (splashViewController) {
        [splashViewController hideSplash];
        [splashViewController release];
        splashViewController = nil;
		
		[window addSubview:mainView.view];
        window.rootViewController = mainView;
		[window bringSubviewToFront:mainView.view];
    }
}

- (void)setMainView:(id<RhoMainView,NSObject>)view {
    if (mainView == view)
        return;

	UIViewController* sv = mainView;
	//[self hideSplash];
    
	// Special hack for solve problem with VerticalTabBar on iPad
	// When we switch from VerticalTabBar to any View - autorotation is disabled and application switch to Portrait orientation
	// But there are no problme on iPhone 4.1 and above
	//I think this is special iPad 3.2 problem
    BOOL isVerticalTab = NO;
	if ([mainView isKindOfClass:[SplittedMainView class]]) {
		BOOL is_iPad = NO;
		
		NSString *model = [[UIDevice currentDevice] model]; // "iPad ..."
		if ([model hasPrefix:@"iPad"]) {
			is_iPad = YES;
		}
		
		isVerticalTab = is_iPad;
	}
	
	CGRect main_frame = mainView.view.frame;
	
	UIWindow* www = window;
	if (isVerticalTab) {
		
		window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
		window.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
		window.autoresizesSubviews = YES;
		
		//view.view.frame = main_frame;
		mainView = nil;
	}
	else {
		[mainView.view removeFromSuperview];
		[mainView release];
	}
	
    mainView = [view retain];
    [window addSubview:mainView.view];
    
    //[window setRootViewController:mainView];
    
    window.rootViewController = mainView;
    
    
    
	//[window bringSubviewToFront:mainView.view];
	
	if (isVerticalTab) {
		[window makeKeyAndVisible];

		[sv.view removeFromSuperview];
		[sv release];
		[www release];
	}
	[window layoutSubviews];
	
}

- (id<RhoMainView,NSObject>)mainView {
    return mainView;
}



// make splash screen
// return YES if SplashScreen maked 
- (BOOL) showLoadingPagePre 
{
    NSFileManager *fileManager = [NSFileManager defaultManager];
    
    NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
    NSString *htmPath = [NSString stringWithFormat:@"%@/apps/app/loading.html", resourcePath];
    
    if ([SplashViewController hasLoadingImage] || [SplashViewController hasLoadingScreenStoryboard]) {
        splashViewController = [[SplashViewController alloc] initWithParentView:window];
    }
    else if ([fileManager fileExistsAtPath:htmPath]) {
        NSError *err;
        NSString *data = [NSString stringWithContentsOfFile:htmPath encoding:NSUTF8StringEncoding error:&err];
        [mainView loadHTMLString:data];
    }
	return [SplashViewController hasLoadingImage] || [SplashViewController hasLoadingScreenStoryboard];
}

// execute rho_splash_screen_start(); - we can do it only after Rhodes initialization
- (void) showLoadingPagePost
{
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
    NSString *htmPath = [NSString stringWithFormat:@"%@/apps/app/loading.html", resourcePath];
    
    if (splashViewController != nil) {
		rho_splash_screen_start();
    }
    else if ([fileManager fileExistsAtPath:htmPath]) {
		rho_splash_screen_start();
		
    }
}


- (void)doRhoActivate {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    @try {
        [AppManager startupLogging:@"doRhoActivate thread started"];
        while (!app_created)
            [NSThread sleepForTimeInterval:0.1];
        [Rhodes performOnUiThread:[RhoActivateTask class] wait:NO];
    }
    @finally {
        [AppManager startupLogging:@"doRhoActivate thread finished"];
        [pool release];
    }
}

- (void)initAppManager {
    // log not ready
    [AppManager startupLogging:@"Init appManager"];

    appManager = [AppManager instance];
    //Configure AppManager - setup files and symlinks etc.
    [appManager configure];
    
    // log not ready
    [AppManager startupLogging:@"Create rhodes logging and rhoconfig.txt loading"];
    rho_logconf_Init_with_separate_user_path(rho_native_rhopath(), rho_native_rhopath(), "", rho_native_rhouserpath());
    [AppManager startupLogging:@"Create rhodes app"];
    rho_rhodesapp_create_with_separate_user_path(rho_native_rhopath(), rho_native_rhouserpath());
    RAWLOG_INFO("Rhodes started");
    
}

- (void)doRhoInit {
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    @try {
        //NSLog(@"Init appManager");
        //appManager = [AppManager instance];
        ////Configure AppManager
        //[appManager configure];
        
        const char *szRootPath = rho_native_rhopath();
        const char *szUserPath = rho_native_rhouserpath();

        // Now this methods executed not in separate thread - executed in thread of app's delegate
        //NSLog(@"Init logconf");
        //rho_logconf_Init_with_separate_user_path(szRootPath, szRootPath, "", szUserPath);
        InitMemoryInfoCollector();
        //NSLog(@"Create rhodes app");
        //rho_rhodesapp_create_with_separate_user_path(szRootPath, szUserPath);
        
        app_created = YES;
        
        rotationLocked = rho_conf_getBool("disable_screen_rotation");
		
		RAWLOG_INFO("Init network monitor");
        mNetworkPollCondition = [[NSCondition alloc] init];

		initNetworkMonitoring();
		[self performSelectorInBackground:@selector(monitorNetworkStatus) withObject:nil];
        
        RAWLOG_INFO("Show loading page");
        [self performSelectorOnMainThread:@selector(showLoadingPagePost) withObject:nil waitUntilDone:NO];
        
        RAWLOG_INFO("Start rhodes app");
        rho_rhodesapp_start();
		rho_rhodesapp_callUiCreatedCallback();
        [self registerForNotifications];
	}
    @finally {
        [pool release];
    }
    
    if (rho_conf_getBool("full_screen") != 0) {
        [Rhodes setStatusBarHidden:YES];
    }
    
}

static char* myObserver="anObserver";

- (void)registerForNotifications {
    //Screen lock notifications
    CFNotificationCenterAddObserver(CFNotificationCenterGetDarwinNotifyCenter(), //center
                                    (void*)myObserver, // observer
                                    displayStatusChanged, // callback
                                    CFSTR("com.apple.iokit.hid.displayStatus"), // event name
                                    NULL, // object
                                    CFNotificationSuspensionBehaviorDeliverImmediately);
}

- (void)unregisterNotifications {
    CFNotificationCenterRemoveObserver(CFNotificationCenterGetDarwinNotifyCenter(), //center
                                    (void*)myObserver, // observer
                                    CFSTR("com.apple.iokit.hid.displayStatus"), // event name
                                    NULL // object
    );
}

static void displayStatusChanged(CFNotificationCenterRef center, void *observer, CFStringRef name, const void *object, CFDictionaryRef userInfo) {
    if ([Rhodes sharedInstance])
    {
        NSString* notificationName = (__bridge NSString*)name;
        
        if ([notificationName isEqualToString:@"com.apple.iokit.hid.displayStatus"]) {
            [[Rhodes sharedInstance] setMScreenStateChanged:YES];
        }
    }
}

- (void)doStartUp {
    // log not ready
    [AppManager startupLogging:@"Rhodes starting application..."];
    
    [CRhoURLProtocol initAndRegister];
    
    [NSThread setThreadPriority:1.0];
    
    const char *szRootPath = rho_native_rhopath();
    const char *szUserPath = rho_native_rhouserpath();
    //NSLog(@"Init logconf");
    
    // now executed in [self initAppManager]
    //rho_logconf_Init_with_separate_user_path(szRootPath, szRootPath, "", szUserPath);

    [self initAppManager];
    
    RAWLOG_INFO("Create new detached thread for initialization stuff");
    [NSThread detachNewThreadSelector:@selector(doRhoInit) toTarget:self withObject:nil];
    
    RAWLOG_INFO("Init all windows");

    [[NSHTTPCookieStorage sharedHTTPCookieStorage] setCookieAcceptPolicy:NSHTTPCookieAcceptPolicyAlways];
    
#ifndef RHO_STANDALONE_LIB

    [[UIApplication sharedApplication] setStatusBarStyle:UIStatusBarStyleDefault animated:NO];
    
    const char* fs = get_app_build_config_item("iphone_full_screen");
    if (fs == NULL) {
        fs = "0";
    }
    
    if ((rho_conf_getBool("full_screen") != 0) || (fs[0] != '0')) {
        [Rhodes setStatusBarHidden:YES];
#ifdef __IPHONE_3_2
        if ([[Rhodes application] respondsToSelector:@selector(setStatusBarHidden:withAnimation:)]) {
            [[Rhodes application] setStatusBarHidden:YES withAnimation:NO];
        }
#else
        [[Rhodes application] setStatusBarHidden:YES animated:NO];
#endif    
    }
    
    window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    window.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    window.autoresizesSubviews = YES;
	
    mainView = nil;
    mainView = [[SimpleMainView alloc] initWithParentView:window frame:[Rhodes applicationFrame]];
    mainView.view.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    mainView.view.autoresizesSubviews = YES;
    
	BOOL is_splash_screen_maked = [self showLoadingPagePre];
	
	if (!is_splash_screen_maked) {
		[window addSubview:mainView.view];
        window.rootViewController = mainView;
	}
	
    [window makeKeyAndVisible];
#else
    
    // standalone lib mode - no own UI
    mainView = nil;
    mainView = [[RhoMainViewStubImpl alloc] init];
    
#endif
 
	CGRect rrr = [application statusBarFrame];
	
    RAWLOG_INFO("Init cookies");
    cookies = [[NSMutableDictionary alloc] initWithCapacity:0];
    
    // Init controllers
    RAWLOG_INFO("Init controllers");
    logOptionsController = [[LogOptionsController alloc] init];
    logViewController = [[LogViewController alloc] init];
    
    RAWLOG_INFO("Init delegates");
    dateTimePickerDelegate = [[DateTimePickerDelegate alloc] init];
    pickImageDelegate = [[PickImageDelegate alloc] init];
    signatureDelegate = [SignatureDelegate getSharedInstance];
    nvDelegate = [[NVDelegate alloc] init];

    RAWLOG_INFO("Initialization finished");
}

- (void) registerForPushNotificationsInternal:(id<IPushNotificationsReceiver>)receiver;
{
    pushReceiver = receiver;

    
//#ifdef __IPHONE_8_0
//    if (SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"8.0")) {
//        [[UIApplication sharedApplication] registerUserNotificationSettings:[UIUserNotificationSettings settingsForTypes:(UIUserNotificationTypeSound | UIUserNotificationTypeAlert | UIUserNotificationTypeBadge) categories:nil]];
//        [[UIApplication sharedApplication] registerForRemoteNotifications];    }
//    else {
//        [[UIApplication sharedApplication] registerForRemoteNotificationTypes:(UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeAlert | UIRemoteNotificationTypeSound)];
//        UIRemoteNotificationType nt = [[UIApplication sharedApplication] enabledRemoteNotificationTypes];
//        NSLog(@"Enabled notification types: %i", (int)nt);
//    }
//#else
//    [[UIApplication sharedApplication] registerForRemoteNotificationTypes:
//     (UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeAlert | UIRemoteNotificationTypeSound)];
//    UIRemoteNotificationType nt = [[UIApplication sharedApplication] enabledRemoteNotificationTypes];
//    NSLog(@"Enabled notification types: %i", (int)nt);
//#endif
    
   [self registerForRemoteNotification];

}

- (void) registerForPushNotifications:(id<IPushNotificationsReceiver>)receiver;
{
#ifdef APP_BUILD_CAPABILITY_PUSH
    //[self performSelectorOnMainThread:@selector(registerForPushNotificationsInternal:) withObject:receiver waitUntilDone:NO];
     pushReceiver = receiver;
    if (mPushStoredData_DeviceToken != nil) {
        [pushReceiver onPushRegistrationSucceed:mPushStoredData_DeviceToken];
        mPushStoredData_DeviceToken = nil;
    }
    if (mPushStoredData_RegisterError != nil) {
        [pushReceiver onPushRegistrationFailed:mPushStoredData_RegisterError];
        mPushStoredData_RegisterError = nil;
    }
    if (mPushStoredData_UserInfo != nil) {
        [pushReceiver onPushMessageReceived:mPushStoredData_UserInfo];
        mPushStoredData_UserInfo = nil;
    }
#endif
}


- (void) signalNetworkStatusPollIntervalChanged
{
    [mNetworkPollCondition signal];
}

- (void) setAppMessageReceiver:(id<IAppMessageReceiver>)receiver
{
    appMessageReceiver = receiver;
}


- (void) monitorNetworkStatus
{
    while(true)
    {
        Reachability* r = [Reachability reachabilityForInternetConnection];
        NetworkStatus status = [r currentReachabilityStatus];
        rho_sysimpl_sethas_network( status==ReachableViaWiFi?1:0 );
        rho_sysimpl_sethas_cellnetwork( status==ReachableViaWWAN?1:0 );
        
        networkStatusNotify(status==NotReachable?0:1);
        [r release];
        
        [mNetworkPollCondition lock];
        [mNetworkPollCondition waitUntilDate:[NSDate dateWithTimeIntervalSinceNow:getNetworkStatusPollInterval()]];
        [mNetworkPollCondition unlock];
    }
}

#ifdef __IPHONE_3_0
- (void)processDoSync:(NSDictionary *)userInfo
{
	if ( rho_rcclient_have_rhoconnect_client() ) {
	NSArray *do_sync = [userInfo objectForKey:@"do_sync"];
	if (do_sync) {
		NSEnumerator *enumerator = [do_sync objectEnumerator];
		id url;
		
		RAWLOG_INFO("do_sync array: ");
		bool sync_all = false;
		while ( url = [enumerator nextObject] ) {
			RAWLOG_INFO1( "url = %s", [(NSString*)url UTF8String] );
			if ([@"all" caseInsensitiveCompare:url] == NSOrderedSame) {
				sync_all = true;
			} else {
				//do sync of individual source
				NSString* srcUrl = [url stringByTrimmingCharactersInSet: [NSCharacterSet characterSetWithCharactersInString:@" \t\r\n"]];
				rho_rcclient_doSyncSourceByName([srcUrl cStringUsingEncoding:[NSString defaultCStringEncoding]]);
			}
		}
		
		if (sync_all) {
			rho_rcclient_doSyncAllSources(TRUE,"",false);
		}
	}
	}
}

#endif

- (void) saveLastUsedTime {
}

- (void)playStart:(NSString *)fileName mediaType:(NSString *)type {
    NSString *soundFilePath;
	//hack to work around iphone limitation when it will play push alerts only from the main bundle root
	if ([fileName hasPrefix:@"/public/alerts/"] || [fileName hasPrefix:@"/apps/public/alerts/"]) {
		NSString *file = [fileName lastPathComponent];
		soundFilePath = [[[NSBundle mainBundle] resourcePath] stringByAppendingPathComponent:file];
    // do not add root path if it is already there
	} else if ([fileName hasPrefix:[AppManager getApplicationsRootPath]]) {
        soundFilePath = fileName;
    } else{
		soundFilePath = [[AppManager getApplicationsRootPath] stringByAppendingPathComponent:fileName];
	}
	RAWLOG_INFO1("Playing %s: ", [soundFilePath UTF8String]);
	
	NSURL *fileURL = [[NSURL alloc] initFileURLWithPath: soundFilePath];
	NSError* err = nil;
	AVAudioPlayer *newPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:fileURL error:&err];
	RAWLOG_INFO1("Init media player returns: %s", [[err localizedDescription] UTF8String]);
	
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
		RAWLOG_INFO("Audio player finished playing...");
	}	
}

- (void)audioPlayerDecodeErrorDidOccur:(AVAudioPlayer *)player error:(NSError *)error {
	RAWLOG_INFO1("Audio player decoding error %s", [[error localizedDescription] UTF8String]);
}










- (void)doStartUpRe:(NSObject*)arg {
    RAWLOG_INFO("Rhodes starting application...");
    rotationLocked = NO;
    
    [self initAppManager];
    
    RAWLOG_INFO("Create new detached thread for initialization stuff");
    [NSThread detachNewThreadSelector:@selector(doRhoInit) toTarget:self withObject:nil];
    
    if (mainView != nil) {
        [mainView.view removeFromSuperview];
        [mainView release];
    }
    
    mainView = nil;
    [SimpleMainView enableHiddenOnStart];
    mainView = [[SimpleMainView alloc] initWithParentView:window frame:[Rhodes applicationFrame]];
    mainView.view.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    mainView.view.autoresizesSubviews = YES;
    
	BOOL is_splash_screen_maked = [self showLoadingPagePre];
	
	if (!is_splash_screen_maked) {
		[window addSubview:mainView.view];
	}
	
    RAWLOG_INFO("Init cookies");
    cookies = [[NSMutableDictionary alloc] initWithCapacity:0];
    
    rho_rhodesapp_canstartapp("", ", ");
    
    RAWLOG_INFO("Initialization finished");
}





-(void)restart_app_func
{
    [self performSelectorOnMainThread:@selector(doStartUpRe:) withObject:nil waitUntilDone:NO];
}

+(void)restart_app
{
    [[Rhodes sharedInstance] restart_app_func];

}

// UIApplicationDelegate implementation

- (void)registerForRemoteNotification {
#ifdef __IPHONE_8_0
    if (SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"8.0")) {
#ifdef APP_BUILD_CAPABILITY_PUSH
        UIUserNotificationType types = UIUserNotificationTypeSound | UIUserNotificationTypeBadge | UIUserNotificationTypeAlert;
#else
        UIUserNotificationType types = UIUserNotificationTypeBadge;
#endif
        UIUserNotificationSettings *notificationSettings = [UIUserNotificationSettings settingsForTypes:types categories:nil];
        [[UIApplication sharedApplication] registerUserNotificationSettings:notificationSettings];
#ifdef APP_BUILD_CAPABILITY_PUSH
        [[UIApplication sharedApplication] registerForRemoteNotificationTypes:(UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound | UIRemoteNotificationTypeAlert)];
#endif
    } else
#endif
    {
#ifdef APP_BUILD_CAPABILITY_PUSH
        [[UIApplication sharedApplication] registerForRemoteNotificationTypes:(UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound | UIRemoteNotificationTypeAlert)];
        UIRemoteNotificationType nt = [[UIApplication sharedApplication] enabledRemoteNotificationTypes];
        RAWLOG_INFO1("Enabled notification types: %d", (int)nt);
#else
        [[UIApplication sharedApplication] registerForRemoteNotificationTypes:(UIRemoteNotificationTypeBadge)];
#endif
    }
}

#ifdef __IPHONE_8_0
- (void)application:(UIApplication *)application didRegisterUserNotificationSettings:(UIUserNotificationSettings *)notificationSettings {
    [application registerForRemoteNotifications];
}
#endif



#ifdef __IPHONE_3_0

- (BOOL)didFinishLaunchingWithOptionsInternal:(NSDictionary *)launchOptions application:(UIApplication *)application {
    
    self.mBlockExit = NO;

    mPushStoredData_UserInfo = nil;
    mPushStoredData_RegisterError = nil;
    mPushStoredData_DeviceToken = nil;
    
    splashViewControllerSnapShot = nil;

    instance = self;
    eventStore = nil; 
    self->application = [UIApplication sharedApplication];
    rotationLocked = NO;
    
    if ( !rho_sys_check_rollback_bundle(rho_native_rhopath()) )
    {
        
        rho_sys_impl_exit_with_errormessage( "Bundle update.", "Application is currupted. Reinstall it , please.");
        return NO;
    }    
    
    

	NSURL* url = [launchOptions objectForKey:UIApplicationLaunchOptionsURLKey];
    // log not ready
    [AppManager startupLogging:[NSString stringWithFormat:@"didFinishLaunchingWithOptions: %@", [url absoluteString]]];


#ifdef APP_BUILD_CAPABILITY_PUSH
    if (floor(NSFoundationVersionNumber) <= NSFoundationVersionNumber_iOS_7_1) {
        // iOS 7.1 or earlier. Disable the deprecation warnings.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        UIRemoteNotificationType allNotificationTypes =
        (UIRemoteNotificationTypeSound |
         UIRemoteNotificationTypeAlert |
         UIRemoteNotificationTypeBadge);
        [application registerForRemoteNotificationTypes:allNotificationTypes];
#pragma clang diagnostic pop
    } else {
        // iOS 8 or later
        // [START register_for_notifications]
        if (floor(NSFoundationVersionNumber) <= NSFoundationVersionNumber_iOS_9_x_Max) {
            UIUserNotificationType allNotificationTypes =
            (UIUserNotificationTypeSound | UIUserNotificationTypeAlert | UIUserNotificationTypeBadge);
            UIUserNotificationSettings *settings =
            [UIUserNotificationSettings settingsForTypes:allNotificationTypes categories:nil];
            [application registerUserNotificationSettings:settings];
        } else {
            // iOS 10 or later
#if 0
#if defined(__IPHONE_10_0) && __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_10_0
            // For iOS 10 display notification (sent via APNS)
            [UNUserNotificationCenter currentNotificationCenter].delegate = self;
            UNAuthorizationOptions authOptions =
            UNAuthorizationOptionAlert
            | UNAuthorizationOptionSound
            | UNAuthorizationOptionBadge;
            [[UNUserNotificationCenter currentNotificationCenter] requestAuthorizationWithOptions:authOptions completionHandler:^(BOOL granted, NSError * _Nullable error) {
            }];
#endif
#endif
        }
        
        //[application registerForRemoteNotifications];
        // [END register_for_notifications]
    }
    [self registerForRemoteNotification];
#endif

	// store start parameter
	NSString* start_parameter = [NSString stringWithUTF8String:""];
	if (url != nil) {
		NSString* url_str = [url absoluteString];
		if (url_str != nil) {
			NSRange range = [url_str rangeOfString:@":"];
			if ((range.location >= 0) && (range.length > 0)) {
				start_parameter = [url_str substringFromIndex:range.location + 1];
			}
		}
	}	

    [self doStartUp];
    [self processDoSync:launchOptions];
    
    [appMessageReceiver onAppMessageReceived:start_parameter app:@""];

    if ( !rho_rhodesapp_canstartapp([start_parameter UTF8String], ", ") )
    {
        //const char* szAppSecToken = get_app_build_config_item("security_token");
		RAWLOG_ERROR("This is hidden app and can be started only with security key.");
        /*
        {
            
            NSString* message = @"Invalid security token !";
            
            rho_conf_Init(rho_native_rhopath());
            
            if (rho_conf_is_property_exists("invalid_security_token_message")) {
                const char* conf_message = rho_conf_getString("invalid_security_token_message");
                message = [NSString stringWithUTF8String:conf_message];
            }
            
            
            UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"" 
                                                            message:message
                                                           delegate:self 
                                                  cancelButtonTitle:@"OK" 
                                                  otherButtonTitles: nil];
            [alert show];
            [alert release];
        }
        */
        //exit(EXIT_SUCCESS);
    }
	
	return NO;
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    return [self didFinishLaunchingWithOptionsInternal:launchOptions application:application];
}


#ifdef APP_BUILD_CAPABILITY_PUSH
#if 0
#if defined(__IPHONE_10_0) && __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_10_0
// Handle incoming notification messages while app is in the foreground.
- (void)userNotificationCenter:(UNUserNotificationCenter *)center
       willPresentNotification:(UNNotification *)notification
         withCompletionHandler:(void (^)(UNNotificationPresentationOptions))completionHandler {
    NSDictionary *userInfo = notification.request.content.userInfo;
    
    [self processPushMessage:userInfo];
    
    // Change this to your preferred presentation option
    completionHandler(UNNotificationPresentationOptionNone);
}

// Handle notification messages after display notification is tapped by the user.
- (void)userNotificationCenter:(UNUserNotificationCenter *)center
didReceiveNotificationResponse:(UNNotificationResponse *)response
#if defined(__IPHONE_11_0)
         withCompletionHandler:(void(^)(void))completionHandler {
#else
withCompletionHandler:(void(^)())completionHandler {
#endif
    NSDictionary *userInfo = response.notification.request.content.userInfo;
    //if (userInfo[kGCMMessageIDKey]) {
    //    NSLog(@"Message ID: %@", userInfo[kGCMMessageIDKey]);
    //}
    
    // Print full message.
    NSLog(@"%@", userInfo);
    
    completionHandler();
}
#endif
#endif
#endif

- (void) exit_with_errormessage:(NSString*)title message:(NSString*)message 
{
    [Rhodes sharedInstance].mBlockExit = YES;
    
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title 
                                                    message:message
                                                   delegate:self 
                                          cancelButtonTitle:@"OK" 
                                          otherButtonTitles: nil];
    [alert show];
    [alert release];
    
}


- (void)alertView:(UIAlertView *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex {
    exit(EXIT_SUCCESS);
}



#endif
/*
- (void)applicationDidFinishLaunching:(UIApplication *)application 
{
	[self doStartUp];
} */


#ifdef __IPHONE_3_0
#ifdef APP_BUILD_CAPABILITY_PUSH

- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
    NSLog(@"PUSH My token is: %@", deviceToken);
    //RAWLOG_INFO1([[NSString stringWithFormat:@"PUSH My token is: %@", deviceToken] UTF8String]);
    if ( pushReceiver != nil ) {
        [pushReceiver onPushRegistrationSucceed:deviceToken];
    }
    else {
        mPushStoredData_DeviceToken = [deviceToken copy];
    }
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
    NSLog(@"PUSH Failed to get token, error: %@", error);
    //RAWLOG_ERROR1([[NSString stringWithFormat:@"PUSH Failed to get token, error: %@", error] UTF8String]);
    if ( pushReceiver != nil ) {
        [pushReceiver onPushRegistrationFailed:error];
    }
    else {
        mPushStoredData_RegisterError = [error copy];
    }
}

-(void) processPushMessage:(NSDictionary*)userInfo {
    NSLog(@"PUSH Received notification: %@", userInfo);
#ifdef APP_BUILD_CAPABILITY_PUSH
    //RAWLOG_INFO1([[NSString stringWithFormat:@"PUSH Received notification: %@", userInfo] UTF8String]);
    if ( pushReceiver != nil ) {
        [pushReceiver onPushMessageReceived:userInfo];
    }
    else {
        mPushStoredData_UserInfo = [userInfo copy];
    }
#endif
}
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
{
    [self processPushMessage:userInfo];
}


- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionHandler:(void (^)(UIBackgroundFetchResult result))completionHandler {
    if(application.applicationState == UIApplicationStateInactive) {
        NSLog(@"didReceiveRemoteNotification: Inactive");
        [self processPushMessage:userInfo];
        completionHandler(UIBackgroundFetchResultNoData);
    } else if (application.applicationState == UIApplicationStateBackground) {
        NSLog(@"didReceiveRemoteNotification: Background");
        completionHandler(UIBackgroundFetchResultNoData);
    } else {
        //RAWLOG_INFO("didReceiveRemoteNotification: Active");
        [self processPushMessage:userInfo];
        completionHandler(UIBackgroundFetchResultNoData);
    }
}

#endif //APP_BUILD_CAPABILITY_PUSH
#endif //__IPHONE_3_0

- (void)applicationDidBecomeActive:(UIApplication *)application {
    RAWLOG_INFO("Application did become active");
	
	if (syncBackgroundTask != UIBackgroundTaskInvalid) {
		[application endBackgroundTask: syncBackgroundTask];
		syncBackgroundTask = UIBackgroundTaskInvalid;
	}
	
    if (!app_created) {
        RAWLOG_INFO("Application is not created yet so postpone activation callback");
        [NSThread detachNewThreadSelector:@selector(doRhoActivate) toTarget:self withObject:nil];
    }
    else
    {
        [Rhodes performOnUiThread:[RhoActivateTask class] wait:NO];
        
        [[RhoExtManagerSingletone getExtensionManager] applicationDidBecomeActive: application];
    }
    
#ifndef RHO_STANDALONE_LIB
    if ([SplashViewController isReplaceContentWhenSnapshot]) {
        [self unblurCirrentView];
    }
#endif

}

- (void)applicationWillResignActive:(UIApplication *)application {
    RAWLOG_INFO("Application will resign active");
    if (mScreenStateChanged)
    {
        rho_rhodesapp_callScreenOffCallback();
        [self setMScreenStateChanged:NO];
    } else {
        [self unregisterNotifications];
    }
    rho_rhodesapp_callAppActiveCallback(0);
    rho_rhodesapp_canstartapp("", ", ");
    
#ifndef RHO_STANDALONE_LIB
    if ([SplashViewController isReplaceContentWhenSnapshot]) {
        [self blurCurrentView];
    }
#endif
    
    [self saveLastUsedTime];
}

#ifdef __IPHONE_4_0
- (void)applicationDidEnterBackground:(UIApplication *)app {
    
    
#ifndef RHO_STANDALONE_LIB
    //if ([SplashViewController isReplaceContentWhenSnapshot]) {
    //    if (splashViewControllerSnapShot == nil) {
    //        splashViewControllerSnapShot = [[SplashViewController alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    //    }
    //    [self.window.rootViewController presentViewController:[splashViewControllerSnapShot retain] animated:NO completion:NULL];
    //}
#endif

    RAWLOG_INFO("Application go to background");
    rho_rhodesapp_callUiDestroyedCallback();
    rho_rhodesapp_canstartapp("", ", ");
    
	if (rho_rcclient_have_rhoconnect_client()) {
        if (rho_conf_getBool("finish_sync_in_background")/* && (rho_rcclient_issyncing()==1)*/) {
            if ([[UIDevice currentDevice] respondsToSelector:@selector(isMultitaskingSupported)]) {
                if ([[UIDevice currentDevice] isMultitaskingSupported]) { //Check if device supports mulitasking
                    
                    syncBackgroundTask = [app beginBackgroundTaskWithExpirationHandler: ^ {
                        RAWLOG_ERROR("$$$ Background task is terminated by System !!!");
                        
                        // If the background task is already invalid, don't try to end it.
                        if (syncBackgroundTask != UIBackgroundTaskInvalid) {
                            [app endBackgroundTask: syncBackgroundTask]; //Tell the system that we are done with the tasks
                            syncBackgroundTask = UIBackgroundTaskInvalid; //Set the task to be invalid
                        }
                    }];
                    
                    RAWLOG_INFO("Will wait sync thread to finish sync");
                    
                    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
                        RAWLOG_INFO("Waiting for sync thread");
                        
                        do
                        {
                            RAWLOG_INFO1("Check sync, rho_rcclient_issyncing = %d", rho_rcclient_issyncing());
                            [NSThread sleepForTimeInterval:1];
                        } while (rho_rcclient_issyncing() == 1);
                        
                        RAWLOG_INFO1("Sync is finished, rho_rcclient_issyncing = %d", rho_rcclient_issyncing());
                        
                        // If the background task is already invalid, don't try to end it.
                        if (syncBackgroundTask != UIBackgroundTaskInvalid) {
                            [app endBackgroundTask: syncBackgroundTask]; //End the task so the system knows that you are done with what you need to perform
                            syncBackgroundTask = UIBackgroundTaskInvalid; //Invalidate the background_task
                        }
                    });
                }
            }
        }
	}
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
#ifndef RHO_STANDALONE_LIB
    //if ([SplashViewController isReplaceContentWhenSnapshot]) {
    //    [self.window.rootViewController dismissViewControllerAnimated:NO completion:NO];
    //}
#endif
    [self registerForNotifications];
}
#endif


- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{
    RAWLOG_INFO("Application receive Memory Warning !!!");
    /*
    int level = OSMemoryNotificationCurrentLevel();
    
    int o;
    o = 9;
    */
}

- (void)applicationWillTerminate:(UIApplication *)application {
    RAWLOG_INFO("Runner will terminate");
    rho_rhodesapp_callUiDestroyedCallback();
    [self saveLastUsedTime];
    rho_rhodesapp_destroy();
}

// UITabBarControllerDelegate implementation

- (void)tabBarController:(UITabBarController *)tabBarController
 didSelectViewController:(UIViewController *)viewController {
    SEL sel = @selector(onSwitchTab);
    if ([mainView respondsToSelector:sel])
        [mainView performSelector:sel];
}

//- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url
- (BOOL)application:(UIApplication *)application_arg
    openURL:(NSURL *)url
    sourceApplication:(NSString *)sourceApplication
    annotation:(id)annotation
{
    if (!url) {  return NO; }

    rho_rhodesapp_setStartParametersOriginal( [[url absoluteString] UTF8String] );
    
    [[RhoExtManagerSingletone getExtensionManager] application:application_arg openURL:url
                                             sourceApplication:sourceApplication
                                                    annotation:annotation];
    
    NSBundle* mb = [NSBundle mainBundle];
    NSDictionary* md = [mb infoDictionary];
    NSArray* schemes = [md objectForKey:@"CFBundleURLTypes"];
    NSDictionary* scheme = [schemes objectAtIndex:0];
    NSArray* urls = [scheme objectForKey:@"CFBundleURLSchemes"];
    NSString* url0 = [urls objectAtIndex:0];
    
    if ([[url scheme] isEqualToString:url0]) {
        NSString *startparams = @"";
        NSString *fullurl = [[url absoluteString] stringByRemovingPercentEncoding];
        
        NSRange range = [fullurl rangeOfString:@":"];
        if ((range.location > 0) && (range.length > 0)) {
            startparams = [fullurl substringFromIndex:range.location + 1];
        }
        
        [appMessageReceiver onAppMessageReceived:startparams app:[url scheme]];

        // set start params
        rho_rhodesapp_canstartapp([startparams UTF8String], ", ");
        return YES;
    }
    return NO;	
    /*
    NSString *URLString = [url absoluteString];
    [[NSUserDefaults standardUserDefaults] setObject:URLString forKey:@"url"];
    [[NSUserDefaults standardUserDefaults] synchronize];

	//NSString* msg = @"handleOpenURL: ";
	//msg = [msg stringByAppendingString:URLString];
	
	UIAlertView *alert = [[[UIAlertView alloc]
						   initWithTitle:@"handleOpenURL:"
						   message:URLString
						   delegate:self
						   cancelButtonTitle:@"Close"
						   otherButtonTitles:nil] autorelease];
    
    [alert show];
	*/
    return YES;
}



@end

// Native functions


void rho_sys_impl_exit_with_errormessage(const char* szTitle, const char* szMsg) {

    [[Rhodes sharedInstance] exit_with_errormessage:[NSString stringWithUTF8String:szTitle] message:[NSString stringWithUTF8String:szMsg]];
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


BOOL rho_main_is_rotation_locked() {
    return [[Rhodes sharedInstance] isRotationLocked];
}

void rho_main_set_rotation_locked(BOOL locked) {
    [[Rhodes sharedInstance] setRotationLocked:locked];
}



//void rhomobile_lib_start_app() {
 //   [Rhodes makeSharedInstance];
//    [[Rhodes sharedInstance] didFinishLaunchingWithOptionsInternal:[NSMutableDictionary dictionaryWithCapacity:5] application:[UIApplication sharedApplication]];
    
//}
