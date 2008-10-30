#import "WebViewController.h"

@implementation WebViewController

- (void)viewDidLoad {
	[super viewDidLoad];
}

-(void)navigate:(NSString*)url {
    printf("Navigating to the specifyed URL\n");
 	[webView loadRequest:[NSURLRequest requestWithURL: [NSURL URLWithString:url]]];
}

- (void)webViewDidStartLoad:(UIWebView *)webview
{
	// starting the load, show the activity indicator in the status bar
	[UIApplication sharedApplication].isNetworkActivityIndicatorVisible = YES;
}

- (void)webViewDidFinishLoad:(UIWebView *)webview
{
	// finished loading, hide the activity indicator in the status bar
	[UIApplication sharedApplication].isNetworkActivityIndicatorVisible = NO;
	
	//navItem.title = [webview stringByEvaluatingJavaScriptFromString:@"document.title"];
	syncBtn = [[[UIBarButtonItem alloc]
				initWithTitle:@"Sync"
				style:UIBarButtonItemStyleBordered
				target:self 
				action:@selector(runSync:)] autorelease];
	self.navigationItem.leftBarButtonItem = syncBtn;
}

- (void)runSync
{
	wake_up_sync_engine();
}

@end
