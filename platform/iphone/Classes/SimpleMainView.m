//
//  SimpleMainView.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 07.03.10.
//  Copyright 2010 Rhomobile Inc. All rights reserved.
//

#import "SimpleMainView.h"
#import "AppManager.h"
#import "Rhodes.h"

#include "common/RhoConf.h"
#include "common/RhodesApp.h"
#include "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SimpleMainView"

#define RHO_TAG_WEBVIEW 1
#define RHO_TAG_TOOLBAR 2
#define RHO_TAG_NAVBAR 3

int rho_sys_get_screen_width();
int rho_sys_get_screen_height();

@interface RhoToolbarButtonItemAction : NSObject
{
    NSString *url;
}

@property (nonatomic,copy) NSString *url;

- (id)init:(NSString*)u;
- (void)dealloc;
- (void)onAction:(id)sender;

@end

@implementation RhoToolbarButtonItemAction

@synthesize url;

- (id)init:(NSString*)u {
    self.url = u;
    return self;
}

- (void)dealloc {
    self.url = nil;
    [super dealloc];
}

- (void)onAction:(id)sender {
    const char *s = [url UTF8String];
    rho_rhodesapp_load_url(s);
}

@end

@implementation SimpleMainView

@synthesize webView, toolbar, navbar, nativeViewType, nativeViewView;


-(CGRect)getContentRect {
	if (nativeViewView != nil) {
		return nativeViewView.frame;
	}
	return webView.frame;
}

-(void)setContentRect:(CGRect)rect {
	if (nativeViewView != nil) {
		nativeViewView.frame = rect;
		return;
	}
	webView.frame = rect;
}


- (UIBarButtonItem*)newButton:(NSString*)url label:(NSString*)label icon:(NSString*)icon {
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
    else if ([url compare:@"forward"] == NSOrderedSame) 
	{
		if ( !rho_conf_getBool("jqtouch_mode") )
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
        id action = [[RhoToolbarButtonItemAction alloc] init:url];
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
    
    return btn;
}

- (UIToolbar*)newToolbar:(NSArray*)items frame:(CGRect)mainFrame {
    if ([items count] % 4 != 0) {
        RAWLOG_ERROR("Illegal arguments for createNewToolbar");
        return nil;
    }
    
    UIToolbar *tb = [UIToolbar new];
    tb.barStyle = UIBarStyleBlack;//Opaque;
    
    [tb sizeToFit];
    
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
        int index = i*4 - 1;
        NSString *label = (NSString*)[items objectAtIndex:++index];
        NSString *url = (NSString*)[items objectAtIndex:++index];
        NSString *icon = (NSString*)[items objectAtIndex:++index];
        //NSString *reload = (NSString*)[items objectAtIndex:++index];
        
        if ([url length] == 0) {
            RAWLOG_ERROR("Illegal arguments for createNewToolbar");
            [tb release];
            [fixed release];
            return nil;
        }
        
        UIBarButtonItem *btn = [self newButton:url label:label icon:icon];
        
        if (btn) {
            [btns addObject:fixed];
            [btns addObject:btn];
            [btn release];
        }
    }
    
    [tb setItems:btns];
    
    [fixed release];
    
    tb.hidden = NO;
    tb.userInteractionEnabled = YES;
    tb.autoresizesSubviews = YES;
    tb.autoresizingMask = UIViewAutoresizingFlexibleLeftMargin | UIViewAutoresizingFlexibleTopMargin | UIViewAutoresizingFlexibleWidth;
 	
	assert([tb retainCount] == 1);
    return tb;
}

- (void)removeToolbar {
    if (toolbar) {
        CGRect wFrame = [self getContentRect];
        CGRect tbFrame = toolbar.frame;
        wFrame.size.height += tbFrame.size.height;
        [self setContentRect:wFrame];
    }
    
    [toolbar removeFromSuperview];
    assert(!toolbar || [toolbar retainCount] == 1);
    self.toolbar = nil;
}

