#import "WebViewController.h"
#import "AppManager.h"
//#import "ruby/ext/rho/rhoruby.h"
//#import "UniversalLock.h"
#import "common/RhoConf.h"
//#import "sync/syncthread.h"
#import "common/RhodesApp.h"
#import "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "WebViewCtrl"
/*
static char currentLocation[4096] = "";

//INIT_LOCK(current_location); // racing condition on thid lock, need to fix it somehow

void set_current_location(CFStringRef location) {
	//LOCK(current_location);
	CFStringGetCString((CFStringRef)location, currentLocation, sizeof(currentLocation), CFStringGetSystemEncoding());
	
	char* fragment = strstr(currentLocation,"#");
	if (fragment) *fragment = 0; //cut out fragment
	RAWLOG_INFO1("Current location: %s",currentLocation);
	//UNLOCK(current_location);

	if (rho_conf_getBool("KeepTrackOfLastVisitedPage")) {
		rho_conf_setString("LastVisitedPage",currentLocation);
		rho_conf_save();
	}
	
}

char* get_current_location() {
	//LOCK(current_location);
	return currentLocation;
	//UNLOCK(current_location);
}*/

extern int webview_active_tab();

@implementation WebViewController

//@synthesize viewHomeUrl, viewOptionsUrl;
@synthesize actionTarget, onShowLog, toolbar, webView;

- (void)dealloc {
    [backBtn release];
    [forwardBtn release];
    [homeBtn release];
    [refreshBtn release];
    [optionsBtn release];
    [activity release];
    [toolbar release];
    [super dealloc];
}

- (void)createNewToolbar:(NSArray*)items {
    
}

- (UIToolbar*)newToolbar:(CGRect)mainFrame {
    UIToolbar *tb = [UIToolbar new];
    tb.barStyle = UIBarStyleBlackOpaque;
    
    [tb sizeToFit];
    
    CGFloat tbHeight = [tb frame].size.height;
    CGRect tbFrame = CGRectMake(CGRectGetMinX(mainFrame),
                                CGRectGetHeight(mainFrame) - tbHeight,
                                CGRectGetWidth(mainFrame),
                                tbHeight);
    [tb setFrame:tbFrame];
    
    backBtn = [[UIBarButtonItem alloc]
               initWithImage:[UIImage imageNamed:@"back_btn.png"]
               style:UIBarButtonItemStylePlain target:self
               action:@selector(goBack:)];
    
    forwardBtn = [[UIBarButtonItem alloc]
                  initWithImage:[UIImage imageNamed:@"forward_btn.png"]
                  style:UIBarButtonItemStylePlain target:self
                  action:@selector(goForward:)];
    
    homeBtn = [[UIBarButtonItem alloc]
               initWithImage:[UIImage imageNamed:@"home_btn.png"]
               style:UIBarButtonItemStylePlain target:self
               action:@selector(goHome:)];
    
    refreshBtn = [[UIBarButtonItem alloc]
                  initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh
                  target:self action:@selector(onRefresh:)];
    
    optionsBtn = [[UIBarButtonItem alloc]
                  initWithImage:[UIImage imageNamed:@"gears.png"]
                  style:UIBarButtonItemStylePlain target:self
                  action:@selector(goOptions:)];
    
    activity = [[UIActivityIndicatorView alloc]
                initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhite];
    activity.hidesWhenStopped = YES;
    
    UIBarButtonItem *fixedSpace = [[UIBarButtonItem alloc]
                                   initWithBarButtonSystemItem:UIBarButtonSystemItemFixedSpace
                                   target:nil action:nil];
    
    UIBarButtonItem *flexSpace = [[UIBarButtonItem alloc]
                                  initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace
                                  target:nil action:nil];
    
    NSArray *items = [NSArray arrayWithObjects:backBtn, fixedSpace, forwardBtn, flexSpace,
                      homeBtn, fixedSpace, refreshBtn, fixedSpace, optionsBtn, nil];
    [tb setItems:items];
    
    [fixedSpace release];
    [flexSpace release];
    
    return tb;
}

