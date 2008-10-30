#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#include "SyncEngine.h"

@interface WebViewController : UIViewController <UIActionSheetDelegate> {
	IBOutlet UINavigationBar* navBar;
	IBOutlet UINavigationItem* navItem;
	IBOutlet UIBarButtonItem* syncBtn;
	IBOutlet UIWebView *webView;
}

-(void)navigate:(NSString*)url;

-(void)runSync;

@end
