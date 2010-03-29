#import "Rhodes.h"
#import "SimpleMainView.h"

#include "logging/RhoLogConf.h"
#include "common/RhodesApp.h"

@implementation Rhodes

@synthesize window, mainView;

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
    
    mainView = nil;
    self.mainView = [[SimpleMainView alloc] initWithParentView:window];
    
    const char *szRootPath = rho_native_rhopath();
    rho_logconf_Init(szRootPath);
    
    appManager = [AppManager instance]; 
	//Configure AppManager
	[appManager configure];
    
    rho_rhodesapp_create(szRootPath);
    rho_rhodesapp_start();
}

- (void)processDoSync:(NSDictionary*)options {
    // TODO:
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
