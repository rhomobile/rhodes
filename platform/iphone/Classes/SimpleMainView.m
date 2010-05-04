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

@synthesize root, webView, toolbar, navbar;

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

- (id)initWithParentView:(UIView *)v {
    return [self initWithParentView:v toolbar:nil];
}

- (id)initWithParentView:(UIView *)v frame:(CGRect)frame {
    return [self initWithParentView:v frame:frame toolbar:nil];
}

- (id)initWithParentView:(UIView *)v toolbar:(NSArray*)items {
    return [self initWithParentView:v frame:v.frame toolbar:items];
}

- (id)initWithParentView:(UIView *)v frame:(CGRect)frame toolbar:(NSArray*)items {
    parent = v;
    
    root = [[UIView alloc] initWithFrame:frame];
    root.userInteractionEnabled = YES;
    
    CGRect wFrame = frame;
    wFrame.origin.y = 0;
    webView = [[UIWebView alloc] initWithFrame:wFrame];
    webView.scalesPageToFit = YES;
    webView.userInteractionEnabled = YES;
    //webView.detectsPhoneNumbers = YES;
    webView.multipleTouchEnabled = YES;
    webView.autoresizesSubviews = YES;
    webView.clipsToBounds = NO;
    webView.delegate = [Rhodes sharedInstance];
    
    [root addSubview:webView];
    
    if (items) {
        UIToolbar *tb = [self newToolbar:items frame:wFrame];
        self.toolbar = tb;
        [root addSubview:toolbar];
        
        CGRect newFrame = wFrame;
        CGRect tbFrame = tb.frame;
        newFrame.size.height -= tbFrame.size.height;
        webView.frame = newFrame;
        
        [tb release];
    }
    
    navbar = nil;
    
    self.view = root;
    
    return self;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    [root removeFromSuperview];
}

- (void)dealloc {
    [navbar release];
    [toolbar release];
    [webView release];
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
    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:escapedUrl]];
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

- (void)setNavBar:(NSString*)title left:(NSArray*)left right:(NSArray*)right {
    [navbar removeFromSuperview];
    self.navbar = nil;
    
    UINavigationBar *nb = [[UINavigationBar alloc] initWithFrame:CGRectZero];
    [nb sizeToFit];
    
    UINavigationItem *ni = [[UINavigationItem alloc] initWithTitle:title];
    
    NSArray *btns[] = {left, right};
    for (int i = 0, lim = sizeof(btns)/sizeof(btns[0]); i < lim; ++i) {
        NSArray *btn = btns[i];
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
    self.navbar = nb;
    [nb release];
    
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
    self.navbar = nil;
}

@end