- (id)initWithParentWindow:(UIWindow*)w {
    window = w;
    
    CGRect mainFrame = window.frame;
    
    toolbar = [self newToolbar:window.frame];
    toolbar.hidden = YES;
    [window addSubview:toolbar];
    
    CGRect tbFrame = toolbar.frame;
    
    CGRect wvFrame = CGRectMake(CGRectGetMinX(mainFrame), CGRectGetMinY(mainFrame),
                                CGRectGetWidth(mainFrame),
                                CGRectGetHeight(mainFrame) - CGRectGetHeight(tbFrame));
    webView = [[UIWebView alloc] initWithFrame:wvFrame];
    webView.scalesPageToFit = YES;
    webView.userInteractionEnabled = YES;
    webView.multipleTouchEnabled = YES;
    webView.autoresizesSubviews = YES;
    webView.delegate = self;
    [window addSubview:webView];
    
    return self;
}

- (void)viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];
}

-(void)viewDidLoad {
	[super viewDidLoad];

    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	bool logEnabled = [defaults boolForKey:@"log_enabled_preference"];
	
	if (logEnabled && toolbar) {
		NSArray *items = toolbar.items;

		// create a bordered style button with custom title
		UIBarButtonItem *item = [[UIBarButtonItem alloc] 
			initWithTitle:@"Log"
			style:UIBarButtonItemStyleBordered	
			target:self
			action:@selector(actionShowLog:)];
		items = [items arrayByAddingObject:item];
		
		[toolbar setItems:items animated: YES];
		[item release];
	}
}

-(void)loadData:(NSData*)data mimeType:(NSString*)type {
    [webView loadData:data MIMEType:type textEncodingName:@"utf-8" baseURL:[NSURL URLWithString:@""]];
}

-(void)showToolbar:(BOOL)show {
    toolbar.hidden = !show;
    CGRect frame = window.frame;
    if (show) {
        [window sendSubviewToBack:webView];
        [window bringSubviewToFront:toolbar];
        CGRect tbFrame = toolbar.frame;
        frame.size.height -= tbFrame.size.height;
    }
    else {
        [window sendSubviewToBack:toolbar];
        [window bringSubviewToFront:webView];
    }
    [webView sizeToFit];
    [webView setFrame:frame];
}

-(void)navigate:(NSString*)url {
    //RAWLOG_INFO1("Navigating to the specifyed URL: %s",  [url cStringUsingEncoding:[NSString defaultCStringEncoding]]);
    NSString *escapedUrl = [url stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
	[webView loadRequest:[NSURLRequest requestWithURL: [NSURL URLWithString:escapedUrl]]];
}

-(void)navigateRedirect:(NSString*)url {
    //RAWLOG_INFO1("Navigate (redirect) to: %s", [url cStringUsingEncoding:[NSString defaultCStringEncoding]]);
	NSString* escapedUrl = [url stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
	NSString* homeurl = [NSString stringWithCString:rho_rhodesapp_gethomeurl() encoding:[NSString defaultCStringEncoding]];
	NSString* redirector1 = [@"/system/redirect_to?url=" stringByAppendingString:escapedUrl];
	NSString* redirector = [homeurl stringByAppendingString:redirector1];
	[webView loadRequest:[NSURLRequest requestWithURL: [NSURL URLWithString:redirector]]];
}

-(void)setActivityInfo:(NSString *)labelText {
	if (!labelText) {
		activityInfo.hidden = YES;
	} else {
		activityInfo.hidden = NO;
		[activityInfo setText:labelText];
	}
}

-(void)active {
	[activity startAnimating];
	activity.hidden = NO;
}

-(void)inactive {
	[activity stopAnimating];
	activity.hidden = YES;
}

/*
- (void)runSync
{
	rho_sync_doSyncAllSources(TRUE);
}*/

- (void)actionShowLog:(id)sender
{
	if(actionTarget && [actionTarget respondsToSelector:onShowLog]) {
		[actionTarget performSelector:onShowLog];
	}	
}

@end


