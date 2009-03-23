#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#include "SyncEngine.h"

@interface WebViewController : UIViewController <UIActionSheetDelegate> {
	IBOutlet UINavigationBar* navBar;
	IBOutlet UIBarButtonItem* homeBtn;
	IBOutlet UIBarButtonItem* optionsBtn;
	IBOutlet UIBarButtonItem* backBtn;
	//IBOutlet UIBarButtonItem* syncBtn;
	IBOutlet UIBarButtonItem* forwardBtn;
	IBOutlet UILabel* activityInfo;
	IBOutlet UIWebView *webView;
	IBOutlet UIActivityIndicatorView *activity;
	NSString *viewHomeUrl,*viewOptionsUrl;
}

@property (assign) NSString *viewHomeUrl;
@property (assign) NSString *viewOptionsUrl;

-(void)navigate:(NSString*)url;
-(void)navigateRedirect:(NSString*)url;

-(void)setActivityInfo:(NSString *)labelText;

-(IBAction)goBack;

-(IBAction)goForward;

-(IBAction)goHome;

-(IBAction)goOptions;

-(IBAction)refresh;

-(void)active;

-(void)inactive;

-(void)runSync;

@end
