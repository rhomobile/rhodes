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
    
    mainView = [[SimpleMainView alloc] initWithParentWindow:window andDelegate:self];
    
    const char *szRootPath = rho_native_rhopath();
    rho_logconf_Init(szRootPath);
    
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


@end
