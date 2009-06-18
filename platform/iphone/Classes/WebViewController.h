#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
//#include "SyncEngine.h"

@interface WebViewController : UIViewController <UIActionSheetDelegate> {
	IBOutlet UINavigationBar* navBar;
	IBOutlet UIToolbar* toolbar;
	IBOutlet UIBarButtonItem* homeBtn;
	IBOutlet UIBarButtonItem* optionsBtn;
	IBOutlet UIBarButtonItem* backBtn;
	//IBOutlet UIBarButtonItem* syncBtn;
	IBOutlet UIBarButtonItem* forwardBtn;
	IBOutlet UILabel* activityInfo;
	IBOutlet UIWebView *webView;
	IBOutlet UIActivityIndicatorView *activity;
	NSString *viewHomeUrl,*viewOptionsUrl;
	
@public
	id   actionTarget;
	SEL  onShowLog;
}

@property (assign) NSString *viewHomeUrl;
@property (assign) NSString *viewOptionsUrl;

@property (assign) id   actionTarget;
@property (assign) SEL  onShowLog;


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
