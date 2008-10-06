#import <syslog.h>

#import "WebViewController.h"

@implementation WebViewController

- (void)viewDidLoad {
	[super viewDidLoad];
}

-(void)navigate:(NSString*)url {
    printf("Navigating to the specifyed URL\n");
 	[webView loadRequest:[NSURLRequest requestWithURL: [NSURL URLWithString:url]]];
}

@end
