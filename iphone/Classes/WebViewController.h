#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>

@interface WebViewController : UIViewController <UIActionSheetDelegate> {
	IBOutlet UIWebView *webView;
}

-(void)navigate:(NSString*)url;

@end
