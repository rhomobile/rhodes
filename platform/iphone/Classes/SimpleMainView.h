//
//  SimpleMainView.h
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 07.03.10.
//  Copyright 2010 Rhomobile Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RhoMainView.h"

@interface SimpleMainView : UIViewController <RhoMainView, UIWebViewDelegate> {

    UIWebView *webView;
    UIToolbar *toolbar;
    UINavigationBar *navbar;
    CGRect rootFrame;
}

@property (nonatomic,retain) UIWebView *webView;
@property (nonatomic,retain) UIToolbar *toolbar;
@property (nonatomic,retain) UINavigationBar *navbar;

- (id)initWithMainView:(id<RhoMainView>)v parent:(UIWindow*)p;
- (id)initWithMainView:(id<RhoMainView>)v parent:(UIWindow*)p toolbar:(NSArray*)items;
- (id)initWithParentView:(UIView *)p frame:(CGRect)frame;

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation;
- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation;
- (void)loadView;

@end