- (void)addToolbar:(NSArray*)items {
    [self removeToolbar];
    assert(!toolbar);
    
    if (!items)
        return;
    
    CGRect wFrame = [self getContentRect];
	wFrame.size.height += wFrame.origin.y;
	wFrame.origin.y = 0;
    
    toolbar = [self newToolbar:items frame:wFrame];
    assert([toolbar retainCount] == 1);
    toolbar.tag = RHO_TAG_TOOLBAR;
    UIView* root = self.view;
    [root addSubview:toolbar];
    assert([toolbar retainCount] == 2);
    
    CGRect tbFrame = toolbar.frame;
	wFrame = [self getContentRect];
    wFrame.size.height -= tbFrame.size.height;
    [self setContentRect:wFrame];
}

- (UIWebView*)newWebView:(CGRect)frame {
    UIWebView *w = [[UIWebView alloc] initWithFrame:frame];
    w.scalesPageToFit = YES;
    w.userInteractionEnabled = YES;
    w.multipleTouchEnabled = YES;
    w.autoresizesSubviews = YES;
    w.clipsToBounds = NO;
    w.dataDetectorTypes = UIDataDetectorTypeNone;
    //w.delegate = self;
    w.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
    w.tag = RHO_TAG_WEBVIEW;
    
    assert([w retainCount] == 1);
    return w;
}

- (id)init:(UIView*)p webView:(UIWebView*)w frame:(CGRect)frame toolbar:(NSArray*)items {
	[self init];
    
    rootFrame = frame;
    
    UIView* root = self.view;
    
    assert(!webView || [webView retainCount] == 2);
    [webView removeFromSuperview];
    assert(!webView || [webView retainCount] == 1);
    self.webView = nil;
    webView = w;
    assert(!webView || [webView retainCount] == 1);
    if (!webView)
        webView = [self newWebView:frame];
    assert(webView && [webView retainCount] == 1);
    
	CGRect wFrame = frame;
    wFrame.origin.y = 0;
    webView.frame = wFrame;
    
    [root addSubview:webView];
    assert([webView retainCount] == 2);

    [self addToolbar:items];
    self.navbar = nil;
	nativeView = nil;
	nativeViewType = nil;
	nativeViewView = nil;
    
   // DO NOT REMOVE THIS LINE!!!
    // First call of self.view (when self.view is nil) trigger loadView
    // and viewDidLoad which add all our subviews to the root view
    NSLog(@"root view: %@", self.view);

    return self;
}

- (void)loadView {
    UIView* root = [[UIView alloc] init];
    root.frame = rootFrame;
    self.view = root;
    [root release];
    assert([root retainCount] == 1);
}

- (void) didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
	if ([self interfaceOrientation] == fromInterfaceOrientation) {
		return;
	}
	int width = rho_sys_get_screen_width();
	int height = rho_sys_get_screen_height();
	// send after rotate message
	//CGRect wFrame = [webView frame];
	int angle = 0;
	switch (fromInterfaceOrientation) {
		case UIInterfaceOrientationPortrait: {
			switch ([self interfaceOrientation]) {
				case UIInterfaceOrientationLandscapeLeft: {
					angle = 90;
				}
					break;
				case UIInterfaceOrientationPortraitUpsideDown: {
					angle = 180;
				}
					break;
				case UIInterfaceOrientationLandscapeRight: {
					angle = -90;
				}
					break;
			}
		}
		break;
		case UIInterfaceOrientationLandscapeLeft: {
			switch ([self interfaceOrientation]) {
				case UIInterfaceOrientationPortrait: {
					angle = -90;
				}
					break;
				case UIInterfaceOrientationPortraitUpsideDown: {
					angle = 90;
				}
					break;
				case UIInterfaceOrientationLandscapeRight: {
					angle = 180;
				}
					break;
			}
		}
		break;
		case UIInterfaceOrientationPortraitUpsideDown: {
			switch ([self interfaceOrientation]) {
				case UIInterfaceOrientationPortrait: {
					angle = 180;
				}
					break;
				case UIInterfaceOrientationLandscapeLeft: {
					angle = -90;
				}
					break;
				case UIInterfaceOrientationLandscapeRight: {
					angle = 90;
				}
					break;
			}
		}
		break;
		case UIInterfaceOrientationLandscapeRight: {
			switch ([self interfaceOrientation]) {
				case UIInterfaceOrientationPortrait: {
					angle = 90;
				}
					break;
				case UIInterfaceOrientationLandscapeLeft: {
					angle = 180;
				}
					break;
				case UIInterfaceOrientationPortraitUpsideDown: {
					angle = -90;
				}
					break;
			}
		}
		break;
	}
	if (([self interfaceOrientation] == UIInterfaceOrientationLandscapeLeft) || ([self interfaceOrientation] == UIInterfaceOrientationLandscapeRight)) {
		int t = width;
		width = height;
		height = t;
	}
	//rho_rhodesapp_callScreenRotationCallback((int)wFrame.size.width, (int)wFrame.size.height, angle);
	rho_rhodesapp_callScreenRotationCallback(width, height, angle);
}

