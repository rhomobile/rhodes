#import "WebViewController.h"
#import "AppManager.h"
//#import "ruby/ext/rho/rhoruby.h"
//#import "UniversalLock.h"
#import "common/RhoConf.h"
//#import "sync/syncthread.h"
#import "common/RhodesApp.h"
#import "logging/RhoLog.h"
#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "WebViewCtrl"
/*
static char currentLocation[4096] = "";

//INIT_LOCK(current_location); // racing condition on thid lock, need to fix it somehow

void set_current_location(CFStringRef location) {
	//LOCK(current_location);
	CFStringGetCString((CFStringRef)location, currentLocation, sizeof(currentLocation), CFStringGetSystemEncoding());
	
	char* fragment = strstr(currentLocation,"#");
	if (fragment) *fragment = 0; //cut out fragment
	RAWLOG_INFO1("Current location: %s",currentLocation);
	//UNLOCK(current_location);

	if (rho_conf_getBool("KeepTrackOfLastVisitedPage")) {
		rho_conf_setString("LastVisitedPage",currentLocation);
		rho_conf_save();
	}
	
}

char* get_current_location() {
	//LOCK(current_location);
	return currentLocation;
	//UNLOCK(current_location);
}*/

extern int webview_active_tab();

@interface UIBarButtonItemAction : NSObject
{
    WebViewController *wc;
    NSString *url;
    bool callback;
}

@property (nonatomic,copy) NSString *url;

- (id)init:(WebViewController*)w url:(NSString*)u callback:(bool)c;
- (void)onAction:(id)sender;

@end

@implementation UIBarButtonItemAction

@synthesize url;

- (id)init:(WebViewController *)w url:(NSString*)u callback:(bool)c {
    wc = w;
    self.url = u;
    callback = c;
    return self;
}

- (void)doAction {
    if (callback)
        rho_net_request([url UTF8String]);
    else
        [wc navigate:url];
}

- (void)onAction:(id)sender {
    [self performSelectorOnMainThread:@selector(doAction) withObject:nil waitUntilDone:NO];
}

@end


@implementation WebViewController

//@synthesize viewHomeUrl, viewOptionsUrl;
@synthesize actionTarget, onShowLog, toolbar, webView;

- (void)dealloc {
    [backBtn release];
    [forwardBtn release];
    [homeBtn release];
    [refreshBtn release];
    [optionsBtn release];
    [activity release];
    [toolbar release];
    [super dealloc];
}

