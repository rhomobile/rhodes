#import "WebViewController.h"
#include "rhoruby.h"

@implementation WebViewController

NSString *loadingText = @"Loading...";

@synthesize viewHomeUrl;

-(void)viewDidLoad {
	[super viewDidLoad];
	backBtn.enabled = NO;
}

-(void)navigate:(NSString*)url {
    printf("Navigating to the specifyed URL\n");
 	[webView loadRequest:[NSURLRequest requestWithURL: [NSURL URLWithString:url]]];
	if ([url isEqualToString:viewHomeUrl]) {
		backBtn.enabled = NO;
	} else {
		backBtn.enabled = YES;
	}
}

-(IBAction)goBack {
	[webView goBack];
}

-(IBAction)goHome {
	if (viewHomeUrl != NULL) {
		[self navigate:viewHomeUrl];
	}
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
	
	self.navigationItem.title = [webview stringByEvaluatingJavaScriptFromString:@"document.title"];

	//syncBtn = [[[UIBarButtonItem alloc]
//				initWithTitle:@"Sync"
//				style:UIBarButtonItemStyleBordered
//				target:self 
//				action:@selector(runSync:)] autorelease];
//	self.navigationItem.leftBarButtonItem = syncBtn;
}

-(void)refresh {
	[webView reload];
}

- (void)runSync
{
	wake_up_sync_engine();
}

@end
