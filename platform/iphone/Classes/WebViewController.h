#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "JSString.h"
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
@property (assign) UIToolbar* toolbar;
@property (assign) UIWebView* webView;
@property (assign) SEL  onShowLog;


-(void)loadHTMLString:(NSString*)data;

-(void)navigate:(NSString*)url;
-(void)navigateRedirect:(NSString*)url;

-(void)setActivityInfo:(NSString *)labelText;

-(IBAction)goBack;

-(IBAction)goForward;

-(IBAction)goHome;

-(IBAction)goOptions;

-(IBAction)refresh;

-(void)executeJs:(JSString *)js;

-(void)active;

-(void)inactive;

-(void)runSync;

@end
