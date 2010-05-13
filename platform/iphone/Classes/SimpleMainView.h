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
    NSMutableDictionary *cookies;
}

@property (retain) UIView *root;
@property (retain) UIWebView *webView;
@property (retain) UIToolbar *toolbar;
@property (retain) UINavigationBar *navbar;
@property (retain) NSMutableDictionary *cookies;

- (id)initWithParentView:(UIView *)v;
- (id)initWithParentView:(UIView *)v frame:(CGRect)frame;
- (id)initWithParentView:(UIView *)v toolbar:(NSArray*)items;
- (id)initWithParentView:(UIView *)v frame:(CGRect)frame toolbar:(NSArray*)items;

@end