- (id)initWithParentView:(UIView *)p frame:(CGRect)frame toolbar:(NSArray*)items {
    return [self init:p webView:nil frame:frame toolbar:items];
}

- (id)initWithParentView:(UIView *)p frame:(CGRect)frame {
    return [self initWithParentView:p frame:frame toolbar:nil];
}

- (id)initWithMainView:(id<RhoMainView>)v parent:(UIWindow*)p {
    return [self initWithMainView:v parent:p toolbar:nil];
}

- (id)initWithMainView:(id<RhoMainView>)v parent:(UIWindow*)p toolbar:(NSArray*)items {
    CGRect frame = [[v view] frame];
    //UIWebView *w = (UIWebView*)[Rhodes subviewWithTag:RHO_TAG_WEBVIEW ofView:[v view]];
    UIWebView *w = [v detachWebView];
    id result = [self init:p webView:w frame:frame toolbar:items];
    return result;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
}

- (void)viewDidLoad {
    UIView *root = self.view;
    
    root.userInteractionEnabled = YES;
 	root.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
	root.autoresizesSubviews = YES;
    
	if (nativeView) {
          [root addSubview:[nativeView getView]];
	}
	else {
		if (webView)
			[root addSubview:webView];
                         webView.delegate = self;
	}
	assert(!nativeView || [nativeView retainCount] == 2);
	assert(!webView || [webView retainCount] == 2);
    if (toolbar)
        [root addSubview:toolbar];
    assert(!toolbar || [toolbar retainCount] == 2);
    if (navbar)
        [root addSubview:navbar];
    assert(!navbar || [navbar retainCount] == 2);
}

- (void)viewDidUnload {
    [super viewDidUnload];
    //assert(!nativeView || [nativeView retainCount] == 1);
    assert(!webView || [webView retainCount] == 1);
    assert(!toolbar || [toolbar retainCount] == 1);
    assert(!navbar || [navbar retainCount] == 1);
}

- (void)dealloc {
     webView.delegate = nil;
    nativeView = nil;
    nativeViewView = nil;
    self.webView = nil;
    self.toolbar = nil;
    self.navbar = nil;
    [super dealloc];
}

// Toolbar handlers

- (void)goBack:(id)sender {
    rho_rhodesapp_navigate_back();
}

- (void)goForward:(id)sender {
	[self restoreWebView];
    [self forward:0];
}

- (void)goHome:(id)sender {
    const char* url = rho_rhodesapp_getstarturl();
    [self navigate:[NSString stringWithUTF8String:url] tab:0];
}

- (void)goOptions:(id)sender {
    const char *url = rho_rhodesapp_getoptionsurl();
    [self navigate:[NSString stringWithUTF8String:url] tab:0];
}

- (void)onRefresh:(id)sender {
    [self reload:0];
}


- (UIWebView*)detachWebView {
	[self restoreWebView];
	UIWebView *w = [webView retain];
	[w removeFromSuperview];
    webView.delegate = nil;
    self.webView = nil;
    
    assert(w && [w retainCount] == 1);
    return w;
}

- (void)loadHTMLString:(NSString *)data {
	[self restoreWebView];
    [webView loadHTMLString:data baseURL:[NSURL URLWithString:@""]];
}

- (void)back:(int)index {
	//if (nativeViewView != nil) {
		[self restoreWebView];
		//[webView setNeedsDisplay];
	//}
	//else {
		[webView goBack];
	//}
}

- (void)forward:(int)index {
	[self restoreWebView];
    [webView goForward];
}

