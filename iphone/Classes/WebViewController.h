#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#include "SyncEngine.h"

@interface WebViewController : UIViewController <UIActionSheetDelegate> {
	IBOutlet UINavigationBar* navBar;
	IBOutlet UIBarButtonItem* homeBtn;
	IBOutlet UIBarButtonItem* backBtn;
	IBOutlet UIBarButtonItem* syncBtn;
	IBOutlet UILabel* activityInfo;
	IBOutlet UIWebView *webView;
	IBOutlet UIActivityIndicatorView *activity;
	NSString *viewHomeUrl;
}

@property (assign) NSString *viewHomeUrl;

-(void)navigate:(NSString*)url;

-(void)setActivityInfo:(NSString *)labelText;

-(IBAction)goBack;

-(IBAction)goHome;

-(void)active;

-(void)inactive;

-(void)refresh;

-(void)runSync;

@end