- (void)createNewToolbar:(NSArray*)items {
    if ([items count] % 4 != 0) {
        RAWLOG_ERROR("Illegal arguments for createNewToolbar");
        return;
    }
    
    UIToolbar *tb = [UIToolbar new];
    tb.barStyle = UIBarStyleBlackOpaque;
    
    [tb sizeToFit];
    
    CGRect mainFrame = window.frame;
    
    CGFloat tbHeight = [tb frame].size.height;
    CGRect tbFrame = CGRectMake(CGRectGetMinX(mainFrame),
                                CGRectGetHeight(mainFrame) - tbHeight,
                                CGRectGetWidth(mainFrame),
                                tbHeight);
    [tb setFrame:tbFrame];
    
    UIBarButtonItem *fixed = [[UIBarButtonItem alloc]
                              initWithBarButtonSystemItem:UIBarButtonSystemItemFixedSpace
                              target:nil action:nil];

    NSMutableArray *btns = [NSMutableArray arrayWithCapacity:[items count]/4];
    for(int i = 0, lim = [items count]/4; i < lim; i++) {
        int index = i*4;
        NSString *label = (NSString*)[items objectAtIndex:index++];
        NSString *url = (NSString*)[items objectAtIndex:index++];
        NSString *icon = (NSString*)[items objectAtIndex:index++];
        //NSString *reload = (NSString*)[items objectAtIndex:index++];
        
        if ([url length] == 0) {
            RAWLOG_ERROR("Illegal arguments for createNewToolbar");
            return;
        }
        
        UIImage *img = nil;
        if ([icon length] > 0) {
            NSString *imagePath = [[AppManager getApplicationsRootPath] stringByAppendingPathComponent:icon];
            img = [UIImage imageWithContentsOfFile:imagePath];
        }
        
        UIBarButtonItem *btn = nil;
        if ([url compare:@"back"] == NSOrderedSame) {
            btn = [[UIBarButtonItem alloc]
                    initWithImage:(img ? img : [UIImage imageNamed:@"back_btn.png"])
                    style:UIBarButtonItemStylePlain target:self
                    action:@selector(goBack:)];
        }
        else if ([url compare:@"forward"] == NSOrderedSame) {
            btn = [[UIBarButtonItem alloc]
                   initWithImage:(img ? img : [UIImage imageNamed:@"forward_btn.png"])
                   style:UIBarButtonItemStylePlain target:self
                   action:@selector(goForward:)];
        }
        else if ([url compare:@"home"] == NSOrderedSame) {
            btn = [[UIBarButtonItem alloc]
                   initWithImage:(img ? img : [UIImage imageNamed:@"home_btn.png"])
                   style:UIBarButtonItemStylePlain target:self
                   action:@selector(goHome:)];
        }
        else if ([url compare:@"options"] == NSOrderedSame) {
            btn = [[UIBarButtonItem alloc]
                   initWithImage:(img ? img : [UIImage imageNamed:@"gears.png"])
                   style:UIBarButtonItemStylePlain target:self
                   action:@selector(goOptions:)];
        }
        else if ([url compare:@"refresh"] == NSOrderedSame) {
            if (img)
                btn = [[UIBarButtonItem alloc]
                       initWithImage:img
                       style:UIBarButtonItemStylePlain target:self
                       action:@selector(onRefresh:)];
            else
                btn = [[UIBarButtonItem alloc]
                       initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh
                       target:self action:@selector(onRefresh:)];
        }
        else if ([url compare:@"separator"] == NSOrderedSame) {
            btn = [[UIBarButtonItem alloc]
                   initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace
                   target:nil action:nil];
        }
        else {
            bool callback = false;
            if ([url length] > 9 && [[url substringToIndex:9] isEqual:@"callback:"]) {
                callback = true;
                url = [url substringFromIndex:9];
            }
            NSString *u = [NSString stringWithUTF8String:rho_http_normalizeurl([url UTF8String])];
            UIBarButtonItemAction *action = [[UIBarButtonItemAction alloc] init:self url:u callback:callback];
            if (img) {
                btn = [[UIBarButtonItem alloc]
                       initWithImage:img style:UIBarButtonItemStylePlain
                       target:action action:@selector(onAction:)];
            }
            else if ([label length] > 0) {
                btn = [[UIBarButtonItem alloc]
                       initWithTitle:label style:UIBarButtonItemStylePlain
                       target:action action:@selector(onAction:)];
            }
        }
        
        if (btn) {
            [btns addObject:fixed];
            [btns addObject:btn];
        }
    }
    
    [tb setItems:btns];
    
    [fixed release];
    
    [toolbar removeFromSuperview];
    toolbar = tb;
    toolbar.hidden = YES;
    [window addSubview:toolbar];
}