- (NSString*)encodeUrl:(NSString*)url {
    // This decode/encode trick allow to work properly with urls which are already encoded
    // in the same manner as with those which are not. In case if 'url' is already encoded,
    // encodedUrl will be exactly the same as original one whereas if original url was not
    // encoded, encodedUrl will contain correct encoded version
    NSString *decodedUrl = [url stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    NSString *encodedUrl = [decodedUrl stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    return encodedUrl;
}

-(void)restoreWebView {
	UIView *root = self.view;
	self.nativeViewType = nil;
	if (nativeViewView != nil) {
		CGRect rect = [nativeViewView frame];
		[nativeViewView removeFromSuperview];
		nativeViewView = nil;
		
		webView.frame = rect;
		[root addSubview:webView];
	}
	if (nativeView != nil) {
		[RhoNativeViewManagerOC destroyNativeView:nativeView];
		nativeView = nil;
	}
}

- (NSString*)processForNativeView:(NSString*)url {

	//stringWithUTF8String
	NSRange range = [url rangeOfString:@":"];
	NSRange callback_range = [url rangeOfString:@"call_stay_native:"];
	UIView *root = self.view;
	if ((callback_range.location == 0) && (callback_range.length > 0)) {
		NSString* cleared_url = [url substringFromIndex:callback_range.length];
		return cleared_url;
	}
	else {
		if((range.location >= 0) && (range.length > 0)) {
			NSString* protocol = [url substringToIndex:range.location];
			NSString* navto = [url substringFromIndex:(range.location+1)];
			if ([protocol compare:self.nativeViewType] == NSOrderedSame) {
				// just navigate
				if (nativeView != nil) {
					[nativeView navigate:navto];
				}
				return nil;
			}
			else {
				// try to make native view
				id<NativeViewOC,NSObject> nv = [RhoNativeViewManagerOC getNativeView:protocol];
				if (nv != nil) {
					[self restoreWebView];
					nativeView = nv;
					self.nativeViewType = [[NSString alloc] initWithString:protocol] ;
					// replace webView with NativeView
					nativeViewView = [nativeView getView];
					if (nativeViewView != nil) {
						CGRect rect = [webView frame];
						[webView removeFromSuperview];
						nativeViewView.frame = rect;
						//w.userInteractionEnabled = YES;
						//w.multipleTouchEnabled = YES;
						nativeViewView.autoresizesSubviews = YES;
						nativeViewView.clipsToBounds = NO;
						//w.delegate = self;
						nativeViewView.autoresizingMask = UIViewAutoresizingFlexibleHeight | UIViewAutoresizingFlexibleWidth;
						[root addSubview:nativeViewView];
					}
					[nativeView navigate:navto];
					return nil;
				}
				else {
					[self restoreWebView];
					return url;
				}
			}
		}
		else {
			[self restoreWebView];
			return url;
		}

	}
}

- (void)navigate:(NSString *)url tab:(int)index {
    NSString *encodedUrl = [self encodeUrl:url];
	NSString* cleared_url = [self processForNativeView:encodedUrl];
	if (cleared_url == nil) {
		return;
	}
	else {
		//NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:encodedUrl]];
		NSURLRequest *request = [[NSMutableURLRequest alloc] initWithURL:[NSURL URLWithString:cleared_url]];
		[webView loadRequest:request];
		[request release];
	}
}

- (void)navigateRedirect:(NSString *)url tab:(int)index {
    NSString *encodedUrl = [self encodeUrl:url];
	NSString* cleared_url = [self processForNativeView:encodedUrl];
	if (cleared_url == nil) {
		return;
	}
	else {
		NSString* homeurl = [NSString stringWithUTF8String:rho_rhodesapp_gethomeurl()];
		NSString *redirect = [NSString stringWithFormat:@"%@/system/redirect_to?url=%@", homeurl, cleared_url];
		//[self navigate:redirect tab:index];
		NSURLRequest *request = [[NSMutableURLRequest alloc] initWithURL:[NSURL URLWithString:redirect]];
		[webView loadRequest:request];
		[request release];
	}
}

- (void)reload:(int)index {
	if (nativeViewView != nil) {
		[nativeViewView setNeedsDisplay];
	}
	else {
		[webView reload];
	}
}

- (void)executeJs:(NSString*)js tab:(int)index {
    RAWLOG_INFO1("Executing JS: %s", [js UTF8String]);
    [webView stringByEvaluatingJavaScriptFromString:js];
}

- (NSString*)currentLocation:(int)index {
    return [[webView.request mainDocumentURL] absoluteString];
}

- (void)switchTab:(int)index {
    // Nothing
}

- (int)activeTab {
    return 0;
}


- (void)addNavBar:(UINavigationBar*)navb {
    [self removeNavBar];
    
    navbar = navb;
    assert([navbar retainCount] == 1);
    NSLog(@"navbar retain count: %d", [navbar retainCount]);
	navbar.autoresizingMask = UIViewAutoresizingFlexibleWidth;
	navbar.autoresizesSubviews = YES;
	
	UIView* root = self.view;
    [root addSubview:navbar];
    assert([navbar retainCount] == 2);
    
    CGRect nFrame = navbar.frame;
    CGRect wFrame = [self getContentRect];
    wFrame.origin.y += nFrame.size.height;
    wFrame.size.height -= nFrame.size.height;
    [self setContentRect:wFrame];
}

- (void)addNavBar:(NSString*)title left:(NSArray*)left right:(NSArray*)right {
    UINavigationBar *nb = [[UINavigationBar alloc] initWithFrame:CGRectZero];
    nb.tag = RHO_TAG_NAVBAR;
    [nb sizeToFit];
    
    UINavigationItem *ni = [[UINavigationItem alloc] initWithTitle:title];
    
    NSArray *btns[] = {left, right};
    for (int i = 0, lim = sizeof(btns)/sizeof(btns[0]); i < lim; ++i) {
        NSArray *btn = btns[i];
        if ([btn count] < 3)
            continue;
        NSString *action = [btn objectAtIndex:0];
        NSString *label = [btn objectAtIndex:1];
        NSString *icon = [btn objectAtIndex:2];
        UIBarButtonItem *button = [self newButton:action label:label icon:icon];
        
        if (btn == left)
            [ni setLeftBarButtonItem:button];
        else
            [ni setRightBarButtonItem:button];
    }
    
    [nb pushNavigationItem:ni animated:NO];
	
	[self addNavBar:nb];
}

- (void)removeNavBar {
    if (navbar) {
        CGRect nFrame = navbar.frame;
        CGRect wFrame = [self getContentRect];
        wFrame.origin.y -= nFrame.size.height;
        wFrame.size.height += nFrame.size.height;
        [self setContentRect:wFrame];
    }

    [navbar removeFromSuperview];
    assert(!navbar || [navbar retainCount] == 1);
    self.navbar = nil;
}

// UIWebViewDelegate imlementation

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request
 navigationType:(UIWebViewNavigationType)navigationType {
    NSURL *url = [request URL];
    if (!url)
        return NO;
    if (![url.scheme isEqualToString:@"http"] && ![url.scheme isEqualToString:@"https"]) {
        // This is not http url so try to open external application for it
        [[UIApplication sharedApplication] openURL:url];
        return NO;
    }
    
    // Retrieve cookie for http url
    NSString *c = [[Rhodes sharedInstance] cookie:[url absoluteString]];
    if (c && [request isKindOfClass:[NSMutableURLRequest class]]) {
        NSMutableURLRequest *r = (NSMutableURLRequest*)request;
        [r addValue:c forHTTPHeaderField:@"Cookie"];
    }
    return YES;
}

- (void)webViewDidStartLoad:(UIWebView *)webview {
    // TODO
    //[self active];
}

- (void)webViewDidFinishLoad:(UIWebView *)webview {
    // Disable default context menu on touch
    [webview stringByEvaluatingJavaScriptFromString:@"document.documentElement.style.webkitTouchCallout = \"none\";"];
    
    // Set empty application cache. Otherwise memory used by UIWebView increased rapidly
    // and finally application got out of memory
    NSURLCache *sharedCache = [[NSURLCache alloc] initWithMemoryCapacity:0 diskCapacity:0 diskPath:nil];
	[NSURLCache setSharedURLCache:sharedCache];
	[sharedCache release];
    
    // TODO
    /*
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
     */
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error {
    // TODO
}

@end
