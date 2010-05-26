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

@synthesize root;

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
    tb.barStyle = UIBarStyleBlackOpaque;
    
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
    return tb;
}

- (void)removeToolbar {
    if (toolbar) {
        CGRect wFrame = webView.frame;
        CGRect tbFrame = toolbar.frame;
        wFrame.size.height += tbFrame.size.height;
        webView.frame = wFrame;
    }
    
    [toolbar removeFromSuperview];
    toolbar = nil;
}

- (void)addToolbar:(NSArray*)items {
    [self removeToolbar];
    
    if (!items)
        return;
    
    CGRect wFrame = webView.frame;
    
    [toolbar removeFromSuperview];
    toolbar = [self newToolbar:items frame:wFrame];
    toolbar.tag = RHO_TAG_TOOLBAR;
    [root addSubview:toolbar];
    
    CGRect tbFrame = toolbar.frame;
    wFrame.size.height -= tbFrame.size.height;
    webView.frame = wFrame;
}

- (UIWebView*)newWebView:(CGRect)frame {
    UIWebView *w = [[UIWebView alloc] initWithFrame:frame];
    w.scalesPageToFit = YES;
    w.userInteractionEnabled = YES;
    w.multipleTouchEnabled = YES;
    w.autoresizesSubviews = YES;
    w.clipsToBounds = NO;
    w.delegate = self;
    w.tag = RHO_TAG_WEBVIEW;
    
    return w;
}

- (id)init:(UIView*)p webView:(UIWebView*)w frame:(CGRect)frame toolbar:(NSArray*)items {
    parent = p;
    
    root = [[UIView alloc] initWithFrame:frame];
    root.userInteractionEnabled = YES;
    
    [webView removeFromSuperview];
    webView = w;
    if (!webView)
        webView = [self newWebView:frame];
    [root addSubview:webView];
    CGRect wFrame = frame;
    wFrame.origin.y = 0;
    webView.frame = wFrame;
    
    [self addToolbar:items];
    navbar = nil;
    
    self.view = root;
    
    return self;
}

- (id)initWithParentView:(UIView *)p frame:(CGRect)frame toolbar:(NSArray*)items {
    return [self init:p webView:nil frame:frame toolbar:items];
}

- (id)initWithParentView:(UIView *)p frame:(CGRect)frame {
    return [self initWithParentView:p frame:frame toolbar:nil];
}

- (id)initWithMainView:(id<RhoMainView>)v {
    return [self initWithMainView:v toolbar:nil];
}

- (id)initWithMainView:(id<RhoMainView>)v toolbar:(NSArray*)items {
    UIView *p = [v parent];
    CGRect frame = [[v view] frame];
    //UIWebView *w = (UIWebView*)[Rhodes subviewWithTag:RHO_TAG_WEBVIEW ofView:[v view]];
    UIWebView *w = [v detachWebView];
    return [self init:p webView:w frame:frame toolbar:items];
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
}

- (void)dealloc {
    [root removeFromSuperview];
    [root release];
    [super dealloc];
}

// Toolbar handlers

- (void)goBack:(id)sender {
    rho_rhodesapp_navigate_back();
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

- (UIView*)view {
    return root;
}

- (UIView*)parent {
    return parent;
}

- (UIWebView*)detachWebView {
    UIWebView *w = [webView retain];
    [w removeFromSuperview];
    webView = nil;
    
    return w;
}

- (void)loadHTMLString:(NSString *)data {
    [webView loadHTMLString:data baseURL:[NSURL URLWithString:@""]];
}

- (void)back:(int)index {
    [webView goBack];
}

- (void)forward:(int)index {
    [webView goForward];
}

- (void)navigate:(NSString *)url tab:(int)index {
    NSString *escapedUrl = [url stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:escapedUrl]];
    [webView loadRequest:request];
}

- (void)navigateRedirect:(NSString *)url tab:(int)index {
    NSString* escapedUrl = [url stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    NSString* redirector = [@"/system/redirect_to?url=" stringByAppendingString:escapedUrl];
    NSString* homeurl = [NSString stringWithUTF8String:rho_rhodesapp_gethomeurl()];
    NSString* redirector1 = [homeurl stringByAppendingString:redirector];
    [self navigate:redirector1 tab:index];
}

- (void)reload:(int)index {
    //[webView reload];
    const char *url = rho_rhodesapp_getcurrenturl(0);
    [self navigateRedirect:[NSString stringWithUTF8String:url] tab:0];
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

- (void)addNavBar:(NSString*)title left:(NSArray*)left right:(NSArray*)right {
    [self removeNavBar];
    
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
    [navbar removeFromSuperview];
    navbar = nb;
    [root addSubview:navbar];
    
    CGRect nFrame = navbar.frame;
    CGRect wFrame = webView.frame;
    wFrame.origin.y += nFrame.size.height;
    wFrame.size.height -= nFrame.size.height;
    webView.frame = wFrame;
}

- (void)removeNavBar {
    if (navbar) {
        CGRect nFrame = navbar.frame;
        CGRect wFrame = webView.frame;
        wFrame.origin.y -= nFrame.size.height;
        wFrame.size.height += nFrame.size.height;
        webView.frame = wFrame;
    }
    
    [navbar removeFromSuperview];
    navbar = nil;
}

// UIWebViewDelegate imlementation

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request
 navigationType:(UIWebViewNavigationType)navigationType {
    NSString *url = [[request URL] absoluteString];
    if (url) {
        NSString *c = [[Rhodes sharedInstance] cookie:url];
        if (c) {
            NSMutableURLRequest *r = (NSMutableURLRequest*)request;
            [r addValue:c forHTTPHeaderField:@"Cookie"];
        }
    }
    return YES;
}

- (void)webViewDidStartLoad:(UIWebView *)webview {
    // TODO
    //[self active];
}

- (void)webViewDidFinishLoad:(UIWebView *)webview {
    [webview stringByEvaluatingJavaScriptFromString:@"document.documentElement.style.webkitTouchCallout = \"none\";"];
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
