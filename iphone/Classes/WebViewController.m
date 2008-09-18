#import <syslog.h>

#import "WebViewController.h"

@implementation WebViewController

- (void)viewDidLoad {
	[super viewDidLoad];
    printf("Navigating to the specifyed URL\n");
 	[webView loadRequest:[NSURLRequest requestWithURL: [NSURL URLWithString:@"http://localhost:8080/"]]];
}

@end
