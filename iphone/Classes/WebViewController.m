#import "WebViewController.h"
#include "rhoruby.h"

@implementation WebViewController

NSString *loadingText = @"Loading...";

-(void)viewDidLoad {
	[super viewDidLoad];
	backBtn.enabled = NO;
}

-(void)navigate:(NSString*)url {
	[self active];
	[self setActivityInfo:loadingText];
    printf("Navigating to the specifyed URL\n");
 	[webView loadRequest:[NSURLRequest requestWithURL: [NSURL URLWithString:url]]];
	[self inactive];
	backBtn.enabled = YES;
}

-(IBAction)goBack {
	[webView goBack];
}

-(IBAction)goHome {
	NSString* ref_start_page = [NSString stringWithCString:callGetStartPage() 
												  encoding:NSUTF8StringEncoding];
	[self navigate:ref_start_page];
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
	[self setActivityInfo:loadingText];
	[self active];
}

- (void)webViewDidFinishLoad:(UIWebView *)webview
{
	// finished loading, hide the activity indicator in the status bar
	[UIApplication sharedApplication].isNetworkActivityIndicatorVisible = NO;
	[self setActivityInfo:nil];
	[self inactive];
	
	self.navigationItem.title = [webview stringByEvaluatingJavaScriptFromString:@"document.title"];
}

@end
