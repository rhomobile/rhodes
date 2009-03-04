#import "WebViewController.h"
#include "rhoruby.h"

static char currentLocation[4096] = "";

@implementation WebViewController

NSString *loadingText = @"Loading...";

@synthesize viewHomeUrl, viewOptionsUrl;

-(void)viewDidLoad {
	[super viewDidLoad];
}

-(void)navigate:(NSString*)url {
    printf("Navigating to the specifyed URL\n");
	[webView loadRequest:[NSURLRequest requestWithURL: [NSURL URLWithString:url]]];
}

-(IBAction)goBack {
	[webView goBack];
}

-(IBAction)goForward {
	[webView goForward];
}

-(IBAction)goHome {
	if (viewHomeUrl != NULL) {
		[self navigate:viewHomeUrl];
	}
}

-(IBAction)goOptions {
	if (viewOptionsUrl != NULL) {
		[self navigate:viewOptionsUrl];
	}
}

-(IBAction)refresh {
	[webView reload];
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
	// starting the load, show the activity indicator in the status bar
	[UIApplication sharedApplication].isNetworkActivityIndicatorVisible = YES;
	[self active];
}

- (void)webViewDidFinishLoad:(UIWebView *)webview
{
	// finished loading, hide the activity indicator in the status bar
	[UIApplication sharedApplication].isNetworkActivityIndicatorVisible = NO;
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
	
	NSString* location = [webview stringByEvaluatingJavaScriptFromString:@"location.href"];
	CFStringGetCString((CFStringRef)location, currentLocation, sizeof(currentLocation), CFStringGetSystemEncoding());
	char* fragment = strstr(currentLocation,"#");
	if (fragment) *fragment = 0; //cut out fragment
	printf("Current location: %s\n",currentLocation);
	
	/*self.navigationItem.title = [webview stringByEvaluatingJavaScriptFromString:@"document.title"];
	 
	 syncBtn = [[[UIBarButtonItem alloc]
	 initWithTitle:@"Sync"
	 style:UIBarButtonItemStyleBordered
	 target:self 
	 action:@selector(runSync:)] autorelease];
	 self.navigationItem.leftBarButtonItem = syncBtn;*/
	
}

- (void)runSync
{
	wake_up_sync_engine();
}

@end

char* get_current_location() {
	return currentLocation;
}

