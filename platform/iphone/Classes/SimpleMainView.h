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
    UIView *parent;
    UIView *root;
    UIWebView *webView;
    UIToolbar *toolbar;
    UINavigationBar *navbar;
}

@property (retain) UIView *root;
@property (retain) UIWebView *webView;
@property (retain) UIToolbar *toolbar;
@property (retain) UINavigationBar *navbar;

- (id)initWithMainView:(id<RhoMainView>)v;
- (id)initWithMainView:(id<RhoMainView>)v toolbar:(NSArray*)items;
- (id)initWithParentView:(UIView *)v frame:(CGRect)frame;

@end
