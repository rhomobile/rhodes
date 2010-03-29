    //
//  SimpleMainView.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 07.03.10.
//  Copyright 2010 Rhomobile Inc. All rights reserved.
//

#import "SimpleMainView.h"
#import "AppManager.h"
#import "common/RhodesApp.h"
#import "logging/RhoLog.h"

#undef DEFAULT_LOGCATEGORY
#define DEFAULT_LOGCATEGORY "SimpleMainView"

@interface RhoToolbarButtonItemAction : NSObject
{
    id<RhoMainView> view;
    NSString *url;
}

@property (nonatomic,retain) id<RhoMainView> view;
@property (nonatomic,copy) NSString *url;

- (id)init:(id<RhoMainView>)v url:(NSString*)u;
- (void)dealloc;
- (void)onAction:(id)sender;

@end

@implementation RhoToolbarButtonItemAction

@synthesize view,url;

- (id)init:(id<RhoMainView>)v url:(NSString*)u {
    self.view = v;
    self.url = u;
    return self;
}

- (void)dealloc {
    self.view = nil;
    self.url = nil;
    [super dealloc];
}

- (void)onAction:(id)sender {
    [view navigate:url tab:[view activeTab]];
}

@end

@implementation SimpleMainView

@synthesize webView, toolbar;

- (void)createToolbar:(NSArray*)items {
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
            NSString *u = [NSString stringWithUTF8String:rho_http_normalizeurl([url UTF8String])];
            id action = [[RhoToolbarButtonItemAction alloc] init:self url:u];
            if (!img) {
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
    
    toolbar = tb;
    toolbar.hidden = NO;
    [window addSubview:toolbar];
}

- (id)initWithParentWindow:(UIWindow *)w andDelegate:(id)delegate {
    return [self initWithParentWindow:w toolbar:nil andDelegate:delegate];
}

- (id)initWithParentWindow:(UIWindow *)w toolbar:(NSArray*)items andDelegate:(id)delegate {
    window = w;
    
    webView = [[UIWebView alloc] initWithFrame:window.frame];
    webView.scalesPageToFit = YES;
    webView.userInteractionEnabled = YES;
    //webView.detectsPhoneNumbers = YES;
    webView.multipleTouchEnabled = YES;
    webView.autoresizesSubviews = YES;
    webView.delegate = delegate;
    [window addSubview:webView];
    webView.frame = window.frame;
    
    if (items)
        [self createToolbar:items];
    
    if (toolbar) {
        CGFloat tbHeight = [toolbar frame].size.height;
        CGRect frame = webView.frame;
        frame.size.height -= tbHeight;
        webView.frame = frame;
    }
    
    return self;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    webView = nil;
}

- (void)dealloc {
    [super dealloc];
}

// Toolbar handlers

- (void)goBack:(id)sender {
    [self back:0];
}

- (void)goForward:(id)sender {
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

// RhoMainView implementation

- (UIView*)getView {
    return webView;
}

- (void)loadHTMLString:(NSString *)data {
    [webView loadHTMLString:data baseURL:[NSURL URLWithString:@""]];
}

- (void)back:(int)index {
    const char* szBackUrl = rho_rhodesapp_getappbackurl();
    if ( szBackUrl && *szBackUrl )
        [self navigate:[NSString stringWithUTF8String:szBackUrl] tab:0];
    else
	    [webView goBack];
}

- (void)forward:(int)index {
    [webView goForward];
}

- (void)navigate:(NSString *)url tab:(int)index {
    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:url]];
    [webView loadRequest:request];
}

- (void)reload:(int)index {
    //[webView reload];
    const char *url = rho_rhodesapp_getcurrenturl(0);
    [self navigate:[NSString stringWithUTF8String:url] tab:0];
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

@end
