#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@interface WebViewController : UIViewController <UIActionSheetDelegate> {
	IBOutlet UINavigationBar* navBar;
	IBOutlet UINavigationItem* navItem;
	IBOutlet UIBarButtonItem* backBtn;
	IBOutlet UIWebView *webView;
}

-(void)navigate:(NSString*)url;

@end
