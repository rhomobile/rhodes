#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
//#include "SyncEngine.h"

@interface WebViewController : UIViewController <UIActionSheetDelegate, UIWebViewDelegate> {
    UINavigationBar* navBar;
    UIToolbar* toolbar;
    UIBarButtonItem* backBtn;
    UIBarButtonItem* forwardBtn;
    UIBarButtonItem* homeBtn;
    UIBarButtonItem* refreshBtn;
    UIBarButtonItem* optionsBtn;
    UILabel* activityInfo;
    UIWebView *webView;
    UIActivityIndicatorView *activity;
    //NSString *viewHomeUrl,*viewOptionsUrl;

@public
    id   actionTarget;
    SEL  onShowLog;
    UIWindow *window;
}

//@property (assign) NSString *viewHomeUrl;
//@property (assign) NSString *viewOptionsUrl;

@property (assign) id   actionTarget;
@property (assign) UIToolbar* toolbar;
@property (assign) UIWebView* webView;
@property (assign) SEL  onShowLog;

-(id)initWithParentWindow:(UIWindow*)w;

-(void)loadData:(NSData*)data mimeType:(NSString*)type;

-(void)showToolbar:(BOOL)show;
-(void)createNewToolbar:(NSArray*)items;

-(void)navigate:(NSString*)url;
-(void)navigateRedirect:(NSString*)url;

-(void)setActivityInfo:(NSString *)labelText;

-(void)active;

-(void)inactive;

//-(void)runSync;

@end