- (UIToolbar*)newToolbar:(CGRect)mainFrame {
    UIToolbar *tb = [UIToolbar new];
    tb.barStyle = UIBarStyleBlackOpaque;
    
    [tb sizeToFit];
    
    CGFloat tbHeight = [tb frame].size.height;
    CGRect tbFrame = CGRectMake(CGRectGetMinX(mainFrame),
                                CGRectGetHeight(mainFrame) - tbHeight,
                                CGRectGetWidth(mainFrame),
                                tbHeight);
    [tb setFrame:tbFrame];
    
    backBtn = [[UIBarButtonItem alloc]
               initWithImage:[UIImage imageNamed:@"back_btn.png"]
               style:UIBarButtonItemStylePlain target:self
               action:@selector(goBack:)];
    
    forwardBtn = [[UIBarButtonItem alloc]
                  initWithImage:[UIImage imageNamed:@"forward_btn.png"]
                  style:UIBarButtonItemStylePlain target:self
                  action:@selector(goForward:)];
    
    homeBtn = [[UIBarButtonItem alloc]
               initWithImage:[UIImage imageNamed:@"home_btn.png"]
               style:UIBarButtonItemStylePlain target:self
               action:@selector(goHome:)];
    
    refreshBtn = [[UIBarButtonItem alloc]
                  initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh
                  target:self action:@selector(onRefresh:)];
    
    optionsBtn = [[UIBarButtonItem alloc]
                  initWithImage:[UIImage imageNamed:@"gears.png"]
                  style:UIBarButtonItemStylePlain target:self
                  action:@selector(goOptions:)];
    
    activity = [[UIActivityIndicatorView alloc]
                initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhite];
    activity.hidesWhenStopped = YES;
    
    UIBarButtonItem *fixedSpace = [[UIBarButtonItem alloc]
                                   initWithBarButtonSystemItem:UIBarButtonSystemItemFixedSpace
                                   target:nil action:nil];
    
    UIBarButtonItem *flexSpace = [[UIBarButtonItem alloc]
                                  initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace
                                  target:nil action:nil];
    
    NSArray *items = [NSArray arrayWithObjects:backBtn, fixedSpace, forwardBtn, flexSpace,
                      homeBtn, fixedSpace, refreshBtn, fixedSpace, optionsBtn, nil];
    [tb setItems:items];
    
    [fixedSpace release];
    [flexSpace release];
    
    return tb;
}

- (id)initWithParentWindow:(UIWindow*)w {
    window = w;
    
    CGRect mainFrame = window.frame;
    
    toolbar = [self newToolbar:window.frame];
    toolbar.hidden = YES;
    [window addSubview:toolbar];
    
    CGRect tbFrame = toolbar.frame;
    
    CGRect wvFrame = CGRectMake(CGRectGetMinX(mainFrame), CGRectGetMinY(mainFrame),
                                CGRectGetWidth(mainFrame),
                                CGRectGetHeight(mainFrame) - CGRectGetHeight(tbFrame));
    webView = [[UIWebView alloc] initWithFrame:wvFrame];
    webView.scalesPageToFit = YES;
    webView.userInteractionEnabled = YES;
    //webView.detectsPhoneNumbers = YES;
    webView.multipleTouchEnabled = YES;
    webView.autoresizesSubviews = YES;
    webView.delegate = self;
    [window addSubview:webView];
    
    return self;
}

- (void)viewDidAppear:(BOOL)animated
{
	[super viewDidAppear:animated];
}

-(void)viewDidLoad {
	[super viewDidLoad];

    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	bool logEnabled = [defaults boolForKey:@"log_enabled_preference"];
	
	if (logEnabled && toolbar) {
		NSArray *items = toolbar.items;

		// create a bordered style button with custom title
		UIBarButtonItem *item = [[UIBarButtonItem alloc] 
			initWithTitle:@"Log"
			style:UIBarButtonItemStyleBordered	
			target:self
			action:@selector(actionShowLog:)];
		items = [items arrayByAddingObject:item];
		
		[toolbar setItems:items animated: YES];
		[item release];
	}
}

-(void)loadData:(NSData*)data mimeType:(NSString*)type {
    [webView loadData:data MIMEType:type textEncodingName:@"utf-8" baseURL:[NSURL URLWithString:@""]];
}

-(void)showToolbar:(BOOL)show {
    toolbar.hidden = !show;
    CGRect frame = window.frame;
    if (show) {
        [window sendSubviewToBack:webView];
        [window bringSubviewToFront:toolbar];
        CGRect tbFrame = toolbar.frame;
        frame.size.height -= tbFrame.size.height;
    }
    else {
        [window sendSubviewToBack:toolbar];
        [window bringSubviewToFront:webView];
    }
    [webView sizeToFit];
    [webView setFrame:frame];
}

