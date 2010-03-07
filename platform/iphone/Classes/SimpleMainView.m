    //
//  SimpleMainView.m
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 07.03.10.
//  Copyright 2010 Rhomobile Inc. All rights reserved.
//

#import "SimpleMainView.h"

@implementation SimpleMainView

@synthesize webView;

- (id)initWithParentWindow:(UIWindow *)w andDelegate:(id)delegate {
    window = w;
    
    webView = [[UIWebView alloc] initWithFrame:window.frame];
    webView.scalesPageToFit = YES;
    webView.userInteractionEnabled = YES;
    //webView.detectsPhoneNumbers = YES;
    webView.multipleTouchEnabled = YES;
    webView.autoresizesSubviews = YES;
    webView.delegate = delegate;
    [window addSubview:webView];
    
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

// RhoMainView implementation

- (UIView*)getView {
    return webView;
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
    [webView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:url]]];
}

- (void)reload:(int)index {
    [webView reload];
}

- (void)executeJs:(JSString*)js tab:(int)index {
    js.outputJs = [webView stringByEvaluatingJavaScriptFromString:js.inputJs];
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
