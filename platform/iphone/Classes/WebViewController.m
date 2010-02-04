#import "WebViewController.h"
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

@implementation WebViewController

//@synthesize viewHomeUrl, viewOptionsUrl;
@synthesize actionTarget, onShowLog, toolbar, webView;

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

-(void)loadHTMLString:(NSString*)data {
	RAWLOG_INFO("Loading specified HTML string");
	[webView loadHTMLString:data baseURL: [NSURL URLWithString:@""]];
}

-(void)showToolbar:(BOOL)show {
    toolbar.hidden = !show;
    if (show) {
        [window sendSubviewToBack:webView];
        [window bringSubviewToFront:toolbar];
    }
    else {
        [window sendSubviewToBack:toolbar];
        [window bringSubviewToFront:webView];
    }
    [webView sizeToFit];
}

-(void)navigate:(NSString*)url {
    //RAWLOG_INFO1("Navigating to the specifyed URL: %s",  [url cStringUsingEncoding:[NSString defaultCStringEncoding]]);
    NSString *escapedUrl = [url stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
	[webView loadRequest:[NSURLRequest requestWithURL: [NSURL URLWithString:escapedUrl]]];
}

-(void)executeJs:(JSString*)js {
	RAWLOG_INFO1("Executing JS: %s", [js.inputJs UTF8String] );
	js.outputJs = [webView stringByEvaluatingJavaScriptFromString:js.inputJs];
}

-(void)navigateRedirect:(NSString*)url {
    //RAWLOG_INFO1("Navigate (redirect) to: %s", [url cStringUsingEncoding:[NSString defaultCStringEncoding]]);
	NSString* escapedUrl = [url stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]; 
	NSString* redirector = [@"http://localhost:8080/system/redirect_to?url=" stringByAppendingString:escapedUrl];
	[webView loadRequest:[NSURLRequest requestWithURL: [NSURL URLWithString:redirector]]];
}

-(IBAction)goBack 
{
    const char* szBackUrl = rho_rhodesapp_getappbackurl();
    if ( szBackUrl && *szBackUrl )
        [self navigateRedirect:[NSString stringWithCString:szBackUrl encoding:[NSString defaultCStringEncoding]]];    
    else
	    [webView goBack];
}

-(IBAction)goForward {
	[webView goForward];
}

-(IBAction)goHome {
	const char* url = rho_rhodesapp_getstarturl();
	[self navigateRedirect:[NSString stringWithCString:url encoding:[NSString defaultCStringEncoding]]];
	//if (viewHomeUrl != NULL) {
	//	[self navigateRedirect:viewHomeUrl];
	//}
}

-(IBAction)goOptions {
	const char* url = rho_rhodesapp_getoptionsurl();
	[self navigateRedirect:[NSString stringWithCString:url encoding:[NSString defaultCStringEncoding]]];
	
	//if (viewOptionsUrl != NULL) {
	//	[self navigateRedirect:viewOptionsUrl];
	//}
}

-(IBAction)refresh {
	[webView reload];
	//const char* url = rho_rhodesapp_getcurrenturl();
	//[self navigateRedirect:[NSString stringWithCString:url encoding:[NSString defaultCStringEncoding]]];
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

- (void)webViewDidStartLoad:(UIWebView *)webview
{
	[self active];
}

- (void)webViewDidFinishLoad:(UIWebView *)webview
{
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