-(void)navigate:(NSString*)url {
    //RAWLOG_INFO1("Navigating to the specifyed URL: %s",  [url cStringUsingEncoding:[NSString defaultCStringEncoding]]);
    NSString *escapedUrl = [url stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
	[webView loadRequest:[NSURLRequest requestWithURL: [NSURL URLWithString:escapedUrl]]];
}

-(void)executeJs:(JSString*)js {
	RAWLOG_INFO1("Executing JS: %s", [js.inputJs UTF8String] );
	js.outputJs = [webView stringByEvaluatingJavaScriptFromString:js.inputJs];
}

-(void)navigateRedirect:(NSString*)url {
    //RAWLOG_INFO1("Navigate (redirect) to: %s", [url cStringUsingEncoding:[NSString defaultCStringEncoding]]);
	NSString* escapedUrl = [url stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
	NSString* homeurl = [NSString stringWithCString:rho_rhodesapp_gethomeurl() encoding:[NSString defaultCStringEncoding]];
	NSString* redirector1 = [@"/system/redirect_to?url=" stringByAppendingString:escapedUrl];
	NSString* redirector = [homeurl stringByAppendingString:redirector1];
	[webView loadRequest:[NSURLRequest requestWithURL: [NSURL URLWithString:redirector]]];
}

-(void)goBack:(id)sender {
    const char* szBackUrl = rho_rhodesapp_getappbackurl();
    if ( szBackUrl && *szBackUrl )
        [self navigateRedirect:[NSString stringWithCString:szBackUrl encoding:[NSString defaultCStringEncoding]]];    
    else
	    [webView goBack];
}

-(void)goForward:(id)sender {
	[webView goForward];
}

-(void)goHome:(id)sender {
	const char* url = rho_rhodesapp_getstarturl();
	[self navigateRedirect:[NSString stringWithCString:url encoding:[NSString defaultCStringEncoding]]];
	//if (viewHomeUrl != NULL) {
	//	[self navigateRedirect:viewHomeUrl];
	//}
}

-(void)goOptions:(id)sender {
	const char* url = rho_rhodesapp_getoptionsurl();
	[self navigateRedirect:[NSString stringWithCString:url encoding:[NSString defaultCStringEncoding]]];
	
	//if (viewOptionsUrl != NULL) {
	//	[self navigateRedirect:viewOptionsUrl];
	//}
}

-(void)refresh {
    //[webView reload];
    const char *appUrl = rho_rhodesapp_getcurrenturl(webview_active_tab());
    [self navigate:[NSString stringWithUTF8String:appUrl]];
}

-(void)onRefresh:(id)sender {
    [self refresh];
}

-(void)setActivityInfo:(NSString *)labelText {
	if (!labelText) {
		activityInfo.hidden = YES;
	} else {
		activityInfo.hidden = NO;
		[activityInfo setText:labelText];
	}
}

-(void)active {
	[activity startAnimating];
	activity.hidden = NO;
}

-(void)inactive {
	[activity stopAnimating];
	activity.hidden = YES;
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request
                navigationType:(UIWebViewNavigationType)navigationType {
    return YES;
}

- (void)webViewDidStartLoad:(UIWebView *)webview
{
	[self active];
}

- (void)webViewDidFinishLoad:(UIWebView *)webview
{
	[self inactive];
	
	if ([webView canGoBack]) {
		backBtn.enabled = YES;
	} else {
		backBtn.enabled = NO;
	}
	if ([webView canGoForward]) {
		forwardBtn.enabled = YES;
	} else {
		forwardBtn.enabled = NO;
	}
    
	//NSString* location = [webview stringByEvaluatingJavaScriptFromString:@"location.href"];
	//rho_rhodesapp_keeplastvisitedurl( [location cStringUsingEncoding:[NSString defaultCStringEncoding]] );									 
    
    if ([actionTarget respondsToSelector:@selector(hideSplash)])
        [actionTarget performSelectorOnMainThread:@selector(hideSplash) withObject:nil waitUntilDone:NO];
}
/*
- (void)runSync
{
	rho_sync_doSyncAllSources(TRUE);
}*/

- (void)actionShowLog:(id)sender
{
	if(actionTarget && [actionTarget respondsToSelector:onShowLog]) {
		[actionTarget performSelector:onShowLog];
	}	
}

@end


