//
//  SimpleMainView.h
//  rhorunner
//
//  Created by Dmitry Moskalchuk on 07.03.10.
//  Copyright 2010 Rhomobile Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RhoMainView.h"

@interface SimpleMainView : UIViewController <RhoMainView> {
    UIView *parent;
    UIWebView *webView;
    UIToolbar *toolbar;
}

@property (retain) UIWebView *webView;
@property (retain) UIToolbar *toolbar;

- (id)initWithParentView:(UIView *)v;
- (id)initWithParentView:(UIView *)v frame:(CGRect)frame;
- (id)initWithParentView:(UIView *)v toolbar:(NSArray*)items;
- (id)initWithParentView:(UIView *)v frame:(CGRect)frame toolbar:(NSArray*)items;

@end
